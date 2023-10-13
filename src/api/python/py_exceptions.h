#pragma once

#include <stdexcept>

namespace API {

// Exception that is the base class of all other error exceptions.
// You can use this to catch all errors with one single except statement.
struct Error : public std::runtime_error {
    Error(const std::string& msg) : std::runtime_error(msg) { }
};
} // namespace API