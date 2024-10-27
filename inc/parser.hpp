#pragma once
#include "lexer.hpp"
#include <typeinfo>

namespace parser {
struct Node {
    Node() {};
    virtual ~Node(){};

};

// struct Program: public Node {
//     std::vector<Node*> features;
//     Program(): Node(), features() {};
// };
//
// struct Statement: virtual Node {
//     Statement(): Node() {};
// };
//
// struct Expression: virtual Node {
//     Expression* expression;
//     Expression(): Node() {};
// };
//
// struct Literal: Node {
//     Literal* literal;
//     Literal(): Node() {};
// };

 struct IntegerLiteral: virtual Node {
    int value;
    IntegerLiteral(int val): Node(), value(val) {};

};

struct AdditiveExpression: virtual Node {
    int left;
    int right;
    token::TokenType op;
    AdditiveExpression(int l, int r, token::TokenType t): Node(), left(l), right(r), op(t) {};
};

struct MultiplicativeExpression: virtual Node {
    int left;
    int right;
    token::TokenType op;
    MultiplicativeExpression(int l, int r, token::TokenType t): Node(), left(l), right(r), op(t) {};
};

//  struct StringLiteral: virtual Node {
//     std::string value;
//     StringLiteral(): Node() {};
//
// };

// struct ArithmeticExpression: Node {
//     
// };
//

struct Parser {
private:
    Node* parseIntegerLiteral();
Node* parseMultiplicativeExpression();
    Node* parseAdditiveExpression();
    void readLex();

public:
    Parser(std::istream& istream) : lexer(istream) {
        current = lexer.nextLexem();
        peek = lexer.nextLexem();
    }
    Node* generateParsingTree();
    std::optional<Node*> parse();

private:
    lexer::Lexer lexer;
    lexer::Lexem current;
    lexer::Lexem peek;
};

} // namespace parser
