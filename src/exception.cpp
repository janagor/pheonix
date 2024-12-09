#include "exception.hpp"

namespace pheonix::exception {

size_t LexerException::getLine() const { return line; }
size_t LexerException::getColumn() const { return column; }

} // namespace pheonix::exception
