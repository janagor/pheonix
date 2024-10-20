#include <string>

using namespace std;

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
    TokenType tType;
    int line;
    int column;
};

struct Lexer {
    string input;
    int position;
    int readPosition;
    char ch;

    void readChar();
    char peak();
}

int main () {}
