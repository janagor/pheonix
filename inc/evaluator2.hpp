#pragma once

#include "evaluator_context.hpp"
#include "node2.hpp"
#include "node_def.hpp"
#include "operator.hpp"

#include <iostream>
#include <variant>

namespace pheonix {

struct Evaluator {

  Evaluator()
      : m_globalEnvironment(), m_evaluatorContext(m_globalEnvironment),
        m_result() {}

  void operator()(std::monostate) const {}

  void operator()(Literal const &I) { m_result = I.value(); }

  void operator()(InfixExpression const &I) {
    auto const &lhs = I.lhs();
    eval(lhs);
    auto lhsv = m_result;

    auto const &rhs = I.rhs();
    eval(rhs);
    auto rhsv = m_result;

    m_result =
        std::visit(Operator(), std::variant<OperatorType>(I.op()), lhsv, rhsv);
  }

  void operator()(Block const &I) {
    BlockScopeGuard guard(m_evaluatorContext);
    for (auto const *statement : I.statements()) {
      eval(*statement);
    }
  }

  void operator()(ExpressionStatement const &I) {
    auto const &exprStmt = I.exprStmt();
    eval(exprStmt);
  }

  void operator()(Identifier const &I) {
    if (auto val = m_evaluatorContext.getVariable(I.value()); val.has_value())
      m_result = *val;
  }

  void operator()(VariableDeclaration const &I) {
    auto const &ident = I.ident();
    auto const &expr = I.exprStmt();
    eval(expr);
    auto val = m_result;
    m_evaluatorContext.defineVariable(ident, val);
  }

  void eval(Node const &node) { std::visit(*this, node); }

  [[nodiscard]] Value const &result() const { return m_result; }
  [[nodiscard]] Value &result() { return m_result; }

private:
  GlobalEnvironment m_globalEnvironment;
  EvaluatorContext m_evaluatorContext;
  Value m_result;
};

} // namespace pheonix
