#include "evaluator.hpp"
#include <variant>

#include <iomanip>
#include <sstream>

// NOTE: remove this after it is done
#define UNUSED(x) (void)(x)

namespace pheonix::eval {

Primitive Evaluator::getResult() { return result; };

void Evaluator::visit(node::Program &p) {
  for (size_t i = 0; i < p.statements.size(); ++i) {
    p.statements[i]->accept(*this);
  }
}
void Evaluator::visit(node::Parameter &p) { UNUSED(p); }

void Evaluator::visit(node::DeclarationArguments &da) { UNUSED(da); }

void Evaluator::visit(node::Block &b) {
  for (size_t i = 0; i < b.statements.size(); ++i) {
    b.statements[i]->accept(*this);
  }
}

void Evaluator::visit(node::FunctionDeclaration &fd) { UNUSED(fd); }

void Evaluator::visit(node::WhileLoopStatement &wls) {
  while (true) {
    wls.expression->accept(*this);
    if (auto *predicate = std::get_if<bool>(&result)) {
      if (predicate) {
        wls.statements->accept(*this);
        result = std::monostate();
        continue;
      }
      result = std::monostate();
      return;
    }
    // TODO: throw here because of a wrong type of expression
    result = std::monostate();
    return;
  }
}

void Evaluator::visit(node::VariableDeclaration &vd) { UNUSED(vd); }

void Evaluator::visit(node::IfStatement &is) {
  is.predicate->accept(*this);
  if (auto *predicate = std::get_if<bool>(&result)) {
    if (predicate) {
      is.ifBody->accept(*this);
      result = std::monostate();
      return;
    }
    if (is.ifBody) {
      is.ifBody->accept(*this);
      result = std::monostate();
      return;
    }

    result = std::monostate();
  }
  // TODO: throw if predicate is not of bool type

  result = std::monostate();
}

void Evaluator::visit(node::ReturnStatement &rs) { UNUSED(rs); }

void Evaluator::visit(node::ExpressionStatement &es) {
  es.expression->accept(*this);
}

void Evaluator::visit(node::NullStatement &) {}

void Evaluator::visit(node::AssignementExpression &ae) {
  ae.expression->accept(*this);
  context.context[ae.identifier] = result;
}

void Evaluator::visit(node::OrExpression &oe) {
  oe.left->accept(*this);
  auto left = result;
  oe.right->accept(*this);
  auto right = result;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(oe.op));
}

void Evaluator::visit(node::AndExpression &ae) {
  ae.left->accept(*this);
  auto left = result;
  ae.right->accept(*this);
  auto right = result;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(ae.op));
}

void Evaluator::visit(node::ComparisonExpression &ce) {
  ce.left->accept(*this);
  auto left = result;
  ce.right->accept(*this);
  auto right = result;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(ce.op));
}

void Evaluator::visit(node::RelationalExpression &re) {
  re.left->accept(*this);
  auto left = result;
  re.right->accept(*this);
  auto right = result;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(re.op));
}

void Evaluator::visit(node::MultiplicativeExpression &me) {
  me.left->accept(*this);
  auto left = result;
  me.right->accept(*this);
  auto right = result;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(me.op));
}

void Evaluator::visit(node::CompositiveExpression &me) {
  me.left->accept(*this);
}

void Evaluator::visit(node::AdditiveExpression &ae) {
  ae.left->accept(*this);
  auto left = result;
  ae.right->accept(*this);
  auto right = result;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(ae.op));
}

void Evaluator::visit(node::CastExpression &ce) {
  ce.expression->accept(*this);
}

void Evaluator::visit(node::PrefixExpression &pe) {
  pe.expression->accept(*this);
  auto expression = result;
  result = std::visit(OperatorVisitor{}, expression,
                      std::variant<std::string>(pe.op));
}

void Evaluator::visit(node::CallExpression &ce) { UNUSED(ce); }

void Evaluator::visit(node::DebugExpression &de) { UNUSED(de); }

void Evaluator::visit(node::CallArguments &ca) { UNUSED(ca); }

void Evaluator::visit(node::LambdaExpression &le) { UNUSED(le); }

void Evaluator::visit(node::Identifier &i) {
  result = context.context.at(i.value);
}

void Evaluator::visit(node::ParentExpression &pe) {
  pe.expression->accept(*this);
}

void Evaluator::visit(node::Literal &l) { result = l.value; }

void Evaluator::visit(node::TypeSpecifier &ts) { UNUSED(ts); }

} // namespace pheonix::eval
