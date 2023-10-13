#pragma once

#include <vector>

#include <boost/asio.hpp>

#include "api/connection.h"
#include "api/graph_object.h"
#include "api/result_parser.h"
#include "network/mql/communication_protocol.h"

namespace API {

class Cursor {
friend class API::Connection;
private:
    struct CursorConnection {
        boost::asio::ip::tcp::socket socket;
        uint_fast32_t binding_size;

        CursorConnection(boost::asio::ip::tcp::socket&& socket,
                         uint_fast32_t                  binding_size) :
            socket       (std::move(socket)),
            binding_size (std::move(binding_size)) { }
    };

public:
    Connection& connection;
    std::unique_ptr<CursorConnection> conn;
    unsigned char* buffer;

    // Only Connection can create this object
    Cursor(Connection& connection) :
        connection(connection)
    {
        buffer = new unsigned char[CommunicationProtocol::BUFFER_SIZE];
    }

    ~Cursor() {
        delete[](buffer);
        if (conn != nullptr) {
            conn->socket.close();
        }
    }

    // TODO: something to close conn before destructor?
    // send fetchmany with N=0 to end?

    void execute(const std::string& query);

    std::unique_ptr<std::vector<GraphObject>> fetchone();

    std::unique_ptr<std::vector<std::vector<GraphObject>>> fetchmany(uint_fast32_t N);

    // same as fetch many N = 2^32
    std::unique_ptr<std::vector<std::vector<GraphObject>>> fetchall() {
        return fetchmany(UINT32_MAX);
    }

private:
    // assumes conn != nullptr
    std::unique_ptr<std::vector<std::vector<GraphObject>>> get_results();
};
} // namespace API
