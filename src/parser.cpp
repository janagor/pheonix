#include "parser.hpp"

#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace pheonix::parser {

std::unique_ptr<node::Node> Parser::parseProgram() {
  auto program = std::make_unique<node::Program>();
  while (current.token.getTokenType() != token::TokenType::END_OF_FILE) {
    program->statements.push_back(parseStatement());
  }
  return program;
}

std::unique_ptr<node::Node> Parser::parseBlock() {
  auto block = std::make_unique<node::Block>();
  assert(current.token.getTokenType() == token::TokenType::LBRACE);
  readLex();
  while (current.token.getTokenType() != token::TokenType::RBRACE) {
    auto statement = parseStatement();
    block->statements.push_back(std::move(statement));
  }
  if (current.token.getTokenType() != token::TokenType::RBRACE)
    throw exception::ParserException("Expected '}' in the end of the block.");

  readLex();

  return block;
}

std::unique_ptr<node::Node> Parser::parseStatement() {
  std::unique_ptr<node::Node> node;
  switch (current.token.getTokenType()) {
  case token::TokenType::LET:
    return parseVariableDeclaration();
  case token::TokenType::WHILE:
    return parseWhileLoopStatement();
  case token::TokenType::IF:
    return parseIfStatement();
  case token::TokenType::FN:
    return parseFunctionDeclaration();
  case token::TokenType::RETURN:
    return parseReturnStatement();
  default:
    return parseExpressionStatement();
  }
}

std::unique_ptr<node::Node> Parser::parseDeclarationArguments() {
  auto args = std::make_unique<node::DeclarationArguments>();
  assert(current.token.getTokenType() == token::TokenType::LPARENT);
  readLex();
  while (current.token.getTokenType() == token::TokenType::MUT ||
         current.token.getTokenType() == token::TokenType::IDENTIFIER) {
    bool isMutable = false;
    if (current.token.getTokenType() == token::TokenType::MUT) {
      isMutable = true;
      readLex();
    }
    std::string paramIdentifier =
        std::get<std::string>(*current.token.getValue());
    readLex();
    auto arg = std::make_unique<node::Parameter>(isMutable, paramIdentifier);
    args->arguments.push_back(std::move(arg));
    if (current.token.getTokenType() == token::TokenType::COMMA)
      readLex();
  }
  assert(current.token.getTokenType() == token::TokenType::RPARENT);
  readLex();
  return args;
}

std::unique_ptr<node::Node> Parser::parseFunctionDeclaration() {
  assert(current.token.getTokenType() == token::TokenType::FN);
  readLex();
  assert(current.token.getTokenType() == token::TokenType::IDENTIFIER);
  std::string identifier = std::get<std::string>(*current.token.getValue());
  auto functionDeclaration =
      std::make_unique<node::FunctionDeclaration>(identifier);
  readLex();
  assert(current.token.getTokenType() == token::TokenType::LPARENT);
  auto declarationArguments = parseDeclarationArguments();
  functionDeclaration->arguments = std::move(declarationArguments);
  assert(current.token.getTokenType() == token::TokenType::LBRACE);
  auto block = parseBlock();
  functionDeclaration->statements = std::move(block);
  return functionDeclaration;
}

std::unique_ptr<node::Node> Parser::parseVariableDeclaration() {
  assert(current.token.getTokenType() == token::TokenType::LET);
  readLex();
  bool isMutable = false;
  if (current.token.getTokenType() == token::TokenType::MUT) {
    isMutable = true;
    readLex();
  }
  assert(current.token.getTokenType() == token::TokenType::IDENTIFIER);
  std::string identifier = std::get<std::string>(*current.token.getValue());
  readLex();
  assert(current.token.getTokenType() == token::TokenType::ASSIGN);
  readLex();
  auto expression = parseExpressionStatement();
  return std::make_unique<node::VariableDeclaration>(isMutable, identifier,
                                                     std::move(expression));
}

std::unique_ptr<node::Node> Parser::parseWhileLoopStatement() {
  assert(current.token.getTokenType() == token::TokenType::WHILE);
  readLex();
  assert(current.token.getTokenType() == token::TokenType::LPARENT);
  readLex();
  auto expression = parseExpression();

  assert(current.token.getTokenType() == token::TokenType::RPARENT);
  readLex();
  auto block = parseBlock();

  auto whileLoopStmt = std::make_unique<node::WhileLoopStatement>(
      std::move(expression), std::move(block));
  return whileLoopStmt;
}

std::unique_ptr<node::Node> Parser::parseIfStatement() {
  assert(current.token.getTokenType() == token::TokenType::IF);
  readLex();
  if (current.token.getTokenType() != token::TokenType::LPARENT)
    throw exception::ParserException("Expected '(' after 'if'");
  readLex();
  auto predicate = parseExpression();
  auto ifStmt = std::make_unique<node::IfStatement>(std::move(predicate));
  assert(current.token.getTokenType() == token::TokenType::RPARENT);
  readLex();
  auto ifBody = parseBlock();
  ifStmt->ifBody = std::move(ifBody);
  if (current.token.getTokenType() == token::TokenType::ELSE) {
    readLex();
    if (current.token.getTokenType() == token::TokenType::IF) {
      auto elseIfStmt = parseIfStatement();
      ifStmt->elseBody = std::move(elseIfStmt);
    } else {
      assert(current.token.getTokenType() == token::TokenType::LBRACE);
      auto elseBody = parseBlock();
      ifStmt->elseBody = std::move(elseBody);
    }
  }
  return ifStmt;
}

std::unique_ptr<node::Node> Parser::parseReturnStatement() {
  assert(current.token.getTokenType() == token::TokenType::RETURN);
  readLex();
  auto expression = parseExpressionStatement();
  return std::make_unique<node::ReturnStatement>(std::move(expression));
}

std::unique_ptr<node::Node> Parser::parseExpressionStatement() {
  auto expression = parseExpression();
  if (current.token.getTokenType() != token::TokenType::SEMICOLON)
    throw exception::ParserException("Expected ';' after the expression.");
  readLex();
  return std::make_unique<node::ExpressionStatement>(std::move(expression));
}

std::unique_ptr<node::Node> Parser::parseExpression() {
  if (current.token.getTokenType() == token::TokenType::DOLAR) {
    return parseAssignementExpression();
  }
  return parseOrExpression();
}

std::unique_ptr<node::Node> Parser::parseAssignementExpression() {
  readLex();
  if (current.token.getTokenType() != token::TokenType::IDENTIFIER)
    throw exception::ParserException(
        "Expected 'identifier' after the '$' sign.");

  std::string identifier = std::get<std::string>(*current.token.getValue());
  readLex();
  if (current.token.getTokenType() != token::TokenType::ASSIGN)
    throw exception::ParserException("Expected '=' after the '$ " + identifier +
                                     "'.");
  readLex();
  auto expression = parseOrExpression();
  return std::make_unique<node::AssignementExpression>(identifier,
                                                       std::move(expression));
}

std::unique_ptr<node::Node> Parser::parseOrExpression() {
  auto left = parseAndExpression();
  while (current.token.getTokenType() == token::TokenType::OR) {
    token::TokenType op = current.token.getTokenType();
    readLex();
    auto right = parseAndExpression();
    left = std::make_unique<node::OrExpression>(std::move(left),
                                                std::move(right), op);
  }
  return left;
}

std::unique_ptr<node::Node> Parser::parseAndExpression() {
  auto left = parseComparisonExpression();
  while (current.token.getTokenType() == token::TokenType::AND) {
    token::TokenType op = current.token.getTokenType();
    readLex();
    auto right = parseComparisonExpression();
    left = std::make_unique<node::AndExpression>(std::move(left),
                                                 std::move(right), op);
  }
  return left;
}
std::unique_ptr<node::Node> Parser::parseComparisonExpression() {
  auto left = parseRelationalExpression();
  while (current.token.getTokenType() == token::TokenType::EQUALS ||
         current.token.getTokenType() == token::TokenType::NEQ) {
    token::TokenType op = current.token.getTokenType();
    readLex();
    auto right = parseRelationalExpression();
    left = std::make_unique<node::ComparisonExpression>(std::move(left),
                                                        std::move(right), op);
  }
  return left;
}

std::unique_ptr<node::Node> Parser::parseRelationalExpression() {
  auto left = parseAdditiveExpression();
  while (current.token.getTokenType() == token::TokenType::LESS ||
         current.token.getTokenType() == token::TokenType::GREATER ||
         current.token.getTokenType() == token::TokenType::LEQ ||
         current.token.getTokenType() == token::TokenType::GEQ) {
    token::TokenType op = current.token.getTokenType();
    readLex();
    auto right = parseAdditiveExpression();
    left = std::make_unique<node::RelationalExpression>(std::move(left),
                                                        std::move(right), op);
  }
  return left;
}

std::unique_ptr<node::Node> Parser::parseAdditiveExpression() {
  auto left = parseMultiplicativeExpression();
  while (current.token.getTokenType() == token::TokenType::PLUS ||
         current.token.getTokenType() == token::TokenType::MINUS) {
    token::TokenType op = current.token.getTokenType();
    readLex();
    auto right = parseMultiplicativeExpression();
    left = std::make_unique<node::AdditiveExpression>(std::move(left),
                                                      std::move(right), op);
  }
  return left;
}

std::unique_ptr<node::Node> Parser::parseMultiplicativeExpression() {
  std::unique_ptr<node::Node> left = parseCompositiveExpression();
  while (current.token.getTokenType() == token::TokenType::STAR ||
         current.token.getTokenType() == token::TokenType::SLASH ||
         current.token.getTokenType() == token::TokenType::PERCENT) {
    token::TokenType op = current.token.getTokenType();
    readLex();
    std::unique_ptr<node::Node> right = parseCompositiveExpression();
    left = std::make_unique<node::MultiplicativeExpression>(
        std::move(left), std::move(right), op);
  }
  return left;
}

std::unique_ptr<node::Node> Parser::parseCompositiveExpression() {
  std::unique_ptr<node::Node> left = parseCastExpression();
  while (current.token.getTokenType() == token::TokenType::PIPE) {
    readLex();
    std::unique_ptr<node::Node> right = parseCastExpression();
    left = std::make_unique<node::CompositiveExpression>(std::move(left),
                                                         std::move(right));
  }
  return left;
}

std::unique_ptr<node::Node> Parser::parseCastExpression() {
  std::unique_ptr<node::Node> expression = parsePrefixExpression();
  while (current.token.getTokenType() == token::TokenType::LARROW) {
    readLex();
    std::unique_ptr<node::Node> type = parseTypeSpecifier();
    return std::make_unique<node::CastExpression>(std::move(expression),
                                                  std::move(type));
  }
  return expression;
}

std::unique_ptr<node::Node> Parser::parsePrefixExpression() {
  std::unique_ptr<node::Node> expression;
  if (current.token.getTokenType() == token::TokenType::BANG ||
      current.token.getTokenType() == token::TokenType::MINUS) {
    token::TokenType op = current.token.getTokenType();
    readLex();
    if (current.token.getTokenType() == token::TokenType::IDENTIFIER)
      expression = parseIdentifierLike();
    else if (current.token.getTokenType() == token::TokenType::LPARENT)
      expression = parseParentExpression();
    else if (current.token.getTokenType() == token::TokenType::HASH)
      expression = parseLambdaExpression();
    else if (current.token.getTokenType() == token::TokenType::LBRACKET)
      expression = parseDebugExpression();
    else
      expression = parseLiteral();
    return std::make_unique<node::PrefixExpression>(op, std::move(expression));
  }
  if (current.token.getTokenType() == token::TokenType::IDENTIFIER)
    expression = parseIdentifierLike();
  else if (current.token.getTokenType() == token::TokenType::LPARENT)
    expression = parseParentExpression();
  else if (current.token.getTokenType() == token::TokenType::HASH)
    expression = parseLambdaExpression();
  else if (current.token.getTokenType() == token::TokenType::LBRACKET)
    expression = parseDebugExpression();
  else
    expression = parseLiteral();
  return expression;
}

std::unique_ptr<node::Node> Parser::parseIdentifierLike() {
  std::string val = std::get<std::string>(*current.token.getValue());
  readLex();
  auto ident = std::make_unique<node::Identifier>(val);
  if (current.token.getTokenType() == token::TokenType::LPARENT)
    return parseCallExpression(std::move(ident));
  return ident;
}

std::unique_ptr<node::Node> Parser::parseCallArguments() {
  auto arguments = std::make_unique<node::CallArguments>();
  assert(current.token.getTokenType() == token::TokenType::LPARENT);
  readLex();
  while (current.token.getTokenType() != token::TokenType::RPARENT) {
    arguments->arguments.push_back(parseExpression());
    if (current.token.getTokenType() != token::TokenType::COMMA)
      break;
    readLex();
  }
  if (current.token.getTokenType() != token::TokenType::RPARENT)
    throw exception::ParserException(
        "Expected ')' in the end of call arguments.");
  readLex();
  return arguments;
}

std::unique_ptr<node::Node> Parser::parseParentExpression() {
  assert(current.token.getTokenType() == token::TokenType::LPARENT);
  readLex();
  auto pExpression =
      std::make_unique<node::ParentExpression>(parseExpression());
  if (current.token.getTokenType() != token::TokenType::RPARENT)
    throw exception::ParserException(
        "Expected ')' in the end of parent arguments.");

  readLex();
  if (current.token.getTokenType() == token::TokenType::LPARENT)
    return parseCallExpression(std::move(pExpression));
  return pExpression;
}

std::unique_ptr<node::Node>
Parser::parseCallExpression(std::unique_ptr<node::Node> function) {
  std::unique_ptr<node::Node> result = std::move(function);
  assert(current.token.getTokenType() == token::TokenType::LPARENT);
  do {
    std::unique_ptr<node::Node> callArguments = parseCallArguments();
    result = std::make_unique<node::CallExpression>(std::move(result),
                                                    std::move(callArguments));
  } while (current.token.getTokenType() == token::TokenType::LPARENT);
  return result;
}

std::unique_ptr<node::Node> Parser::parseDebugExpression() {
  assert(current.token.getTokenType() == token::TokenType::LBRACKET);
  readLex();
  auto function = parseExpression();
  if (current.token.getTokenType() != token::TokenType::RBRACKET)
    throw exception::ParserException("Expected ']' in debug expression.");
  readLex();

  std::unique_ptr<node::Node> result = std::move(function);
  if (current.token.getTokenType() != token::TokenType::LPARENT)
    throw exception::ParserException("Expected '(' in debug expression.");
  std::unique_ptr<node::Node> callArguments = parseCallArguments();
  result = std::make_unique<node::DebugExpression>(std::move(result),
                                                   std::move(callArguments));

  while (current.token.getTokenType() == token::TokenType::LPARENT) {
    std::unique_ptr<node::Node> callArguments = parseCallArguments();
    result = std::make_unique<node::CallExpression>(std::move(result),
                                                    std::move(callArguments));
  }
  return result;
}

std::unique_ptr<node::Node> Parser::parseLambdaExpression() {
  auto lambda = std::make_unique<node::LambdaExpression>();
  assert(current.token.getTokenType() == token::TokenType::HASH);
  readLex();
  if (current.token.getTokenType() != token::TokenType::LPARENT)
    throw exception::ParserException("Expected '(' in lambda expression.");

  auto declarationArguments = parseDeclarationArguments();
  lambda->arguments = std::move(declarationArguments);
  if (current.token.getTokenType() != token::TokenType::LBRACE)
    throw exception::ParserException("Expected '{' in lambda expression.");
  auto block = parseBlock();
  lambda->statements = std::move(block);
  if (current.token.getTokenType() == token::TokenType::LPARENT)
    return parseCallExpression(std::move(lambda));
  return lambda;
}

std::unique_ptr<node::Node> Parser::parseLiteral() {
  if (current.token.getTokenType() == token::TokenType::INTEGER) {
    return parseIntegerLiteral();
  } else if (current.token.getTokenType() == token::TokenType::FLOAT) {
    return parseFloatLiteral();
  } else if (current.token.getTokenType() == token::TokenType::STRING) {
    return parseStringLiteral();
  } else if (current.token.getTokenType() == token::TokenType::TRUE ||
             current.token.getTokenType() == token::TokenType::FALSE) {
    return parseBoolLiteral();
  }
  throw exception::ParserException("Literal type does not exist.");
}

std::unique_ptr<node::Node> Parser::parseIntegerLiteral() {
  types::Integer val = std::get<types::Integer>(*current.token.getValue());
  readLex();
  return std::make_unique<node::IntegerLiteral>(val);
}

std::unique_ptr<node::Node> Parser::parseFloatLiteral() {
  types::Float val = std::get<types::Float>(*current.token.getValue());
  readLex();
  return std::make_unique<node::FloatLiteral>(val);
}

std::unique_ptr<node::Node> Parser::parseBoolLiteral() {
  bool val = current.token.getTokenType() == token::TokenType::TRUE;
  readLex();
  return std::make_unique<node::BoolLiteral>(val);
}

std::unique_ptr<node::Node> Parser::parseStringLiteral() {
  std::string val = std::get<std::string>(*current.token.getValue());
  readLex();
  return std::make_unique<node::StringLiteral>(val);
}

std::unique_ptr<node::Node> Parser::parseTypeSpecifier() {
  auto type = types::TokenToType.find(current.token.getTokenType());
  if (type != types::TokenToType.end()) {
    readLex();
    return std::make_unique<node::TypeSpecifier>(type->second);
  }
  throw exception::ParserException("Expected type specifier.");
}

std::unique_ptr<node::Node> Parser::generateParsingTree() {
  std::optional<std::unique_ptr<node::Node>> maybeResult;
  maybeResult = parse();
  if (maybeResult)
    return std::move(*maybeResult);
  return nullptr; // add sensible handling of empty
}

std::optional<std::unique_ptr<node::Node>> Parser::parse() {
  return parseProgram();
}

void Parser::readLex() {
  if (current.token.getTokenType() == token::TokenType::END_OF_FILE) {
    return;
  }
  do {
    current = lexer.nextLexem();
  } while (current.token.getTokenType() == token::TokenType::ONE_LINE_COMMENT ||
           current.token.getTokenType() == token::TokenType::MULTILINE_COMMENT);
}

} // namespace pheonix::parser
