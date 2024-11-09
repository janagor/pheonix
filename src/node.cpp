#include "../inc/node.hpp"
#include "../inc/visitor.hpp"

std::string IntegerLiteral::toString() const {
    return "(IntegerLiteral: value=" + std::to_string(value) + ")";
}

std::string AdditiveExpression::toString() const  {
    return "(AdditiveExpression: left=" + left->toString() + 
    ", operator=[" + (op==token::PLUS ? "+" : "-" ) +
    "], right=" + right->toString() + ")";
}

std::string MultiplicativeExpression::toString() const  {
    return "(AdditiveExpression: left=" + left->toString() + 
    ", operator=[" + (op==token::STAR ? "*" : "/" )+
    "], right=" + right->toString() + ")";
}

void IntegerLiteral::accept(Visitor& v) {
    v.visit(*this);
}

void AdditiveExpression::accept(Visitor& v) {
    v.visit(*this);
}

void MultiplicativeExpression::accept(Visitor& v) {
    v.visit(*this);
}
