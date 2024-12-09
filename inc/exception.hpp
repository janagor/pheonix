#include <cstdlib>
#include <stdexcept>

namespace pheonix::exception {

class LexerException : public std::runtime_error {
  size_t line;
  size_t column;

public:
  LexerException(const std::string &msg, size_t ln, size_t col)
      : std::runtime_error(msg), line(ln), column(col) {}

  size_t getLine() const;
  size_t getColumn() const;
};

} // namespace pheonix::exception
