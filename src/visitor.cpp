#include "../inc/visitor.hpp"
#include "../inc/node.hpp"



void PrintVisitor::visit(IntegerLiteral &il) {
    std::cout << (
        "(IntegerLiteral: value=" + std::to_string(il.value) + ")"
    ) << std::endl;
}
void PrintVisitor::visit(AdditiveExpression &il) {
    std::cout << (
        "(AdditiveExpression: left=" + il.left->toString() + 
        ", operator=[" + (il.op==token::STAR ? "*" : "/" )+
        "], right=" + il.right->toString() + ")"
    ) << std::endl;
}
void PrintVisitor::visit(MultiplicativeExpression &il) {
    std::cout << (
        "(AdditiveExpression: left=" + il.left->toString() + 
        ", operator=[" + (il.op==token::PLUS ? "+" : "-" ) +
        "], right=" + il.right->toString() + ")"
    ) << std::endl;
}

