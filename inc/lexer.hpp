#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <optional>
#include <map>

namespace lexer {

// enum Keyword : int {
//     LET,
//     STRUCT,
//     RETURN,
//     FOR,
//     WHILE,
//     MUT,
//     
//     INT,
//     STR,
//     DBL
//
//     /* */
// };

enum TokenType : int {
    ERROR = 0,
    ERROR_NUMBER, // ex. 123a 123adfs
    ERROR_STRING,
    UNFINISHED_COMMENT,

    END_OF_FILE,

    PLUS,
    MINUS,
    STAR,
    SLASH,
    ASSIGN,

    EQUALS,
    LESS,
    LEQ,
    GREATER,
    GEQ,

    LPARENT,
    RPARENT,
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,

    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    COLON,
    SEMICOLON,
    COMMA,
    DOT,
    RARROW,
    LARROW,

    ONE_LINE_COMMENT,
    MULTILINE_COMMENT,

    INTEGER,
    DOUBLE,
    STRING,
    IDENTIFIER, // letter {letter | number | '_'};

    NOT_A_KEYWORD,
    /* */
    LET,
    STRUCT,
    RETURN,
    WHILE,
    MUT,
    IF,
    
    INT,
    STR,
    DBL
};

static const std::map<std::string, TokenType> Keywords = {
    {"if", IF},
    {"while", WHILE},

    {"let", LET},
    {"struct", STRUCT},
    {"return",RETURN },
    {"mut", MUT},
    {"int", INT},
    {"str", STR},
    {"dbl", DBL},
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
    Token handleOnelineCommentToken();
    Token handleMultilineCommentToken();
    Token handleNumber();
    Token handleIdentifier();
    Token handleString();
public:
    Lexer(std::istream& istream) : istream_(istream), offset(1), line(1), column(1){
        ch = istream_.get();
        peek = istream_.peek();
    }
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
