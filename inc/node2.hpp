#pragma once

#include "node_def.hpp"
#include "token.hpp"
#include "visitor.hpp"

namespace pheonix::node2 {

struct Literal {
public:
  Literal(int value) : m_value(value) {}
  int const &value() const { return m_value; }
  int &value() { return m_value; }

private:
  int m_value;
};

} // namespace pheonix::node2
