#include "../inc/visitor.hpp"
#include "../inc/node.hpp"

std::string TreeGenVisitor::getResult() {
    return result;
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
