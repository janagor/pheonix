#include "token.hpp"

namespace pheonix {

std::ostream &operator<<(std::ostream &os, const Primitive &value) {
  if (std::holds_alternative<types::Integer>(value)) {
    os << std::get<types::Integer>(value).getValue();
  } else if (std::holds_alternative<types::Float>(value)) {
    os << std::get<types::Float>(value).getValue();
  } else if (std::holds_alternative<std::string>(value)) {
    os << std::get<std::string>(value);
  } else if (std::holds_alternative<bool>(value)) {
    os << (std::get<bool>(value) ? "true" : "false");
  } else {
    os << "[]";
  }
  return os;
}

} // namespace pheonix

namespace pheonix::token {

TokenType Token::getTokenType() const { return tokenType; }

Primitive Token::getValue() const { return value; }

bool Token::operator==(const Token &t) const {
  return this->tokenType == t.tokenType && this->value == t.value;
}

std::ostream &operator<<(std::ostream &os, const Token &t) {
  using pheonix::operator<<; // NOTE: Why do I need it explicitely?
  os << "Token(\n\t\ttokenType:``"
     << types::tokenTypeToLiteral(t.getTokenType()) << "``, \n\t\tvalue:``"
     << t.getValue() << "``,\n\t)";
  return os;
}

} // namespace pheonix::token
