#pragma once
#include <string>

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
struct IntegerLiteral;
struct FloatLiteral;
struct BoolLiteral;
struct StringLiteral;
struct TypeSpecifier;

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
  virtual void visit(node::IntegerLiteral &) = 0;
  virtual void visit(node::FloatLiteral &) = 0;
  virtual void visit(node::BoolLiteral &) = 0;
  virtual void visit(node::StringLiteral &) = 0;
  virtual void visit(node::TypeSpecifier &) = 0;
  virtual ~Visitor() = default;
};

class TreeGenVisitor : public Visitor {
public:
  TreeGenVisitor() : Visitor(), result(), shift_size(0), s("\n") {};
  std::string getResult();

  void visit(node::Program &p) override;
  void visit(node::Parameter &p) override;
  void visit(node::DeclarationArguments &p) override;
  void visit(node::Block &p) override;
  void visit(node::FunctionDeclaration &fd) override;
  void visit(node::VariableDeclaration &vd) override;
  void visit(node::WhileLoopStatement &wls) override;
  void visit(node::IfStatement &wls) override;
  void visit(node::ReturnStatement &rs) override;
  void visit(node::ExpressionStatement &es) override;
  void visit(node::AssignementExpression &ae) override;
  void visit(node::OrExpression &oe) override;
  void visit(node::AndExpression &ae) override;
  void visit(node::ComparisonExpression &ce) override;
  void visit(node::RelationalExpression &re) override;
  void visit(node::MultiplicativeExpression &me) override;
  void visit(node::CompositiveExpression &me) override;
  void visit(node::AdditiveExpression &ae) override;
  void visit(node::CastExpression &ce) override;
  void visit(node::PrefixExpression &pe) override;
  void visit(node::CallExpression &ce) override;
  void visit(node::DebugExpression &de) override;
  void visit(node::CallArguments &ca) override;
  void visit(node::LambdaExpression &le) override;
  void visit(node::Identifier &i) override;
  void visit(node::ParentExpression &pe) override;
  void visit(node::IntegerLiteral &il) override;
  void visit(node::FloatLiteral &fl) override;
  void visit(node::BoolLiteral &il) override;
  void visit(node::StringLiteral &sl) override;
  void visit(node::TypeSpecifier &ts) override;

private:
  std::string result;
  int shift_size;
  std::string s; // separator

  void inc() {
    ++shift_size;
    s += std::string(4, ' ');
  }
  void dec() {
    --shift_size;
    s.resize(s.size() - 4);
  }
};

} // namespace pheonix::visitor
