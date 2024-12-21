#pragma once

#include "helpers.hpp"
#include "token_type.hpp"
#include "types.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <variant>

namespace pheonix {
using Primitive = std::variant<std::monostate, types::Integer, types::Float,
                               std::string, bool>;
}

namespace pheonix::token {

struct Token {
  Token() : tokenType(token::TokenType::NOT_A_TOKEN), value() {}
  Token(TokenType t) : tokenType(t), value() {}
  Token(TokenType t, const std::string &val) : tokenType(t), value(val) {}
  Token(TokenType t, const char *val) : tokenType(t), value(std::string(val)) {}
  Token(TokenType t, bool val) : tokenType(t), value(val) {}
  Token(TokenType t, char val) : tokenType(t), value(std::string(1, val)) {}
  Token(TokenType t, const types::Integer &val) : tokenType(t), value(val) {}
  Token(TokenType t, int val) : tokenType(t), value(types::Integer(val)) {}
  Token(TokenType t, double val) : tokenType(t), value(types::Float(val)) {}
  Token(TokenType t, const types::Float &val) : tokenType(t), value(val) {}
  bool operator==(const Token &t) const;
  token::TokenType getTokenType() const;
  Primitive getValue() const;

private:
  token::TokenType tokenType;
  Primitive value;
};
std::ostream &operator<<(std::ostream &os, Primitive &opt);
std::ostream &operator<<(std::ostream &os, const Token &t);

} // namespace pheonix::token
