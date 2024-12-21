#pragma once
#include "context.hpp"
#include "node.hpp"
#include "visitor.hpp"
#include <fmt/core.h>
#include <string>
#include <variant>

namespace pheonix::eval {

inline bool operator==(const Primitive &lhs, const Primitive &rhs) {
  return std::visit([](const auto &lhs_val,
                       const auto &rhs_val) { return lhs_val == rhs_val; },
                    lhs, rhs);
}

struct OperatorVisitor {
  // infix operators
  Primitive operator()(const types::Integer &lhs, const types::Integer &rhs,
                       const std::string &op) const {
    // arithmetic
    if (op == "+")
      return lhs + rhs;
    else if (op == "-")
      return lhs - rhs;
    else if (op == "*")
      return lhs * rhs;
    else if (op == "/")
      return lhs / rhs;
    else if (op == "%")
      return lhs % rhs;
    // comparison
    else if (op == "==")
      return bool(lhs == rhs);
    else if (op == "!=")
      return bool(lhs != rhs);
    else if (op == "<")
      return lhs < rhs;
    else if (op == ">")
      return (lhs > rhs);
    else if (op == "<=")
      return (lhs <= rhs);
    else if (op == ">=")
      return bool(lhs >= rhs);
    else
      return false;
  }

  Primitive operator()(const types::Float &lhs, const types::Float &rhs,
                       const std::string &op) const {
    // arithmetic
    if (op == "+")
      return lhs + rhs;
    else if (op == "-")
      return lhs - rhs;
    else if (op == "*")
      return lhs * rhs;
    else if (op == "/")
      return lhs / rhs;
    // comparison
    else if (op == "==")
      return Primitive(bool(lhs == rhs));
    else if (op == "!=")
      return Primitive(bool(lhs != rhs));
    else if (op == "<")
      return Primitive(bool(lhs < rhs));
    else if (op == ">")
      return Primitive(bool(lhs > rhs));
    else if (op == "<=")
      return Primitive(bool(lhs <= rhs));
    else if (op == ">=")
      return Primitive(bool(lhs >= rhs));
    else
      return false;
  }

  Primitive operator()(const std::string &lhs, const std::string &rhs,
                       const std::string &op) const {
    if (op == "+")
      return lhs + rhs;
    else
      return types::Float(0);
  }

  Primitive operator()(bool lhs, bool rhs, const std::string &op) const {
    if (op == "&&")
      return lhs && rhs;
    else if (op == "||")
      return lhs || rhs;
    else
      return types::Float(0);
  }

  Primitive operator()(const auto &, const auto &, const auto &) const {
    throw std::runtime_error("Invalid transition");
  }

  // prefix operators
  Primitive operator()(const types::Integer &exp, const std::string &op) const {
    if (op == "-")
      return -exp;
    else
      return types::Integer(0);
  }

  Primitive operator()(const types::Float &exp, const std::string &op) const {
    if (op == "-")
      return -exp;
    else
      return types::Integer(0);
  }

  Primitive operator()(bool exp, const std::string &op) const {
    if (op == "!")
      return !exp;
    else
      return types::Integer(0);
  }

  Primitive operator()(const auto &, const auto &) const {
    throw std::runtime_error("Invalid transition");
  }
};

class Evaluator : public visitor::Visitor {
public:
  Evaluator() : visitor::Visitor(), result(), context() {};
  Primitive getResult();

  void visit(node::Program &p) override;
  void visit(node::Parameter &p) override;
  void visit(node::DeclarationArguments &p) override;
  void visit(node::Block &p) override;
  void visit(node::FunctionDeclaration &fd) override;
  void visit(node::VariableDeclaration &vd) override;
  void visit(node::WhileLoopStatement &wls) override;
  void visit(node::IfStatement &wls) override;
  void visit(node::ReturnStatement &rs) override;
  void visit(node::ExpressionStatement &es) override;
  void visit(node::NullStatement &ns) override;
  void visit(node::AssignementExpression &ae) override;
  void visit(node::OrExpression &oe) override;
  void visit(node::AndExpression &ae) override;
  void visit(node::ComparisonExpression &ce) override;
  void visit(node::RelationalExpression &re) override;
  void visit(node::MultiplicativeExpression &me) override;
  void visit(node::CompositiveExpression &me) override;
  void visit(node::AdditiveExpression &ae) override;
  void visit(node::CastExpression &ce) override;
  void visit(node::PrefixExpression &pe) override;
  void visit(node::CallExpression &ce) override;
  void visit(node::DebugExpression &de) override;
  void visit(node::CallArguments &ca) override;
  void visit(node::LambdaExpression &le) override;
  void visit(node::Identifier &i) override;
  void visit(node::ParentExpression &pe) override;
  void visit(node::Literal &il) override;
  void visit(node::TypeSpecifier &ts) override;

private:
  Primitive result;
  context::Context context;
};

} // namespace pheonix::eval
