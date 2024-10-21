#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <optional>

namespace lexer {

enum Keyword : int {
    LET,
    STRUCT,
    RETURN
    /* */
};

enum TokenType : int {
    END_OF_FILE,
    PLUS,
    MINUS,
    STAR,
    SLASH
    /* */
};

struct Token {
    Token(TokenType t) : tokenType(t), value(std::nullopt) {}
    TokenType tokenType;
    std::optional<std::string> value;
};

struct Lexem {
    Token token;
    int line;
    int column;
    // for some reason on lecture presentation there is also offset of the lexem in the source
};

struct Lexer {
public:
    Lexer(std::istream& istream) : istream_(istream), position(0), readPosition(1), line(1), column(1){
        ch = istream_.get();
        position = readPosition;
        readPosition++;
    }
    std::vector<Lexem> lexerize();
private:
    std::istream& istream_;
    int position;
    int readPosition;
    char ch;
    int line;
    int column;
    // void readChar();
    // char peak();
    // void skipWhitesAndComms();
    Lexem nextLexem();
};

} // namespace lexer;
