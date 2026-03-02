#pragma once

#include "context.hpp"
#include "node.hpp"
#include "object.hpp"
#include "operator_visitor.hpp"
#include "types.hpp"
#include "visitor.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace pheonix {
template <typename SubClass> class InstVisitor {

  void visitLiteral(node::Literal &) {
    static_cast<SubClass *>(this)->visitLiteral();
  }
  void visitAdditiveExpression(node::AdditiveExpression &) {
    static_cast<SubClass *>(this)->visitAdditiveExpression();
  }
};

} // namespace pheonix
