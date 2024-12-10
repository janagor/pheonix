#include "exception.hpp"

namespace pheonix::exception {

size_t LexerException::getLine() const { return line; }
size_t LexerException::getColumn() const { return column; }

size_t ParserException::getLine() const { return line; }
size_t ParserException::getColumn() const { return column; }

} // namespace pheonix::exception
