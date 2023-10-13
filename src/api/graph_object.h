#pragma once

#include <cstdint>
#include <string>
#include <variant>

using GraphObject = std::variant<std::string, int64_t, float, std::monostate>;
