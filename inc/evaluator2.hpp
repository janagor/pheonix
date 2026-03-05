#pragma once

#include "evaluator_context.hpp"
#include "node2.hpp"
#include "node_def.hpp"
#include "operator.hpp"

#include <iostream>

namespace pheonix {

struct Evaluator {

  Evaluator()
      : m_globalEnvironment(), m_evaluatorContext(m_globalEnvironment),
        m_result(0) {}

  void operator()(std::monostate) const {}

  void operator()(Literal const &I) { m_result = I.value(); }

  void operator()(InfixExpression const &I) {
    auto const &lhs = I.lhs();
    eval(lhs);
    auto lhsv = m_result;

    auto const &rhs = I.rhs();
    eval(rhs);
    auto rhsv = m_result;

    m_result = Operator()(I.op(), lhsv, rhsv);
  }

  void operator()(Block const &I) {
    BlockScopeGuard guard(m_evaluatorContext);
    for (auto const *statement : I.statements()) {
      eval(*statement);
    }
  }

  void eval(Node const &node) { std::visit(*this, node); }

  [[nodiscard]] int const &result() const { return m_result; }
  [[nodiscard]] int &result() { return m_result; }

private:
  GlobalEnvironment m_globalEnvironment;
  EvaluatorContext m_evaluatorContext;
  int m_result;
};

} // namespace pheonix
