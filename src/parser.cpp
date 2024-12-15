#include "parser.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

namespace pheonix::parser {
/*
 * PROGRAM = { STATEMENT } ;
 */
std::unique_ptr<node::Node> Parser::parseProgram() {
  auto program = std::make_unique<node::Program>();
  while (auto statement = parseStatement()) {
    program->statements.push_back(std::move(statement));
  }
  expect(token::TokenType::END_OF_FILE);
  return program;
}

/*
 * "{" { STATEMENT } "}"
 */
std::unique_ptr<node::Node> Parser::parseBlock() {
  auto block = std::make_unique<node::Block>();
  consumeIf(token::TokenType::LBRACE);
  while (auto statement = parseStatement())
    block->statements.push_back(std::move(statement));
  consumeIf(token::TokenType::RBRACE);
  return block;
}

/*
 * STATEMENT = FUNCTION_DECLARATION
 *           | VARIABLE_DECLARATION
 *           | WHILE_LOOP_STATEMENT
 *           | IF_STATEMENT
 *           | RETURN_STATEMENT
 *           | EXPRESSION_STATEMENT
 *           | ";" ;
 */
std::unique_ptr<node::Node> Parser::parseStatement() {
  // TODO: remove this testing and do it via failing parseExpressionStatement()
  if (current == token::TokenType::END_OF_FILE) {
    return nullptr;
  }
  if (current == token::TokenType::RBRACE) {
    return nullptr;
  }

  if (auto node = parseFunctionDeclaration())
    return node;
  if (auto node = parseVariableDeclaration())
    return node;
  if (auto node = parseWhileLoopStatement())
    return node;
  if (auto node = parseIfStatement())
    return node;
  if (auto node = parseReturnStatement())
    return node;
  if (auto node = parseNullStatement())
    return node;
  if (auto node = parseExpressionStatement())
    return node;
  return nullptr;
}

std::unique_ptr<node::Node> Parser::parseNullStatement() {
  if (current != token::TokenType::SEMICOLON)
    return nullptr;
  readLex();
  return std::make_unique<node::NullStatement>();
}

std::unique_ptr<node::Node> Parser::parseParameter() {
  bool isMutable;
  if (current == token::TokenType::MUT) {
    isMutable = true;
    readLex();
  }
  if (current != token::TokenType::IDENTIFIER) {
    if (isMutable)
      throw exception::ParserException("Error parsing parameter.", current.line,
                                       current.column);
    return nullptr;
  }
  std::string paramIdentifier =
      std::get<std::string>(*current.token.getValue());
  readLex();

  return std::make_unique<node::Parameter>(isMutable, paramIdentifier);
}

/*
 * ENCLOSED_PARAMETER_LIST = "(" PARAMETER_LIST ")" ;
 *          PARAMETER_LIST = [
 *                           [ "MUT" ] IDENTIFIER
 *                           { "," [ "MUT" ] IDENTIFIER }
 *                           ] ;
 */
std::unique_ptr<node::Node> Parser::parseDeclarationArguments() {
  consumeIf(token::TokenType::LPARENT);
  auto args = std::make_unique<node::DeclarationArguments>();
  if (auto param = parseParameter()) {
    args->arguments.push_back(std::move(param));
  }

  while (current == token::TokenType::COMMA) {
    readLex();
    auto param = parseParameter();
    if (!param)
      throw exception::ParserException("Error parsing parameter list.",
                                       current.line, current.column);
    args->arguments.push_back(std::move(param));
  }
  consumeIf(token::TokenType::RPARENT);
  return args;
}

/*
 * FUNCTION_DECLARATION = "FN" IDENTIFIER
 *                        ENCLOSED_PARAMETER_LIST FUNCTION_BODY ;
 */
std::unique_ptr<node::Node> Parser::parseFunctionDeclaration() {
  if (current != token::TokenType::FN)
    return nullptr;
  readLex();
  expect(token::TokenType::IDENTIFIER);
  std::string identifier = std::get<std::string>(*current.token.getValue());
  auto functionDeclaration =
      std::make_unique<node::FunctionDeclaration>(identifier);
  readLex();
  expect(token::TokenType::LPARENT);
  auto declarationArguments = parseDeclarationArguments();
  functionDeclaration->arguments = std::move(declarationArguments);
  expect(token::TokenType::LBRACE);
  auto block = parseBlock();
  functionDeclaration->statements = std::move(block);
  return functionDeclaration;
}

/*
 * VARIABLE_DECLARATION = "LET" [ "MUT"] IDENTIFIER "=" EXPESSION ";" ;
 */
std::unique_ptr<node::Node> Parser::parseVariableDeclaration() {
  if (current != token::TokenType::LET)
    return nullptr;
  readLex();
  bool isMutable = false;
  if (current == token::TokenType::MUT) {
    isMutable = true;
    readLex();
  }
  expect(token::TokenType::IDENTIFIER);
  std::string identifier = std::get<std::string>(*current.token.getValue());
  readLex();
  consumeIf(token::TokenType::ASSIGN);
  auto expression = parseExpression();
  consumeIf(token::TokenType::SEMICOLON);
  return std::make_unique<node::VariableDeclaration>(isMutable, identifier,
                                                     std::move(expression));
}

/*
 * WHILE_LOOP_STATEMENT = "WHILE"
 *                        "(" EXPRESSION ")"
 *                        "{"  { STATEMENT } "}" ;
 */
std::unique_ptr<node::Node> Parser::parseWhileLoopStatement() {
  if (current != token::TokenType::WHILE)
    return nullptr;
  readLex();
  consumeIf(token::TokenType::LPARENT);
  if (current == token::TokenType::RPARENT)
    throw exception::ParserException("Expected predicate in while statement",
                                     current.line, current.column);
  auto expression = parseExpression();

  consumeIf(token::TokenType::RPARENT);
  auto block = parseBlock();

  auto whileLoopStmt = std::make_unique<node::WhileLoopStatement>(
      std::move(expression), std::move(block));
  return whileLoopStmt;
}

/*
 * IF_STATEMENT = IF_CLAUSE [ ELSE_CLAUSE ] ;
 *    IF_CLAUSE = "IF"
 *                "(" EXPRESSION ")"
 *                "{" { STATEMENT } "}" ;
 *  ELSE_CLAUSE = "ELSE"
 *                (
 *                IF_STATEMENT
 *                | ( "{" { STATEMENT } "}" )
 *                ) ;
 */
std::unique_ptr<node::Node> Parser::parseIfStatement() {
  if (current != token::TokenType::IF)
    return nullptr;
  readLex();
  consumeIf(token::TokenType::LPARENT);
  if (current == token::TokenType::RPARENT)
    throw exception::ParserException("Expected predicate in if statement",
                                     current.line, current.column);

  auto predicate = parseExpression();
  auto ifStmt = std::make_unique<node::IfStatement>(std::move(predicate));
  consumeIf(token::TokenType::RPARENT);
  auto ifBody = parseBlock();
  if (!ifBody)
    throw exception::ParserException("Error parsing IfBody", current.line,
                                     current.column);

  ifStmt->ifBody = std::move(ifBody);
  auto elseBody = parseElse();
  ifStmt->elseBody = std::move(elseBody);
  return ifStmt;
}

std::unique_ptr<node::Node> Parser::parseElse() {
  if (current != token::TokenType::ELSE)
    return nullptr;
  readLex();
  if (current == token::TokenType::IF)
    return parseIfStatement();
  expect(token::TokenType::LBRACE);
  return parseBlock();
}

/*
 * RETURN_STATEMENT = "RETURN" [ EXPRESSION ] ";" ;
 */
std::unique_ptr<node::Node> Parser::parseReturnStatement() {
  if (current != token::TokenType::RETURN)
    return nullptr;
  readLex();
  // TODO: Change it so that parse expression just returns nullptr
  if (current == token::TokenType::SEMICOLON) {
    readLex();
    return std::make_unique<node::ReturnStatement>(nullptr);
  }
  auto expression = parseExpression();
  consumeIf(token::TokenType::SEMICOLON);
  return std::make_unique<node::ReturnStatement>(std::move(expression));
}

/*
 * EXPRESSION_STATEMENT = EXPRESSION ";" ;
 */
std::unique_ptr<node::Node> Parser::parseExpressionStatement() {
  auto expression = parseExpression();
  consumeIf(token::TokenType::SEMICOLON);
  return std::make_unique<node::ExpressionStatement>(std::move(expression));
}

/*
 * EXPRESSION = ASSIGNEMENT_EXPRESSION
 *            | OR_EXPRESSION ;
 */
std::unique_ptr<node::Node> Parser::parseExpression() {
  if (auto node = parseAssignementExpression())
    return node;
  return parseOrExpression();
}

/*
 * ASSIGNEMENT_EXPRESSION = "$" IDENTIFIER "=" OR_EXPRESSION ;
 */
std::unique_ptr<node::Node> Parser::parseAssignementExpression() {
  if (current != token::TokenType::DOLAR)
    return nullptr;
  readLex();
  expect(token::TokenType::IDENTIFIER);

  std::string identifier = std::get<std::string>(*current.token.getValue());
  readLex();
  consumeIf(token::TokenType::ASSIGN);
  auto expression = parseOrExpression();
  return std::make_unique<node::AssignementExpression>(identifier,
                                                       std::move(expression));
}

/*
 * OR_EXPRESSION = AND_EXPRESSION
 *    { "||" AND_EXPRESSION } ;
 */
std::unique_ptr<node::Node> Parser::parseOrExpression() {
  auto left = parseAndExpression();
  while (current == token::TokenType::OR) {
    readLex();
    auto right = parseAndExpression();
    left = std::make_unique<node::OrExpression>(std::move(left),
                                                std::move(right), "||");
  }
  return left;
}

/*
 * AND_EXPRESSION = COMPARISON_EXPRESSION
 *                  { "&&" COMPARISON_EXPRESSION } ;
 */
std::unique_ptr<node::Node> Parser::parseAndExpression() {
  auto left = parseComparisonExpression();
  while (current == token::TokenType::AND) {
    readLex();
    auto right = parseComparisonExpression();
    left = std::make_unique<node::AndExpression>(std::move(left),
                                                 std::move(right), "&&");
  }
  return left;
}

/*
 * COMPARISON_EXPRESSION = RELATIONAL_EXPRESSION
 *                         { ( "==" | "!=" )
 *                         RELATIONAL_EXPRESSION } ;
 */
std::unique_ptr<node::Node> Parser::parseComparisonExpression() {
  auto left = parseRelationalExpression();
  while (current == token::TokenType::EQUALS ||
         current == token::TokenType::NEQ) {
    token::TokenType op = current.token.getTokenType();
    readLex();
    auto right = parseRelationalExpression();
    left = std::make_unique<node::ComparisonExpression>(
        std::move(left), std::move(right), types::opToString(op));
  }
  return left;
}

/*
 * RELATIONAL_EXPRESSION = ADDITIVE_EXPRESSION
 *                         { ( "<" | ">" | "<=" | ">=" )
 *                         ADDITIVE_EXPRESSION } ;
 */
std::unique_ptr<node::Node> Parser::parseRelationalExpression() {
  auto left = parseAdditiveExpression();
  while (current == token::TokenType::LESS ||
         current == token::TokenType::GREATER ||
         current == token::TokenType::LEQ || current == token::TokenType::GEQ) {
    token::TokenType op = current.token.getTokenType();
    readLex();
    auto right = parseAdditiveExpression();
    left = std::make_unique<node::RelationalExpression>(
        std::move(left), std::move(right), types::opToString(op));
  }
  return left;
}

/*
 * ADDITIVE_EXPRESSION = MULTIPLICATIVE_EXPRESSION
 *                       { ( "+" | "-" )
 *                       MULTIPLICATIVE_EXPRESSION } ;
 */
std::unique_ptr<node::Node> Parser::parseAdditiveExpression() {
  auto left = parseMultiplicativeExpression();
  while (current == token::TokenType::PLUS ||
         current == token::TokenType::MINUS) {
    token::TokenType op = current.token.getTokenType();
    readLex();
    auto right = parseMultiplicativeExpression();
    left = std::make_unique<node::AdditiveExpression>(
        std::move(left), std::move(right), types::opToString(op));
  }
  return left;
}

/*
 * MULTIPLICATIVE_EXPRESSION = COMPOSITIVE_EXPRESSION
 *                             { ( "*" | "/" | "%" )
 *                             COMPOSITIVE_EXPRESSION } ;
 */
std::unique_ptr<node::Node> Parser::parseMultiplicativeExpression() {
  std::unique_ptr<node::Node> left = parseCompositiveExpression();
  while (current == token::TokenType::STAR ||
         current == token::TokenType::SLASH ||
         current == token::TokenType::PERCENT) {
    token::TokenType op = current.token.getTokenType();
    readLex();
    std::unique_ptr<node::Node> right = parseCompositiveExpression();
    left = std::make_unique<node::MultiplicativeExpression>(
        std::move(left), std::move(right), types::opToString(op));
  }
  return left;
}

/*
 * COMPOSITIVE_EXPRESSION = CAST_EXPRESSION
 *                          { "|" CAST_EXPRESSION } ;
 */
std::unique_ptr<node::Node> Parser::parseCompositiveExpression() {
  std::unique_ptr<node::Node> left = parseCastExpression();
  while (current == token::TokenType::PIPE) {
    readLex();
    std::unique_ptr<node::Node> right = parseCastExpression();
    left = std::make_unique<node::CompositiveExpression>(std::move(left),
                                                         std::move(right));
  }
  return left;
}

/*
 * CAST_EXPRESSION = PREFIX_EXPRESSION { "<-" TYPE_NAME } ;
 */
std::unique_ptr<node::Node> Parser::parseCastExpression() {
  std::unique_ptr<node::Node> expression = parsePrefixExpression();
  while (current == token::TokenType::LARROW) {
    readLex();
    std::unique_ptr<node::Node> type = parseTypeSpecifier();
    return std::make_unique<node::CastExpression>(std::move(expression),
                                                  std::move(type));
  }
  return expression;
}

/*
 * PREFIX_EXPRESSION = "!" PRIMARY_EXPRESSION
 *                   | "-" PRIMARY_EXPRESSION
 *                   | PRIMARY_EXPRESSION ;
 */
std::unique_ptr<node::Node> Parser::parsePrefixExpression() {
  std::unique_ptr<node::Node> expression;
  if (current == token::TokenType::BANG || current == token::TokenType::MINUS) {
    token::TokenType op = current.token.getTokenType();
    readLex();
    if (current == token::TokenType::IDENTIFIER)
      expression = parseIdentifierLike();
    else if (current == token::TokenType::LPARENT)
      expression = parseParentExpression();
    else if (current == token::TokenType::HASH)
      expression = parseLambdaExpression();
    else if (current == token::TokenType::LBRACKET)
      expression = parseDebugExpression();
    else
      expression = parseLiteral();
    return std::make_unique<node::PrefixExpression>(types::opToString(op),
                                                    std::move(expression));
  }
  if (current == token::TokenType::IDENTIFIER)
    expression = parseIdentifierLike();
  else if (current == token::TokenType::LPARENT)
    expression = parseParentExpression();
  else if (current == token::TokenType::HASH)
    expression = parseLambdaExpression();
  else if (current == token::TokenType::LBRACKET)
    expression = parseDebugExpression();
  else
    expression = parseLiteral();
  return expression;
}

/*
 * PRIMARY_EXPRESSION = (
 *                      IDENTIFIER
 *                      | ( "#" ENCLOSED_PARAMETER_LIST FUNCTION_BODY )
 *                      )
 *                      { "(" EXPRESSION_LIST ")" }
 *                    | ...
 */
std::unique_ptr<node::Node> Parser::parseIdentifierLike() {
  std::string val = std::get<std::string>(*current.token.getValue());
  readLex();
  auto ident = std::make_unique<node::Identifier>(val);
  if (current == token::TokenType::LPARENT)
    return parseCallExpression(std::move(ident));
  return ident;
}

/*
 * PRIMARY_EXPRESSION = (
 *                      IDENTIFIER
 *                      | ( "#" ENCLOSED_PARAMETER_LIST FUNCTION_BODY )
 *                      )
 *                      { "(" EXPRESSION_LIST ")" }
 *                    | "(" EXPRESSION ")" { "(" EXPRESSION_LIST ")" }
 *                    | "[" EXPRESSION "]" "(" EXPRESSION_LIST ")" { "("
 *                      EXPRESSION_LIST ")" }
 *                    | LITERAL ;
 *
 *    EXPRESSION_LIST = [ EXPRESSION { "," EXPRESSION } ] ;
 *
 * NOTE: in my EBNF notation parenthesis expression is one variant of the
 * `primary_expression`.
 */
std::unique_ptr<node::Node> Parser::parseCallArguments() {
  auto arguments = std::make_unique<node::CallArguments>();
  consumeIf(token::TokenType::LPARENT);
  while (current != token::TokenType::RPARENT) {
    arguments->arguments.push_back(parseExpression());
    if (current != token::TokenType::COMMA)
      break;
    readLex();
  }
  consumeIf(token::TokenType::RPARENT);
  return arguments;
}

/*
 * PRIMARY_EXPRESSION = (
 *                      IDENTIFIER
 *                      | ( "#" ENCLOSED_PARAMETER_LIST FUNCTION_BODY )
 *                      )
 *                      { "(" EXPRESSION_LIST ")" } ;
 *                    | "(" EXPRESSION ")" { "(" EXPRESSION_LIST ")" }
 *                    | "[" EXPRESSION "]" "(" EXPRESSION_LIST ")" { "("
 *                      EXPRESSION_LIST ")" }
 *                    | LITERAL ;
 *
 * NOTE: in my EBNF notation parenthesis expression is one variant of the
 * `primary_expression`.
 */
std::unique_ptr<node::Node> Parser::parseParentExpression() {
  consumeIf(token::TokenType::LPARENT);
  auto pExpression =
      std::make_unique<node::ParentExpression>(parseExpression());
  consumeIf(token::TokenType::RPARENT);
  if (current == token::TokenType::LPARENT)
    return parseCallExpression(std::move(pExpression));
  return pExpression;
}

/*
 * PRIMARY_EXPRESSION = (
 *                      IDENTIFIER
 *                      | ( "#" ENCLOSED_PARAMETER_LIST FUNCTION_BODY )
 *                      )
 *                      { "(" EXPRESSION_LIST ")" } ;
 *                    | "(" EXPRESSION ")" { "(" EXPRESSION_LIST ")" }
 *                    | "[" EXPRESSION "]" "(" EXPRESSION_LIST ")" { "("
 *                      EXPRESSION_LIST ")" }
 *                    | LITERAL ;
 *
 * NOTE: in my EBNF notation call expression is one variant of the
 * `primary_expression`.
 */
std::unique_ptr<node::Node>
Parser::parseCallExpression(std::unique_ptr<node::Node> function) {
  std::unique_ptr<node::Node> result = std::move(function);
  expect(token::TokenType::LPARENT);
  do {
    std::unique_ptr<node::Node> callArguments = parseCallArguments();
    result = std::make_unique<node::CallExpression>(std::move(result),
                                                    std::move(callArguments));
  } while (current == token::TokenType::LPARENT);
  return result;
}

/*
 * PRIMARY_EXPRESSION = (
 *                      IDENTIFIER
 *                      | ( "#" ENCLOSED_PARAMETER_LIST FUNCTION_BODY )
 *                      )
 *                      { "(" EXPRESSION_LIST ")" } ;
 *                    | "(" EXPRESSION ")" { "(" EXPRESSION_LIST ")" }
 *                    | "[" EXPRESSION "]" "(" EXPRESSION_LIST ")" { "("
 *                      EXPRESSION_LIST ")" }
 *                    | LITERAL ;
 *
 * NOTE: in my EBNF notation debug expression is one variant of the
 * `primary_expression`.
 */
std::unique_ptr<node::Node> Parser::parseDebugExpression() {
  consumeIf(token::TokenType::LBRACKET);
  auto function = parseExpression();
  consumeIf(token::TokenType::RBRACKET);

  std::unique_ptr<node::Node> result = std::move(function);
  expect(token::TokenType::LPARENT);
  std::unique_ptr<node::Node> callArguments = parseCallArguments();
  result = std::make_unique<node::DebugExpression>(std::move(result),
                                                   std::move(callArguments));

  while (current == token::TokenType::LPARENT) {
    std::unique_ptr<node::Node> callArguments = parseCallArguments();
    result = std::make_unique<node::CallExpression>(std::move(result),
                                                    std::move(callArguments));
  }
  return result;
}

/*
 * PRIMARY_EXPRESSION = (
 *                      IDENTIFIER
 *                      | ( "#" ENCLOSED_PARAMETER_LIST FUNCTION_BODY )
 *                      )
 *                      { "(" EXPRESSION_LIST ")" } ;
 *                    | "(" EXPRESSION ")" { "(" EXPRESSION_LIST ")" }
 *                    | "[" EXPRESSION "]" "(" EXPRESSION_LIST ")" { "("
 *                      EXPRESSION_LIST ")" }
 *                    | LITERAL ;
 *
 * NOTE: in my EBNF notation lambda expression is one variant of the
 * `primary_expression`.
 */
std::unique_ptr<node::Node> Parser::parseLambdaExpression() {
  auto lambda = std::make_unique<node::LambdaExpression>();
  consumeIf(token::TokenType::HASH);
  expect(token::TokenType::LPARENT);

  auto declarationArguments = parseDeclarationArguments();
  lambda->arguments = std::move(declarationArguments);
  expect(token::TokenType::LBRACE);
  auto block = parseBlock();
  lambda->statements = std::move(block);
  if (current == token::TokenType::LPARENT)
    return parseCallExpression(std::move(lambda));
  return lambda;
}

/*
 * LITERAL = BOOL_LITERAL
 *         | FLOAT_LITERAL
 *         | INTEGER_LITERAL
 *         | STRING_LITERAL ;
 */
std::unique_ptr<node::Node> Parser::parseLiteral() {
  if (current == token::TokenType::INTEGER ||
      current == token::TokenType::STRING ||
      current == token::TokenType::FLOAT ||
      current == token::TokenType::FALSE || current == token::TokenType::TRUE) {
    auto node = std::make_unique<node::Literal>(*current.token.getValue());
    readLex();
    return node;
  };
  // TODO: make it return nullptr rather then throw if failed parsing
  if (current == token::TokenType::END_OF_FILE ||
      current == token::TokenType::SEMICOLON)
    throw exception::ParserException("Expected expression.", current.line,
                                     current.column);
  throw exception::ParserException("Could not parse.", current.line,
                                   current.column);
}

/*
 * TYPE_NAME = "BOL" | "FLT" | "INT" | "STR" ;
 */
std::unique_ptr<node::Node> Parser::parseTypeSpecifier() {
  std::string type = types::tokenTypeToLiteral(current.token.getTokenType());

  if (type == "FLT" || type == "INT" || type == "STR" || type == "BOL") {
    readLex();
    return std::make_unique<node::TypeSpecifier>(type);
  }
  throw exception::ParserException("Expected type specifier.", current.line,
                                   current.column);
}

void Parser::expect(token::TokenType token) {
  if (current != token)
    throw exception::ParserException(
        "Expected: " + types::tokenTypeToLiteral(token) + ", Got: " +
            types::tokenTypeToLiteral(current.token.getTokenType()) + ".",
        current.line, current.column);
}

void Parser::consumeIf(token::TokenType token) {
  if (current == token) {
    readLex();
    return;
  }
  throw exception::ParserException(
      "Expected: " + types::tokenTypeToLiteral(token) + ", Got: " +
          types::tokenTypeToLiteral(current.token.getTokenType()) + ".",
      current.line, current.column);
}

std::unique_ptr<node::Node> Parser::generateParsingTree() {
  std::optional<std::unique_ptr<node::Node>> maybeResult;
  maybeResult = parse();
  if (maybeResult)
    return std::move(*maybeResult);
  return nullptr; // TODO: add sensible handling of empty
}

std::optional<std::unique_ptr<node::Node>> Parser::parse() {
  return parseProgram();
}

void Parser::readLex() {
  current = next;
  if (current == token::TokenType::END_OF_FILE) {
    return;
  }
  do {
    next = lexer.nextLexem();
  } while (next == token::TokenType::ONE_LINE_COMMENT ||
           next == token::TokenType::MULTILINE_COMMENT);
}

} // namespace pheonix::parser
