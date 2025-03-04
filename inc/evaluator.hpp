#pragma once

#include "context.hpp"
#include "node.hpp"
#include "object.hpp"
#include "operator_visitor.hpp"
#include "types.hpp"
#include "visitor.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace pheonix::eval {

bool operator==(const ObjectValue &lhs, const ObjectValue &rhs);

class Evaluator : public visitor::Visitor {
public:
  Evaluator();
  Evaluator(const context::Context &context);
  Object getResult();
  std::vector<Object> getResultVec();
  context::Context getContext();
  void setContext(const context::Context &con);

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
  void visit(node::NullStatement &ns) override;
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
  void visit(node::Literal &il) override;
  void visit(node::TypeSpecifier &ts) override;
  void visit(node::PrintFunction &ts) override;

public:
  std::string lastName;
  std::vector<std::string> lastNames;

private:
  bool isReturning;
  bool isDebugging;
  Object result;
  std::vector<Object> resultVec;
  context::Context context;
};

} // namespace pheonix::eval
