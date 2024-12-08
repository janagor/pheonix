#pragma once

#include "token.hpp"
#include "types.hpp"
#include "visitor.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace pheonix::node {

std::string opToString(const token::TokenType &tok);

struct Node {
  virtual ~Node() = default;
  virtual std::string toString(const int shift_size) const = 0;
  virtual void accept(visitor::Visitor &v) = 0;
};

struct Block : public Node {
  std::vector<std::unique_ptr<Node>> statements;

  Block() : Node() {};

  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct Program : public Node {
  std::vector<std::unique_ptr<Node>> statements;

  Program() : Node() {};

  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct Parameter : public Node {
  Parameter(bool isMut, const std::string &ident)
      : Node(), isMutable(isMut), identifier(ident) {}
  bool isMutable;
  std::string identifier;

  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct DeclarationArguments : public Node {
  DeclarationArguments() : Node() {}
  std::vector<std::unique_ptr<Node>> arguments;

  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};
// Statements
struct FunctionDeclaration : public Node {
  std::string identifier;
  std::unique_ptr<Node> arguments;
  std::unique_ptr<Node> statements;

  FunctionDeclaration(std::string i) : Node(), identifier(i) {};

  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct VariableDeclaration : public Node {
  bool isMutable;
  std::string identifier;
  std::unique_ptr<Node> expression;

  VariableDeclaration(bool isMut, std::string i, std::unique_ptr<Node> e)
      : Node(), isMutable(isMut), identifier(i), expression(std::move(e)) {};

  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct WhileLoopStatement : public Node {
  std::unique_ptr<Node> expression;
  std::unique_ptr<Node> statements;

  WhileLoopStatement(std::unique_ptr<Node> e, std::unique_ptr<Node> s)
      : Node(), expression(std::move(e)), statements(std::move(s)) {};

  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct IfStatement : public Node {
  std::unique_ptr<Node> predicate;
  std::unique_ptr<Node> ifBody;
  std::unique_ptr<Node> elseBody;

  IfStatement(std::unique_ptr<Node> pred)
      : Node(), predicate(std::move(pred)), ifBody(nullptr),
        elseBody(nullptr) {};

  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct ReturnStatement : public Node {
  std::unique_ptr<Node> expression;
  ReturnStatement(std::unique_ptr<Node> e)
      : Node(), expression(std::move(e)) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct ExpressionStatement : public Node {
  std::unique_ptr<Node> expression;
  ExpressionStatement(std::unique_ptr<Node> e)
      : Node(), expression(std::move(e)) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};
// Expressions
struct AssignementExpression : public Node {
  std::string identifier;
  std::unique_ptr<Node> expression;

  AssignementExpression(std::string i, std::unique_ptr<Node> e)
      : Node(), identifier(i), expression(std::move(e)) {};

  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct OrExpression : public Node {
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  token::TokenType op;
  OrExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r,
               token::TokenType t)
      : Node(), left(std::move(l)), right(std::move(r)), op(t) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct AndExpression : public Node {
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  token::TokenType op;
  AndExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r,
                token::TokenType t)
      : Node(), left(std::move(l)), right(std::move(r)), op(t) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct ComparisonExpression : public Node {
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  token::TokenType op;
  ComparisonExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r,
                       token::TokenType t)
      : Node(), left(std::move(l)), right(std::move(r)), op(t) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct RelationalExpression : public Node {
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  token::TokenType op;
  RelationalExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r,
                       token::TokenType t)
      : Node(), left(std::move(l)), right(std::move(r)), op(t) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct AdditiveExpression : public Node {
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  token::TokenType op;
  AdditiveExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r,
                     token::TokenType t)
      : Node(), left(std::move(l)), right(std::move(r)), op(t) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct MultiplicativeExpression : public Node {
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  token::TokenType op;
  MultiplicativeExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r,
                           token::TokenType t)
      : Node(), left(std::move(l)), right(std::move(r)), op(t) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct CompositiveExpression : public Node {
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  CompositiveExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r)
      : Node(), left(std::move(l)), right(std::move(r)) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct CastExpression : public Node {
  std::unique_ptr<Node> expression;
  std::unique_ptr<Node> type;
  CastExpression(std::unique_ptr<Node> e, std::unique_ptr<Node> t)
      : Node(), expression(std::move(e)), type(std::move(t)) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct PrefixExpression : public Node {
  token::TokenType op;
  std::unique_ptr<Node> expression;
  PrefixExpression(token::TokenType o, std::unique_ptr<Node> e)
      : Node(), op(o), expression(std::move(e)) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct CallExpression : public Node {
  std::unique_ptr<Node> function;
  std::unique_ptr<Node> arguments;
  CallExpression(std::unique_ptr<Node> f, std::unique_ptr<Node> a)
      : Node(), function(std::move(f)), arguments(std::move(a)) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct DebugExpression : public Node {
  std::unique_ptr<Node> function;
  std::unique_ptr<Node> arguments;
  DebugExpression(std::unique_ptr<Node> f, std::unique_ptr<Node> a)
      : Node(), function(std::move(f)), arguments(std::move(a)) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct LambdaExpression : public Node {
  std::unique_ptr<Node> arguments;
  std::unique_ptr<Node> statements;

  LambdaExpression() : Node() {};

  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct Identifier : public Node {
  std::string value;
  Identifier(const std::string &val) : Node(), value(val) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct ParentExpression : public Node {
  std::unique_ptr<Node> expression;
  ParentExpression(std::unique_ptr<Node> e)
      : Node(), expression(std::move(e)) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct IntegerLiteral : public Node {
  types::Integer value;
  IntegerLiteral(types::Integer val) : Node(), value(val) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct FloatLiteral : public Node {
  types::Float value;
  FloatLiteral(types::Float val) : Node(), value(val) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct BoolLiteral : public Node {
  bool value;
  BoolLiteral(bool val) : Node(), value(val) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct StringLiteral : public Node {
  std::string value;
  StringLiteral(const std::string &val) : Node(), value(val) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

enum TypeName : int {
  ERROR = 0,
  STR,
  INT,
  FLT,
  BOL,
};

static const std::map<token::TokenType, TypeName> TokenToType{
    {token::TokenType::INT, TypeName::INT},
    {token::TokenType::STR, TypeName::STR},
    {token::TokenType::FLT, TypeName::FLT},
    {token::TokenType::BOL, TypeName::BOL}};

static const std::map<TypeName, std::string> TypeToString{
    {TypeName::INT, "int"},
    {TypeName::STR, "str"},
    {TypeName::FLT, "flt"},
    {TypeName::BOL, "bol"}};

struct TypeSpecifier : public Node {
  TypeName typeName;
  TypeSpecifier(const TypeName &type) : Node(), typeName(type) {};
  TypeSpecifier(const token::TokenType &token)
      : Node(), typeName(TokenToType.at(token)) {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

struct CallArguments : public Node {
  std::vector<std::unique_ptr<Node>> arguments;
  CallArguments() : Node() {};
  std::string toString(const int shift_size) const override;
  void accept(visitor::Visitor &v) override;
};

} // namespace pheonix::node
