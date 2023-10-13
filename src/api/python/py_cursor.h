#pragma once

#include <memory>

#include <pybind11/pybind11.h>

#include "api/cursor.h"

namespace API {
class PyConnection;

class PyCursor {
public:
    /**
     * @brief Construct a new Cursor object
     *
     * @param
     */
    PyCursor(std::unique_ptr<Cursor> cursor);

    /**
     * @brief Close the cursor
     *
     */
    void close();

    /**
     * @brief Send an operation to the server for execution
     *
     * @param operation the operation string
     */
    void execute(const std::string& operation);

    /**
     * @brief Fetch the next binding of the executed query
     *
     * @return the next binding (pybind11::list) or pybind11::none if no more
     */
    pybind11::object fetchone();

    /**
     * @brief Fetch at most the next `num_bindings` bindings of the executed query
     *
     * @return the next `num_bindings` bindings (pybind11::list) or pybind11::none if no more
     *
     */
    pybind11::object fetchmany(uint_fast32_t num_bindings);

    /**
     * @brief Fetch all the remaining bindings of the executed query
     *
     * @return the remaining bindings (pybind11::list) or pybind11::none if no more
     */
    pybind11::object fetchall();

private:
    std::unique_ptr<Cursor> cursor;

    /**
     * @brief Cast a GraphObject to a python object
     *
     * @param graph_object the GraphObject
     * @return the casted python object
     */
    pybind11::object cast_to_python(const GraphObject& graph_object);
};
} // namespace API