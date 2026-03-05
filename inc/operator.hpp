#pragma once

namespace pheonix {

enum class OperatorType {
  Add,
};

class Operator {

public:
  [[nodiscard]] constexpr int operator()(OperatorType op, int lhs,
                                         int rhs) const noexcept {
    switch (op) {
    case OperatorType::Add:
      return lhs + rhs;
    }
    return 0;
  }
};

} // namespace pheonix
