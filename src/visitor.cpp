#include "../inc/visitor.hpp"
#include "../inc/node.hpp"

std::string TreeGenVisitor::getResult() {
    return result;
}
void TreeGenVisitor::visit(Program &p) {
    result += p.toString(1);
}
void TreeGenVisitor::visit(Block &b) {
    result += b.toString(1);
}
void TreeGenVisitor::visit(FunctionDeclaration &fd) {
    result += fd.toString(1);
}
void TreeGenVisitor::visit(VariableDeclaration &vd) {
    result += vd.toString(1);
}
void TreeGenVisitor::visit(WhileLoopStatement &wls) {
    result += wls.toString(1);
}
void TreeGenVisitor::visit(IfStatement &is) {
    result += is.toString(1);
}
void TreeGenVisitor::visit(ReturnStatement &rs) {
    result += rs.toString(1);
}
void TreeGenVisitor::visit(ExpressionStatement &es) {
    result += es.toString(1);
}
void TreeGenVisitor::visit(AssignementExpression &ae) {
    result += ae.toString(1);
}
void TreeGenVisitor::visit(OrExpression &oe) {
    result += oe.toString(1);
}
void TreeGenVisitor::visit(AndExpression &ae) {
    result += ae.toString(1);
}
void TreeGenVisitor::visit(ComparisonExpression &ce) {
    result += ce.toString(1);
}
void TreeGenVisitor::visit(RelationalExpression &re) {
    result += re.toString(1);
}
void TreeGenVisitor::visit(MultiplicativeExpression &me) {
    result += me.toString(1);
}
void TreeGenVisitor::visit(AdditiveExpression &ae) {
    result += ae.toString(1);
}
void TreeGenVisitor::visit(CastExpression &ce) {
    result += ce.toString(1);
}
void TreeGenVisitor::visit(PrefixExpression &pe) {
    result += pe.toString(1);
}
void TreeGenVisitor::visit(TypeSpecifier &ts) {
    result += ts.toString(1);
}
void TreeGenVisitor::visit(IntegerLiteral &il) {
    result += il.toString(1);
}
