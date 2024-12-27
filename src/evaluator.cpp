#include "evaluator.hpp"
#include "token.hpp"
#include "types.hpp"
#include <iostream>
#include <stdexcept>
#include <variant>

#include <iomanip>
#include <sstream>

// NOTE: remove this after it is done
#define UNUSED(x) (void)(x)

namespace pheonix::eval {

Object Evaluator::getResult() { return Object(result); };

void Evaluator::visit(node::Program &p) {
  for (size_t i = 0; i < p.statements.size(); ++i) {
    p.statements[i]->accept(*this);
  }
}
void Evaluator::visit(node::Parameter &p) {
  resultVec.push_back(ObjectValue(p.identifier));
}

void Evaluator::visit(node::DeclarationArguments &da) {
  resultVec = {};
  for (size_t i = 0; i < da.arguments.size(); ++i) {
    da.arguments[i]->accept(*this);
  }
}

void Evaluator::visit(node::Block &b) {
  for (size_t i = 0; i < b.statements.size(); ++i) {
    if (!isReturning)
      b.statements[i]->accept(*this);
  }
}

void Evaluator::visit(node::FunctionDeclaration &fd) {
  fd.arguments->accept(*this);
  context.insert(fd.identifier,
                 Object(Function(resultVec, fd.statements->clone())));
  result = ObjectValue(std::monostate());
}

void Evaluator::visit(node::WhileLoopStatement &wls) {
  while (true) {
    wls.expression->accept(*this);
    if (auto *predicate = std::get_if<bool>(&result.value)) {
      if (*predicate) {
        wls.statements->accept(*this);
        result = Primitive(std::monostate());
        continue;
      }
      result = Primitive(std::monostate());
      return;
    }
    // TODO: throw here because of a wrong type of expression
    result = Primitive(std::monostate());
    return;
  }
}

void Evaluator::visit(node::VariableDeclaration &vd) {
  vd.expression->accept(*this);
  if (context.find_in_current_scope(vd.identifier) == context.end()) {
    context.insert(vd.identifier, result);
    return;
  }
  throw std::runtime_error("Redeclaration of variable");
}

void Evaluator::visit(node::IfStatement &is) {
  is.predicate->accept(*this);
  if (auto *predicate = std::get_if<bool>(&result.value)) {
    if (*predicate) {
      is.ifBody->accept(*this);
      return;
    }
    if (is.elseBody) {
      is.elseBody->accept(*this);
      return;
    }
  }
  // TODO: throw if predicate is not of bool type

  result = Object();
}

void Evaluator::visit(node::ReturnStatement &rs) {
  rs.expression->accept(*this);
  isReturning = true;
}

void Evaluator::visit(node::ExpressionStatement &es) {
  es.expression->accept(*this);
}

void Evaluator::visit(node::NullStatement &) { result = Primitive(); }

void Evaluator::visit(node::AssignementExpression &ae) {
  if (context.find(ae.identifier) != context.end()) {
    ae.expression->accept(*this);
    context.at(ae.identifier) = result;
    return;
  }
  throw std::runtime_error("variable was not declared");
}

void Evaluator::visit(node::OrExpression &oe) {
  oe.left->accept(*this);
  auto left = result.value;
  oe.right->accept(*this);
  auto right = result.value;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(oe.op));
}

void Evaluator::visit(node::AndExpression &ae) {
  ae.left->accept(*this);
  auto left = result.value;
  ae.right->accept(*this);
  auto right = result.value;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(ae.op));
}

void Evaluator::visit(node::ComparisonExpression &ce) {
  ce.left->accept(*this);
  auto left = result.value;
  ce.right->accept(*this);
  auto right = result.value;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(ce.op));
}

void Evaluator::visit(node::RelationalExpression &re) {
  re.left->accept(*this);
  auto left = result.value;
  re.right->accept(*this);
  auto right = result.value;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(re.op));
}

void Evaluator::visit(node::MultiplicativeExpression &me) {
  me.left->accept(*this);
  auto left = result.value;
  me.right->accept(*this);
  auto right = result.value;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(me.op));
}

void Evaluator::visit(node::CompositiveExpression &me) {
  me.left->accept(*this);
}

void Evaluator::visit(node::AdditiveExpression &ae) {
  ae.left->accept(*this);
  auto left = result.value;
  ae.right->accept(*this);
  auto right = result.value;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(ae.op));
}

void Evaluator::visit(node::CastExpression &ce) {
  // TODO: implement casting
  ce.expression->accept(*this);
}

void Evaluator::visit(node::PrefixExpression &pe) {
  pe.expression->accept(*this);
  auto expression = result.value;
  result = std::visit(OperatorVisitor{}, expression,
                      std::variant<std::string>(pe.op));
}

void Evaluator::visit(node::CallExpression &ce) {
  bool curIsReturning = isReturning;
  isReturning = false;
  context.push_scope();
  ce.function->accept(*this);
  auto function = std::get<Function>(result.value);
  ce.arguments->accept(*this);
  for (size_t i = 0; i < resultVec.size(); ++i) {
    if (lastNames[i] != "")
      context.insertRef(function.args[i], lastNames[i]);
    else
      context.insert(function.args[i], resultVec[i]);
  }
  function.body->accept(*this);
  context.pop_scope();
  isReturning = curIsReturning;
}

void Evaluator::visit(node::DebugExpression &de) { UNUSED(de); }

void Evaluator::visit(node::CallArguments &ca) {
  resultVec = {};
  for (size_t i = 0; i < ca.arguments.size(); ++i) {
    lastNames = {};
    ca.arguments[i]->accept(*this);
    resultVec.push_back(result);
    lastNames.push_back(lastName);
  }
}

void Evaluator::visit(node::LambdaExpression &le) {
  le.arguments->accept(*this);
  result = Object(Function(resultVec, le.statements->clone()));
}

void Evaluator::visit(node::Identifier &i) {
  result = context.at(i.value);
  lastName = i.value;
}

void Evaluator::visit(node::ParentExpression &pe) {
  pe.expression->accept(*this);
}

void Evaluator::visit(node::Literal &l) { result = l.value; }

void Evaluator::visit(node::TypeSpecifier &ts) {
  result = Primitive(ts.typeName);
}

} // namespace pheonix::eval
