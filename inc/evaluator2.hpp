#pragma once

#include "node2.hpp"
#include "node_def.hpp"
#include "operator.hpp"

#include <iostream>

namespace pheonix {

struct Evaluator {

  constexpr Evaluator() noexcept : m_result(0) {}

  constexpr void operator()(std::monostate) const noexcept {}

  constexpr void operator()(Literal const &I) noexcept { m_result = I.value(); }

  constexpr void operator()(InfixExpression const &I) noexcept {
    auto const &lhs = I.lhs();
    eval(lhs);
    auto lhsv = m_result;

    auto const &rhs = I.rhs();
    eval(rhs);
    auto rhsv = m_result;

    m_result = Operator()(OperatorType::Add, lhsv, rhsv);
  }

  constexpr void eval(Node const &node) noexcept { std::visit(*this, node); }

  [[nodiscard]] constexpr int const &result() const noexcept {
    return m_result;
  }
  [[nodiscard]] constexpr int &result() noexcept { return m_result; }

private:
  int m_result;
};

} // namespace pheonix
