#include "../inc/node.hpp"
#include "../inc/visitor.hpp"
#include <sstream>
#include <iomanip>

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
    case token::BANG:
        return "!";
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

std::string Program::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    std::string result = "(Program:" + s;
    for (size_t i = 0; i < statements.size(); ++i) {
        result += statements[i]->toString(shift_size + 1);
        if (i!=statements.size()-1)
            result += "," + s;
    }
    result += ")";
    return result;
}

std::string Parameter::toString(const int shift_size) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(Parameter:" + s + 
    "isMutable=" + (isMutable ? "true" : "false") + "," + s +
    "identifier=" + identifier + ")";
}

std::string DeclarationArguments::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    std::string result = "(DeclarationArguments:";
    if (arguments.size()) result += s;
    for (size_t i = 0; i < arguments.size(); ++i) {
        result += arguments[i]->toString(shift_size + 1);
        if (i != arguments.size()-1)
            result += "," + s;
    }
    result += ")";
    return result;
}

std::string Block::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    std::string result = "(Block:" + s;
    for (size_t i = 0; i < statements.size(); ++i) {
        result += statements[i]->toString(shift_size + 1);
        if (i!=statements.size()-1)
            result += "," + s;
    }
    result += ")";
    return result;
}

// statements
std::string FunctionDeclaration::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(FunctionDeclaration:" + s +
    "identifier=" + identifier + "," + s +
    "arguments=" + arguments->toString(shift_size + 1) + "," + s +
    "statements=" + statements->toString(shift_size + 1) +
    ")";
}

std::string WhileLoopStatement::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(WhileLoopStatement:" + s + 
    "expression=" + expression->toString(shift_size + 1) + "," + s +
    "statements=" + statements->toString(shift_size + 1) +
    ")";
}

std::string IfStatement::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(IfStatement:" + s + 
    "predicate=" + predicate->toString(shift_size + 1) + "," + s +
    "ifBody=" + ifBody->toString(shift_size + 1) + "," + s +
    "elseBody=" + (elseBody.get() ? elseBody->toString(shift_size + 1) : "") +
    ")";
}

std::string VariableDeclaration::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(VariableDeclaration:" + s +
    "isMutable=" + (isMutable ? "true" : "false") + "," +  s +
    "identifier=" + identifier + "," + s +
    "expression=" + expression->toString(shift_size + 1) +
    ")";
}

std::string ReturnStatement::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return
    "(ReturnStatement:" + s +
    "expression=" + expression->toString(shift_size + 1) +
    ")";
}

std::string ExpressionStatement::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return
    "(ExpressionStatement:" + s +
    "expression=" + expression->toString(shift_size + 1) +
    ")";
}
// expression
std::string AssignementExpression::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(AssignementExpression:" + s +
    "identifier=" + identifier + "," + s +
    "expression=" + expression->toString(shift_size + 1) +
    ")";
}

std::string OrExpression::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(OrExpression:" + s +
    "left=" + left->toString(shift_size+1) + "," + s +
    "operator=[||]," + s +
    "right=" + right->toString(shift_size + 1) +
    ")";
}

std::string AndExpression::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(AndExpression:" + s +
    "left=" + left->toString(shift_size+1) + "," + s +
    "operator=[&&]," + s +
    "right=" + right->toString(shift_size + 1) +
    ")";
}

std::string ComparisonExpression::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    std::string oper = opToString(op);
    return "(ComparisonExpression:" + s +
    "left=" + left->toString(shift_size+1) + "," + s +
    "operator=[" + oper + "]," + s +
    "right=" + right->toString(shift_size + 1) +
    ")";
}

std::string RelationalExpression::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    std::string oper = opToString(op);
    return "(RelationalExpression:" + s +
    "left=" + left->toString(shift_size+1) + "," + s +
    "operator=[" + oper + "]," + s +
    "right=" + right->toString(shift_size + 1) +
    ")";
}

std::string MultiplicativeExpression::toString(const int shift_size) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    std::string oper = opToString(op);
    return "(MultiplicativeExpression:" + s +
    "left=" + left->toString(shift_size+1) + "," + s +
    "operator=[" + oper + "]," + s +
    "right=" + right->toString(shift_size + 1) +
    ")";
}

std::string AdditiveExpression::toString(const int shift_size) const  {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    std::string oper = opToString(op);
    return "(AdditiveExpression:" + s +
    "left=" + left->toString(shift_size+1) + "," + s +
    "operator=[" + oper + "]," + s +
    "right=" + right->toString(shift_size + 1) +
    ")";
}

std::string CastExpression::toString(const int shift_size) const  {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(CastExpression:" + s +
    "expression=" + expression->toString(shift_size+1) + "," + s +
    "type=" + type->toString(shift_size+1) +
    ")";
}

std::string PrefixExpression::toString(const int shift_size) const  {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    std::string oper = opToString(op);
    return "(PrefixExpression:" + s +
    "operator=[" + oper + "]," + s +
    "expression=" + expression->toString(shift_size+1) +
    ")";
}

std::string CallExpression::toString(const int shift_size) const  {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(CallExpression:" + s +
    "function=" + function->toString(shift_size+1) + "," + s +
    "arguments=" + arguments->toString(shift_size+1) +
    ")";
}

std::string CallArguments::toString(const int shift_size) const  {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    std::string result = "(CallArguments:";
    if (arguments.size()) result += s;
    for (size_t i = 0; i < arguments.size(); ++i) {
        result += arguments[i]->toString(shift_size + 1);
        if (i!=arguments.size()-1)
            result += "," + s;
    }
    result += ")";
    return result;
}

std::string Identifier::toString(const int shift_size) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(Identifier:" + s +
    "value=" + value +
    ")";
}

std::string ParentExpression::toString(const int shift_size=1) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return
    "(ParentExpression:" + s +
    "expression=" + expression->toString(shift_size + 1) +
    ")";
}

std::string IntegerLiteral::toString(const int shift_size) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(IntegerLiteral:" + s +
    "value=" + std::to_string(value) +
    ")";
}

std::string FloatLiteral::toString(const int shift_size) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    std::ostringstream dblAsStr;
    dblAsStr << std::fixed << std::setprecision(3) << value;
    return "(FloatLiteral:" + s +
    "value=" + dblAsStr.str() +
    ")";
}

std::string BoolLiteral::toString(const int shift_size) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(BoolLiteral:" + s +
    "value=" + (value ? "true" : "false") +
    ")";
}

std::string StringLiteral::toString(const int shift_size) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(StringLiteral:" + s +
    "value=" + value +
    ")";
}

std::string TypeSpecifier::toString(const int shift_size) const {
    std::string s = "\n" + std::string(shift_size*4, ' ');
    return "(TypeSpecifier:" + s +
    "value=" + TypeToString.at(typeName) +
    ")";
}

void Program::accept(Visitor& v) {
    v.visit(*this);
}
void Parameter::accept(Visitor& v) {
    v.visit(*this);
}
void DeclarationArguments::accept(Visitor& v) {
    v.visit(*this);
}
void Block::accept(Visitor& v) {
    v.visit(*this);
}
void FunctionDeclaration::accept(Visitor& v) {
    v.visit(*this);
}
void VariableDeclaration::accept(Visitor& v) {
    v.visit(*this);
}
void WhileLoopStatement::accept(Visitor& v) {
    v.visit(*this);
}
void IfStatement::accept(Visitor& v) {
    v.visit(*this);
}
void ReturnStatement::accept(Visitor& v) {
    v.visit(*this);
}
void ExpressionStatement::accept(Visitor& v) {
    v.visit(*this);
}
void AssignementExpression::accept(Visitor& v) {
    v.visit(*this);
}
void OrExpression::accept(Visitor& v) {
    v.visit(*this);
}
void AndExpression::accept(Visitor& v) {
    v.visit(*this);
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
void CastExpression::accept(Visitor& v) {
    v.visit(*this);
}
void PrefixExpression::accept(Visitor& v) {
    v.visit(*this);
}
void CallExpression::accept(Visitor& v) {
    v.visit(*this);
}
void CallArguments::accept(Visitor& v) {
    v.visit(*this);
}
void Identifier::accept(Visitor& v) {
    v.visit(*this);
}
void ParentExpression::accept(Visitor& v) {
    v.visit(*this);
}
void IntegerLiteral::accept(Visitor& v) {
    v.visit(*this);
}
void FloatLiteral::accept(Visitor& v) {
    v.visit(*this);
}
void BoolLiteral::accept(Visitor& v) {
    v.visit(*this);
}
void StringLiteral::accept(Visitor& v) {
    v.visit(*this);
}
void TypeSpecifier::accept(Visitor& v) {
    v.visit(*this);
}
