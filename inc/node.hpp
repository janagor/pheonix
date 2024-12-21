#pragma once

#include "ast_view.hpp"
#include "token.hpp"
#include "types.hpp"
#include "visitor.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace pheonix::node {

struct Node {
  virtual ~Node() = default;
  virtual void accept(visitor::Visitor &v) = 0;
};

struct Block : public Node {
  std::vector<std::unique_ptr<Node>> statements;

  Block() : Node() {};

  void accept(visitor::Visitor &v) override;
};

struct Program : public Node {
  std::vector<std::unique_ptr<Node>> statements;

  Program() : Node() {};

  void accept(visitor::Visitor &v) override;
};

struct Parameter : public Node {
  Parameter(bool isMut, const std::string &ident)
      : Node(), isMutable(isMut), identifier(ident) {}
  bool isMutable;
  std::string identifier;

  void accept(visitor::Visitor &v) override;
};

struct DeclarationArguments : public Node {
  DeclarationArguments() : Node() {}
  std::vector<std::unique_ptr<Node>> arguments;

  void accept(visitor::Visitor &v) override;
};
// Statements
struct FunctionDeclaration : public Node {
  std::string identifier;
  std::unique_ptr<Node> arguments;
  std::unique_ptr<Node> statements;

  FunctionDeclaration(std::string i) : Node(), identifier(i) {};

  void accept(visitor::Visitor &v) override;
};

struct VariableDeclaration : public Node {
  bool isMutable;
  std::string identifier;
  std::unique_ptr<Node> expression;

  VariableDeclaration(bool isMut, std::string i, std::unique_ptr<Node> e)
      : Node(), isMutable(isMut), identifier(i), expression(std::move(e)) {};

  void accept(visitor::Visitor &v) override;
};

struct WhileLoopStatement : public Node {
  std::unique_ptr<Node> expression;
  std::unique_ptr<Node> statements;

  WhileLoopStatement(std::unique_ptr<Node> e, std::unique_ptr<Node> s)
      : Node(), expression(std::move(e)), statements(std::move(s)) {};

  void accept(visitor::Visitor &v) override;
};

struct IfStatement : public Node {
  std::unique_ptr<Node> predicate;
  std::unique_ptr<Node> ifBody;
  std::unique_ptr<Node> elseBody;

  IfStatement(std::unique_ptr<Node> pred)
      : Node(), predicate(std::move(pred)), ifBody(nullptr),
        elseBody(nullptr) {};

  void accept(visitor::Visitor &v) override;
};

struct ReturnStatement : public Node {
  std::unique_ptr<Node> expression;
  ReturnStatement(std::unique_ptr<Node> e)
      : Node(), expression(std::move(e)) {};
  void accept(visitor::Visitor &v) override;
};

struct ExpressionStatement : public Node {
  std::unique_ptr<Node> expression;
  ExpressionStatement(std::unique_ptr<Node> e)
      : Node(), expression(std::move(e)) {};
  void accept(visitor::Visitor &v) override;
};

struct NullStatement : public Node {
  NullStatement() : Node() {};
  void accept(visitor::Visitor &v) override;
};

// Expressions
struct AssignementExpression : public Node {
  std::string identifier;
  std::unique_ptr<Node> expression;

  AssignementExpression(std::string i, std::unique_ptr<Node> e)
      : Node(), identifier(i), expression(std::move(e)) {};

  void accept(visitor::Visitor &v) override;
};

struct OrExpression : public Node {
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  std::string op;
  OrExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r, std::string t)
      : Node(), left(std::move(l)), right(std::move(r)), op(t) {};
  void accept(visitor::Visitor &v) override;
};

struct AndExpression : public Node {
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  std::string op;
  AndExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r, std::string t)
      : Node(), left(std::move(l)), right(std::move(r)), op(t) {};
  void accept(visitor::Visitor &v) override;
};

struct ComparisonExpression : public Node {
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  std::string op;
  ComparisonExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r,
                       std::string t)
      : Node(), left(std::move(l)), right(std::move(r)), op(t) {};
  void accept(visitor::Visitor &v) override;
};

struct RelationalExpression : public Node {
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  std::string op;
  RelationalExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r,
                       std::string t)
      : Node(), left(std::move(l)), right(std::move(r)), op(t) {};
  void accept(visitor::Visitor &v) override;
};

struct AdditiveExpression : public Node {
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  std::string op;
  AdditiveExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r,
                     std::string t)
      : Node(), left(std::move(l)), right(std::move(r)), op(t) {};
  void accept(visitor::Visitor &v) override;
};

struct MultiplicativeExpression : public Node {
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  std::string op;
  MultiplicativeExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r,
                           std::string t)
      : Node(), left(std::move(l)), right(std::move(r)), op(t) {};
  void accept(visitor::Visitor &v) override;
};

struct CompositiveExpression : public Node {
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  CompositiveExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r)
      : Node(), left(std::move(l)), right(std::move(r)) {};
  void accept(visitor::Visitor &v) override;
};

struct CastExpression : public Node {
  std::unique_ptr<Node> expression;
  std::unique_ptr<Node> type;
  CastExpression(std::unique_ptr<Node> e, std::unique_ptr<Node> t)
      : Node(), expression(std::move(e)), type(std::move(t)) {};
  void accept(visitor::Visitor &v) override;
};

struct PrefixExpression : public Node {
  std::string op;
  std::unique_ptr<Node> expression;
  PrefixExpression(std::string o, std::unique_ptr<Node> e)
      : Node(), op(o), expression(std::move(e)) {};
  void accept(visitor::Visitor &v) override;
};

struct CallExpression : public Node {
  std::unique_ptr<Node> function;
  std::unique_ptr<Node> arguments;
  CallExpression(std::unique_ptr<Node> f, std::unique_ptr<Node> a)
      : Node(), function(std::move(f)), arguments(std::move(a)) {};
  void accept(visitor::Visitor &v) override;
};

struct DebugExpression : public Node {
  std::unique_ptr<Node> function;
  std::unique_ptr<Node> arguments;
  DebugExpression(std::unique_ptr<Node> f, std::unique_ptr<Node> a)
      : Node(), function(std::move(f)), arguments(std::move(a)) {};
  void accept(visitor::Visitor &v) override;
};

struct LambdaExpression : public Node {
  std::unique_ptr<Node> arguments;
  std::unique_ptr<Node> statements;

  LambdaExpression() : Node() {};
  LambdaExpression(std::unique_ptr<Node> a, std::unique_ptr<Node> s)
      : Node(), arguments(std::move(a)), statements(std::move(s)) {};

  void accept(visitor::Visitor &v) override;
};

struct Identifier : public Node {
  std::string value;
  Identifier(const std::string &val) : Node(), value(val) {};
  void accept(visitor::Visitor &v) override;
};

struct ParentExpression : public Node {
  std::unique_ptr<Node> expression;
  ParentExpression(std::unique_ptr<Node> e)
      : Node(), expression(std::move(e)) {};
  void accept(visitor::Visitor &v) override;
};

struct Literal : public Node {
  Primitive value;
  Literal(Primitive val) : Node(), value(val) {};
  void accept(visitor::Visitor &v) override;
};

struct TypeSpecifier : public Node {
  std::string typeName;
  TypeSpecifier(const std::string &t) : Node(), typeName(t) {};
  void accept(visitor::Visitor &v) override;
};

struct CallArguments : public Node {
  std::vector<std::unique_ptr<Node>> arguments;
  CallArguments() : Node() {};
  void accept(visitor::Visitor &v) override;
};

} // namespace pheonix::node
