#include "lexer.hpp"
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace pheonix::lexer {

bool Lexem::operator==(const Lexem &l) const {
  return this->token == l.token && this->line == l.line &&
         this->column == l.column;
}
bool Lexem::operator==(const token::TokenType &t) const {
  return this->token.getTokenType() == t;
}
bool Lexem::operator!=(const token::TokenType &t) const {
  return !((*this) == t);
}

std::ostream &operator<<(std::ostream &os, const Lexem &l) {
  os << "Lexem(  token: `" << l.token << "`,\n\tline: `" << l.line
     << "`,\n\tcolumn: `" << l.column << "`,\n),";
  return os;
}

Lexem Lexer::nextLexem() {
  skipWhiteSpaces();
  if (auto result = tryEndOfFile())
    return result.value();
  if (auto result = trySlashOrToken())
    return result.value();
  if (auto result = tryParenthesis())
    return result.value();
  if (auto result = tryTwoCharOperator())
    return result.value();
  if (auto result = tryOneCharOperator())
    return result.value();
  if (auto result = trySeparator())
    return result.value();
  if (auto result = tryString())
    return result.value();
  return tryLiteralOrNotAToken();
}

std::optional<Lexem> Lexer::tryEndOfFile() {
  if (ch == EOF)
    return Lexem{token::Token(token::TokenType::END_OF_FILE), line, column};
  return std::nullopt;
}

std::optional<Lexem> Lexer::trySlashOrToken() {
  size_t sline = line;
  size_t scolumn = column;
  token::Token token;
  switch (ch) {
  case '/':
    switch (peek) {
    case '/':
      token = handleOnelineCommentToken(sline, scolumn);
      return Lexem{token, sline, scolumn};
    case '*':
      token = handleMultilineCommentToken(sline, scolumn);
      return Lexem{token, sline, scolumn};
    default:
      break;
    }
    readChar();
    return Lexem{token::Token(token::TokenType::SLASH), sline, scolumn};
  default:
    return std::nullopt;
  }
}

std::optional<Lexem> Lexer::tryParenthesis() {
  size_t sline = line;
  size_t scolumn = column;
  switch (ch) {
  case '(':
    readChar();
    return Lexem{token::Token(token::TokenType::LPARENT), sline, scolumn};
  case ')':
    readChar();
    return Lexem{token::Token(token::TokenType::RPARENT), sline, scolumn};
  case '{':
    readChar();
    return Lexem{token::Token(token::TokenType::LBRACE), sline, scolumn};
  case '}':
    readChar();
    return Lexem{token::Token(token::TokenType::RBRACE), sline, scolumn};
  case '[':
    readChar();
    return Lexem{token::Token(token::TokenType::LBRACKET), sline, scolumn};
  case ']':
    readChar();
    return Lexem{token::Token(token::TokenType::RBRACKET), sline, scolumn};
  default:
    return std::nullopt;
  }
}

std::optional<Lexem> Lexer::tryTwoCharOperator() {
  size_t sline = line;
  size_t scolumn = column;
  switch (ch) {
  case '<':
    readChar();
    if (ch == '=') {
      readChar();
      return Lexem{token::Token(token::TokenType::LEQ), sline, scolumn};
    }
    if (ch == '-') {
      readChar();
      return Lexem{token::Token(token::TokenType::LARROW), sline, scolumn};
    }
    return Lexem{token::Token(token::TokenType::LESS), sline, scolumn};
  case '>':
    readChar();
    if (ch == '=') {
      readChar();
      return Lexem{token::Token(token::TokenType::GEQ), sline, scolumn};
    }
    return Lexem{token::Token(token::TokenType::GREATER), sline, scolumn};
  case '=':
    readChar();
    if (ch == '=') {
      readChar();
      return Lexem{token::Token(token::TokenType::EQUALS), sline, scolumn};
    }
    return Lexem{token::Token(token::TokenType::ASSIGN), sline, scolumn};
  case '!':
    readChar();
    if (ch == '=') {
      readChar();
      return Lexem{token::Token(token::TokenType::NEQ), sline, scolumn};
    }
    return Lexem{token::Token(token::TokenType::BANG), sline, scolumn};
  case '&':
    readChar();
    if (ch == '&') {
      readChar();
      return Lexem{token::Token(token::TokenType::AND), sline, scolumn};
    }
    throw exception::NotAToken(sline, scolumn);
  case '|':
    readChar();
    if (ch == '|') {
      readChar();
      return Lexem{token::Token(token::TokenType::OR), sline, scolumn};
    }
    return Lexem{token::Token(token::TokenType::PIPE), sline, scolumn};
  default:
    return std::nullopt;
  }
}

std::optional<Lexem> Lexer::tryOneCharOperator() {
  size_t sline = line;
  size_t scolumn = column;
  switch (ch) {
  case '-':
    readChar();
    return Lexem{token::Token(token::TokenType::MINUS), sline, scolumn};
  case '+':
    readChar();
    return Lexem{token::Token(token::TokenType::PLUS), sline, scolumn};
  case '*':
    readChar();
    return Lexem{token::Token(token::TokenType::STAR), sline, scolumn};
  case '%':
    readChar();
    return Lexem{token::Token(token::TokenType::PERCENT), sline, scolumn};
  case '$':
    readChar();
    return Lexem{token::Token(token::TokenType::DOLAR), sline, scolumn};
  case '#':
    readChar();
    return Lexem{token::Token(token::TokenType::HASH), sline, scolumn};
  default:
    return std::nullopt;
  }
}

std::optional<Lexem> Lexer::trySeparator() {
  size_t sline = line;
  size_t scolumn = column;
  switch (ch) {
  case ';':
    readChar();
    return Lexem{token::Token(token::TokenType::SEMICOLON), sline, scolumn};
  case ',':
    readChar();
    return Lexem{token::Token(token::TokenType::COMMA), sline, scolumn};
  default:
    return std::nullopt;
  }
}
std::optional<Lexem> Lexer::tryString() {
  size_t sline = line;
  size_t scolumn = column;
  token::Token token;
  if (ch == '"') {
    token = handleString(sline, scolumn);
    return Lexem{token, sline, scolumn};
  }
  return std::nullopt;
}

Lexem Lexer::tryLiteralOrNotAToken() {
  size_t sline = line;
  size_t scolumn = column;
  token::Token token;
  if (!isalnum(ch)) {
    throw exception::NotAToken(sline, scolumn);
  }
  if (isdigit(ch)) {
    token = handleNumber(sline, scolumn);
    return Lexem{token, sline, scolumn};
  }
  token = handleIdentifier(sline, scolumn);
  return Lexem{token, sline, scolumn};
}

void Lexer::readChar() {
  if (ch == '\n') {
    // Linux
    ++line;
    column = 0;
  }
  if (ch == '\r') {
    // MacOS or Windows
    if (peek == '\n') {
      // Windows
      ++offset;
      ch = istream_.get();
      peek = istream_.peek();
    }
    ++line;
    column = 0;
  }

  if (ch == EOF) {
    peek = ch;
    return;
  }
  ch = istream_.get();
  peek = istream_.peek();
  ++offset;
  ++column;
}

void Lexer::skipWhiteSpaces() {
  while (true) {
    switch (ch) {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
      readChar();
      break;
    default:
      return;
    }
  }
}

token::Token Lexer::handleOnelineCommentToken(size_t row, size_t column) {
  std::string buffer = "";
  buffer += ch;
  readChar();
  while (buffer.size() <= COMMENT_MAX_SIZE) {
    switch (ch) {
    case '\r':
      if (peek == '\n')
        readChar();
      [[fallthrough]];
    case '\n':
    case EOF:
      readChar();
      return token::Token(token::TokenType::ONE_LINE_COMMENT, buffer);
      break;
    default:
      buffer += ch;
      readChar();
    }
  }
  throw exception::OneLineCommentTooLong(row, column);
}

token::Token Lexer::handleMultilineCommentToken(size_t row, size_t column) {
  std::string buffer = "";
  assert(ch == '/');
  buffer += ch;
  readChar();
  assert(ch == '*');
  buffer += ch;
  assert(buffer == "/*");
  readChar();
  while (buffer.size() <= COMMENT_MAX_SIZE) {
    switch (ch) {
    case '*':
      buffer += ch;
      readChar();
      if (ch == '/') {
        buffer += ch;
        readChar();
        return token::Token(token::TokenType::MULTILINE_COMMENT, buffer);
      }
      if (ch == EOF) {
        throw exception::UnfinishedMultilineComment(row, column);
      }
      buffer += ch;
      readChar();
      break;
    case EOF:
      throw exception::UnfinishedMultilineComment(row, column);
    default:
      buffer += ch;
      readChar();
    }
  }
  throw exception::MultilineCommentTooLong(row, column);
}

token::Token Lexer::handleIdentifier(size_t row, size_t column) {
  std::string buffer = "";
  buffer += ch;
  readChar();
  while ((isalnum(ch) || ch == '_') && buffer.size() <= IDENTIFIER_MAX_SIZE) {
    buffer += ch;
    readChar();
  }
  if (buffer.size() > IDENTIFIER_MAX_SIZE)
    throw exception::LexerException("Identifier too long.", row, column);

  if (auto result = types::stringToTokenType(buffer)) {
    if (result == token::TokenType::TRUE)
      return token::Token(*result, true);
    if (result == token::TokenType::FALSE)
      return token::Token(*result, false);
    return token::Token(*result);
  }

  return token::Token(token::TokenType::IDENTIFIER, buffer);
}

token::Token Lexer::handleNumber(size_t row, size_t column) {
  types::Integer integerPart;

  while (isdigit(ch)) {
    integerPart = integerPart * 10 + static_cast<long>(ch - '0');
    readChar();
  }
  if (ch == '.') {
    readChar();
    return handleFloat(row, column, integerPart.getValue());
  }
  if (isalpha(ch))
    throw exception::LexerException("Undefined value.", row, column);

  return token::Token(token::TokenType::INTEGER, integerPart);
}

token::Token Lexer::handleFloat(size_t row, size_t column, long intPart) {
  int fractionalPart = 0;
  int length = 0;

  while (isdigit(ch) && fractionalPart <= std::numeric_limits<int>::max()) {
    fractionalPart = 10 * fractionalPart + static_cast<long>(ch - '0');
    ++length;
    readChar();
  }
  if (isalpha(ch))
    throw exception::LexerException("Undefined value.", row, column);

  double result = static_cast<double>(intPart) +
                  static_cast<double>(fractionalPart) *
                      std::pow(10., static_cast<double>(-length));
  return token::Token(token::TokenType::FLOAT, result);
}

token::Token Lexer::handleString(size_t line, size_t column) {
  std::string buffer = "";
  assert(ch == '"');
  readChar();
  while (ch != '"' && ch != EOF && buffer.size() <= STRING_MAX_SIZE) {
    if (ch == '\\') {
      readChar();
      switch (ch) {
      case '\\':
        buffer += R"(\)";
        break;
      case '"':
        buffer += R"(")";
        break;
      case 'r':
        buffer += '\r';
        break;
      case 'n':
        buffer += '\n';
        break;
      case 't':
        buffer += '\t';
        break;
      default:
        throw exception::BackSlashInString(line, column);
      }
      readChar();
      continue;
    }
    buffer += ch;
    readChar();
  }

  if (ch == '"' && buffer.size() <= STRING_MAX_SIZE) {
    readChar();
    return token::Token(token::TokenType::STRING, buffer);
  }

  if (ch == EOF && buffer.size() <= STRING_MAX_SIZE) {
    throw exception::LexerException("Unfinished string literal.", line, column);
  }

  throw exception::LexerException("String literal to long.", line, column);
}

} // namespace pheonix::lexer
