#include "token.hpp"

#include <map>

namespace pheonix::token {

types::TokenType Token::getTokenType() const { return tokenType; }

std::optional<std::variant<types::Integer, types::Float, std::string>>
Token::getValue() const {
  return value;
}

bool Token::operator==(const Token &t) const {
  return this->tokenType == t.tokenType && this->value == t.value;
}

std::ostream &operator<<(
    std::ostream &os,
    const std::optional<std::variant<types::Integer, types::Float, std::string>>
        &opt) {
  if (opt) {
    std::visit([&os](const auto &value) { os << value; }, *opt);
  } else {
    os << "[]";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, const Token &t) {
  os << "Token(\n\t\ttokenType:``"
     << types::TokenTypeToLiteral.at(t.getTokenType()) << "``, \n\t\tvalue:``"
     << t.getValue() << "``,\n\t)";
  return os;
}

} // namespace pheonix::token
