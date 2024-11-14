#include "../inc/visitor.hpp"
#include "../inc/node.hpp"

void TreeGenVisitor::visit(IntegerLiteral &il) {
    result += il.toString(1);
}
void TreeGenVisitor::visit(AdditiveExpression &ae) {
    result += ae.toString(1);
}
void TreeGenVisitor::visit(MultiplicativeExpression &me) {
    result += me.toString(1);
}
std::string TreeGenVisitor::getResult() {
    return result;
}

