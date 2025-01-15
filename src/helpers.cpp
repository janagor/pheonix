#include "helpers.hpp"

namespace pheonix::types {

std::string tokenTypeToLiteral(token::TokenType t) {
  switch (t) {
  case token::TokenType::END_OF_FILE:
    return "END_OF_FILE";
  case token::TokenType::ONE_LINE_COMMENT:
    return "ONE_LINE_COMMENT";
  case token::TokenType::MULTILINE_COMMENT:
    return "MULTILINE_COMMENT";
  case token::TokenType::ASSIGN:
    return "ASSIGN";
  case token::TokenType::BANG:
    return "BANG";
  case token::TokenType::PLUS:
    return "PLUS";
  case token::TokenType::MINUS:
    return "MINUS";
  case token::TokenType::STAR:
    return "STAR";
  case token::TokenType::SLASH:
    return "SLASH";
  case token::TokenType::PERCENT:
    return "PERCENT";
  case token::TokenType::HASH:
    return "HASH";
  case token::TokenType::DOLAR:
    return "DOLAR";
  case token::TokenType::SEMICOLON:
    return "SEMICOLON";
  case token::TokenType::COMMA:
    return "COMMA";
  case token::TokenType::PIPE:
    return "PIPE";
  case token::TokenType::LPARENT:
    return "LPARENT";
  case token::TokenType::RPARENT:
    return "RPARENT";
  case token::TokenType::LBRACE:
    return "LBRACE";
  case token::TokenType::RBRACE:
    return "RBRACE";
  case token::TokenType::LBRACKET:
    return "LBRACKET";
  case token::TokenType::RBRACKET:
    return "RBRACKET";
  case token::TokenType::AND:
    return "AND";
  case token::TokenType::OR:
    return "OR";
  case token::TokenType::EQUALS:
    return "EQUALS";
  case token::TokenType::NEQ:
    return "NEQ";
  case token::TokenType::LEQ:
    return "LEQ";
  case token::TokenType::GEQ:
    return "GEQ";
  case token::TokenType::GREATER:
    return "GREATER";
  case token::TokenType::LESS:
    return "LESS";
  case token::TokenType::LARROW:
    return "LARROW";
  case token::TokenType::IDENTIFIER:
    return "IDENTIFIER";
  case token::TokenType::STRING:
    return "STRING";
  case token::TokenType::INTEGER:
    return "INTEGER";
  case token::TokenType::FLOAT:
    return "FLOAT";
  case token::TokenType::FN:
    return "FN";
  case token::TokenType::LET:
    return "LET";
  case token::TokenType::MUT:
    return "MUT";
  case token::TokenType::RETURN:
    return "RETURN";
  case token::TokenType::IF:
    return "IF";
  case token::TokenType::ELSE:
    return "ELSE";
  case token::TokenType::WHILE:
    return "WHILE";
  case token::TokenType::INT:
    return "INT";
  case token::TokenType::STR:
    return "STR";
  case token::TokenType::FLT:
    return "FLT";
  case token::TokenType::BOL:
    return "BOL";
  case token::TokenType::TRUE:
    return "TRUE";
  case token::TokenType::FALSE:
    return "FALSE";
  case token::TokenType::NOT_A_TOKEN:
    return "NOT_A_TOKEN";
  };
  throw std::runtime_error("the token was not properly handled");
}

std::optional<token::TokenType> Keywords(const std::string &input) {
  if (input == "fn")
    return token::TokenType::FN;
  if (input == "let")
    return token::TokenType::LET;
  if (input == "mut")
    return token::TokenType::MUT;
  if (input == "return")
    return token::TokenType::RETURN;
  if (input == "if")
    return token::TokenType::IF;
  if (input == "else")
    return token::TokenType::ELSE;
  if (input == "while")
    return token::TokenType::WHILE;
  if (input == "int")
    return token::TokenType::INT;
  if (input == "str")
    return token::TokenType::STR;
  if (input == "bol")
    return token::TokenType::BOL;
  if (input == "flt")
    return token::TokenType::FLT;
  if (input == "true")
    return token::TokenType::TRUE;
  if (input == "false")
    return token::TokenType::FALSE;
  return {};
}

std::optional<token::TokenType> stringToTokenType(std::string &word) {
  auto token = types::Keywords(word);
  if (token.has_value()) {
    return token.value();
  }
  return {};
}

std::string opToString(const token::TokenType &tok) {
  switch (tok) {
  case token::TokenType::EQUALS:
    return "==";
    break;
  case token::TokenType::NEQ:
    return "!=";
    break;
  case token::TokenType::PLUS:
    return "+";
    break;
  case token::TokenType::MINUS:
    return "-";
    break;
  case token::TokenType::BANG:
    return "!";
    break;
  case token::TokenType::STAR:
    return "*";
    break;
  case token::TokenType::SLASH:
    return "/";
    break;
  case token::TokenType::PERCENT:
    return "%";
    break;
  case token::TokenType::LEQ:
    return "<=";
    break;
  case token::TokenType::LESS:
    return "<";
    break;
  case token::TokenType::GEQ:
    return ">=";
    break;
  case token::TokenType::GREATER:
    return ">";
    break;
  default:
    return "";
  }
}
} // namespace pheonix::types
