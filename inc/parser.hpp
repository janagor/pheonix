#pragma once
#include "lexer.hpp"
#include <typeinfo>
#include <memory>

namespace parser {
struct Node {
    virtual ~Node() = default;
    virtual std::string toString() const = 0;
};

 struct IntegerLiteral: public Node {
    int value;
    IntegerLiteral(int val): Node(), value(val) {};
    virtual std::string toString() const override;
};

struct AdditiveExpression: public Node {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    token::TokenType op;
    AdditiveExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r, token::TokenType t):
        Node(), left(std::move(l)), right(std::move(r)), op(t) {};
    std::string toString() const override;
};

struct MultiplicativeExpression: public Node {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    token::TokenType op;
    MultiplicativeExpression(std::unique_ptr<Node> l, std::unique_ptr<Node> r, token::TokenType t):
        Node(), left(std::move(l)), right(std::move(r)), op(t) {};
    std::string toString() const override;
};

struct Parser {
private:
    std::unique_ptr<Node> parseIntegerLiteral();
std::unique_ptr<Node> parseMultiplicativeExpression();
    std::unique_ptr<Node> parseAdditiveExpression();
    void readLex();

public:
    Parser(std::istream& istream) : lexer(istream) {
        current = lexer.nextLexem();
    }
    std::unique_ptr<Node> generateParsingTree();
    std::optional<std::unique_ptr<Node>> parse();

private:
    lexer::Lexer lexer;
    lexer::Lexem current;
    // lexer::Lexem peek;
};

} // namespace parser
