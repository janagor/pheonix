#pragma once

#include "exception.hpp"
#include "helpers.hpp"
#include "token.hpp"

namespace pheonix::lexer {

static const int IDENTIFIER_MAX_SIZE = 1200;
static const int NUMERIC_MAX_SIZE = 1200;
static const int STRING_MAX_SIZE = 1200;
static const int COMMENT_MAX_SIZE = 1200;

struct Lexem {
  token::Token token;
  size_t line;
  size_t column;
  bool operator==(const Lexem &l) const;
  bool operator==(const token::TokenType &t) const;
  bool operator!=(const token::TokenType &t) const;
};
std::ostream &operator<<(std::ostream &os, const Lexem &l);

struct Lexer {
private:
  void skipWhiteSpaces();
  void readChar();
  token::Token handleOnelineCommentToken(size_t row, size_t column);
  token::Token handleMultilineCommentToken(size_t row, size_t column);
  token::Token handleNumber(size_t row, size_t column);
  token::Token handleFloat(size_t row, size_t column, long intPart);
  token::Token handleIdentifier(size_t row, size_t column);
  token::Token handleString(size_t row, size_t column);

  std::optional<Lexem> tryEndOfFile();
  std::optional<Lexem> trySlashOrToken();
  std::optional<Lexem> tryParenthesis();
  std::optional<Lexem> tryTwoCharOperator();
  std::optional<Lexem> tryOneCharOperator();
  std::optional<Lexem> trySeparator();
  std::optional<Lexem> tryString();
  Lexem tryLiteralOrNotAToken();

public:
  Lexer(std::istream &istream)
      : istream_(istream), offset(1), line(1), column(1) {
    ch = istream_.get();
    peek = istream_.peek();
  }

  Lexem nextLexem();

private:
  std::istream &istream_;
  char ch;
  char peek;
  size_t offset;
  size_t line;
  size_t column;
};

} // namespace pheonix::lexer
