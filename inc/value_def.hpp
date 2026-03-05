#pragma once

#include <variant>

namespace pheonix {

class Int;

// clang-format off
using Value = std::variant<
    std::monostate,
    Int
>;
// clang-format on

} // namespace pheonix
