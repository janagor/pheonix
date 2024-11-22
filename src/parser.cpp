#include "../inc/parser.hpp"
#include <string>
#include <cassert>
#include <cstdlib>

namespace parser {

std::unique_ptr<Node> Parser::parseExpression() {
    return parseOrExpression();
}

std::unique_ptr<Node> Parser::parseOrExpression() {
    auto left = parseAndExpression();
    while (
        current.token.tokenType == token::OR
    ) {
        token::TokenType op = current.token.tokenType;
        readLex();
        auto right = parseAndExpression();
        return std::make_unique<OrExpression>(
            std::move(left), std::move(right), op
        );
    }
    return left;
}
std::unique_ptr<Node> Parser::parseAndExpression() {
    auto left = parseComparisonExpression();
    while (
        current.token.tokenType == token::AND
    ) {
        token::TokenType op = current.token.tokenType;
        readLex();
        auto right = parseComparisonExpression();
        return std::make_unique<AndExpression>(
            std::move(left), std::move(right), op
        );
    }
    return left;
}
std::unique_ptr<Node> Parser::parseComparisonExpression() {
    auto left = parseRelationalExpression();
    while (
        current.token.tokenType == token::EQUALS
        || current.token.tokenType == token::NEQ
    ) {
        token::TokenType op = current.token.tokenType;
        readLex();
        auto right = parseRelationalExpression();
        return std::make_unique<ComparisonExpression>(
            std::move(left), std::move(right), op
        );
    }
    return left;
}

std::unique_ptr<Node> Parser::parseRelationalExpression() {
    auto left = parseAdditiveExpression();
    while (
        current.token.tokenType == token::LESS
        || current.token.tokenType == token::GREATER
        || current.token.tokenType == token::LEQ
        || current.token.tokenType == token::GEQ
    ) {
        token::TokenType op = current.token.tokenType;
        readLex();
        auto right = parseAdditiveExpression();
        return std::make_unique<RelationalExpression>(
            std::move(left), std::move(right), op
        );
    }
    return left;

}


std::unique_ptr<Node> Parser::parseAdditiveExpression() {
    auto left = parseMultiplicativeExpression();
    while (
        current.token.tokenType == token::PLUS
        || current.token.tokenType == token::MINUS
    ) {
        token::TokenType op = current.token.tokenType;
        readLex();
        auto right = parseMultiplicativeExpression();
        return std::make_unique<AdditiveExpression>(
            std::move(left), std::move(right), op
        );
    }
    return left;
} 

std::unique_ptr<Node> Parser::parseMultiplicativeExpression() {
    std::unique_ptr<Node> left = parseCastExpression();
    while (
        current.token.tokenType == token::STAR
        || current.token.tokenType == token::SLASH
        || current.token.tokenType == token::PERCENT
    ) {
        token::TokenType op = current.token.tokenType;
        readLex();
        std::unique_ptr<Node> right = parseCastExpression();
        return std::make_unique<MultiplicativeExpression>(
            std::move(left), std::move(right), op
        );
    }
    return left;
}

std::unique_ptr<Node> Parser::parseCastExpression() {
    std::unique_ptr<Node> expression = parseIntegerLiteral();
    while (
        current.token.tokenType == token::LARROW
    ) {
        readLex();
        std::unique_ptr<Node> type = parseTypeSpecifier();
        return std::make_unique<CastExpression>(
            std::move(expression), std::move(type)
        );
    }
    return expression;
}

std::unique_ptr<Node> Parser::parseTypeSpecifier() {
    // TypeName::iterator type = TokenToType.at(current.token.tokenType);
    auto type = TokenToType.find(current.token.tokenType);
    if(type != TokenToType.end()) {
        readLex();
        return std::make_unique<TypeSpecifier>(type->second);
    }
    // TODO: ERROR HANDLING
    return std::make_unique<TypeSpecifier>(token::TokenType::STR);
}

std::unique_ptr<Node> Parser::parseIntegerLiteral() {
    int val = static_cast<int>(std::get<int>(*current.token.value));
    readLex();
    return std::make_unique<IntegerLiteral>(val);
}

std::unique_ptr<Node> Parser::generateParsingTree() {
    std::optional<std::unique_ptr<Node>> maybeResult;
    maybeResult = parse();
    if (maybeResult) return std::move(*maybeResult);
    return nullptr; // add sensible handling of empty
}

std::optional<std::unique_ptr<Node>> Parser::parse() {
    // std::unique_ptr<Node> root;
    // switch (current.token.tokenType) {
    //     case token::INTEGER:
    return std::move(parseExpression());
    //         break;
    //     default:
    //         return std::nullopt;
    // }
    // return std::nullopt;
}

void Parser::readLex() {
    if (current.token.tokenType == token::END_OF_FILE) {
        return;
    }
    current = lexer.nextLexem();
}

} // parser
