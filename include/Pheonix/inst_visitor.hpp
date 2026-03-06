#pragma once

#include "Pheonix/context.hpp"
#include "Pheonix/node.hpp"
#include "Pheonix/object.hpp"
#include "Pheonix/operator_visitor.hpp"
#include "Pheonix/types.hpp"
#include "Pheonix/visitor.hpp"

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
