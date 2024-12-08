#include "parser.hpp"

#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace pheonix::parser {

std::unique_ptr<node::Node> Parser::parseProgram() {
  auto program = std::make_unique<node::Program>();
  while (current.token.getTokenType() != types::TokenType::END_OF_FILE) {
    program->statements.push_back(parseStatement());
  }
  return program;
}

std::unique_ptr<node::Node> Parser::parseBlock() {
  auto block = std::make_unique<node::Block>();
  assert(current.token.getTokenType() == types::TokenType::LBRACE);
  readLex();
  while (current.token.getTokenType() != types::TokenType::RBRACE) {
    auto statement = parseStatement();
    block->statements.push_back(std::move(statement));
  }
  assert(current.token.getTokenType() == types::TokenType::RBRACE);
  readLex();

  return block;
}

std::unique_ptr<node::Node> Parser::parseStatement() {
  std::unique_ptr<node::Node> node;
  switch (current.token.getTokenType()) {
  case types::TokenType::LET:
    return parseVariableDeclaration();
  case types::TokenType::WHILE:
    return parseWhileLoopStatement();
  case types::TokenType::IF:
    return parseIfStatement();
  case types::TokenType::FN:
    return parseFunctionDeclaration();
  case types::TokenType::RETURN:
    return parseReturnStatement();
  default:
    return parseExpressionStatement();
  }
}

std::unique_ptr<node::Node> Parser::parseDeclarationArguments() {
  auto args = std::make_unique<node::DeclarationArguments>();
  assert(current.token.getTokenType() == types::TokenType::LPARENT);
  readLex();
  while (current.token.getTokenType() == types::TokenType::MUT ||
         current.token.getTokenType() == types::TokenType::IDENTIFIER) {
    bool isMutable = false;
    if (current.token.getTokenType() == types::TokenType::MUT) {
      isMutable = true;
      readLex();
    }
    std::string paramIdentifier =
        std::get<std::string>(*current.token.getValue());
    auto arg = std::make_unique<node::Parameter>(isMutable, paramIdentifier);
    args->arguments.push_back(std::move(arg));
    if (current.token.getTokenType() == types::TokenType::COMMA)
      readLex();
    readLex();
  }
  assert(current.token.getTokenType() == types::TokenType::RPARENT);
  readLex();
  return args;
}

std::unique_ptr<node::Node> Parser::parseFunctionDeclaration() {
  assert(current.token.getTokenType() == types::TokenType::FN);
  readLex();
  assert(current.token.getTokenType() == types::TokenType::IDENTIFIER);
  std::string identifier = std::get<std::string>(*current.token.getValue());
  auto functionDeclaration =
      std::make_unique<node::FunctionDeclaration>(identifier);
  readLex();
  assert(current.token.getTokenType() == types::TokenType::LPARENT);
  auto declarationArguments = parseDeclarationArguments();
  functionDeclaration->arguments = std::move(declarationArguments);
  assert(current.token.getTokenType() == types::TokenType::LBRACE);
  auto block = parseBlock();
  functionDeclaration->statements = std::move(block);
  return functionDeclaration;
}

std::unique_ptr<node::Node> Parser::parseVariableDeclaration() {
  assert(current.token.getTokenType() == types::TokenType::LET);
  readLex();
  bool isMutable = false;
  if (current.token.getTokenType() == types::TokenType::MUT) {
    isMutable = true;
    readLex();
  }
  assert(current.token.getTokenType() == types::TokenType::IDENTIFIER);
  std::string identifier = std::get<std::string>(*current.token.getValue());
  readLex();
  assert(current.token.getTokenType() == types::TokenType::ASSIGN);
  readLex();
  auto expression = parseExpressionStatement();
  return std::make_unique<node::VariableDeclaration>(isMutable, identifier,
                                                     std::move(expression));
}

std::unique_ptr<node::Node> Parser::parseWhileLoopStatement() {
  assert(current.token.getTokenType() == types::TokenType::WHILE);
  readLex();
  assert(current.token.getTokenType() == types::TokenType::LPARENT);
  readLex();
  auto expression = parseExpression();

  assert(current.token.getTokenType() == types::TokenType::RPARENT);
  readLex();
  auto block = parseBlock();

  auto whileLoopStmt = std::make_unique<node::WhileLoopStatement>(
      std::move(expression), std::move(block));
  return whileLoopStmt;
}

std::unique_ptr<node::Node> Parser::parseIfStatement() {

  assert(current.token.getTokenType() == types::TokenType::IF);
  readLex();
  assert(current.token.getTokenType() == types::TokenType::LPARENT);
  readLex();
  auto predicate = parseExpression();
  auto ifStmt = std::make_unique<node::IfStatement>(std::move(predicate));
  assert(current.token.getTokenType() == types::TokenType::RPARENT);
  readLex();
  auto ifBody = parseBlock();
  ifStmt->ifBody = std::move(ifBody);
  if (current.token.getTokenType() == types::TokenType::ELSE) {
    readLex();
    if (current.token.getTokenType() == types::TokenType::IF) {
      auto elseIfStmt = parseIfStatement();
      ifStmt->elseBody = std::move(elseIfStmt);
    } else {
      assert(current.token.getTokenType() == types::TokenType::LBRACE);
      auto elseBody = parseBlock();
      ifStmt->elseBody = std::move(elseBody);
    }
  }
  return ifStmt;
}

std::unique_ptr<node::Node> Parser::parseReturnStatement() {
  assert(current.token.getTokenType() == types::TokenType::RETURN);
  readLex();
  auto expression = parseExpressionStatement();
  return std::make_unique<node::ReturnStatement>(std::move(expression));
}

std::unique_ptr<node::Node> Parser::parseExpressionStatement() {
  auto expression = parseExpression();
  assert(current.token.getTokenType() == types::TokenType::SEMICOLON);
  readLex();
  return std::make_unique<node::ExpressionStatement>(std::move(expression));
}

std::unique_ptr<node::Node> Parser::parseExpression() {
  if (current.token.getTokenType() == types::TokenType::DOLAR) {
    return parseAssignementExpression();
  }
  return parseOrExpression();
}

std::unique_ptr<node::Node> Parser::parseAssignementExpression() {
  readLex();
  assert(current.token.getTokenType() == types::TokenType::IDENTIFIER);
  std::string identifier = std::get<std::string>(*current.token.getValue());
  readLex();
  readLex();
  assert(current.token.getTokenType() == types::TokenType::INTEGER);
  auto expression = parseOrExpression();
  return std::make_unique<node::AssignementExpression>(identifier,
                                                       std::move(expression));
}

std::unique_ptr<node::Node> Parser::parseOrExpression() {
  auto left = parseAndExpression();
  while (current.token.getTokenType() == types::TokenType::OR) {
    types::TokenType op = current.token.getTokenType();
    readLex();
    auto right = parseAndExpression();
    left = std::make_unique<node::OrExpression>(std::move(left),
                                                std::move(right), op);
  }
  return left;
}

std::unique_ptr<node::Node> Parser::parseAndExpression() {
  auto left = parseComparisonExpression();
  while (current.token.getTokenType() == types::TokenType::AND) {
    types::TokenType op = current.token.getTokenType();
    readLex();
    auto right = parseComparisonExpression();
    left = std::make_unique<node::AndExpression>(std::move(left),
                                                 std::move(right), op);
  }
  return left;
}
std::unique_ptr<node::Node> Parser::parseComparisonExpression() {
  auto left = parseRelationalExpression();
  while (current.token.getTokenType() == types::TokenType::EQUALS ||
         current.token.getTokenType() == types::TokenType::NEQ) {
    types::TokenType op = current.token.getTokenType();
    readLex();
    auto right = parseRelationalExpression();
    left = std::make_unique<node::ComparisonExpression>(std::move(left),
                                                        std::move(right), op);
  }
  return left;
}

std::unique_ptr<node::Node> Parser::parseRelationalExpression() {
  auto left = parseAdditiveExpression();
  while (current.token.getTokenType() == types::TokenType::LESS ||
         current.token.getTokenType() == types::TokenType::GREATER ||
         current.token.getTokenType() == types::TokenType::LEQ ||
         current.token.getTokenType() == types::TokenType::GEQ) {
    types::TokenType op = current.token.getTokenType();
    readLex();
    auto right = parseAdditiveExpression();
    left = std::make_unique<node::RelationalExpression>(std::move(left),
                                                        std::move(right), op);
  }
  return left;
}

std::unique_ptr<node::Node> Parser::parseAdditiveExpression() {
  auto left = parseMultiplicativeExpression();
  while (current.token.getTokenType() == types::TokenType::PLUS ||
         current.token.getTokenType() == types::TokenType::MINUS) {
    types::TokenType op = current.token.getTokenType();
    readLex();
    auto right = parseMultiplicativeExpression();
    left = std::make_unique<node::AdditiveExpression>(std::move(left),
                                                      std::move(right), op);
  }
  return left;
}

std::unique_ptr<node::Node> Parser::parseMultiplicativeExpression() {
  std::unique_ptr<node::Node> left = parseCompositiveExpression();
  while (current.token.getTokenType() == types::TokenType::STAR ||
         current.token.getTokenType() == types::TokenType::SLASH ||
         current.token.getTokenType() == types::TokenType::PERCENT) {
    types::TokenType op = current.token.getTokenType();
    readLex();
    std::unique_ptr<node::Node> right = parseCompositiveExpression();
    left = std::make_unique<node::MultiplicativeExpression>(
        std::move(left), std::move(right), op);
  }
  return left;
}

std::unique_ptr<node::Node> Parser::parseCompositiveExpression() {
  std::unique_ptr<node::Node> left = parseCastExpression();
  while (current.token.getTokenType() == types::TokenType::PIPE) {
    readLex();
    std::unique_ptr<node::Node> right = parseCastExpression();
    left = std::make_unique<node::CompositiveExpression>(std::move(left),
                                                         std::move(right));
  }
  return left;
}

std::unique_ptr<node::Node> Parser::parseCastExpression() {
  std::unique_ptr<node::Node> expression = parsePrefixExpression();
  while (current.token.getTokenType() == types::TokenType::LARROW) {
    readLex();
    std::unique_ptr<node::Node> type = parseTypeSpecifier();
    return std::make_unique<node::CastExpression>(std::move(expression),
                                                  std::move(type));
  }
  return expression;
}

std::unique_ptr<node::Node> Parser::parsePrefixExpression() {
  std::unique_ptr<node::Node> expression;
  if (current.token.getTokenType() == types::TokenType::BANG ||
      current.token.getTokenType() == types::TokenType::MINUS) {
    types::TokenType op = current.token.getTokenType();
    readLex();
    if (current.token.getTokenType() == types::TokenType::IDENTIFIER)
      expression = parseIdentifierLike();
    else if (current.token.getTokenType() == types::TokenType::LPARENT)
      expression = parseParentExpression();
    else if (current.token.getTokenType() == types::TokenType::HASH)
      expression = parseLambdaExpression();
    else if (current.token.getTokenType() == types::TokenType::LBRACKET)
      expression = parseDebugExpression();
    else
      expression = parseLiteral();
    return std::make_unique<node::PrefixExpression>(op, std::move(expression));
  }
  if (current.token.getTokenType() == types::TokenType::IDENTIFIER)
    expression = parseIdentifierLike();
  else if (current.token.getTokenType() == types::TokenType::LPARENT)
    expression = parseParentExpression();
  else if (current.token.getTokenType() == types::TokenType::HASH)
    expression = parseLambdaExpression();
  else if (current.token.getTokenType() == types::TokenType::LBRACKET)
    expression = parseDebugExpression();
  else
    expression = parseLiteral();
  return expression;
}

std::unique_ptr<node::Node> Parser::parseIdentifierLike() {
  std::string val = std::get<std::string>(*current.token.getValue());
  readLex();
  auto ident = std::make_unique<node::Identifier>(val);
  if (current.token.getTokenType() == types::TokenType::LPARENT)
    return parseCallExpression(std::move(ident));
  return ident;
}

std::unique_ptr<node::Node> Parser::parseCallArguments() {
  auto arguments = std::make_unique<node::CallArguments>();
  assert(current.token.getTokenType() == types::TokenType::LPARENT);
  readLex();
  while (current.token.getTokenType() != types::TokenType::RPARENT) {
    arguments->arguments.push_back(parseExpression());
    if (current.token.getTokenType() != types::TokenType::COMMA)
      break;
    readLex();
  }
  assert(current.token.getTokenType() == types::TokenType::RPARENT);
  readLex();
  return arguments;
}

std::unique_ptr<node::Node> Parser::parseParentExpression() {
  assert(current.token.getTokenType() == types::TokenType::LPARENT);
  readLex();
  auto pExpression =
      std::make_unique<node::ParentExpression>(parseExpression());
  assert(current.token.getTokenType() == types::TokenType::RPARENT);
  readLex();
  if (current.token.getTokenType() == types::TokenType::LPARENT)
    return parseCallExpression(std::move(pExpression));
  return pExpression;
}

std::unique_ptr<node::Node>
Parser::parseCallExpression(std::unique_ptr<node::Node> function) {
  std::unique_ptr<node::Node> result = std::move(function);
  assert(current.token.getTokenType() == types::TokenType::LPARENT);
  do {
    std::unique_ptr<node::Node> callArguments = parseCallArguments();
    result = std::make_unique<node::CallExpression>(std::move(result),
                                                    std::move(callArguments));
  } while (current.token.getTokenType() == types::TokenType::LPARENT);
  return result;
}

std::unique_ptr<node::Node> Parser::parseDebugExpression() {
  assert(current.token.getTokenType() == types::TokenType::LBRACKET);
  readLex();
  auto function = parseExpression();
  assert(current.token.getTokenType() == types::TokenType::RBRACKET);
  readLex();

  std::unique_ptr<node::Node> result = std::move(function);
  assert(current.token.getTokenType() == types::TokenType::LPARENT);
  std::unique_ptr<node::Node> callArguments = parseCallArguments();
  result = std::make_unique<node::DebugExpression>(std::move(result),
                                                   std::move(callArguments));

  while (current.token.getTokenType() == types::TokenType::LPARENT) {
    std::unique_ptr<node::Node> callArguments = parseCallArguments();
    result = std::make_unique<node::CallExpression>(std::move(result),
                                                    std::move(callArguments));
  }
  return result;
}

std::unique_ptr<node::Node> Parser::parseLambdaExpression() {
  auto lambda = std::make_unique<node::LambdaExpression>();
  assert(current.token.getTokenType() == types::TokenType::HASH);
  readLex();
  assert(current.token.getTokenType() == types::TokenType::LPARENT);
  auto declarationArguments = parseDeclarationArguments();
  lambda->arguments = std::move(declarationArguments);
  assert(current.token.getTokenType() == types::TokenType::LBRACE);
  auto block = parseBlock();
  lambda->statements = std::move(block);
  if (current.token.getTokenType() == types::TokenType::LPARENT)
    return parseCallExpression(std::move(lambda));
  return lambda;
}

std::unique_ptr<node::Node> Parser::parseLiteral() {
  if (current.token.getTokenType() == types::TokenType::INTEGER) {
    return parseIntegerLiteral();
  } else if (current.token.getTokenType() == types::TokenType::FLOAT) {
    return parseFloatLiteral();
  } else if (current.token.getTokenType() == types::TokenType::STRING) {
    return parseStringLiteral();
  } else if (current.token.getTokenType() == types::TokenType::TRUE ||
             current.token.getTokenType() == types::TokenType::FALSE

  ) {
    return parseBoolLiteral();
  }
  throw std::runtime_error("Literal type does not exist.");
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
  bool val = current.token.getTokenType() == types::TokenType::TRUE;
  readLex();
  return std::make_unique<node::BoolLiteral>(val);
}

std::unique_ptr<node::Node> Parser::parseStringLiteral() {
  std::string val = std::get<std::string>(*current.token.getValue());
  readLex();
  return std::make_unique<node::StringLiteral>(val);
}

std::unique_ptr<node::Node> Parser::parseTypeSpecifier() {
  // TypeName::iterator type = TokenToType.at(current.token.getTokenType());
  auto type = types::TokenToType.find(current.token.getTokenType());
  if (type != types::TokenToType.end()) {
    readLex();
    return std::make_unique<node::TypeSpecifier>(type->second);
  }
  // TODO: ERROR HANDLING
  return std::make_unique<node::TypeSpecifier>(types::TokenType::STR);
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
  if (current.token.getTokenType() == types::TokenType::END_OF_FILE) {
    return;
  }
  current = lexer.nextLexem();
}

} // namespace pheonix::parser
