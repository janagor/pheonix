#pragma once

#include "node_def.hpp"
#include "token.hpp"
#include "visitor.hpp"

namespace pheonix::node2 {

struct Literal {
public:
  Literal(int value) : m_value(value) {}
  [[nodiscard]] int const &value() const noexcept { return m_value; }
  [[nodiscard]] int &value() noexcept { return m_value; }

private:
  int m_value;
};

struct InfixExpression {
public:
  InfixExpression(Node &lhs, Node &rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
  [[nodiscard]] Node const &lhs() const noexcept { return m_lhs; }
  [[nodiscard]] Node &lhs() noexcept { return m_lhs; }
  [[nodiscard]] Node const &rhs() const noexcept { return m_rhs; }
  [[nodiscard]] Node &rhs() noexcept { return m_rhs; }

private:
  Node m_lhs;
  Node m_rhs;
};

} // namespace pheonix::node2
