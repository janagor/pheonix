#pragma once

#include "Pheonix/Value.hpp"
#include "Pheonix/ValueDef.hpp"

#include <variant>

namespace pheonix {

enum class OperatorType {
  Add,
  Sub,
  Mul,
  Div,
};

class Operator {

public:
  [[nodiscard]] constexpr Value operator()(OperatorType op, Int const &lhs,
                                           Int const &rhs) const noexcept {
    switch (op) {
    case OperatorType::Add:
      return Value(lhs + rhs);
    case OperatorType::Sub:
      return Value(lhs - rhs);
    case OperatorType::Mul:
      return Value(lhs * rhs);
    case OperatorType::Div:
      return Value(lhs / rhs);
    }
    return Value{};
  }

  [[nodiscard]] constexpr Value operator()(OperatorType, auto const &,
                                           auto const &) const noexcept {
    return Value{};
  }
};

} // namespace pheonix
