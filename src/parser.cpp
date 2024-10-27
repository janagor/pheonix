
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

std::optional<Node*> Parser::parse() {
    Node* root;
    switch (current.token.tokenType) {
    case lexer::INTEGER:
        root = parseIntegerLiteral();
        return root;
        break;
    default:
        return std::nullopt;
        break;
    }
    return std::nullopt;
}
} // parser
