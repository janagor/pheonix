#pragma once

#include <memory>
#include <typeinfo>

#include "lexer.hpp"
#include "node.hpp"
#include "visitor.hpp"

namespace parser {

struct Parser {
private:
  std::unique_ptr<Node> parseProgram();
  std::unique_ptr<Node> parseBlock();
  std::unique_ptr<Node> parseDeclarationArguments();
  std::unique_ptr<Node> parseParameter();
  std::unique_ptr<Node> parseFunctionDeclaration();
  std::unique_ptr<Node> parseVariableDeclaration();
  std::unique_ptr<Node> parseWhileLoopStatement();
  std::unique_ptr<Node> parseIfStatement();
  std::unique_ptr<Node> parseReturnStatement();
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
  std::unique_ptr<Node> parseCompositiveExpression();
  std::unique_ptr<Node> parseCastExpression();
  std::unique_ptr<Node> parsePrefixExpression();
  std::unique_ptr<Node> parseIdentifierLike();
  std::unique_ptr<Node> parseCallArguments();
  std::unique_ptr<Node> parseParentExpression();
  std::unique_ptr<Node> parseCallExpression(std::unique_ptr<Node> function);
  std::unique_ptr<Node> parseDebugExpression();
  std::unique_ptr<Node> parseLambdaExpression();
  std::unique_ptr<Node> parseLiteral();
  std::unique_ptr<Node> parseIntegerLiteral();
  std::unique_ptr<Node> parseFloatLiteral();
  std::unique_ptr<Node> parseBoolLiteral();
  std::unique_ptr<Node> parseStringLiteral();
  std::unique_ptr<Node> parseTypeSpecifier();
  void readLex();

public:
  Parser(std::istream &istream) : lexer(istream) {
    current = lexer.nextLexem();
  }
  std::unique_ptr<Node> generateParsingTree();
  std::optional<std::unique_ptr<Node>> parse();

private:
  lexer::Lexer lexer;
  lexer::Lexem current;
};

} // namespace parser
