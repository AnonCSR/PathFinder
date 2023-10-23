#include "session.h"

#include <memory>
#include <mutex>

#include <boost/bind/bind.hpp>

#include "graph_models/quad_model/quad_model.h"
#include "network/exceptions.h"
#include "network/mql/server.h"
#include "network/mql/tcp_buffer.h"
#include "query/optimizer/quad_model/cursor_constructor.h"
#include "query/optimizer/quad_model/executor_constructor.h"
#include "query/parser/grammar/error_listener.h"
#include "query/parser/mql_query_parser.h"
#include "storage/tmp_manager.h"


using namespace boost;
using namespace MQL;
using namespace std::chrono;

void Session::run() {
    boost::asio::async_read(
        socket,
        boost::asio::buffer(query_size_b, CommunicationProtocol::BYTES_FOR_QUERY_LENGTH),
        boost::bind(
            &Session::do_read,
            shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}


void Session::do_read(const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
    std::string query;

    if (ec) {
        logger.ostream(Category::Error) << " Error receiving the query size.\n";
        return;
    }

    bool is_cursor = false;
    try {
        assert(CommunicationProtocol::BYTES_FOR_QUERY_LENGTH == 4);
        unsigned char cursor_mask = 0b1000'0000;
        if ((query_size_b[3] & cursor_mask) != 0) {
            is_cursor = true;
            query_size_b[3] ^= cursor_mask;
        }

        int query_size = 0;
        for (int i = 0, offset = 0; i < CommunicationProtocol::BYTES_FOR_QUERY_LENGTH; i++, offset += 8) {
            query_size += query_size_b[i] << offset;
        }
        query.resize(query_size);
        boost::asio::read(socket, boost::asio::buffer(query.data(), query_size));
        logger.ostream(Category::Query) << '\n' << query << '\n';
    } catch (...) {
        logger.ostream(Category::Error) << " Error receiving the query.\n";
        return;
    }

    TcpBuffer tcp_buffer(socket);
    std::ostream os(&tcp_buffer);

    // without this line ConnectionException won't be caught properly
    os.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    tmp_manager.reset();
    get_query_ctx().reset();

    if (is_cursor) {
        auto logical_plan = create_logical_plan(query);

        MQL::CursorConstructor query_optimizer;
        logical_plan->accept_visitor(query_optimizer);

        auto cursor = std::move(query_optimizer.executor);
        // send binding_size
        uint32_t binding_size = cursor->get_projection_vars_size();
        for (int i = 0, offset = 0; i < CommunicationProtocol::BYTES_FOR_QUERY_LENGTH; i++, offset += 8) {
            unsigned char c = (binding_size >> offset) & 0xFF;
            query_size_b[i] = c;
        }
        // try {
        boost::system::error_code ec;
        boost::asio::write(
            socket,
            boost::asio::buffer(query_size_b, CommunicationProtocol::BYTES_FOR_QUERY_LENGTH),
            ec
        );
        if (ec) {
            socket.close();
            return;
        }

        bool finished = false;
        while (!finished) {

            boost::asio::read( // TODO: read timeout?
                socket,
                boost::asio::buffer(query_size_b, CommunicationProtocol::BUFFER_SIZE),
                ec
            );
            if (ec) {
                socket.close();
                return;
            }

            uint_fast32_t results_asked = 0;
            for (int i = 0, offset = 0; i < CommunicationProtocol::BYTES_FOR_QUERY_LENGTH; i++, offset += 8) {
                results_asked += query_size_b[i] << offset;
            }

            if (results_asked == 0) break;

            uint_fast32_t remaining = results_asked;

            while (remaining > 0) {
                if (cursor->next()) {
                    cursor->print_current(os);
                    remaining--;
                } else {
                    finished = true;
                    break;
                }
            }
            tcp_buffer.end_cursor_request();
        }
        return;
    }

    std::unique_ptr<QueryExecutor> physical_plan;
    try {
        std::shared_lock s_lock(server.execution_mutex);
        auto logical_plan = create_logical_plan(query);
        if (logical_plan->read_only()) {
            physical_plan = create_readonly_physical_plan(*logical_plan);
        } else {
            std::unique_lock u_lock(server.execution_mutex);
            quad_model.exec_inserts(*reinterpret_cast<OpInsert*>(logical_plan.get()));
        }
    }
    catch (const QueryException& e) {
        logger.ostream(Category::Error)
            << "\n---------------------------------------\n"
            << "Query Exception: " << e.what() << "\n"
            << "---------------------------------------\n";
        tcp_buffer.set_status(CommunicationProtocol::StatusCodes::query_error);
    }
    catch (const LogicException& e) {
        logger.ostream(Category::Error)
            << "\n---------------------------------------\n"
            << "Logic Exception: " << e.what() << "\n"
            << "---------------------------------------\n";
        tcp_buffer.set_status(CommunicationProtocol::StatusCodes::logic_error);
    }

    if (physical_plan == nullptr) {
        tcp_buffer.set_status(CommunicationProtocol::StatusCodes::logic_error);
        return;
    }

    try {
        execute_plan(*physical_plan, os);
        logger.ostream(Category::Info)
            << "\nParser duration:" << parser_duration.count() << "ms.\n"
            << "Optimizer duration:" << optimizer_duration.count() << "ms.\n"
            << "Execution duration:" << execution_duration.count() << "ms.\n"
            << "---------------------------------------\n";
        tcp_buffer.set_status(CommunicationProtocol::StatusCodes::success);
    }
    catch (const ConnectionException& e) {
        logger.ostream(Category::Error)
            << "\n---------------------------------------\n"
            << "Connection Exception: " << e.what() << "\n"
            << "---------------------------------------\n";
    }
    catch (const InterruptedException& e) {
        logger.ostream(Category::Info)
                << "\n---------------------------------------\n"
                << "Timeout thrown after "
                << std::chrono::duration_cast<std::chrono::milliseconds>(execution_duration).count()
                << " milliseconds.\n"
                << "---------------------------------------\n";
        tcp_buffer.set_status(CommunicationProtocol::StatusCodes::timeout);
    }
    catch (const QueryExecutionException& e) {
        logger.ostream(Category::Error)
            << "\n---------------------------------------\n"
            << "Query Execution Exception: " << e.what() << "\n"
            << "---------------------------------------\n";
        tcp_buffer.set_status(CommunicationProtocol::StatusCodes::unexpected_error);
    }
}


std::unique_ptr<Op> Session::create_logical_plan(const std::string& query) {
    auto start_parser = std::chrono::system_clock::now();
    {
        std::lock_guard<std::mutex> lock(server.thread_info_vec_mutex);
        get_query_ctx().thread_info.interruption_requested = false; // used in query optimization
        get_query_ctx().thread_info.finished = false;
        get_query_ctx().thread_info.time_start = start_parser;
        get_query_ctx().thread_info.timeout = start_parser + timeout;
    }
    antlr4::MyErrorListener error_listener;
    auto logical_plan = MQL::QueryParser::get_query_plan(query, &error_listener);
    parser_duration = std::chrono::system_clock::now() - start_parser;
    return logical_plan;
}


void Session::execute_plan(QueryExecutor& physical_plan, std::ostream& os) {
    auto execution_start = std::chrono::system_clock::now();
    try {
        logger.log(Category::PhysicalPlan, [&physical_plan](std::ostream& os) {
            physical_plan.analyze(os << '\n', false);
        });

        auto result_count = physical_plan.execute(os);
        execution_duration = std::chrono::system_clock::now() - execution_start;

        logger.log(Category::ExecutionStats, [&physical_plan](std::ostream& os) {
            physical_plan.analyze(os << '\n', true);
        });

        logger.ostream(Category::Info) << " Results: " << result_count << "\n";
        get_query_ctx().thread_info.finished = true;
    }
    catch (const InterruptedException& e) {
        execution_duration = std::chrono::system_clock::now() - execution_start;
        get_query_ctx().thread_info.finished = true;
        throw e;
    }
    catch (const QueryExecutionException& e) {
        execution_duration = std::chrono::system_clock::now() - execution_start;
        get_query_ctx().thread_info.finished = true;
        throw e;
    }
}


std::unique_ptr<QueryExecutor> Session::create_readonly_physical_plan(Op& logical_plan) {
    auto start_optimizer = std::chrono::system_clock::now();

    MQL::ExecutorConstructor query_optimizer(MQL::ReturnType::CSV);
    logical_plan.accept_visitor(query_optimizer);

    optimizer_duration = std::chrono::system_clock::now() - start_optimizer;
    return std::move(query_optimizer.executor);
}

