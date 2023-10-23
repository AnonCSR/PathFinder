#pragma once

#include <chrono>
#include <iostream>
#include <ostream>
#include <memory>

#include <boost/asio.hpp>

#include "network/mql/communication_protocol.h"
#include "network/mql/server.h"
#include "query/query_context.h"
#include "query/executor/query_executor/query_executor.h"

class Op;

namespace MQL {

class Server;

// Handles an HTTP server connection
class Session : public std::enable_shared_from_this<Session> {
    Server& server;

    boost::asio::ip::tcp::socket socket;

    std::chrono::seconds timeout;

public:
    // Take ownership of the stream
    Session(
        Server& server,
        boost::asio::ip::tcp::socket&& socket,
        std::chrono::seconds timeout
    ) :
        server  (server),
        socket  (std::move(socket)),
        timeout (timeout) { }

    // Start the asynchronous operation
    void run();

    void do_read(const boost::system::error_code& e, std::size_t bytes_transferred);

    void fail(boost::system::error_code ec, char const* what) {
        std::cerr << what << ": " << ec.message() << "\n";
    }

private:
    unsigned char query_size_b[CommunicationProtocol::BYTES_FOR_QUERY_LENGTH];

    std::chrono::duration<float, std::milli> parser_duration;
    std::chrono::duration<float, std::milli> optimizer_duration;
    std::chrono::duration<float, std::milli> execution_duration;

    std::unique_ptr<Op> create_logical_plan(const std::string& query);

    std::unique_ptr<QueryExecutor> create_readonly_physical_plan(Op& logical_plan);

    void execute_plan(QueryExecutor& physical_plan, std::ostream& os);
};
}