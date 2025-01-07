#include <cstdlib>
#include <format>
#include <stdexcept>

namespace pheonix::exception {
// Lexer
struct LexerException : public std::runtime_error {
  LexerException(const std::string &msg, size_t ln, size_t col)
      : std::runtime_error(std::format("{}:{}: error: {}", ln, col, msg)) {}
};

struct NotAToken : public LexerException {
  NotAToken(size_t ln, size_t col) : LexerException("Not a token.", ln, col) {}
};

struct OneLineCommentTooLong : public LexerException {
  OneLineCommentTooLong(size_t ln, size_t col)
      : LexerException("Oneline comment too long.", ln, col) {}
};

struct UnfinishedMultilineComment : public LexerException {
  UnfinishedMultilineComment(size_t ln, size_t col)
      : LexerException("Unfinished multiline comment.", ln, col) {}
};

struct MultilineCommentTooLong : public LexerException {
  MultilineCommentTooLong(size_t ln, size_t col)
      : LexerException("Multiline comment too long.", ln, col) {}
};

struct BackSlashInString : public LexerException {
  BackSlashInString(size_t ln, size_t col)
      : LexerException("Wrong usage of \\ character in string literal", ln,
                       col) {}
};

// Parser
struct ParserException : public std::runtime_error {
  ParserException(const std::string &msg, size_t ln, size_t col)
      : std::runtime_error(std::format("{}:{}: error: {}", ln, col, msg)) {}
};

// TODO: Evaluator

} // namespace pheonix::exception
