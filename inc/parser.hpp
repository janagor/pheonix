#pragma once

#include <memory>

#include "lexer.hpp"
#include "node.hpp"

namespace pheonix::parser {

struct Parser {
private:
  std::unique_ptr<node::Node> parseProgram();
  std::unique_ptr<node::Node> parseBlock();
  std::unique_ptr<node::Node> parseStatement();
  std::unique_ptr<node::Node> parseDeclarationArguments();
  std::unique_ptr<node::Node> parseParameter();
  std::unique_ptr<node::Node> parseFunctionDeclaration();
  std::unique_ptr<node::Node> parseVariableDeclaration();
  std::unique_ptr<node::Node> parseWhileLoopStatement();
  std::unique_ptr<node::Node> parseIfStatement();
  std::unique_ptr<node::Node> parseElse();
  std::unique_ptr<node::Node> parseReturnStatement();
  std::unique_ptr<node::Node> parseNullStatement();
  std::unique_ptr<node::Node> parseExpressionStatement();
  std::unique_ptr<node::Node> parseExpression();
  std::unique_ptr<node::Node> parseAssignementExpression();
  std::unique_ptr<node::Node> parseOrExpression();
  std::unique_ptr<node::Node> parseAndExpression();
  std::unique_ptr<node::Node> parseComparisonExpression();
  std::unique_ptr<node::Node> parseRelationalExpression();
  std::unique_ptr<node::Node> parseAdditiveExpression();
  std::unique_ptr<node::Node> parseMultiplicativeExpression();
  std::unique_ptr<node::Node> parseCompositiveExpression();
  std::unique_ptr<node::Node> parseCastExpression();
  std::unique_ptr<node::Node> parsePrefixExpression();
  std::unique_ptr<node::Node> parseOtherExpression();
  std::unique_ptr<node::Node> parseMaybeCallExpression();
  std::unique_ptr<node::Node> parseMaybeIdentifierCall();
  std::unique_ptr<node::Node> parseMaybeLambdaCall();
  std::unique_ptr<node::Node> parseMaybeParentCall();
  std::unique_ptr<node::Node> parseMaybeDebugCall();
  std::unique_ptr<node::Node> parseIdentifier();
  std::unique_ptr<node::Node> parseLambdaExpression();
  std::unique_ptr<node::Node> parseParentExpression();
  std::unique_ptr<node::Node> parseDebugExpression();
  std::unique_ptr<node::Node> parseCallArguments();
  std::unique_ptr<node::Node> parseLiteral();
  std::unique_ptr<node::Node> parseTypeSpecifier();
  void readLex();

private:
  lexer::Lexer lexer;
  lexer::Lexem current;
  lexer::Lexem next;

public:
  Parser(std::istream &istream) : lexer(istream) {
    do {
      next = lexer.nextLexem();
    } while (next.token.getTokenType() == token::TokenType::ONE_LINE_COMMENT ||
             next.token.getTokenType() == token::TokenType::MULTILINE_COMMENT);
    readLex();
  };
  void consumeIf(token::TokenType token);
  void expect(token::TokenType token);
  std::unique_ptr<node::Node> generateParsingTree();
  std::optional<std::unique_ptr<node::Node>> parse();
};

} // namespace pheonix::parser
