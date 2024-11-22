#pragma once
#include <string>
#include <memory>

#include "token.hpp"
#include "visitor.hpp"


std::string opToString(const token::TokenType& tok);

struct Node {
    virtual ~Node() = default;
    virtual std::string toString(const int shift_size) const = 0;
    virtual void accept(Visitor& v) = 0;
};

struct VariableDeclaration: public Node {
    bool isMutable;
    std::string identifier;
    std::unique_ptr<Node> expression;
    
    VariableDeclaration(bool isMut, std::string i, std::unique_ptr<Node> e):
        Node(), isMutable(isMut), identifier(i), expression(std::move(e)) {};

    std::string toString(const int shift_size) const override;
    void accept(Visitor& v) override;
};

struct AssignementExpression: public Node {
    std::string identifier;
    std::unique_ptr<Node> expression;
    
    AssignementExpression(std::string i, std::unique_ptr<Node> e):
        Node(), identifier(i), expression(std::move(e)) {};

    std::string toString(const int shift_size) const override;
    void accept(Visitor& v) override;
};

struct OrExpression: public Node {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    token::TokenType op;
    OrExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r, token::TokenType t):
        Node(), left(std::move(l)), right(std::move(r)), op(t) {};
    std::string toString(const int shift_size) const override;
    void accept(Visitor& v) override;
};

struct AndExpression: public Node {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    token::TokenType op;
    AndExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r, token::TokenType t):
        Node(), left(std::move(l)), right(std::move(r)), op(t) {};
    std::string toString(const int shift_size) const override;
    void accept(Visitor& v) override;
};

struct ComparisonExpression: public Node {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    token::TokenType op;
    ComparisonExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r, token::TokenType t):
        Node(), left(std::move(l)), right(std::move(r)), op(t) {};
    std::string toString(const int shift_size) const override;
    void accept(Visitor& v) override;
};

struct RelationalExpression: public Node {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    token::TokenType op;
    RelationalExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r, token::TokenType t):
        Node(), left(std::move(l)), right(std::move(r)), op(t) {};
    std::string toString(const int shift_size) const override;
    void accept(Visitor& v) override;
};

struct AdditiveExpression: public Node {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    token::TokenType op;
    AdditiveExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r, token::TokenType t):
        Node(), left(std::move(l)), right(std::move(r)), op(t) {};
    std::string toString(const int shift_size) const override;
    void accept(Visitor& v) override;
};

struct MultiplicativeExpression: public Node {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    token::TokenType op;
    MultiplicativeExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r, token::TokenType t):
        Node(), left(std::move(l)), right(std::move(r)), op(t) {};
    std::string toString(const int shift_size) const override;
    void accept(Visitor& v) override;
};

struct CastExpression: public Node {
    std::unique_ptr<Node> expression;
    std::unique_ptr<Node> type;
    CastExpression(std::unique_ptr<Node> e, std::unique_ptr<Node> t):
        Node(), expression(std::move(e)), type(std::move(t)) {};
    std::string toString(const int shift_size) const override;
    void accept(Visitor& v) override;
};

struct PrefixExpression: public Node {
    token::TokenType op;
    std::unique_ptr<Node> expression;
    PrefixExpression(token::TokenType o, std::unique_ptr<Node> e):
        Node(), op(o), expression(std::move(e)){};
    std::string toString(const int shift_size) const override;
    void accept(Visitor& v) override;
};

struct ExpressionStatement: public Node {
    std::unique_ptr<Node> expression;
    ExpressionStatement(std::unique_ptr<Node> e):
        Node(), expression(std::move(e)){};
    std::string toString(const int shift_size) const override;
    void accept(Visitor& v) override;
};

enum TypeName : int {
    ERROR = 0,
    STR,
    INT,
    FLT,
    BOL,
};

const std::map<token::TokenType, TypeName> TokenToType {
    { token::TokenType::INT, TypeName::INT },
    { token::TokenType::STR, TypeName::STR },
    { token::TokenType::FLT, TypeName::FLT },
    { token::TokenType::BOL, TypeName::BOL }
};

const std::map<TypeName, std::string> TypeToString {
    { TypeName::INT, "int" },
    { TypeName::STR, "str" },
    { TypeName::FLT, "flt" },
    { TypeName::BOL, "bol" }
};

 struct TypeSpecifier: public Node {
    TypeName typeName;
    TypeSpecifier(const TypeName& type): Node(), typeName(type) {};
    TypeSpecifier(const token::TokenType& token): Node(), typeName(TokenToType.at(token)) {};
    std::string toString(const int shift_size) const override;
    void accept(Visitor& v) override;
};

 struct IntegerLiteral: public Node {
    int value;
    IntegerLiteral(int val): Node(), value(val) {};
    std::string toString(const int shift_size) const override;
    void accept(Visitor& v) override;
};
