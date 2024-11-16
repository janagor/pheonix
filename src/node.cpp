#include "../inc/node.hpp"
#include "../inc/visitor.hpp"

std::string opToString(const token::TokenType& tok) {
    switch (tok) {
    case token::EQUALS:
        return "==";
        break;
    case token::NEQ:
        return "!=";
        break;
    case token::PLUS:
        return "+";
        break;
    case token::MINUS:
        return "-";
        break;
    case token::STAR:
        return "*";
        break;
    case token::SLASH:
        return "/";
        break;
    case token::PERCENT:
        return "%";
        break;
    case token::LEQ:
        return "<=";
        break;
    case token::LESS:
        return "<";
        break;
    case token::GEQ:
        return ">=";
        break;
    case token::GREATER:
        return ">";
        break;
    default:
        return "";
    }
}

std::string ComparisonExpression::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    std::string oper = opToString(op);
    // TODO: create function for handling operators
    return "(ComparisonExpression:" + s + "left=" + left->toString(shift_size+1) + 
    "," + s + "operator=[" + (oper)+
    "]," + s + "right=" + right->toString(shift_size + 1) + ")";
}

std::string RelationalExpression::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    std::string oper = opToString(op);
    return "(RelationalExpression:" + s + "left=" + left->toString(shift_size+1) + 
    "," + s + "operator=[" + (oper)+
    "]," + s + "right=" + right->toString(shift_size + 1) + ")";
}

std::string MultiplicativeExpression::toString(const int shift_size) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    std::string oper = opToString(op);
    return "(MultiplicativeExpression:" + s + "left=" + left->toString(shift_size+1) + 
    "," + s + "operator=[" + oper +
    "]," + s + "right=" + right->toString(shift_size + 1) + ")";
}

std::string AdditiveExpression::toString(const int shift_size) const  {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    std::string oper = opToString(op);
    return "(AdditiveExpression:" + s + "left=" + left->toString(shift_size+1) + 
    "," + s + "operator=[" + oper +
    "]," + s + "right=" + right->toString(shift_size + 1) + ")";
}

std::string IntegerLiteral::toString(const int shift_size) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(IntegerLiteral:" + s + "value=" + std::to_string(value) + ")";
}

void ComparisonExpression::accept(Visitor& v) {
    v.visit(*this);
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
