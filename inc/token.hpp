#pragma once

#include "helpers.hpp"
#include "token_type.hpp"
#include "types.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <variant>

namespace pheonix::token {

struct Token {
  Token() : tokenType(token::TokenType::NOT_A_TOKEN), value(std::nullopt) {}
  Token(TokenType t) : tokenType(t), value(std::nullopt) {}
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
  std::optional<std::variant<bool, types::Integer, types::Float, std::string>>
  getValue() const;

private:
  token::TokenType tokenType;
  std::optional<std::variant<bool, types::Integer, types::Float, std::string>>
      value;
};
std::ostream &operator<<(
    std::ostream &os,
    const std::optional<
        std::variant<bool, types::Integer, types::Float, std::string>> &opt);
std::ostream &operator<<(std::ostream &os, const Token &t);

} // namespace pheonix::token
