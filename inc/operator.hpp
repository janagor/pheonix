#pragma once

namespace pheonix {

enum class OperatorType {
  Add,
  Sub,
  Mul,
  Div,
};

class Operator {

public:
  [[nodiscard]] constexpr int operator()(OperatorType op, int lhs,
                                         int rhs) const noexcept {
    switch (op) {
    case OperatorType::Add:
      return lhs + rhs;
    case OperatorType::Sub:
      return lhs - rhs;
    case OperatorType::Mul:
      return lhs * rhs;
    case OperatorType::Div:
      return lhs / rhs;
    }
    return 0;
  }
};

} // namespace pheonix
