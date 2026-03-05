#pragma once

#include "context.hpp"
#include "node2.hpp"
#include "node_def.hpp"
#include "object.hpp"
#include "operator.hpp"
#include "operator_visitor.hpp"
#include "types.hpp"
#include "visitor.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace pheonix::eval2 {

struct Evaluator {
  Evaluator() : result(0) {}

  void operator()(std::monostate) { std::cout << "Empty node" << std::endl; }

  void operator()(const std::unique_ptr<node2::Literal> &I) {
    result = I->value();
  }

  void operator()(const std::unique_ptr<node2::InfixExpression> &I) {
    auto const &lhs = I->lhs();
    (*this)(lhs);
    auto lhsv = result;

    auto const &rhs = I->rhs();
    (*this)(rhs);
    auto rhsv = result;

    result = oper::Operator()(oper::OperatorType::Add, lhsv, rhsv);
    std::cout << result << std::endl;
  }

  void operator()(const node2::Node &node) { std::visit(*this, node); }

private:
  int result;
};

} // namespace pheonix::eval2
