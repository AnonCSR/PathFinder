#include <pybind11/pybind11.h>

#include "api/python/py_connection.h"
#include "api/python/py_cursor.h"
#include "api/python/py_exceptions.h"

using namespace API;
namespace py = pybind11;

PYBIND11_MODULE(_pypf, m) {
    static py::exception<Error> exc(m, "Error");
    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p)
                std::rethrow_exception(p);
        }
        catch (const std::exception& e) {
            exc(e.what());
        }
        catch (...) {
            exc("Unknown exception");
        }
    });

    py::class_<PyConnection>(m, "PyConnection")
      .def(py::init<const std::string&, int>())
      .def("close", &PyConnection::close)
      .def("cursor", &PyConnection::cursor)
      .def("execute", &PyConnection::execute);

    py::class_<PyCursor>(m, "PyCursor")
      .def("close", &PyCursor::close)
      .def("execute", &PyCursor::execute)
      .def("fetchone", &PyCursor::fetchone)
      .def("fetchmany", &PyCursor::fetchmany)
      .def("fetchall", &PyCursor::fetchall);

    m.attr("__version__") = "dev";
}