#pragma once

namespace pheonix::oper {

enum class OperatorType {
  Add,
};

struct Operator {
  int operator()(OperatorType op, int lhs, int rhs) {
    switch (op) {
    case OperatorType::Add:
      return lhs + rhs;
    }
    return 0;
  }
};

} // namespace pheonix::oper
