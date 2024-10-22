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
    END_OF_FILE = 0,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    EQUALS,
    LPARENT,
    RPARENT,
    LBRACES,
    RBRACES,
    LBRACKETS,
    RBRACKETS,
    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    COLON,
    SEMICOLON,
    COMMA,
    DOT,

    ONE_LINE_COMMENT,
    MULTILINE_COMMENT,


    


    /* */
};

struct Token {
    Token(TokenType t) : tokenType(t), value(std::nullopt) {}
    Token(TokenType t, std::string val) : tokenType(t), value(val) {}
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
private:
    void skipWhiteSpaces();
    void readChar();
    Lexem nextLexem();
    std::string getStringUntilNewLineEnd();
    std::string getMultilineComment();
public:
    Lexer(std::istream& istream) : istream_(istream), offset(1), line(1), column(1){
        ch = istream_.get();
    }
    std::vector<Lexem> lexerize();
private:
    std::istream& istream_;
    char ch;
    int offset;
    int line;
    int column;
};

} // namespace lexer;
