#include "visitor.hpp"
#include "node.hpp"

namespace pheonix::visitor {

std::string TreeGenVisitor::getResult() { return result; }
void TreeGenVisitor::visit(node::Program &p) { result += p.toString(1); }
void TreeGenVisitor::visit(node::Parameter &p) { result += p.toString(1); }
void TreeGenVisitor::visit(node::DeclarationArguments &p) {
  result += p.toString(1);
}
void TreeGenVisitor::visit(node::Block &b) { result += b.toString(1); }
void TreeGenVisitor::visit(node::FunctionDeclaration &fd) {
  result += fd.toString(1);
}
void TreeGenVisitor::visit(node::VariableDeclaration &vd) {
  result += vd.toString(1);
}
void TreeGenVisitor::visit(node::WhileLoopStatement &wls) {
  result += wls.toString(1);
}
void TreeGenVisitor::visit(node::IfStatement &is) { result += is.toString(1); }
void TreeGenVisitor::visit(node::ReturnStatement &rs) {
  result += rs.toString(1);
}
void TreeGenVisitor::visit(node::ExpressionStatement &es) {
  result += es.toString(1);
}
void TreeGenVisitor::visit(node::AssignementExpression &ae) {
  result += ae.toString(1);
}
void TreeGenVisitor::visit(node::OrExpression &oe) { result += oe.toString(1); }
void TreeGenVisitor::visit(node::AndExpression &ae) {
  result += ae.toString(1);
}
void TreeGenVisitor::visit(node::ComparisonExpression &ce) {
  result += ce.toString(1);
}
void TreeGenVisitor::visit(node::RelationalExpression &re) {
  result += re.toString(1);
}
void TreeGenVisitor::visit(node::MultiplicativeExpression &me) {
  result += me.toString(1);
}
void TreeGenVisitor::visit(node::CompositiveExpression &me) {
  result += me.toString(1);
}
void TreeGenVisitor::visit(node::AdditiveExpression &ae) {
  result += ae.toString(1);
}
void TreeGenVisitor::visit(node::CastExpression &ce) {
  result += ce.toString(1);
}
void TreeGenVisitor::visit(node::PrefixExpression &pe) {
  result += pe.toString(1);
}
void TreeGenVisitor::visit(node::CallExpression &ce) {
  result += ce.toString(1);
}
void TreeGenVisitor::visit(node::DebugExpression &de) {
  result += de.toString(1);
}
void TreeGenVisitor::visit(node::CallArguments &ca) {
  result += ca.toString(1);
}
void TreeGenVisitor::visit(node::LambdaExpression &le) {
  result += le.toString(1);
}
void TreeGenVisitor::visit(node::Identifier &i) { result += i.toString(1); }
void TreeGenVisitor::visit(node::ParentExpression &pe) {
  result += pe.toString(1);
}
void TreeGenVisitor::visit(node::IntegerLiteral &il) {
  result += il.toString(1);
}
void TreeGenVisitor::visit(node::FloatLiteral &il) { result += il.toString(1); }
void TreeGenVisitor::visit(node::BoolLiteral &bl) { result += bl.toString(1); }
void TreeGenVisitor::visit(node::StringLiteral &sl) {
  result += sl.toString(1);
}
void TreeGenVisitor::visit(node::TypeSpecifier &ts) {
  result += ts.toString(1);
}

} // namespace pheonix::visitor
