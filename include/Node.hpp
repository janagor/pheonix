#pragma once

#include "NodeDef.hpp"
#include "Operator.hpp"

#include <vector>

namespace pheonix {

class Literal {

public:
  constexpr Literal(Value value) noexcept : m_value(value) {}

  [[nodiscard]] constexpr Value const &value() const noexcept {
    return m_value;
  }
  [[nodiscard]] constexpr Value &value() noexcept { return m_value; }

private:
  Value m_value;
};

class InfixExpression {

public:
  constexpr InfixExpression(OperatorType op, Node *lhs, Node *rhs) noexcept
      : m_op(std::move(op)), m_lhs(lhs), m_rhs(rhs) {}

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

class Block {

public:
  constexpr Block(std::vector<Node *> statements) noexcept
      : m_statements(std::move(statements)) {}

  [[nodiscard]] constexpr std::vector<Node *> const &
  statements() const noexcept {
    return m_statements;
  }
  [[nodiscard]] constexpr std::vector<Node *> &statements() noexcept {
    return m_statements;
  }

private:
  std::vector<Node *> m_statements;
};

class ExpressionStatement {

public:
  constexpr ExpressionStatement(Node *exprStmt) noexcept
      : m_exprStmt(std::move(exprStmt)) {}

  [[nodiscard]] constexpr Node const &exprStmt() const noexcept {
    return *m_exprStmt;
  }
  [[nodiscard]] constexpr Node &exprStmt() noexcept { return *m_exprStmt; }

private:
  Node *m_exprStmt;
};

class VariableDeclaration {

public:
  constexpr VariableDeclaration(std::string ident, Node *exprStmt) noexcept
      : m_ident(std::move(ident)), m_exprStmt(std::move(exprStmt)) {}

  [[nodiscard]] constexpr std::string const &ident() const noexcept {
    return m_ident;
  }
  [[nodiscard]] constexpr std::string &ident() noexcept { return m_ident; }
  [[nodiscard]] constexpr Node const &exprStmt() const noexcept {
    return *m_exprStmt;
  }
  [[nodiscard]] constexpr Node &exprStmt() noexcept { return *m_exprStmt; }

private:
  std::string m_ident;
  Node *m_exprStmt;
};

class Identifier {

public:
  constexpr Identifier(std::string value) noexcept
      : m_value(std::move(value)) {}

  [[nodiscard]] constexpr std::string const &value() const noexcept {
    return m_value;
  }
  [[nodiscard]] constexpr std::string &value() noexcept { return m_value; }

private:
  std::string m_value;
};

class IfExpression {
public:
  constexpr IfExpression(Node *condition, Node *consequence,
                         Node *alternative = nullptr) noexcept
      : m_condition(condition), m_consequence(consequence),
        m_alternative(alternative) {}

  [[nodiscard]] constexpr Node const &condition() const noexcept {
    return *m_condition;
  }
  [[nodiscard]] constexpr Node const &consequence() const noexcept {
    return *m_consequence;
  }
  [[nodiscard]] constexpr Node const *alternative() const noexcept {
    return m_alternative;
  }

private:
  Node *m_condition;
  Node *m_consequence;
  Node *m_alternative;
};

} // namespace pheonix
