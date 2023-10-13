#pragma once

#include <memory>
#include <string>

#include "api/connection.h"
#include "api/python/py_cursor.h"

namespace API {

class PyConnection {
public:
    /**
     * @brief Create a new PyConnection
     *
     * @param host server host
     * @param port server port
     */
    PyConnection(const std::string& host, int port);

    /**
     * @brief Close the connection to the server
     *
     */
    void close();

    /**
     * @brief Return a new PyCursor using the connection
     *
     * @return a new cursor
     */
    std::unique_ptr<PyCursor> cursor();

    /**
     * @brief Return an executed PyCursor using the connection
     *
     * @param operation the operation string
     *
     * @return a new executed cursor
     */
    std::unique_ptr<PyCursor> execute(const std::string& operation);

private:
    Connection connection;
};
} // namespace API