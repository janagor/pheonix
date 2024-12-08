#include "visitor.hpp"
#include "node.hpp"

#include <iomanip>
#include <sstream>

namespace pheonix::visitor {

std::string TreeGenVisitor::getResult() { return result; }
void TreeGenVisitor::visit(node::Program &p) {
  inc();
  result += "(Program:" + s;
  for (size_t i = 0; i < p.statements.size(); ++i) {
    p.statements[i]->accept(*this);
    if (i != p.statements.size() - 1)
      result += "," + s;
  }
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::Parameter &p) {
  inc();
  result += "(Parameter:" + s +
            "isMutable=" + (p.isMutable ? "true" : "false") + "," + s +
            "identifier=" + p.identifier + ")";
  dec();
}

void TreeGenVisitor::visit(node::DeclarationArguments &da) {
  inc();
  result += "(DeclarationArguments:";
  if (da.arguments.size())
    result += s;
  for (size_t i = 0; i < da.arguments.size(); ++i) {
    da.arguments[i]->accept(*this);
    if (i != da.arguments.size() - 1)
      result += "," + s;
  }
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::Block &b) {
  inc();
  result += "(Block:" + s;
  for (size_t i = 0; i < b.statements.size(); ++i) {
    b.statements[i]->accept(*this);
    if (i != b.statements.size() - 1)
      result += "," + s;
  }
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::FunctionDeclaration &fd) {
  inc();
  result += "(FunctionDeclaration:" + s + "identifier=" + fd.identifier + "," +
            s + "arguments=";
  fd.arguments->accept(*this);
  result += "," + s + "statements=";
  fd.statements->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::WhileLoopStatement &wls) {
  inc();
  result += "(WhileLoopStatement:" + s + "expression=";
  wls.expression->accept(*this);
  result += "," + s + "statements=";
  wls.statements->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::VariableDeclaration &vd) {
  inc();
  result += "(VariableDeclaration:" + s +
            "isMutable=" + (vd.isMutable ? "true" : "false") + "," + s +
            "identifier=" + vd.identifier + "," + s + "expression=";
  vd.expression->accept(*this);
  result += +")";
  dec();
}

void TreeGenVisitor::visit(node::IfStatement &is) {
  inc();
  result += "(IfStatement:" + s + "predicate=";
  is.predicate->accept(*this);
  result += "," + s + "ifBody=";
  is.ifBody->accept(*this);
  result += "," + s + "elseBody=";
  if (is.elseBody.get())
    is.elseBody->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::ReturnStatement &rs) {
  inc();
  result += "(ReturnStatement:" + s + "expression=";
  rs.expression->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::ExpressionStatement &es) {
  inc();
  result += "(ExpressionStatement:" + s + "expression=";
  es.expression->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::AssignementExpression &ae) {
  inc();
  result += "(AssignementExpression:" + s + "identifier=" + ae.identifier +
            "," + s + "expression=";
  ae.expression->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::OrExpression &oe) {
  inc();
  result += "(OrExpression:" + s + "left=";
  oe.left->accept(*this);
  result += "," + s + "operator=[||]," + s + "right=";
  oe.right->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::AndExpression &ae) {
  inc();
  result += "(AndExpression:" + s + "left=";
  ae.left->accept(*this);
  result += "," + s + "operator=[&&]," + s + "right=";
  ae.right->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::ComparisonExpression &ce) {
  inc();
  result += "(ComparisonExpression:" + s + "left=";
  ce.left->accept(*this);
  result +=
      "," + s + "operator=[" + types::opToString(ce.op) + "]," + s + "right=";
  ce.right->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::RelationalExpression &re) {
  inc();
  result += "(RelationalExpression:" + s + "left=";
  re.left->accept(*this);
  result +=
      "," + s + "operator=[" + types::opToString(re.op) + "]," + s + "right=";
  re.right->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::MultiplicativeExpression &me) {
  inc();
  result += "(MultiplicativeExpression:" + s + "left=";
  me.left->accept(*this);
  result +=
      "," + s + "operator=[" + types::opToString(me.op) + "]," + s + "right=";
  me.right->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::CompositiveExpression &me) {
  inc();
  result += "(CompositiveExpression:" + s + "left=";
  me.left->accept(*this);
  result += "," + s + "operator=[|]," + s + "right=";
  me.right->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::AdditiveExpression &ae) {
  inc();
  result += "(AdditiveExpression:" + s + "left=";
  ae.left->accept(*this);
  result +=
      "," + s + "operator=[" + types::opToString(ae.op) + "]," + s + "right=";
  ae.right->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::CastExpression &ce) {
  inc();
  result += "(CastExpression:" + s + "expression=";
  ce.expression->accept(*this);
  result += "," + s + "type=";
  ce.type->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::PrefixExpression &pe) {
  inc();
  result += "(PrefixExpression:" + s + "operator=[" + types::opToString(pe.op) +
            "]," + s + "expression=";
  pe.expression->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::CallExpression &ce) {
  inc();
  result += "(CallExpression:" + s + "function=";
  ce.function->accept(*this);
  result += "," + s + "arguments=";
  ce.arguments->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::DebugExpression &de) {
  inc();
  result += "(DebugExpression:" + s + "function=";
  de.function->accept(*this);
  result += "," + s + "arguments=";
  de.arguments->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::CallArguments &ca) {
  inc();
  result += "(CallArguments:";
  if (ca.arguments.size())
    result += s;
  for (size_t i = 0; i < ca.arguments.size(); ++i) {
    ca.arguments[i]->accept(*this);
    if (i != ca.arguments.size() - 1)
      result += "," + s;
  }
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::LambdaExpression &le) {
  inc();
  result += "(LambdaExpression:" + s + "arguments=";
  le.arguments->accept(*this);
  result += "," + s + "statements=";
  le.statements->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::Identifier &i) {
  inc();
  result += "(Identifier:" + s + "value=" + i.value + ")";
  dec();
}

void TreeGenVisitor::visit(node::ParentExpression &pe) {
  inc();
  result += "(ParentExpression:" + s + "expression=";
  pe.expression->accept(*this);
  result += ")";
  dec();
}

void TreeGenVisitor::visit(node::IntegerLiteral &il) {
  inc();
  result += "(IntegerLiteral:" + s +
            "value=" + std::to_string(il.value.getValue()) + ")";
  dec();
}

void TreeGenVisitor::visit(node::FloatLiteral &il) {
  inc();
  std::ostringstream dblAsStr;
  dblAsStr << std::fixed << std::setprecision(3) << il.value;
  result += "(FloatLiteral:" + s + "value=" + dblAsStr.str() + ")";
  dec();
}

void TreeGenVisitor::visit(node::BoolLiteral &bl) {
  inc();
  result +=
      "(BoolLiteral:" + s + "value=" + (bl.value ? "true" : "false") + ")";
  dec();
}

void TreeGenVisitor::visit(node::StringLiteral &sl) {
  inc();
  result += "(StringLiteral:" + s + "value=" + sl.value + ")";
  dec();
}

void TreeGenVisitor::visit(node::TypeSpecifier &ts) {
  inc();
  result += "(TypeSpecifier:" + s +
            "value=" + types::TypeNameToString.at(ts.typeName) + ")";
  dec();
}

} // namespace pheonix::visitor
