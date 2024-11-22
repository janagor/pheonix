#pragma once

#include <typeinfo>
#include <memory>

#include "node.hpp"
#include "lexer.hpp"
#include "visitor.hpp"

namespace parser {

struct Parser {
private:
    std::unique_ptr<Node> parseProgram();
    std::unique_ptr<Node> parseStatement();
    std::unique_ptr<Node> parseExpressionStatement();
    std::unique_ptr<Node> parseExpression();
    std::unique_ptr<Node> parseAssignementExpression();
    std::unique_ptr<Node> parseOrExpression();
    std::unique_ptr<Node> parseAndExpression();
    std::unique_ptr<Node> parseComparisonExpression();
    std::unique_ptr<Node> parseRelationalExpression();
    std::unique_ptr<Node> parseAdditiveExpression();
    std::unique_ptr<Node> parseMultiplicativeExpression();
    std::unique_ptr<Node> parseCastExpression();
    std::unique_ptr<Node> parsePrefixExpression();
    std::unique_ptr<Node> parseTypeSpecifier();
    std::unique_ptr<Node> parseIntegerLiteral();
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
};

} // namespace parser
