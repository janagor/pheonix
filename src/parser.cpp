#include "../inc/parser.hpp"
#include <string>
#include <stdexcept>
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

std::unique_ptr<Node> Parser::parseBlock() {
    auto block = std::make_unique<Block>();
    assert(current.token.tokenType==token::TokenType::LBRACE);
    readLex();
    while (current.token.tokenType != token::RBRACE) {
        auto statement = parseStatement();
        block->statements.push_back(std::move(statement));
    }
    assert(current.token.tokenType==token::TokenType::RBRACE);
    readLex();
    
    return block;
}

std::unique_ptr<Node> Parser::parseStatement() {
    std::unique_ptr<Node> node;
    switch (current.token.tokenType) {
    case token::LET:
        return parseVariableDeclaration();
    case token::WHILE:
        return parseWhileLoopStatement();
    case token::IF:
        return parseIfStatement();
    case token::FN:
        return parseFunctionDeclaration();
    case token::RETURN:
        return parseReturnStatement();
    default:
        return parseExpressionStatement();
    }
}

std::unique_ptr<Node> Parser::parseDeclarationArguments() {
    auto args = std::make_unique<DeclarationArguments>();
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
        std::string paramIdentifier =
            std::get<std::string>(*current.token.value);
        auto arg = std::make_unique<Parameter>(isMutable, paramIdentifier);
        args->arguments.push_back(std::move(arg));
        if (current.token.tokenType==token::TokenType::COMMA)
            readLex();
        readLex();
    }
    assert(current.token.tokenType==token::TokenType::RPARENT);
    readLex();
    return args;
}


std::unique_ptr<Node> Parser::parseFunctionDeclaration() {
    assert(current.token.tokenType==token::TokenType::FN);
    readLex();
    assert(current.token.tokenType==token::TokenType::IDENTIFIER);
    std::string identifier = std::get<std::string>(*current.token.value);
    auto functionDeclaration = std::make_unique<FunctionDeclaration>(identifier);
    readLex();
    assert(current.token.tokenType==token::TokenType::LPARENT);
    auto declarationArguments = parseDeclarationArguments();
    functionDeclaration->arguments = std::move(declarationArguments);
    assert(current.token.tokenType==token::TokenType::LBRACE);
    auto block = parseBlock();
    functionDeclaration->statements = std::move(block);
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
    return std::make_unique<VariableDeclaration>(
        isMutable, identifier, std::move(expression)
    );
}

std::unique_ptr<Node> Parser::parseWhileLoopStatement() {
    assert(current.token.tokenType==token::TokenType::WHILE);
    readLex();
    assert(current.token.tokenType==token::TokenType::LPARENT);
    readLex();
    auto expression = parseExpression();

    assert(current.token.tokenType==token::TokenType::RPARENT);
    readLex();
    auto block = parseBlock();

    auto whileLoopStmt = std::make_unique<WhileLoopStatement>(
        std::move(expression),
        std::move(block)
    );
    return whileLoopStmt;
}

std::unique_ptr<Node> Parser::parseIfStatement() {

    assert(current.token.tokenType==token::TokenType::IF);
    readLex();
    assert(current.token.tokenType==token::TokenType::LPARENT);
    readLex();
    auto predicate = parseExpression();
    auto ifStmt = std::make_unique<IfStatement>(std::move(predicate));
    assert(current.token.tokenType==token::TokenType::RPARENT);
    readLex();
    auto ifBody = parseBlock();
    ifStmt->ifBody = std::move(ifBody);
    if (current.token.tokenType == token::TokenType::ELSE) {
        readLex();
        if (current.token.tokenType == token::TokenType::IF) {
            auto elseIfStmt = parseIfStatement();
            ifStmt->elseBody = std::move(elseIfStmt);
        } else {
            assert(current.token.tokenType==token::TokenType::LBRACE);
            auto elseBody = parseBlock();
            ifStmt->elseBody = std::move(elseBody);
        }
    }
    return ifStmt;
}

std::unique_ptr<Node> Parser::parseReturnStatement() {
    assert(current.token.tokenType==token::TokenType::RETURN);
    readLex();
    auto expression = parseExpressionStatement();
    return std::make_unique<ReturnStatement>(std::move(expression));
}

std::unique_ptr<Node> Parser::parseExpressionStatement() {
    auto expression = parseExpression();
    assert(current.token.tokenType==token::TokenType::SEMICOLON);
    readLex();
    return std::make_unique<ExpressionStatement>(std::move(expression));
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
        left = std::make_unique<OrExpression>(
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
        left = std::make_unique<AndExpression>(
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
        left = std::make_unique<ComparisonExpression>(
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
        left = std::make_unique<RelationalExpression>(
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
        left = std::make_unique<AdditiveExpression>(
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
        left = std::make_unique<MultiplicativeExpression>(
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
        if (current.token.tokenType == token::IDENTIFIER)
            expression = parseIdentifierLike();
        else
            expression = parseLiteral();
        return std::make_unique<PrefixExpression>(
            op, std::move(expression)
        );
    }
    if (current.token.tokenType == token::IDENTIFIER)
        expression = parseIdentifierLike();
    else if (current.token.tokenType == token::LPARENT)
        expression = parseParentExpression();
    else
        expression = parseLiteral();
    return expression;
}

std::unique_ptr<Node> Parser::parseIdentifierLike() {
    std::string val = std::get<std::string>(*current.token.value);
    readLex();
    auto ident = std::make_unique<Identifier>(val);
    if (current.token.tokenType==token::TokenType::LPARENT)
        return parseCallExpression(std::move(ident));
    return ident;
}

std::unique_ptr<Node> Parser::parseCallArguments() {
    auto arguments = std::make_unique<CallArguments>();
    assert(current.token.tokenType==token::TokenType::LPARENT);
    readLex();
    while (current.token.tokenType != token::TokenType::RPARENT){
        arguments->arguments.push_back(std::move(parseExpression()));
        if (current.token.tokenType != token::TokenType::COMMA) break;
        readLex();
    }
    assert(current.token.tokenType==token::TokenType::RPARENT);
    readLex();
    return arguments;
}

std::unique_ptr<Node> Parser::parseParentExpression() {
    assert(current.token.tokenType==token::TokenType::LPARENT);
    readLex();
    auto pExpression = std::make_unique<ParentExpression>(parseExpression());
    assert(current.token.tokenType==token::TokenType::RPARENT);
    readLex();
    if (current.token.tokenType==token::TokenType::LPARENT)
        return parseCallExpression(std::move(pExpression));
    return pExpression;
}

std::unique_ptr<Node> Parser::parseCallExpression(std::unique_ptr<Node> function) {
    std::unique_ptr<Node> result = std::move(function);
    assert(current.token.tokenType==token::TokenType::LPARENT);
    do {
        std::unique_ptr<Node> callArguments = parseCallArguments();
        result = std::make_unique<CallExpression>(
            std::move(result), std::move(callArguments)
        );
    } while (current.token.tokenType == token::LPARENT);
    return result;
}

std::unique_ptr<Node> Parser::parseLiteral() {
    if (current.token.tokenType == token::INTEGER) {
        return parseIntegerLiteral();
    }
    else if (current.token.tokenType == token::FLOAT) {
    return parseFloatLiteral();
    }
    else if (current.token.tokenType == token::STRING) {
        return parseStringLiteral();
    }
    else if (
        current.token.tokenType == token::TRUE ||
        current.token.tokenType == token::FALSE

    ) {
        return parseBoolLiteral();
    }
    throw std::runtime_error("Literal type does not exist.");
}

std::unique_ptr<Node> Parser::parseIntegerLiteral() {
    int val =std::get<int>(*current.token.value);
    readLex();
    return std::make_unique<IntegerLiteral>(val);
}

std::unique_ptr<Node> Parser::parseFloatLiteral() {
    double val =std::get<double>(*current.token.value);
    readLex();
    return std::make_unique<FloatLiteral>(val);
}

std::unique_ptr<Node> Parser::parseBoolLiteral() {
    bool val = current.token.tokenType == token::TRUE;
    readLex();
    return std::make_unique<BoolLiteral>(val);
}

std::unique_ptr<Node> Parser::parseStringLiteral() {
    std::string val = std::get<std::string>(*current.token.value);
    readLex();
    return std::make_unique<StringLiteral>(val);
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
