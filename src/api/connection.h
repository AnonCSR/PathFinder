#pragma once

#include <string>

#include <boost/asio.hpp>

namespace API {
class Cursor;

class Connection {
public:
    boost::asio::io_service io_service;
    const std::string host;
    const std::string port;

    Connection(const std::string& host, int port) :
        host (host),
        port (std::to_string(port)) { }

    std::unique_ptr<Cursor> cursor();

    std::unique_ptr<Cursor> execute(const std::string& query);
};
}
