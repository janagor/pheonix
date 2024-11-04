#include "../inc/parser.hpp"
#include <string>
#include <cassert>
#include <cstdlib>

namespace parser {

std::unique_ptr<Node> Parser::generateParsingTree() {
    std::optional<std::unique_ptr<Node>> maybeResult;
    maybeResult = parse();
    if (maybeResult) return std::move(*maybeResult);
    return nullptr; // add sensible handling of empty
}

std::string MultiplicativeExpression::toString() const  {
    return "(AdditiveExpression: left=" + left->toString() + 
    ", operator=[" + (op==token::STAR ? "*" : "/" )+
    "], right=" + right->toString() + ")";
}
std::string IntegerLiteral::toString() const {
    return "(IntegerLiteral: value=" + std::to_string(value) + ")";
}
std::string AdditiveExpression::toString() const  {
    return "(AdditiveExpression: left=" + left->toString() + 
    ", operator=[" + (op==token::PLUS ? "+" : "-" ) +
    "], right=" + right->toString() + ")";

}

std::unique_ptr<Node> Parser::parseIntegerLiteral() {
    int val = std::get<int>(*current.token.value);
    readLex();
    return std::make_unique<IntegerLiteral>(val);
}

std::unique_ptr<Node> Parser::parseMultiplicativeExpression() {
    auto left = parseIntegerLiteral();
    while (current.token.tokenType == token::STAR || current.token.tokenType == token::SLASH) {
        token::TokenType op = current.token.tokenType;
        readLex();
        auto right = parseIntegerLiteral();
        return std::move(std::make_unique<MultiplicativeExpression>(std::move(left), std::move(right), op));
    }
        return left;
}

std::unique_ptr<Node> Parser::parseAdditiveExpression() {
    auto left = parseMultiplicativeExpression();
    while (current.token.tokenType == token::PLUS || current.token.tokenType == token::MINUS) {
        token::TokenType op = current.token.tokenType;
        readLex();
        auto right = parseMultiplicativeExpression();
        return std::move(std::make_unique<AdditiveExpression>(std::move(left), std::move(right), op));
    }
    return left;
} 

void Parser::readLex() {
    if (current.token.tokenType == token::END_OF_FILE) {
        return;
    }
    current = lexer.nextLexem();
}

std::optional<std::unique_ptr<Node>> Parser::parse() {
    std::unique_ptr<Node> root;
    switch (current.token.tokenType) {
    case token::INTEGER:
        return std::move(parseAdditiveExpression());
        break;
    default:
        return std::nullopt;
    }
    return std::nullopt;
}
} // parser
