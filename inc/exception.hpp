#include <cstdlib>
#include <format>
#include <stdexcept>

namespace pheonix::exception {

struct LexerException : public std::runtime_error {
  LexerException(const std::string &msg, size_t ln, size_t col)
      : std::runtime_error(std::format("{}:{}: error: {}", ln, col, msg)) {}
};

struct ParserException : public std::runtime_error {
  ParserException(const std::string &msg, size_t ln, size_t col)
      : std::runtime_error(std::format("{}:{}: error: {}", ln, col, msg)) {}
};

} // namespace pheonix::exception
