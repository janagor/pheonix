#include "evaluator.hpp"
#include <variant>

#include <iomanip>
#include <sstream>

// NOTE: remove this until it is done
#define UNUSED(x) (void)(x)

namespace pheonix::eval {

std::variant<types::Integer, types::Float, std::string, bool>
Evaluator::getResult() {
  return result;
};

void Evaluator::visit(node::Program &p) {
  for (size_t i = 0; i < p.statements.size(); ++i) {
    p.statements[i]->accept(*this);
  }
}
void Evaluator::visit(node::Parameter &p) { UNUSED(p); }

void Evaluator::visit(node::DeclarationArguments &da) { UNUSED(da); }

void Evaluator::visit(node::Block &b) { UNUSED(b); }

void Evaluator::visit(node::FunctionDeclaration &fd) { UNUSED(fd); }

void Evaluator::visit(node::WhileLoopStatement &wls) { UNUSED(wls); }

void Evaluator::visit(node::VariableDeclaration &vd) { UNUSED(vd); }

void Evaluator::visit(node::IfStatement &is) { UNUSED(is); }

void Evaluator::visit(node::ReturnStatement &rs) { UNUSED(rs); }

void Evaluator::visit(node::ExpressionStatement &es) {
  es.expression->accept(*this);
}

void Evaluator::visit(node::NullStatement &) {}

void Evaluator::visit(node::AssignementExpression &ae) { UNUSED(ae); }

void Evaluator::visit(node::OrExpression &oe) { oe.left->accept(*this); }

void Evaluator::visit(node::AndExpression &ae) { ae.left->accept(*this); }

void Evaluator::visit(node::ComparisonExpression &ce) {
  ce.left->accept(*this);
}

void Evaluator::visit(node::RelationalExpression &re) {
  re.left->accept(*this);
}

void Evaluator::visit(node::MultiplicativeExpression &me) {
  me.left->accept(*this);
}

void Evaluator::visit(node::CompositiveExpression &me) {
  me.left->accept(*this);
}

void Evaluator::visit(node::AdditiveExpression &ae) {
  ae.left->accept(*this);
  auto left = result;
  ae.right->accept(*this);
  auto right = result;
  if (ae.op == "+")
    result = std::visit(AddVisitor{}, left, right);
  else // if (ae.op == "-")
    result = std::visit(SubtractVisitor{}, left, right);
}

void Evaluator::visit(node::CastExpression &ce) {
  ce.expression->accept(*this);
}

void Evaluator::visit(node::PrefixExpression &pe) {
  pe.expression->accept(*this);
}

void Evaluator::visit(node::CallExpression &ce) { UNUSED(ce); }

void Evaluator::visit(node::DebugExpression &de) { UNUSED(de); }

void Evaluator::visit(node::CallArguments &ca) { UNUSED(ca); }

void Evaluator::visit(node::LambdaExpression &le) { UNUSED(le); }

void Evaluator::visit(node::Identifier &i) { UNUSED(i); }

void Evaluator::visit(node::ParentExpression &pe) { UNUSED(pe); }

void Evaluator::visit(node::Literal &l) { result = l.value; }

void Evaluator::visit(node::TypeSpecifier &ts) { UNUSED(ts); }

} // namespace pheonix::eval
