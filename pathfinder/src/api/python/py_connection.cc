#include "py_connection.h"

using namespace API;

PyConnection::PyConnection(const std::string& host, int port) : connection(host, port) { }

void PyConnection::close() {
    // TODO: Close the connection to the server
    // An exception must be thrown if another method is called after this
}

std::unique_ptr<PyCursor> PyConnection::cursor() {
    return std::make_unique<PyCursor>(connection.cursor());
}

std::unique_ptr<PyCursor> PyConnection::execute(const std::string& operation) {
    return std::make_unique<PyCursor>(connection.execute(operation));
}
