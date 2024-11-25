#include "../inc/parser.hpp"
#include <string>
#include <cassert>
#include <cstdlib>
using namespace parser;

std::unique_ptr<Node> Parser::parseProgram() {
    auto program = std::make_unique<Program>();
    while (current.token.tokenType != token::END_OF_FILE) {
        program->statements.push_back(std::move(parseStatement()));
    }
    return program;
}

std::unique_ptr<Node> Parser::parseStatement() {
    std::unique_ptr<Node> node;
    switch (current.token.tokenType) {
    case token::LET:
        return parseVariableDeclaration();
    case token::WHILE:
        return parseWhileLoopStatement();
    case token::FN:
        return parseFunctionDeclaration();
    case token::RETURN:
        return parseReturnStatement();
    default:
        return parseExpressionStatement();
    }
}

std::unique_ptr<Node> Parser::parseFunctionDeclaration() {
    assert(current.token.tokenType==token::TokenType::FN);
    readLex();
    assert(current.token.tokenType==token::TokenType::IDENTIFIER);
    std::string identifier = std::get<std::string>(*current.token.value);
    auto functionDeclaration = std::make_unique<FunctionDeclaration>(identifier);
    readLex();
    assert(current.token.tokenType==token::TokenType::LPARENT);
    readLex();
    while (
        current.token.tokenType==token::TokenType::MUT
        || current.token.tokenType==token::TokenType::IDENTIFIER
    ) {
        bool isMutable = false;
        if (current.token.tokenType==token::TokenType::MUT) {
            isMutable = true;
            readLex();
        }
        std::string param_identifier =
            std::get<std::string>(*current.token.value);
        functionDeclaration->push_parameter(isMutable, param_identifier);
        if (current.token.tokenType==token::TokenType::COMMA)
            readLex();
        readLex();
    }
    assert(current.token.tokenType==token::TokenType::RPARENT);
    readLex();
    assert(current.token.tokenType==token::TokenType::LBRACE);
    readLex();
    while ( current.token.tokenType != token::RBRACE) {
        auto statement = parseStatement();
        functionDeclaration->statements.push_back(std::move(statement));
    }
    assert(current.token.tokenType==token::TokenType::RBRACE);
    readLex();
    return functionDeclaration;
}

std::unique_ptr<Node> Parser::parseVariableDeclaration() {
    assert(current.token.tokenType==token::TokenType::LET);
    readLex();
    bool isMutable = false;
    if ( current.token.tokenType == token::MUT) {
        isMutable = true;
        readLex();
    }
    assert(current.token.tokenType==token::TokenType::IDENTIFIER);
    std::string identifier = std::get<std::string>(*current.token.value);
    readLex();
    assert(current.token.tokenType==token::TokenType::ASSIGN);
    readLex();
    auto expression = parseExpressionStatement();
    return std::make_unique<VariableDeclaration>(isMutable, identifier, std::move(expression));
}

std::unique_ptr<Node> Parser::parseWhileLoopStatement() {
    assert(current.token.tokenType==token::TokenType::WHILE);
    readLex();
    assert(current.token.tokenType==token::TokenType::LPARENT);
    readLex();
    auto expression = parseExpression();
    auto whileLoopStmt = std::make_unique<WhileLoopStatement>(std::move(expression));
    assert(current.token.tokenType==token::TokenType::RPARENT);
    readLex();
    assert(current.token.tokenType==token::TokenType::LBRACE);
    readLex();
    while ( current.token.tokenType != token::RBRACE) {
        auto statement = parseStatement();
        whileLoopStmt->statements.push_back(std::move(statement));
    }
    assert(current.token.tokenType==token::TokenType::RBRACE);
    readLex();
    return whileLoopStmt;
}

std::unique_ptr<Node> Parser::parseReturnStatement() {
    assert(current.token.tokenType==token::TokenType::RETURN);
    readLex();
    auto expression = parseExpressionStatement();
    return std::make_unique<ReturnStatement>(std::move(expression));
}

std::unique_ptr<Node> Parser::parseExpressionStatement() {
    auto expression = parseExpression();
    if ( current.token.tokenType == token::SEMICOLON) {
        readLex();
        return std::make_unique<ExpressionStatement>(std::move(expression));
    }
        return expression;

}

std::unique_ptr<Node> Parser::parseExpression() {
    if ( current.token.tokenType == token::DOLAR) {
        return parseAssignementExpression();
    }
    return parseOrExpression();
}

std::unique_ptr<Node> Parser::parseAssignementExpression() {
    readLex();
    assert(current.token.tokenType==token::TokenType::IDENTIFIER);
    std::string identifier = std::get<std::string>(*current.token.value);
    readLex();
    readLex();
    assert(current.token.tokenType==token::TokenType::INTEGER);
    auto expression = parseOrExpression();
    return std::make_unique<AssignementExpression>(
        identifier, std::move(expression)
    );
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
    std::unique_ptr<Node> expression = parsePrefixExpression();
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

std::unique_ptr<Node> Parser::parsePrefixExpression() {
    std::unique_ptr<Node> expression;
    if (
        current.token.tokenType == token::BANG ||
        current.token.tokenType == token::MINUS
    ) {
        token::TokenType op = current.token.tokenType;
        readLex();
        expression = parseIntegerLiteral();
        return std::make_unique<PrefixExpression>(
            op, std::move(expression)
        );
    }
    expression = parseIntegerLiteral();
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
    int val =std::get<int>(*current.token.value);
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
    return std::move(parseProgram());
}

void Parser::readLex() {
    if (current.token.tokenType == token::END_OF_FILE) {
        return;
    }
    current = lexer.nextLexem();
}
