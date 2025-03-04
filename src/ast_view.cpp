#include "ast_view.hpp"

#include <iomanip>
#include <sstream>

namespace pheonix::ast_view {

std::string ASTView::getResult() const { return result; }

void ASTView::visit(node::Program &p) {
  inc();
  result += "(Program:";
  if (p.statements.size())
    result += s;
  for (size_t i = 0; i < p.statements.size(); ++i) {
    p.statements[i]->accept(*this);
    if (i != p.statements.size() - 1)
      result += "," + s;
  }
  result += ")";
  dec();
}

void ASTView::visit(node::Parameter &p) {
  inc();
  result += "(Parameter:" + s +
            "isMutable=" + (p.isMutable ? "true" : "false") + "," + s +
            "identifier=" + p.identifier + ")";
  dec();
}

void ASTView::visit(node::DeclarationArguments &da) {
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

void ASTView::visit(node::Block &b) {
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

void ASTView::visit(node::FunctionDeclaration &fd) {
  inc();
  result += "(FunctionDeclaration:" + s + "identifier=" + fd.identifier + "," +
            s + "arguments=";
  fd.arguments->accept(*this);
  result += "," + s + "statements=";
  fd.statements->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::WhileLoopStatement &wls) {
  inc();
  result += "(WhileLoopStatement:" + s + "expression=";
  wls.expression->accept(*this);
  result += "," + s + "statements=";
  wls.statements->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::VariableDeclaration &vd) {
  inc();
  result += "(VariableDeclaration:" + s +
            "isMutable=" + (vd.isMutable ? "true" : "false") + "," + s +
            "identifier=" + vd.identifier + "," + s + "expression=";
  vd.expression->accept(*this);
  result += +")";
  dec();
}

void ASTView::visit(node::IfStatement &is) {
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

void ASTView::visit(node::ReturnStatement &rs) {
  inc();
  result += "(ReturnStatement:" + s + "expression=";
  if (rs.expression)
    rs.expression->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::ExpressionStatement &es) {
  inc();
  result += "(ExpressionStatement:" + s + "expression=";
  es.expression->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::NullStatement &) {
  inc();
  result += "(NullStatement:)";
  dec();
}

void ASTView::visit(node::AssignementExpression &ae) {
  inc();
  result += "(AssignementExpression:" + s + "identifier=" + ae.identifier +
            "," + s + "expression=";
  ae.expression->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::OrExpression &oe) {
  inc();
  result += "(OrExpression:" + s + "left=";
  oe.left->accept(*this);
  result += "," + s + "operator=[||]," + s + "right=";
  oe.right->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::AndExpression &ae) {
  inc();
  result += "(AndExpression:" + s + "left=";
  ae.left->accept(*this);
  result += "," + s + "operator=[&&]," + s + "right=";
  ae.right->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::ComparisonExpression &ce) {
  inc();
  result += "(ComparisonExpression:" + s + "left=";
  ce.left->accept(*this);
  result += "," + s + "operator=[" + ce.op + "]," + s + "right=";
  ce.right->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::RelationalExpression &re) {
  inc();
  result += "(RelationalExpression:" + s + "left=";
  re.left->accept(*this);
  result += "," + s + "operator=[" + re.op + "]," + s + "right=";
  re.right->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::MultiplicativeExpression &me) {
  inc();
  result += "(MultiplicativeExpression:" + s + "left=";
  me.left->accept(*this);
  result += "," + s + "operator=[" + me.op + "]," + s + "right=";
  me.right->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::CompositiveExpression &me) {
  inc();
  result += "(CompositiveExpression:" + s + "left=";
  me.left->accept(*this);
  result += "," + s + "operator=[|]," + s + "right=";
  me.right->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::AdditiveExpression &ae) {
  inc();
  result += "(AdditiveExpression:" + s + "left=";
  ae.left->accept(*this);
  result += "," + s + "operator=[" + ae.op + "]," + s + "right=";
  ae.right->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::CastExpression &ce) {
  inc();
  result += "(CastExpression:" + s + "expression=";
  ce.expression->accept(*this);
  result += "," + s + "type=";
  ce.type->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::PrefixExpression &pe) {
  inc();
  result += "(PrefixExpression:" + s + "operator=[" + pe.op + "]," + s +
            "expression=";
  pe.expression->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::CallExpression &ce) {
  inc();
  result += "(CallExpression:" + s + "function=";
  ce.function->accept(*this);
  result += "," + s + "arguments=";
  ce.arguments->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::DebugExpression &de) {
  inc();
  result += "(DebugExpression:" + s + "function=";
  de.function->accept(*this);
  result += "," + s + "arguments=";
  de.arguments->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::CallArguments &ca) {
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

void ASTView::visit(node::LambdaExpression &le) {
  inc();
  result += "(LambdaExpression:" + s + "arguments=";
  le.arguments->accept(*this);
  result += "," + s + "statements=";
  le.statements->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::Identifier &i) {
  inc();
  result += "(Identifier:" + s + "value=" + i.value + ")";
  dec();
}

void ASTView::visit(node::ParentExpression &pe) {
  inc();
  result += "(ParentExpression:" + s + "expression=";
  pe.expression->accept(*this);
  result += ")";
  dec();
}

void ASTView::visit(node::Literal &il) {
  inc();
  result += "(Literal:" + s;

  if (std::holds_alternative<types::Integer>(il.value)) {
    result += "type=Integer," + s + "value=" +
              std::to_string(std::get<types::Integer>(il.value).getValue());
  } else if (std::holds_alternative<types::Float>(il.value)) {
    std::ostringstream dblAsStr;
    dblAsStr << std::fixed << std::setprecision(3)
             << std::get<types::Float>(il.value).getValue();

    result += "type=Float," + s + "value=" + dblAsStr.str();
  } else if (std::holds_alternative<std::string>(il.value)) {
    result += "type=String," + s + "value=" + std::get<std::string>(il.value);
  } else if (std::holds_alternative<bool>(il.value)) {
    result += "type=Bool," + s +
              "value=" + (std::get<bool>(il.value) ? "true" : "false");
  };
  result += ")";
  dec();
}

void ASTView::visit(node::TypeSpecifier &ts) {
  inc();
  result += "(TypeSpecifier:" + s + "value=" + ts.typeName + ")";
  dec();
}

void ASTView::visit(node::PrintFunction &) {}

} // namespace pheonix::ast_view
