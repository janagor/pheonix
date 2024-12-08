#include "../inc/parser.hpp"
#include <string>
#include <stdexcept>
#include <cassert>
#include <cstdlib>
using namespace parser;

std::unique_ptr<Node> Parser::parseProgram() {
    auto program = std::make_unique<Program>();
    while (current.token.getTokenType() != token::END_OF_FILE) {
        program->statements.push_back(std::move(parseStatement()));
    }
    return program;
}

std::unique_ptr<Node> Parser::parseBlock() {
    auto block = std::make_unique<Block>();
    assert(current.token.getTokenType()==token::TokenType::LBRACE);
    readLex();
    while (current.token.getTokenType() != token::RBRACE) {
        auto statement = parseStatement();
        block->statements.push_back(std::move(statement));
    }
    assert(current.token.getTokenType()==token::TokenType::RBRACE);
    readLex();
    
    return block;
}

std::unique_ptr<Node> Parser::parseStatement() {
    std::unique_ptr<Node> node;
    switch (current.token.getTokenType()) {
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
    assert(current.token.getTokenType()==token::TokenType::LPARENT);
    readLex();
    while (
        current.token.getTokenType()==token::TokenType::MUT
        || current.token.getTokenType()==token::TokenType::IDENTIFIER
    ) {
        bool isMutable = false;
        if (current.token.getTokenType()==token::TokenType::MUT) {
            isMutable = true;
            readLex();
        }
        std::string paramIdentifier =
            std::get<std::string>(*current.token.getValue());
        auto arg = std::make_unique<Parameter>(isMutable, paramIdentifier);
        args->arguments.push_back(std::move(arg));
        if (current.token.getTokenType()==token::TokenType::COMMA)
            readLex();
        readLex();
    }
    assert(current.token.getTokenType()==token::TokenType::RPARENT);
    readLex();
    return args;
}


std::unique_ptr<Node> Parser::parseFunctionDeclaration() {
    assert(current.token.getTokenType()==token::TokenType::FN);
    readLex();
    assert(current.token.getTokenType()==token::TokenType::IDENTIFIER);
    std::string identifier = std::get<std::string>(*current.token.getValue());
    auto functionDeclaration = std::make_unique<FunctionDeclaration>(identifier);
    readLex();
    assert(current.token.getTokenType()==token::TokenType::LPARENT);
    auto declarationArguments = parseDeclarationArguments();
    functionDeclaration->arguments = std::move(declarationArguments);
    assert(current.token.getTokenType()==token::TokenType::LBRACE);
    auto block = parseBlock();
    functionDeclaration->statements = std::move(block);
    return functionDeclaration;
}

std::unique_ptr<Node> Parser::parseVariableDeclaration() {
    assert(current.token.getTokenType()==token::TokenType::LET);
    readLex();
    bool isMutable = false;
    if ( current.token.getTokenType() == token::MUT) {
        isMutable = true;
        readLex();
    }
    assert(current.token.getTokenType()==token::TokenType::IDENTIFIER);
    std::string identifier = std::get<std::string>(*current.token.getValue());
    readLex();
    assert(current.token.getTokenType()==token::TokenType::ASSIGN);
    readLex();
    auto expression = parseExpressionStatement();
    return std::make_unique<VariableDeclaration>(
        isMutable, identifier, std::move(expression)
    );
}

std::unique_ptr<Node> Parser::parseWhileLoopStatement() {
    assert(current.token.getTokenType()==token::TokenType::WHILE);
    readLex();
    assert(current.token.getTokenType()==token::TokenType::LPARENT);
    readLex();
    auto expression = parseExpression();

    assert(current.token.getTokenType()==token::TokenType::RPARENT);
    readLex();
    auto block = parseBlock();

    auto whileLoopStmt = std::make_unique<WhileLoopStatement>(
        std::move(expression),
        std::move(block)
    );
    return whileLoopStmt;
}

std::unique_ptr<Node> Parser::parseIfStatement() {

    assert(current.token.getTokenType()==token::TokenType::IF);
    readLex();
    assert(current.token.getTokenType()==token::TokenType::LPARENT);
    readLex();
    auto predicate = parseExpression();
    auto ifStmt = std::make_unique<IfStatement>(std::move(predicate));
    assert(current.token.getTokenType()==token::TokenType::RPARENT);
    readLex();
    auto ifBody = parseBlock();
    ifStmt->ifBody = std::move(ifBody);
    if (current.token.getTokenType() == token::TokenType::ELSE) {
        readLex();
        if (current.token.getTokenType() == token::TokenType::IF) {
            auto elseIfStmt = parseIfStatement();
            ifStmt->elseBody = std::move(elseIfStmt);
        } else {
            assert(current.token.getTokenType()==token::TokenType::LBRACE);
            auto elseBody = parseBlock();
            ifStmt->elseBody = std::move(elseBody);
        }
    }
    return ifStmt;
}

std::unique_ptr<Node> Parser::parseReturnStatement() {
    assert(current.token.getTokenType()==token::TokenType::RETURN);
    readLex();
    auto expression = parseExpressionStatement();
    return std::make_unique<ReturnStatement>(std::move(expression));
}

std::unique_ptr<Node> Parser::parseExpressionStatement() {
    auto expression = parseExpression();
    assert(current.token.getTokenType()==token::TokenType::SEMICOLON);
    readLex();
    return std::make_unique<ExpressionStatement>(std::move(expression));
}

std::unique_ptr<Node> Parser::parseExpression() {
    if ( current.token.getTokenType() == token::DOLAR) {
        return parseAssignementExpression();
    }
    return parseOrExpression();
}

std::unique_ptr<Node> Parser::parseAssignementExpression() {
    readLex();
    assert(current.token.getTokenType()==token::TokenType::IDENTIFIER);
    std::string identifier = std::get<std::string>(*current.token.getValue());
    readLex();
    readLex();
    assert(current.token.getTokenType()==token::TokenType::INTEGER);
    auto expression = parseOrExpression();
    return std::make_unique<AssignementExpression>(
        identifier, std::move(expression)
    );
}

std::unique_ptr<Node> Parser::parseOrExpression() {
    auto left = parseAndExpression();
    while (
        current.token.getTokenType() == token::OR
    ) {
        token::TokenType op = current.token.getTokenType();
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
        current.token.getTokenType() == token::AND
    ) {
        token::TokenType op = current.token.getTokenType();
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
        current.token.getTokenType() == token::EQUALS
        || current.token.getTokenType() == token::NEQ
    ) {
        token::TokenType op = current.token.getTokenType();
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
        current.token.getTokenType() == token::LESS
        || current.token.getTokenType() == token::GREATER
        || current.token.getTokenType() == token::LEQ
        || current.token.getTokenType() == token::GEQ
    ) {
        token::TokenType op = current.token.getTokenType();
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
        current.token.getTokenType() == token::PLUS
        || current.token.getTokenType() == token::MINUS
    ) {
        token::TokenType op = current.token.getTokenType();
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
        current.token.getTokenType() == token::STAR
        || current.token.getTokenType() == token::SLASH
        || current.token.getTokenType() == token::PERCENT
    ) {
        token::TokenType op = current.token.getTokenType();
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
        current.token.getTokenType() == token::LARROW
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
        current.token.getTokenType() == token::BANG ||
        current.token.getTokenType() == token::MINUS
    ) {
        token::TokenType op = current.token.getTokenType();
        readLex();
        if (current.token.getTokenType() == token::IDENTIFIER)
            expression = parseIdentifierLike();
        else if (current.token.getTokenType() == token::LPARENT)
            expression = parseParentExpression();
        else if (current.token.getTokenType() == token::HASH)
            expression = parseLambdaExpression();
        else if (current.token.getTokenType() == token::LBRACKET)
            expression = parseDebugExpression();
        else
            expression = parseLiteral();
        return std::make_unique<PrefixExpression>(
            op, std::move(expression)
        );
    }
    if (current.token.getTokenType() == token::IDENTIFIER)
        expression = parseIdentifierLike();
    else if (current.token.getTokenType() == token::LPARENT)
        expression = parseParentExpression();
    else if (current.token.getTokenType() == token::HASH)
        expression = parseLambdaExpression();
    else if (current.token.getTokenType() == token::LBRACKET)
        expression = parseDebugExpression();
    else
        expression = parseLiteral();
    return expression;
}

std::unique_ptr<Node> Parser::parseIdentifierLike() {
    std::string val = std::get<std::string>(*current.token.getValue());
    readLex();
    auto ident = std::make_unique<Identifier>(val);
    if (current.token.getTokenType()==token::TokenType::LPARENT)
        return parseCallExpression(std::move(ident));
    return ident;
}

std::unique_ptr<Node> Parser::parseCallArguments() {
    auto arguments = std::make_unique<CallArguments>();
    assert(current.token.getTokenType()==token::TokenType::LPARENT);
    readLex();
    while (current.token.getTokenType() != token::TokenType::RPARENT){
        arguments->arguments.push_back(std::move(parseExpression()));
        if (current.token.getTokenType() != token::TokenType::COMMA) break;
        readLex();
    }
    assert(current.token.getTokenType()==token::TokenType::RPARENT);
    readLex();
    return arguments;
}

std::unique_ptr<Node> Parser::parseParentExpression() {
    assert(current.token.getTokenType()==token::TokenType::LPARENT);
    readLex();
    auto pExpression = std::make_unique<ParentExpression>(parseExpression());
    assert(current.token.getTokenType()==token::TokenType::RPARENT);
    readLex();
    if (current.token.getTokenType()==token::TokenType::LPARENT)
        return parseCallExpression(std::move(pExpression));
    return pExpression;
}

std::unique_ptr<Node> Parser::parseCallExpression(std::unique_ptr<Node> function) {
    std::unique_ptr<Node> result = std::move(function);
    assert(current.token.getTokenType()==token::TokenType::LPARENT);
    do {
        std::unique_ptr<Node> callArguments = parseCallArguments();
        result = std::make_unique<CallExpression>(
            std::move(result), std::move(callArguments)
        );
    } while (current.token.getTokenType() == token::LPARENT);
    return result;
}

std::unique_ptr<Node> Parser::parseDebugExpression() {
    assert(current.token.getTokenType()==token::TokenType::LBRACKET);
    readLex();
    auto function = parseExpression();
    assert(current.token.getTokenType()==token::TokenType::RBRACKET);
    readLex();

    std::unique_ptr<Node> result = std::move(function);
    assert(current.token.getTokenType()==token::TokenType::LPARENT);
    std::unique_ptr<Node> callArguments = parseCallArguments();
    result = std::make_unique<DebugExpression>(
        std::move(result), std::move(callArguments));

    while (current.token.getTokenType() == token::LPARENT) {
        std::unique_ptr<Node> callArguments = parseCallArguments();
        result = std::make_unique<CallExpression>(
            std::move(result), std::move(callArguments)
        );
    }
    return result;
}

std::unique_ptr<Node> Parser::parseLambdaExpression() {
    auto lambda = std::make_unique<LambdaExpression>();
    assert(current.token.getTokenType()==token::TokenType::HASH);
    readLex();
    assert(current.token.getTokenType()==token::TokenType::LPARENT);
    auto declarationArguments = parseDeclarationArguments();
    lambda->arguments = std::move(declarationArguments);
    assert(current.token.getTokenType()==token::TokenType::LBRACE);
    auto block = parseBlock();
    lambda->statements = std::move(block);
    if (current.token.getTokenType()==token::TokenType::LPARENT)
        return parseCallExpression(std::move(lambda));
    return lambda;
}

std::unique_ptr<Node> Parser::parseLiteral() {
    if (current.token.getTokenType() == token::INTEGER) {
        return parseIntegerLiteral();
    }
    else if (current.token.getTokenType() == token::FLOAT) {
    return parseFloatLiteral();
    }
    else if (current.token.getTokenType() == token::STRING) {
        return parseStringLiteral();
    }
    else if (
        current.token.getTokenType() == token::TRUE ||
        current.token.getTokenType() == token::FALSE

    ) {
        return parseBoolLiteral();
    }
    throw std::runtime_error("Literal type does not exist.");
}

std::unique_ptr<Node> Parser::parseIntegerLiteral() {
    types::Integer val =std::get<types::Integer>(*current.token.getValue());
    readLex();
    return std::make_unique<IntegerLiteral>(val);
}

std::unique_ptr<Node> Parser::parseFloatLiteral() {
    types::Float val =std::get<types::Float>(*current.token.getValue());
    readLex();
    return std::make_unique<FloatLiteral>(val);
}

std::unique_ptr<Node> Parser::parseBoolLiteral() {
    bool val = current.token.getTokenType() == token::TRUE;
    readLex();
    return std::make_unique<BoolLiteral>(val);
}

std::unique_ptr<Node> Parser::parseStringLiteral() {
    std::string val = std::get<std::string>(*current.token.getValue());
    readLex();
    return std::make_unique<StringLiteral>(val);
}

std::unique_ptr<Node> Parser::parseTypeSpecifier() {
    // TypeName::iterator type = TokenToType.at(current.token.getTokenType());
    auto type = TokenToType.find(current.token.getTokenType());
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
    if (current.token.getTokenType() == token::END_OF_FILE) {
        return;
    }
    current = lexer.nextLexem();
}
