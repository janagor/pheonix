
#include "../inc/parser.hpp"
#include <string>
#include <cassert>
#include <cstdlib>

namespace parser {

Node* Parser::generateParsingTree() {
    std::optional<Node*> maybeResult;
    maybeResult = parse();
    if (maybeResult) return *maybeResult;
    return nullptr; // add sensible handling of empty
}
Node* Parser::parseIntegerLiteral() {
    Node* il = new IntegerLiteral(std::get<int>(*current.token.value));
    return il;
}




Node* Parser::parseMultiplicativeExpression() {
    int left = std::get<int>(*current.token.value);
    readLex();
    token::TokenType op = current.token.tokenType;
    readLex();
    int right = std::get<int>(*current.token.value);
    Node* me = new MultiplicativeExpression(left, right, op);
    return me;
}

Node* Parser::parseAdditiveExpression() {
    int left = std::get<int>(*current.token.value);
    readLex();
    token::TokenType op = current.token.tokenType;
    readLex();
    int right = std::get<int>(*current.token.value);
    Node* ae = new AdditiveExpression(left, right, op);
    return ae;
}

void Parser::readLex() {
    if (current.token.tokenType == token::END_OF_FILE) {
        return;
    }
    current = peek;
    peek = lexer.nextLexem();

}

std::optional<Node*> Parser::parse() {
    Node* root;
    switch (current.token.tokenType) {
    case token::INTEGER:
        switch (peek.token.tokenType) {
        case token::PLUS:
        case token::MINUS:
            root = parseAdditiveExpression();
            return root;
            break;
        case token::STAR:
        case token::SLASH:
            root = parseMultiplicativeExpression();
            return root;
            break;

        default:
        root = parseIntegerLiteral();
        return root;
        }
        break;

    default:
        return std::nullopt;
    }
    return std::nullopt;
}
} // parser
