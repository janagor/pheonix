#pragma once
#include "lexer.hpp"
#include <typeinfo>

namespace parser {
struct Node {
    Node() {};
    virtual ~Node(){};

};

struct Program: public Node {
    std::vector<Node*> features;
    Program(): Node(), features() {};
};

struct Statement: virtual Node {
    Statement(): Node() {};
};

struct Expression: virtual Node {
    Expression* expression;
    Expression(): Node() {};
};

struct Literal: Node {
    Literal* literal;
    Literal(): Node() {};
};
//  struct BoolLiteral: virtual Node {
//     bool value;
//     BoolLiteral(): Node() {};
//
// };
//  struct FloatLiteral: virtual Node {
//     double value;
//     FloatLiteral(): Node() {};
//
// };
 struct IntegerLiteral: virtual Node {
    int value;
    IntegerLiteral(int val): Node(), value(val) {};

};
//  struct StringLiteral: virtual Node {
//     std::string value;
//     StringLiteral(): Node() {};
//
// };

struct ArithmeticExpression: Node {
    
};


struct Parser {
public:
    Parser(std::istream& istream) : lexer(istream), istream_(istream) {
        current = lexer.nextLexem();
        peek = lexer.nextLexem();
    }
    
    Node* generateParsingTree();
    Node* parseIntegerLiteral();

    std::optional<Node*> parse();
private:
    lexer::Lexer lexer;
    std::istream& istream_;
    lexer::Lexem current;
    lexer::Lexem peek;
};

} // namespace parser
