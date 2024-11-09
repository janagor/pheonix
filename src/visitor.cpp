#include "../inc/visitor.hpp"
#include "../inc/node.hpp"

void TreeGenVisitor::visit(IntegerLiteral &il) {
    result += il.toString();
}
void TreeGenVisitor::visit(AdditiveExpression &ae) {
    result += ae.toString();
}
void TreeGenVisitor::visit(MultiplicativeExpression &me) {
    result += me.toString();
}
std::string TreeGenVisitor::getResult() {
    return result;
}

