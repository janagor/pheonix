#include "token.hpp"

namespace pheonix::token {

token::TokenType Token::getTokenType() const { return tokenType; }

std::optional<std::variant<types::Integer, types::Float, std::string, bool>>
Token::getValue() const {
  return value;
}

bool Token::operator==(const Token &t) const {
  return this->tokenType == t.tokenType && this->value == t.value;
}

std::ostream &operator<<(
    std::ostream &os,
    const std::optional<
        std::variant<types::Integer, types::Float, std::string, bool>> &opt) {
  if (opt) {
    const auto &value = *opt;
    if (std::holds_alternative<types::Integer>(value)) {
      os << std::get<types::Integer>(value).getValue();
    } else if (std::holds_alternative<types::Float>(value)) {
      os << std::get<types::Float>(value).getValue();
    } else if (std::holds_alternative<std::string>(value)) {
      os << std::get<std::string>(value);
    } else if (std::holds_alternative<bool>(value)) {
      os << (std::get<bool>(value) ? "true" : "false");
    }
  } else {
    os << "[]";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, const Token &t) {
  os << "Token(\n\t\ttokenType:``"
     << types::tokenTypeToLiteral(t.getTokenType()) << "``, \n\t\tvalue:``"
     << t.getValue() << "``,\n\t)";
  return os;
}

} // namespace pheonix::token
