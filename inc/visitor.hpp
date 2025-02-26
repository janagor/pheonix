#pragma once

namespace pheonix::node {

struct Program;
struct Parameter;
struct DeclarationArguments;
struct Block;
struct FunctionDeclaration;
struct VariableDeclaration;
struct WhileLoopStatement;
struct IfStatement;
struct ReturnStatement;
struct ExpressionStatement;
struct NullStatement;
struct AssignementExpression;
struct OrExpression;
struct AndExpression;
struct ComparisonExpression;
struct RelationalExpression;
struct AdditiveExpression;
struct MultiplicativeExpression;
struct CompositiveExpression;
struct CastExpression;
struct PrefixExpression;
struct CallExpression;
struct DebugExpression;
struct CallArguments;
struct LambdaExpression;
struct Identifier;
struct ParentExpression;
struct Literal;
struct TypeSpecifier;
struct PrintFunction;

} // namespace pheonix::node

namespace pheonix::visitor {

class Visitor {
public:
  virtual void visit(node::Program &) = 0;
  virtual void visit(node::Parameter &) = 0;
  virtual void visit(node::DeclarationArguments &) = 0;
  virtual void visit(node::Block &) = 0;
  virtual void visit(node::FunctionDeclaration &) = 0;
  virtual void visit(node::VariableDeclaration &) = 0;
  virtual void visit(node::WhileLoopStatement &) = 0;
  virtual void visit(node::IfStatement &) = 0;
  virtual void visit(node::ReturnStatement &) = 0;
  virtual void visit(node::ExpressionStatement &) = 0;
  virtual void visit(node::NullStatement &) = 0;
  virtual void visit(node::AssignementExpression &) = 0;
  virtual void visit(node::OrExpression &) = 0;
  virtual void visit(node::AndExpression &) = 0;
  virtual void visit(node::ComparisonExpression &) = 0;
  virtual void visit(node::RelationalExpression &) = 0;
  virtual void visit(node::AdditiveExpression &) = 0;
  virtual void visit(node::MultiplicativeExpression &) = 0;
  virtual void visit(node::CompositiveExpression &) = 0;
  virtual void visit(node::CastExpression &) = 0;
  virtual void visit(node::PrefixExpression &) = 0;
  virtual void visit(node::CallExpression &) = 0;
  virtual void visit(node::DebugExpression &) = 0;
  virtual void visit(node::CallArguments &) = 0;
  virtual void visit(node::LambdaExpression &) = 0;
  virtual void visit(node::Identifier &) = 0;
  virtual void visit(node::ParentExpression &) = 0;
  virtual void visit(node::Literal &) = 0;
  virtual void visit(node::TypeSpecifier &) = 0;
  virtual void visit(node::PrintFunction &) = 0;
  virtual ~Visitor() = default;
};

} // namespace pheonix::visitor
