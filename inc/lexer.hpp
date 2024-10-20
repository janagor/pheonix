#pragma once

#include <sstream>
#include <string>
#include <iostream>

namespace lexer {
enum Keyword : int {
    LET,
    STRUCT,
    RETURN,
    /* */
};

enum TokenType : int {
    PLUS,
    MINUS,
    STAR,
    SLASH,
    /* */
};

struct Lex {
    TokenType tokenType;
    std::string value;
    int line;
    int column;
    // for some reason on lecture presentation there is also offset of the lexem in the source
};

struct Lexer {
    int position;
    int readPosition;
    char ch;

    void readChar();
    char peak();
    void skipWhitesAndComms();
    std::vector<Lex> lexerize(std::istream& istream);
};

} // namespace lexer;
