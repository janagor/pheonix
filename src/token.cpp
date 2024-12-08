#include "token.hpp"

namespace token {

namespace {
static const std::map<TokenType, std::string> TokenTypeToLiteral{
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

TokenType Token::getTokenType() const { return tokenType; }

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
  os << "Token(\n\t\ttokenType:``" << TokenTypeToLiteral.at(t.getTokenType())
     << "``, \n\t\tvalue:``" << t.getValue() << "``,\n\t)";
  return os;
}

std::optional<TokenType> searchForKeyword(std::string &word) {
  auto token = Keywords.find(word);
  if (token != Keywords.end()) {
    return token->second;
  }
  return {};
}

} // namespace token
