#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <optional>
#include <map>

namespace lexer {

enum Keyword : int {
    LET,
    STRUCT,
    RETURN
    /* */
};

enum TokenType : int {
    ERROR = 0,
    ERROR_NUMBER, // ex. 123a 123adfs
    ERROR_STRING,
    END_OF_FILE,
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
    UNFINISHED_COMMENT,

    INTEGER,
    IDENTIFIER, // letter {letter | number | '_'};
    FLOAT,
    
    STRING,


    NOT_A_KEYWORD,
    IF,
    WHILE

    


    /* */
};

static const std::map<std::string, TokenType> Keywords = {
    {"if", IF},
    {"while", WHILE},
};

std::optional<TokenType> searchForKeyword(std::string& word);

struct Token {
    Token() : tokenType(ERROR), value(std::nullopt) {}
    Token(TokenType t) : tokenType(t), value(std::nullopt) {}
    Token(TokenType t, std::string val) : tokenType(t), value(val) {}
    TokenType tokenType;
    std::optional<std::string> value;
};

struct Lexem {
    Token token;
    int line;
    int column;
};

struct Lexer {
private:
    void skipWhiteSpaces();
    void readChar();
    Lexem nextLexem();
    std::string getStringUntilNewLineEnd();
    Token handleMultilineCommentToken();
    Token handleNumber();
    Token handleIdentifier();
    Token handleString();
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
