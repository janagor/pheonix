#include "../inc/node.hpp"
#include "../inc/visitor.hpp"

std::string RelationalExpression::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    std::string oper;
    // TODO: create function for handling operators
    switch (op) {
    case token::LESS:
        oper = "<";
        break;
    case token::LEQ:
        oper = "<=";
        break;
    case token::GREATER:
        oper = ">";
        break;
    default:
        oper = ">=";
    }
    return "(RelationalExpression:" + s + "left=" + left->toString(shift_size+1) + 
    "," + s + "operator=[" + (oper)+
    "]," + s + "right=" + right->toString(shift_size + 1) + ")";
}

std::string MultiplicativeExpression::toString(const int shift_size) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(MultiplicativeExpression:" + s + "left=" + left->toString(shift_size+1) + 
    "," + s + "operator=[" + (op==token::STAR ? "*" : "/" )+
    "]," + s + "right=" + right->toString(shift_size + 1) + ")";
}

std::string AdditiveExpression::toString(const int shift_size) const  {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(AdditiveExpression:" + s + "left=" + left->toString(shift_size+1) + 
    "," + s + "operator=[" + (op==token::PLUS ? "+" : "-" ) +
    "]," + s + "right=" + right->toString(shift_size + 1) + ")";
}

std::string IntegerLiteral::toString(const int shift_size) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(IntegerLiteral:" + s + "value=" + std::to_string(value) + ")";
}

void RelationalExpression::accept(Visitor& v) {
    v.visit(*this);
}

void AdditiveExpression::accept(Visitor& v) {
    v.visit(*this);
}

void MultiplicativeExpression::accept(Visitor& v) {
    v.visit(*this);
}

void IntegerLiteral::accept(Visitor& v) {
    v.visit(*this);
}
