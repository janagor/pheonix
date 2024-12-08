#pragma once

#include "types.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <variant>

namespace pheonix::token {

struct Token {
  Token() : tokenType(types::NOT_A_TOKEN), value(std::nullopt) {}
  Token(types::TokenType t) : tokenType(t), value(std::nullopt) {}
  Token(types::TokenType t, std::string val) : tokenType(t), value(val) {}
  Token(types::TokenType t, char val)
      : tokenType(t), value(std::string(1, val)) {}
  Token(types::TokenType t, const types::Integer &val)
      : tokenType(t), value(val) {}
  Token(types::TokenType t, int val)
      : tokenType(t), value(types::Integer(val)) {}
  Token(types::TokenType t, double val)
      : tokenType(t), value(types::Float(val)) {}
  Token(types::TokenType t, const types::Float &val)
      : tokenType(t), value(val) {}
  bool operator==(const Token &t) const;
  types::TokenType getTokenType() const;
  std::optional<std::variant<types::Integer, types::Float, std::string>>
  getValue() const;

private:
  types::TokenType tokenType;
  std::optional<std::variant<types::Integer, types::Float, std::string>> value;
};
std::ostream &operator<<(
    std::ostream &os,
    const std::optional<std::variant<types::Integer, types::Float, std::string>>
        &opt);
std::ostream &operator<<(std::ostream &os, const Token &t);

} // namespace pheonix::token
