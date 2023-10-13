#include <iostream>

#include <boost/asio.hpp>

#include "api/cursor.h"
#include "network/mql/communication_protocol.h"

using namespace API;

// using GraphObject = std::variant<std::string, int64_t, float, std::monostate>;
struct make_string_functor {
  std::string operator()(const std::string &x) const { return x; }
  std::string operator()(int64_t x) const { return std::to_string(x); }
  std::string operator()(float x) const { return std::to_string(x); }
  std::string operator()(std::monostate) const { return ""; }
};

int main() {
    Connection connection("127.0.0.1", 8080);

    auto cursor1 = connection.cursor();
    cursor1->execute("MATCH (?x) RETURN * LIMIT 10");

    auto one_result = cursor1->fetchone();

    auto cursor2 = connection.execute("MATCH (?x)->(?y) RETURN * LIMIT 10");
    auto all_results = cursor2->fetchall();
    // TODO: cursor need variable names of binding

    // // TODO: print result to debug
    if (all_results != nullptr) {
        for (auto& result : *all_results) {
            bool first = true;
            for (auto& graph_object : result) {
                if (first) {
                    first = false;
                } else {
                    std::cout << ',';
                }
                std::cout << std::visit(make_string_functor(), graph_object);
            }
            std::cout << '\n';
        }
    }

    std::cout << std::endl;

    for (auto result = cursor1->fetchone(); result != nullptr; result = cursor1->fetchone()) {
        for (auto& graph_object : *result) {
            std::cout << std::visit(make_string_functor(), graph_object);
            std::cout << ' ';
        }
        std::cout << '\n';
        std::cin.get();
    }
}

/*
#include "api/python/py_connection.h"
#include "api/python/py_cursor.h"

#include <memory>

using namespace API;
int main() {
    // Connect to the database server
    PyConnection conn("localhost", 8080);

    // Create a new cursor. PyCursor expose the the python-specific client interface, while the base Cursor expose the
    // generic interface and handles the internal communication with the server
    PyCursor cur = conn.cursor();

    // Execute a query
    cur.execute("MATCH (?x)->(?y) RETURN *");

    // Fetch a single result.
    // Internally the Cursor would call at least pull(1) parse exactly 1 binding
    pybind11::object one_result = cur.fetchone();

    // Fetch multiple results.
    // Internally the Cursor would call at least pull(N) and parse exactly N bindings
    pybind11::object many_results = cur.fetchmany(10);

    // Fetch all remaining results.
    // Internally the Cursor would call at least pull(UINT64_MAX) and parse all received bindings
    pybind11::object all_results = cur.fetchall();

    // Close the cursor
    cur.close();

    // Close the connection
    conn.close();

    return 0;
}
*/