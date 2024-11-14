#include "../inc/node.hpp"
#include "../inc/visitor.hpp"

std::string IntegerLiteral::toString(const int shift_size) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(IntegerLiteral:" + s + "value=" + std::to_string(value) + ")";
}

std::string AdditiveExpression::toString(const int shift_size) const  {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(AdditiveExpression:" + s + "left=" + left->toString(shift_size+1) + 
    "," + s + "operator=[" + (op==token::PLUS ? "+" : "-" ) +
    "]," + s + "right=" + right->toString(shift_size + 1) + ")";
}

std::string MultiplicativeExpression::toString(const int shift_size) const {
    std::string separator = std::string(shift_size*4, ' ');
    std::string s = "\n" + separator;
    return "(MultiplicativeExpression:" + s + "left=" + left->toString(shift_size+1) + 
    "," + s + "operator=[" + (op==token::STAR ? "*" : "/" )+
    "]," + s + "right=" + right->toString(shift_size + 1) + ")";
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
