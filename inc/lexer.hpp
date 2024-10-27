#pragma once

#include <sstream>
#include <vector>

#include "token.hpp"

namespace lexer {

struct Lexem {
    token::Token token;
    int line;
    int column;
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
    int offset;
    int line;
    int column;
};

} // namespace lexer;
