#pragma once
#include <string>
#include <memory>

#include "token.hpp"
#include "visitor.hpp"

struct Node {
    virtual ~Node() = default;
    virtual std::string toString(const int shift_size) const = 0;
    virtual void accept(Visitor& v) = 0;
};

 struct IntegerLiteral: public Node {
    int value;
    IntegerLiteral(int val): Node(), value(val) {};
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

