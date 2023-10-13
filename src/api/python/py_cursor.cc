#include "py_cursor.h"

#include "api/graph_object.h"
#include "api/python/py_exceptions.h"

using namespace API;
namespace py = pybind11;

PyCursor::PyCursor(std::unique_ptr<Cursor> cursor_) : cursor(std::move(cursor_)) {};

void PyCursor::close() {
    // TODO: Close the connection
    // An exception must be thrown if another method is called after this
};

void PyCursor::execute(const std::string& operation) {
    cursor->execute(operation);
};

py::object PyCursor::fetchone() {
    auto result = cursor->fetchone();
    if (!result)
        return py::none();
    // Build python object
    py::list py_result;
    for (const auto& graph_object : *result) {
        py_result.append(cast_to_python(graph_object));
    }
    return py_result;
}

py::object PyCursor::fetchmany(uint_fast32_t num_bindings) {
    auto result = cursor->fetchmany(num_bindings);
    if (!result)
        return py::none();
    // Build python object
    py::list py_result;
    for (const auto& row : *result) {
        py::list py_row;
        for (const auto& graph_object : row) {
            py_row.append(cast_to_python(graph_object));
        }
        py_result.append(py_row);
    }
    return py_result;
}

py::object PyCursor::fetchall() {
    auto result = cursor->fetchall();
    if (!result)
        return py::none();
    // Build python object
    py::list py_result;
    for (const auto& row : *result) {
        py::list py_row;
        for (const auto& graph_object : row) {
            py_row.append(cast_to_python(graph_object));
        }
        py_result.append(py_row);
    }
    return py_result;
}

py::object PyCursor::cast_to_python(const GraphObject& graph_object) {
    return std::visit(
      [](auto&& arg) -> py::object {
          using T = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, std::string>)
              return py::str(arg);
          else if constexpr (std::is_same_v<T, int64_t>)
              return py::int_(arg);
          else if constexpr (std::is_same_v<T, float>)
              return py::float_(arg);
          else if constexpr (std::is_same_v<T, std::monostate>)
              return py::none();
          else
              throw Error("Unhandled cast to python type: " + std::string(typeid(arg).name()));
      },
      graph_object);
}