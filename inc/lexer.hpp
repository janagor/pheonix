#pragma once

#include <sstream>
#include <vector>

#include "token.hpp"

namespace lexer {

static const int IDENTIFIER_MAX_SIZE = 100;
static const int NUMERIC_MAX_SIZE = 1200;

struct Lexem {
    token::Token token;
    size_t line;
    size_t column;
    bool operator==(const Lexem& l) const;
};
std::ostream& operator<<(std::ostream& os, const Lexem& l);


struct Lexer {
private:
    void skipWhiteSpaces();
    void readChar();
    token::Token handleOnelineCommentToken();
    token::Token handleMultilineCommentToken();
    token::Token handleNumber();
    token::Token handleFloat(std::string& buffer);
    token::Token handleNumericUndefinedRepresentation(std::string& buffer);
    token::Token handleIdentifier();
    token::Token handleString();
public:
    Lexer(std::istream& istream) : istream_(istream), offset(1), line(1), column(1){
        ch = istream_.get();
        peek = istream_.peek();
    }

    Lexem nextLexem();
    std::vector<Lexem> lexerize();
private:
    std::istream& istream_;
    char ch;
    char peek;
    size_t offset;
    size_t line;
    size_t column;
};

} // namespace lexer;
