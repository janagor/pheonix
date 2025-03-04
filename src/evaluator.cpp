#include "evaluator.hpp"
#include "token.hpp"
#include "types.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <variant>

#include <iomanip>
#include <sstream>

namespace pheonix::eval {

bool operator==(const ObjectValue &lhs, const ObjectValue &rhs) {
  return std::visit([](const auto &lhs_val,
                       const auto &rhs_val) { return lhs_val == rhs_val; },
                    lhs, rhs);
}

Evaluator::Evaluator()
    : visitor::Visitor(), lastName(), lastNames(), isReturning(false),
      isDebugging(false), result(), resultVec(), context() {
  // inserting "print" function
  // NOTE: user cannot declare variable of such name
  std::vector<eval::Param> args;
  args.emplace_back("0", false);
  auto body = std::make_unique<node::PrintFunction>();
  Function f(args, std::move(body));

  context.insert("print", Object(f));
};

Evaluator::Evaluator(const context::Context &context)
    : visitor::Visitor(), lastName(), lastNames(), isReturning(false),
      isDebugging(false), result(), resultVec(), context(context) {};

Object Evaluator::getResult() { return Object(result); };
std::vector<Object> Evaluator::getResultVec() { return resultVec; };
context::Context Evaluator::getContext() { return context.clone(); }
void Evaluator::setContext(const context::Context &con) { context = con; }

void Evaluator::visit(node::Program &p) {
  for (size_t i = 0; i < p.statements.size(); ++i) {
    p.statements[i]->accept(*this);
  }
}
void Evaluator::visit(node::Parameter &p) {
  resultVec.emplace_back(ObjectValue(p.identifier), p.isMutable);
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
    result.mut = vd.isMutable;
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
  if (isDebugging)
    std::cout << "[return: " << result.value << "]\n";
}

void Evaluator::visit(node::ExpressionStatement &es) {
  es.expression->accept(*this);
  if (isDebugging)
    std::cout << "[" << result.value << "]\n";
}

void Evaluator::visit(node::NullStatement &) { result = Primitive(); }

void Evaluator::visit(node::AssignementExpression &ae) {
  if (context.find(ae.identifier) == context.end()) {
    throw std::runtime_error("variable was not declared");
    return;
  }
  if (!context.at(ae.identifier).mut) {
    throw std::runtime_error("variable is not mutable.");
    return;
  }

  ae.expression->accept(*this);
  context.at_ref(ae.identifier).value = result.value;
  return;
}

void Evaluator::visit(node::OrExpression &oe) {
  oe.left->accept(*this);
  auto left = result.value;
  oe.right->accept(*this);
  auto right = result.value;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(oe.op));
  lastName = "";
}

void Evaluator::visit(node::AndExpression &ae) {
  ae.left->accept(*this);
  auto left = result.value;
  ae.right->accept(*this);
  auto right = result.value;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(ae.op));
  lastName = "";
}

void Evaluator::visit(node::ComparisonExpression &ce) {
  ce.left->accept(*this);
  auto left = result.value;
  ce.right->accept(*this);
  auto right = result.value;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(ce.op));
  lastName = "";
}

void Evaluator::visit(node::RelationalExpression &re) {
  re.left->accept(*this);
  auto left = result.value;
  re.right->accept(*this);
  auto right = result.value;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(re.op));
  lastName = "";
}

void Evaluator::visit(node::MultiplicativeExpression &me) {
  me.left->accept(*this);
  auto left = result.value;
  me.right->accept(*this);
  auto right = result.value;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(me.op));
  lastName = "";
}

void Evaluator::visit(node::CompositiveExpression &me) {
  me.left->accept(*this);
  auto left = result.value;
  me.right->accept(*this);
  auto right = result.value;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>("|"));
  lastName = "";
}

void Evaluator::visit(node::AdditiveExpression &ae) {
  ae.left->accept(*this);
  auto left = result.value;
  ae.right->accept(*this);
  auto right = result.value;
  result = std::visit(OperatorVisitor{}, left, right,
                      std::variant<std::string>(ae.op));
  lastName = "";
}

void Evaluator::visit(node::CastExpression &ce) {
  ce.expression->accept(*this);
  if (ce.type) {
    auto expression = result.value;
    ce.type->accept(*this);
    auto type = result.value;
    result = std::visit(OperatorVisitor{}, expression, type,
                        std::variant<std::string>("<-"));
    lastName = "";
    return;
  }
}

void Evaluator::visit(node::PrefixExpression &pe) {
  pe.expression->accept(*this);
  auto expression = result.value;
  result = std::visit(OperatorVisitor{}, expression,
                      std::variant<std::string>(pe.op));
  lastName = "";
}

void Evaluator::visit(node::CallExpression &ce) {
  bool curIsReturning = isReturning;
  isReturning = false;
  context.push_scope();
  ce.function->accept(*this);
  auto function = std::get<Function>(result.value);
  ce.arguments->accept(*this);
  assert(resultVec.size() == lastNames.size());
  assert(resultVec.size() == function.args.size());
  for (size_t i = 0; i < resultVec.size(); ++i) {
    // for (size_t i = 0; i < function.args.size(); ++i) {
    if (lastNames[i] != "") {
      if ((function.args.at(i).mut) && (!context.at(lastNames.at(i)).mut))
        throw std::runtime_error("variable is not mutable.");
      context.insertRef(function.args.at(i).name, lastNames.at(i),
                        function.args.at(i).mut);
    }

    // function.args.at(i).mut  );
    else
      context.insert(function.args.at(i).name, resultVec.at(i));
  }
  function.body.at(0)->accept(*this);
  // if it is a composite function
  for (size_t i = 1; i < function.body.size(); ++i) {
    context.pop_scope();
    context.push_scope();
    context.insert(function.args2.at(i - 1).name, result);
    isReturning = false;
    function.body.at(i)->accept(*this);
  }
  context.pop_scope();
  isReturning = curIsReturning;
  lastName = "";
}

void Evaluator::visit(node::DebugExpression &de) {
  lastName = "";
  // tragedy ...
  bool curIsReturning = isReturning;
  isReturning = false;
  bool curIsDebugging = isDebugging;
  isDebugging = true;
  context.push_scope();
  de.function->accept(*this);
  auto function = std::get<Function>(result.value);
  de.arguments->accept(*this);
  std::cout << "[input: ";
  for (size_t i = 0; i < resultVec.size(); ++i) {
    if (lastNames.at(i) != "") {
      std::cout << context.at(lastNames.at(i)).value;
      context.insertRef(function.args.at(i).name, lastNames.at(i),
                        function.args.at(i).mut);

    } else {
      std::cout << resultVec.at(i).value;
      context.insert(function.args.at(i).name, resultVec.at(i));
    }
    if (i < resultVec.size() - 1)
      std::cout << ", ";
  }
  std::cout << "]\n";
  function.body.at(0)->accept(*this);
  // if it is a composite function
  for (size_t i = 1; i < function.body.size(); ++i) {
    context.pop_scope();
    context.push_scope();
    context.insert(function.args2.at(i - 1).name, result);
    isReturning = false;
    isDebugging = true;
    function.body.at(i)->accept(*this);
  }
  context.pop_scope();
  isReturning = curIsReturning;
  isDebugging = curIsDebugging;
}

void Evaluator::visit(node::CallArguments &ca) {
  resultVec = {};
  lastNames = {};
  auto cVec = resultVec;
  auto cNam = lastNames;
  for (size_t i = 0; i < ca.arguments.size(); ++i) {
    ca.arguments[i]->accept(*this);
    cVec.push_back(result);
    cNam.push_back(lastName);
  }
  resultVec = cVec;
  lastNames = cNam;
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

void Evaluator::visit(node::Literal &l) {
  result = l.value;
  lastName = "";
}

void Evaluator::visit(node::TypeSpecifier &ts) {
  result = Primitive(ts.typeName);
}

void Evaluator::visit([[maybe_unused]] node::PrintFunction &pf) {
  std::cout << context.at("0").value << "\n";
}

} // namespace pheonix::eval
