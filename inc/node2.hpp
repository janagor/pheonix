#pragma once

#include "node_def.hpp"
#include "operator.hpp"

namespace pheonix {

class Literal {

public:
  constexpr Literal(int value) noexcept : m_value(value) {}

  [[nodiscard]] constexpr int const &value() const noexcept { return m_value; }
  [[nodiscard]] constexpr int &value() noexcept { return m_value; }

private:
  int m_value;
};

class InfixExpression {

public:
  constexpr InfixExpression(OperatorType op, Node *lhs, Node *rhs) noexcept
      : m_op(op), m_lhs(lhs), m_rhs(rhs) {}

  [[nodiscard]] constexpr OperatorType const &op() const noexcept {
    return m_op;
  }
  [[nodiscard]] constexpr OperatorType &op() noexcept { return m_op; }
  [[nodiscard]] constexpr Node const &lhs() const noexcept { return *m_lhs; }
  [[nodiscard]] constexpr Node &lhs() noexcept { return *m_lhs; }
  [[nodiscard]] constexpr Node const &rhs() const noexcept { return *m_rhs; }
  [[nodiscard]] constexpr Node &rhs() noexcept { return *m_rhs; }

private:
  OperatorType m_op;
  Node *m_lhs;
  Node *m_rhs;
};

} // namespace pheonix
