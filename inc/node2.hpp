#pragma once

#include "node_def.hpp"

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
  constexpr InfixExpression(Node *lhs, Node *rhs) noexcept
      : m_lhs(lhs), m_rhs(rhs) {}

  [[nodiscard]] constexpr Node const &lhs() const noexcept { return *m_lhs; }
  [[nodiscard]] constexpr Node &lhs() noexcept { return *m_lhs; }
  [[nodiscard]] constexpr Node const &rhs() const noexcept { return *m_rhs; }
  [[nodiscard]] constexpr Node &rhs() noexcept { return *m_rhs; }

private:
  Node *m_lhs;
  Node *m_rhs;
};

} // namespace pheonix::node2
