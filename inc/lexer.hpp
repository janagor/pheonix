#pragma once

#include <sstream>
#include <string>
#include <variant>
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

    STRING,
    IDENTIFIER, // letter {letter | number | '_'};

    INTEGER,
    DOUBLE,
    LET,
    STRUCT,
    RETURN,
    WHILE,
    MUT,
    IF,
    ELSE,
    
    INT,
    STR,
    DBL,

    NOT_A_KEYWORD,
    /* */
};

static const std::map<std::string, TokenType> Keywords = {
    {"if", IF},
    {"else", ELSE},
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
    Token(TokenType t, int val) : tokenType(t), value(val) {}
    Token(TokenType t, double val) : tokenType(t), value(val) {}
    bool operator==(const Token& t) const;

    TokenType tokenType;
    std::optional< std::variant<int, double, std::string > > value;
};
std::ostream& operator<<(std::ostream& os, const std::optional<std::variant<int, double, std::string>>& opt);
std::ostream& operator<<(std::ostream& os, const Token& t);

struct Lexem {
    Token token;
    int line;
    int column;
    bool operator==(const Lexem& l) const;
};
std::ostream& operator<<(std::ostream& os, const Lexem& l);


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
