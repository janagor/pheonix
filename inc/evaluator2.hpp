#pragma once

#include "context.hpp"
#include "node2.hpp"
#include "node_def.hpp"
#include "object.hpp"
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
    std::cout << "Evaluating literal..." << std::endl;
    std::cout << result << std::endl;
  }

  void evaluate(const node2::Node &node) { std::visit(*this, node); }

private:
  uint32_t result;
};

} // namespace pheonix::eval
