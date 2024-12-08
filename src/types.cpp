#include "types.hpp"

#include <limits>
#include <map>
#include <stdexcept>

namespace pheonix::types {
namespace {

const std::map<TokenType, std::string> TokenTypeToLiteral{
    {END_OF_FILE, "END_OF_FILE"},

    {ONE_LINE_COMMENT, "ONE_LINE_COMMENT"},
    {MULTILINE_COMMENT, "MULTILINE_COMMENT"},

    {ASSIGN, "ASSIGN"},
    {BANG, "BANG"},
    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {STAR, "STAR"},
    {SLASH, "SLASH"},
    {PERCENT, "PERCENT"},
    {HASH, "HASH"},
    {DOLAR, "DOLAR"},
    {SEMICOLON, "SEMICOLON"},
    {COMMA, "COMMA"},
    {PIPE, "PIPE"},

    {LPARENT, "LPARENT"},
    {RPARENT, "RPARENT"},
    {LBRACE, "LBRACE"},
    {RBRACE, "RBRACE"},
    {LBRACKET, "LBRACKET"},
    {RBRACKET, "RBRACKET"},
    {AND, "AND"},
    {OR, "OR"},
    {EQUALS, "EQUALS"},
    {NEQ, "NEQ"},
    {LEQ, "LEQ"},
    {GEQ, "GEQ"},
    {GREATER, "GREATER"},
    {LESS, "LESS"},
    {LARROW, "LARROW"},

    {IDENTIFIER, "IDENTIFIER"},
    {STRING, "STRING"},
    {INTEGER, "INTEGER"},
    {FLOAT, "FLOAT"},

    {FN, "FN"},
    {LET, "LET"},
    {MUT, "MUT"},
    {RETURN, "RETURN"},
    {IF, "IF"},
    {ELSE, "ELSE"},
    {WHILE, "WHILE"},
    {INT, "INT"},
    {STR, "STR"},
    {FLT, "FLT"},
    {BOL, "BOL"},
    {TRUE, "TRUE"},
    {FALSE, "FALSE"},

    {NOT_A_TOKEN, "NOT_A_TOKEN"},
};

static const std::map<std::string, TokenType> Keywords = {
    {"fn", FN},     {"let", LET},     {"mut", MUT},     {"return", RETURN},

    {"if", IF},     {"else", ELSE},   {"while", WHILE},

    {"int", INT},   {"str", STR},     {"bol", BOL},     {"flt", FLT},

    {"true", TRUE}, {"false", FALSE},
};

} // namespace

std::optional<types::TokenType> searchForKeyword(std::string &word) {
  auto token = types::Keywords.find(word);
  if (token != Keywords.end()) {
    return token->second;
  }
  return {};
}

// TODO: make it a map
std::string opToString(const types::TokenType &tok) {
  switch (tok) {
  case TokenType::EQUALS:
    return "==";
    break;
  case TokenType::NEQ:
    return "!=";
    break;
  case TokenType::PLUS:
    return "+";
    break;
  case TokenType::MINUS:
    return "-";
    break;
  case TokenType::BANG:
    return "!";
    break;
  case TokenType::STAR:
    return "*";
    break;
  case TokenType::SLASH:
    return "/";
    break;
  case TokenType::PERCENT:
    return "%";
    break;
  case TokenType::LEQ:
    return "<=";
    break;
  case TokenType::LESS:
    return "<";
    break;
  case TokenType::GEQ:
    return ">=";
    break;
  case TokenType::GREATER:
    return ">";
    break;
  default:
    return "";
  }
}

Integer &Integer::operator=(int val) {
  value = val;
  return *this;
}

Integer &Integer::operator=(const Integer &val) {
  value = val.value;
  return *this;
}

Integer &Integer::operator*=(int val) {
  setValue(static_cast<long>(value) * static_cast<long>(val));
  return *this;
}
Integer &Integer::operator/=(int val) {
  if (val == 0)
    throw std::runtime_error("Division by zero");
  setValue(static_cast<long>(value) / static_cast<long>(val));
  return *this;
}
Integer &Integer::operator+=(int val) {
  setValue(static_cast<long>(value) + static_cast<long>(val));
  return *this;
}
Integer &Integer::operator-=(int val) {
  setValue(static_cast<long>(value) - static_cast<long>(val));
  return *this;
}
Integer &Integer::operator%=(int val) {
  if (val == 0)
    throw std::runtime_error("Modulo by zero");
  setValue(static_cast<long>(value) % static_cast<long>(val));
  return *this;
}

Integer &Integer::operator*=(const Integer &val) {
  return (*this) *= val.value;
}
Integer &Integer::operator/=(const Integer &val) {
  return (*this) /= val.value;
}
Integer &Integer::operator+=(const Integer &val) {
  return (*this) += val.value;
}
Integer &Integer::operator-=(const Integer &val) {
  return (*this) -= val.value;
}

Integer Integer::operator*(int val) const {
  Integer res(*this);
  return res *= val;
}
Integer Integer::operator/(int val) const {
  Integer res(*this);
  return res /= val;
}
Integer Integer::operator+(int val) const {
  Integer res(*this);
  return res += val;
}
Integer Integer::operator-(int val) const {
  Integer res(*this);
  return res -= val;
}
Integer Integer::operator%(int val) const {
  Integer res(*this);
  return res %= val;
}

Integer Integer::operator*(const Integer &val) const {
  return (*this) * val.value;
}
Integer Integer::operator/(const Integer &val) const {
  return (*this) / val.value;
}
Integer Integer::operator+(const Integer &val) const {
  return (*this) + val.value;
}
Integer Integer::operator-(const Integer &val) const {
  return (*this) - val.value;
}
Integer Integer::operator%(const Integer &val) const {
  return (*this) % val.value;
}

bool Integer::operator==(const Integer &other) const {
  return value == other.value;
}

bool Integer::operator!=(const Integer &other) const {
  return value != other.value;
}

bool Integer::operator<(const Integer &other) const {
  return value < other.value;
}

bool Integer::operator<=(const Integer &other) const {
  return value <= other.value;
}

bool Integer::operator>(const Integer &other) const {
  return value > other.value;
}

bool Integer::operator>=(const Integer &other) const {
  return value >= other.value;
}

int Integer::getValue() const { return value; }

void Integer::setValue(long val) {
  if (val > std::numeric_limits<int>::max() ||
      val < std::numeric_limits<int>::min()) {
    throw std::runtime_error("Integer literal out of range.");
  }
  value = static_cast<int>(val);
}

std::ostream &operator<<(std::ostream &os, const Integer &obj) {
  os << obj.value;
  return os;
}

Float &Float::operator=(double val) {
  value = val;
  return *this;
}

Float &Float::operator=(const Float &val) {
  value = val.value;
  return *this;
}

Float &Float::operator*=(double val) {
  setValue(value * val);
  return *this;
}
Float &Float::operator/=(double val) {
  if (val == 0.0)
    throw std::runtime_error("Division by zero");
  setValue(value / val);
  return *this;
}
Float &Float::operator+=(double val) {
  setValue(value + val);
  return *this;
}
Float &Float::operator-=(double val) {
  setValue(value - val);
  return *this;
}

Float &Float::operator*=(const Float &val) { return (*this) *= val.value; }
Float &Float::operator/=(const Float &val) { return (*this) /= val.value; }
Float &Float::operator+=(const Float &val) { return (*this) += val.value; }
Float &Float::operator-=(const Float &val) { return (*this) -= val.value; }

Float Float::operator*(double val) const {
  Float res(*this);
  return res *= val;
}
Float Float::operator/(double val) const {
  Float res(*this);
  return res /= val;
}
Float Float::operator+(double val) const {
  Float res(*this);
  return res += val;
}
Float Float::operator-(double val) const {
  Float res(*this);
  return res -= val;
}

Float Float::operator*(const Float &val) const { return (*this) * val.value; }
Float Float::operator/(const Float &val) const { return (*this) / val.value; }
Float Float::operator+(const Float &val) const { return (*this) + val.value; }
Float Float::operator-(const Float &val) const { return (*this) - val.value; }

bool Float::operator==(const Float &other) const {
  return value == other.value;
}

bool Float::operator!=(const Float &other) const {
  return value != other.value;
}

bool Float::operator<(const Float &other) const { return value < other.value; }

bool Float::operator<=(const Float &other) const {
  return value <= other.value;
}

bool Float::operator>(const Float &other) const { return value > other.value; }

bool Float::operator>=(const Float &other) const {
  return value >= other.value;
}

double Float::getValue() const { return value; }

void Float::setValue(double val) { value = val; }

std::ostream &operator<<(std::ostream &os, const Float &obj) {
  os << obj.getValue();
  return os;
}

} // namespace pheonix::types
