#include "../inc/lexer.hpp"
#include <gtest/gtest.h>
#include <cassert>
#include <map>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <limits>

using namespace std;
using namespace token;
using namespace lexer;

// helper
void compareLexemVectors(const vector<Lexem>& expected, const vector<Lexem>& received) {
    EXPECT_EQ(expected.size(), received.size());

    for (size_t i = 0; i < min(expected.size(), received.size()); ++i) {
        EXPECT_EQ(expected[i], received[i]);
    }
}
std::vector<Lexem> lexerize(Lexer& lexer) {
    std::vector<Lexem> result;
    while (true) {
        Lexem l = lexer.nextLexem();
        result.emplace_back(l);
        if (l.token.getTokenType() == token::END_OF_FILE){
            return result;
        }
    }
}

// input
const map<const string, const Token> SPECIAL_CHARS_AND_KEYWORDS {
    { "=", Token(ASSIGN) },
    { "!", Token(BANG) },
    { "+", Token(PLUS) },
    { "-", Token(MINUS) },
    { "*", Token(STAR) },
    { "%", Token(PERCENT) },
    { "#", Token(HASH) },
    { "$", Token(DOLAR) },
    { ";", Token(SEMICOLON) },
    { ",", Token(COMMA) },
    { "(", Token(LPARENT) },
    { ")", Token(RPARENT) },
    { "{", Token(LBRACE) },
    { "}", Token(RBRACE) },
    { "[", Token(LBRACKET) },
    { "]", Token(RBRACKET) },
    { "==", Token(EQUALS) },
    { "&&", Token(AND) },
    { "||", Token(OR) },
    { "<=", Token(LEQ) },
    { ">=", Token(GEQ) },
    { "<-", Token(LARROW) },
    { "fn", Token(FN) },
    { "let", Token(LET) },
    { "mut", Token(MUT) },
    { "return", Token(RETURN) },
    { "if", Token(IF) },
    { "else", Token(ELSE) },
    { "while", Token(WHILE) },
    { "int", Token(INT) },
    { "str", Token(STR) },
    { "flt", Token(FLT) },
    { "bol", Token(BOL) },
    { "true", Token(TRUE) },
    { "false", Token(FALSE) },
};

const map<string, Token> ONE_LINE_COMMENTS {
    { R"(//)", Token(ONE_LINE_COMMENT, "//") },
    { R"(//aa)", Token(ONE_LINE_COMMENT, "//aa") },
    { R"(//  )", Token(ONE_LINE_COMMENT, "//  ") },
    { R"(// c)", Token(ONE_LINE_COMMENT,  "// c") },
    { R"(//////)", Token(ONE_LINE_COMMENT,  "//////") },
    {
        R"(//for while let 1 2 &&)",
        Token(ONE_LINE_COMMENT, "//for while let 1 2 &&")
    },
    {
        "//" + string(COMMENT_MAX_SIZE - 2, 'a'),
        Token(ONE_LINE_COMMENT,  "//" + string(COMMENT_MAX_SIZE - 2, 'a'))
    },
    //errors
    {
        "//" + string(COMMENT_MAX_SIZE, 'a'),
        Token(
            ERROR_ONE_LINE_COMMENT_OUT_OF_BOUND,
            "//" + string(COMMENT_MAX_SIZE - 2, 'a')
        )
    },
    {
        "//" + string(COMMENT_MAX_SIZE * 2, 'a'),
        Token(
            ERROR_ONE_LINE_COMMENT_OUT_OF_BOUND,
            "//" + string(COMMENT_MAX_SIZE - 2, 'a')
        )
    },
};

const map<string, Token> MULTILINE_COMMENTS {
    { R"(/**/)", Token(MULTILINE_COMMENT, "//") },
    { R"(/*aa*/)", Token(MULTILINE_COMMENT, "/*aa*/") },
    { R"(/*  */)", Token(MULTILINE_COMMENT, "/*  */") },
    { R"(/* c*/)", Token(MULTILINE_COMMENT,  "/* c*/") },
    { R"(/*////*/)", Token(MULTILINE_COMMENT,  "/*////*/") },
    {
        R"(/*for while let 1 2 &&*/)",
        Token(ONE_LINE_COMMENT, "/*for while let 1 2 &&*/")
    },
    {
        "/*" + string(COMMENT_MAX_SIZE - 4, 'a') + "*/",
        Token(
            ONE_LINE_COMMENT,
            "/*" + string(COMMENT_MAX_SIZE - 4, 'a') + "*/"
        )
    },
    //errors
    {
        "//" + string(COMMENT_MAX_SIZE, 'a'),
        Token(
            ERROR_ONE_LINE_COMMENT_OUT_OF_BOUND,
            "//" + string(COMMENT_MAX_SIZE - 2, 'a')
        )
    },
    {
        "/*" + string(COMMENT_MAX_SIZE, 'a') + "*/",
        Token(
            ONE_LINE_COMMENT,
            "/*" + string(COMMENT_MAX_SIZE - 2, 'a')
        )
    },
};

const map<const string, const Token> STRINGS {
    { R"("")", Token(STRING, "") },
    { R"("1")", Token(STRING, "1") },
    { R"("&")", Token(STRING, "&") },
    { R"("a")", Token(STRING, "a") },
    { R"("\n")", Token(STRING, "\n") },
    { R"("\r")", Token(STRING, "\r") },
    { R"("\\")", Token(STRING, "\\") },
    { R"("\t")", Token(STRING, "\t") },
    { R"(" ")", Token(STRING, " ") },
    { R"("\"")", Token(STRING, "\"")  },
    {
        "\"" + string(STRING_MAX_SIZE, 'a') + "\"",
        Token(STRING, string(STRING_MAX_SIZE, 'a'))
    },
    // errors
    { R"("\")", Token(ERROR_UNFINISHED_STRING, R"(")")  },
    { R"("\t)", Token(ERROR_UNFINISHED_STRING, "\t")  },
    { R"("\n)", Token(ERROR_UNFINISHED_STRING, "\n")  },
    { R"("\\)", Token(ERROR_UNFINISHED_STRING, "\\")  },
    { R"("\ ")", Token(ERROR_BACK_SLASH_STRING, R"(\ )")  },
    { R"("\a")", Token(ERROR_BACK_SLASH_STRING, R"(\a)")  },
    {
        "\"" + string(STRING_MAX_SIZE + 1, 'a') + "\"",
        Token(ERROR_STRING_OUT_OF_BOUND, string(STRING_MAX_SIZE, 'a'))
    },
    {
        "\"" + string(STRING_MAX_SIZE * 2, 'a') + "\"",
        Token(ERROR_STRING_OUT_OF_BOUND, string(STRING_MAX_SIZE, 'a'))
    },
};

const map<const string, const Token> IDENTIFIERS {
    { "normal", Token(IDENTIFIER, "normal") },
    { "normal123", Token(IDENTIFIER, "normal123") },
    { "normal__123", Token(IDENTIFIER, "normal__123") },
    { "NORMAL", Token(IDENTIFIER, "NORMAL") },
    {
        string(IDENTIFIER_MAX_SIZE, 'a'),
        Token(IDENTIFIER, string(IDENTIFIER_MAX_SIZE, 'a')) 
    },
    {
        string(IDENTIFIER_MAX_SIZE - 1, 'a'),
        Token(IDENTIFIER, string(IDENTIFIER_MAX_SIZE - 1, 'a'))
    },
    // errors
    {
        string(IDENTIFIER_MAX_SIZE + 1, 'a'),
        Token(ERROR_IDENTIFIER_TOO_LONG, string(IDENTIFIER_MAX_SIZE, 'a'))
    },
    {
        string(2 * IDENTIFIER_MAX_SIZE, 'a'),
        Token(ERROR_IDENTIFIER_TOO_LONG, string(IDENTIFIER_MAX_SIZE, 'a'))
    },
    {
        string(10 * IDENTIFIER_MAX_SIZE, 'a'),
        Token(ERROR_IDENTIFIER_TOO_LONG, string(IDENTIFIER_MAX_SIZE, 'a'))
    },
};

const map<const string, const Token> INTEGERS {
    { "0", Token(INTEGER, 0) },
    { "1", Token(INTEGER, 1) },
    { "213", Token(INTEGER, 213) },
    {
        to_string(numeric_limits<int>::max()),
        Token(INTEGER, numeric_limits<int>::max())
    },
    // errors
    {
        string(NUMERIC_MAX_SIZE, '1'),
        Token(ERROR_INTEGER_OUT_OF_BOUND, string(NUMERIC_MAX_SIZE, '1'))
    },
    {
        to_string(static_cast<long>(numeric_limits<int>::max()) + 1),
       Token(
            ERROR_INTEGER_OUT_OF_BOUND,
            to_string(static_cast<long>(numeric_limits<int>::max()) + 1)
        )
    },
    {
        string(NUMERIC_MAX_SIZE+1, '1'),
        Token(ERROR_INTEGER_OUT_OF_BOUND, string(NUMERIC_MAX_SIZE, '1'))
    },
    {
        string("33333" + NUMERIC_MAX_SIZE*2, '1'),
        Token(
            ERROR_INTEGER_OUT_OF_BOUND,
            "33333" + string((NUMERIC_MAX_SIZE-5), '1')
        )
    },
    {
        string(NUMERIC_MAX_SIZE+10000, '1') + "cniewfiqwndqeu2398rq______",
        Token(ERROR_NUMBER_UNDEFINED_REPRESENTATION, string(NUMERIC_MAX_SIZE, '1'))
    },
};

const map<const string, const Token> FLOATS {
    { "0.", Token(FLOAT, stod("0.")) },
    { "1.", Token(FLOAT, stod("1.")) },
    { "213.", Token(FLOAT, stod("213.")) },
    {
        to_string(numeric_limits<double>::max()),
        Token(FLOAT, stod(to_string(numeric_limits<double>::max())))
    },
    // errors
    {
        "1." + string(NUMERIC_MAX_SIZE, '1'),
        Token(ERROR_FLOAT_OUT_OF_BOUND, "1." + string(NUMERIC_MAX_SIZE-2, '1'))
    },
    {
        "0." + string(NUMERIC_MAX_SIZE +200, '1'),
        Token(ERROR_FLOAT_OUT_OF_BOUND, "0." + string(NUMERIC_MAX_SIZE-2, '1'))
    },
    {
        string(NUMERIC_MAX_SIZE*100, '1') + "." + string(NUMERIC_MAX_SIZE*100, '1'),
        Token(
            ERROR_FLOAT_OUT_OF_BOUND, string(NUMERIC_MAX_SIZE, '1')
        )
    },
};

const vector<string> NEW_LINE_CHARACTERS { "\n", "\r", "\r\n", };

const vector<string> NEW_LINE_CHARACTERS_WITH_SOMETHING_AFTER { "\na", "\ra", "\r\na", };

const vector<string> NOT_A_TOKENS { ".", "`", "~", "|", "@", "^", "&", "?", "\\" };

// tests cases
TEST(TestLexer, testEmptyInput) {
    string input = R"()";
    vector<Lexem> expected {
        {Token(END_OF_FILE), 1, 1},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, specialCharsAndKeywords) {
    for (const auto& [key, value] : SPECIAL_CHARS_AND_KEYWORDS) {
        string input = key;
        // NOTE: first character is at the indexes (1, 1)
        size_t shift = input.length() + 1;
        vector<Lexem> expected {
            {value, 1, 1},
            {Token(END_OF_FILE), 1, shift},
        };
        istringstream in(input);
        Lexer l(in);
        vector<Lexem> result = lexerize(l);

        EXPECT_EQ(expected.size(), result.size());

        compareLexemVectors(expected, result);
    }
}

TEST(TestLexer, testOneLineComments) {
    for (const auto& [key, value] : ONE_LINE_COMMENTS) {
        string input = key;
        size_t shift = input.length() + 1;
        vector<Lexem> expected {
            {value, 1, 1},
            {Token(END_OF_FILE), 1, shift},
        };
        istringstream in(input);
        Lexer l(in);
        vector<Lexem> result = lexerize(l);

        EXPECT_EQ(expected.size(), result.size());

        compareLexemVectors(expected, result);
    }
}


TEST(TestLexer, testStrings) {
    for (const auto& [key, value] : STRINGS) {
        string input = key;
        size_t shift = input.length() + 1;
        vector<Lexem> expected {
            {value, 1, 1},
            {Token(END_OF_FILE), 1, shift},
        };
        istringstream in(input);
        Lexer l(in);
        vector<Lexem> result = lexerize(l);
        EXPECT_EQ(expected.size(), result.size());
        compareLexemVectors(expected, result);
    }
}

TEST(TestLexer, testIdentifiers) {
    for (const auto& [key, value] : IDENTIFIERS) {
        string input = key;
        size_t shift = input.length() + 1;
        vector<Lexem> expected {
            {value, 1, 1},
            {Token(END_OF_FILE), 1, shift},
        };
        istringstream in(input);
        Lexer l(in);
        vector<Lexem> result = lexerize(l);
        EXPECT_EQ(expected.size(), result.size());
        compareLexemVectors(expected, result);
    }
}
TEST(TestLexer, testIntegersFrom0to1000) {
    for (int i = 0; i < 1000; ++i) {
        string input = to_string(i);
        size_t shift = input.length() + 1;
        vector<Lexem> expected {
            {Token(INTEGER, i), 1, 1},
            {Token(END_OF_FILE), 1, shift},
        };
        assert(std::get<int>(expected[0].token.getValue().value()) == i);
        istringstream in(input);
        Lexer l(in);
        vector<Lexem> result = lexerize(l);

        EXPECT_EQ(expected.size(), result.size());

        compareLexemVectors(expected, result);
    }
}
TEST(TestLexer, testFloats) {
    for (const auto& [key, value] : FLOATS) {
        string input = key;
        size_t shift = input.length() + 1;
        vector<Lexem> expected {
            {value, 1, 1},
            {Token(END_OF_FILE), 1, shift},
        };
        istringstream in(input);
        Lexer l(in);
        vector<Lexem> result = lexerize(l);
        EXPECT_EQ(expected.size(), result.size());
        compareLexemVectors(expected, result);
    }
}
TEST(TestLexer, testNewLineCharacters) {
    for (const auto& input : NEW_LINE_CHARACTERS) {
        vector<Lexem> expected {
            {Token(END_OF_FILE), 2, 1},
        };
        istringstream in(input);
        Lexer l(in);
        vector<Lexem> result = lexerize(l);
        EXPECT_EQ(expected.size(), result.size());
        compareLexemVectors(expected, result);
    }
}

TEST(TestLexer, testNewLineCharactersWithSomethingAfter) {
    for (const auto& input : NEW_LINE_CHARACTERS_WITH_SOMETHING_AFTER) {
        vector<Lexem> expected {
            {Token(IDENTIFIER, "a"), 2, 1},
            {Token(END_OF_FILE), 2, 2},
        };
        istringstream in(input);
        Lexer l(in);
        vector<Lexem> result = lexerize(l);
        EXPECT_EQ(expected.size(), result.size());
        compareLexemVectors(expected, result);
    }
}

TEST(TestLexer, testNotATokens) {
    for (const auto& input : NOT_A_TOKENS) {
        vector<Lexem> expected {
            {Token(NOT_A_TOKEN, input), 1, 1},
            {Token(END_OF_FILE), 1, 2},
        };
        istringstream in(input);
        Lexer l(in);
        vector<Lexem> result = lexerize(l);
        EXPECT_EQ(expected.size(), result.size());
        compareLexemVectors(expected, result);
    }
}

// example nr 1 from the documentation
TEST(TestLexer, Ex1) {
    string input =
R"(// These are one line comments.
/* these
are
multiline
comments */)";
    vector<Lexem> expected {
        {Token(ONE_LINE_COMMENT, "// These are one line comments."), 1, 1},
        {Token(MULTILINE_COMMENT, R"(/* these
are
multiline
comments */)"), 2, 1},
        {Token(END_OF_FILE), 5, 12},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());
    compareLexemVectors(expected, result);
}

TEST(TestLexer, Ex2) {
    string input =
R"(let a = 12; // int - const
let mut b = 12.12; // flt - mutowalny
let c = "duck"; // str - const
let mut d = true; // bol - mutowalny)";
    vector<Lexem> expected {
        {Token(LET), 1, 1},
        {Token(IDENTIFIER, "a"), 1, 5},
        {Token(ASSIGN), 1, 7},
        {Token(INTEGER, 12), 1, 9},
        {Token(SEMICOLON), 1, 11},
        {Token(ONE_LINE_COMMENT, "// int - const"), 1, 13},

        {Token(LET), 2, 1},
        {Token(MUT), 2, 5},
        {Token(IDENTIFIER, "b"), 2, 9},
        {Token(ASSIGN), 2, 11},
        {Token(FLOAT, 12.12), 2, 13},
        {Token(SEMICOLON), 2, 18},
        {Token(ONE_LINE_COMMENT, "// flt - mutowalny"), 2, 20},

        {Token(LET), 3, 1},
        {Token(IDENTIFIER, "c"), 3, 5},
        {Token(ASSIGN), 3, 7},
        {Token(STRING, "duck"), 3, 9},
        {Token(SEMICOLON), 3, 15},
        {Token(ONE_LINE_COMMENT, "// str - const"), 3, 17},

        {Token(LET), 4, 1},
        {Token(MUT), 4, 5},
        {Token(IDENTIFIER, "d"), 4, 9},
        {Token(ASSIGN), 4, 11},
        {Token(TRUE), 4, 13},
        {Token(SEMICOLON), 4, 17},
        {Token(ONE_LINE_COMMENT, "// bol - mutowalny"), 4, 19},

        {Token(END_OF_FILE), 4, 37},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());
    compareLexemVectors(expected, result);
}

// file reading
TEST(TestLexer, testReadingFromFile) {
    string input =
R"(let kaczka = if (a == 21) { 12 } else { "kaczka" };
while(x < 12) { x = x + 0.; }
let mut b = 123<-str;
)";
    std::ofstream file("test.txt");
    if (file.is_open()) {
        file << input;
        file.close();
    } else {
        FAIL() << "Failed to open the file!";
    }
    vector<Lexem> expected {
        {Token(LET), 1, 1},
        {Token(IDENTIFIER, "kaczka"), 1, 5},
        {Token(ASSIGN), 1, 12},
        {Token(IF), 1, 14},
        {Token(LPARENT), 1, 17},
        {Token(IDENTIFIER, "a"), 1, 18},
        {Token(EQUALS), 1, 20},
        {Token(INTEGER, 21), 1, 23},
        {Token(RPARENT), 1, 25},

        {Token(LBRACE), 1, 27},
        {Token(INTEGER, 12), 1, 29},
        {Token(RBRACE), 1, 32},
        {Token(ELSE), 1, 34},
        {Token(LBRACE), 1, 39},
        {Token(STRING, "kaczka"), 1, 41},
        {Token(RBRACE), 1, 50},
        {Token(SEMICOLON), 1, 51},

        {Token(WHILE), 2, 1},
        {Token(LPARENT), 2, 6},
        {Token(IDENTIFIER, "x"), 2, 7},
        {Token(LESS), 2, 9},
        {Token(INTEGER, 12), 2, 11},
        {Token(RPARENT), 2, 13},
        {Token(LBRACE), 2, 15},
        {Token(IDENTIFIER, "x"), 2, 17},
        {Token(ASSIGN), 2, 19},
        {Token(IDENTIFIER, "x"), 2, 21},
        {Token(PLUS), 2, 23},
        {Token(FLOAT, 0.), 2, 25},
        {Token(SEMICOLON), 2, 27},
        {Token(RBRACE), 2, 29},

        {Token(LET), 3, 1},
        {Token(MUT), 3, 5},
        {Token(IDENTIFIER, "b"), 3, 9},
        {Token(ASSIGN), 3, 11},
        {Token(INTEGER, 123), 3, 13},
        {Token(LARROW), 3, 16},
        {Token(STR), 3,18},
        {Token(SEMICOLON), 3, 21},

        {Token(END_OF_FILE), 4, 1},
    };

    ifstream file_input("test.txt");
    Lexer l(file_input);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
    std::remove("test.txt");
}

///////////////////////////////////////////////////////////////////////////////
// some less ordered tests
TEST(TestLexer, testOperators) {
    string input = R"(+=*/-)";
    vector<Lexem> expected {
        {Token(PLUS), 1, 1},
        {Token(ASSIGN), 1, 2},
        {Token(STAR), 1, 3},
        {Token(SLASH), 1, 4},
        {Token(MINUS), 1, 5},
        {Token(END_OF_FILE), 1, 6},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testBrackets) {
    string input = R"((){}[])";
    vector<Lexem> expected {
        {Token(LPARENT), 1, 1},
        {Token(RPARENT), 1, 2},
        {Token(LBRACE), 1, 3},
        {Token(RBRACE), 1, 4},
        {Token(LBRACKET), 1, 5},
        {Token(RBRACKET), 1, 6},
        {Token(END_OF_FILE), 1, 7},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testOneLineComments2) {
    string input =
"//abcdefg1123\n\
++//3kaczuszka";
    vector<Lexem> expected {
        {Token(ONE_LINE_COMMENT, "//abcdefg1123"), 1, 1},
        {Token(PLUS), 2, 1},
        {Token(PLUS), 2, 2},
        {Token(ONE_LINE_COMMENT, "//3kaczuszka"), 2, 3},
        {Token(END_OF_FILE), 2, 15},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testOneLineComments3) {
    string input =
"//abcdefg    \t\t!@@#$\n//";
    vector<Lexem> expected {
        {Token(ONE_LINE_COMMENT, "//abcdefg    \t\t!@@#$"), 1, 1},
        {Token(ONE_LINE_COMMENT, "//"), 2, 1},
        {Token(END_OF_FILE), 2, 3},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testMultilineComments) {
    string input =
"/*abcdefll\n\
\n\
\n\
;'.,:1*/+=";
    vector<Lexem> expected {
        {Token(MULTILINE_COMMENT, "/*abcdefll\n\n\n;'.,:1*/"), 1, 1},
        {Token(PLUS), 4, 9},
        {Token(ASSIGN), 4, 10},
        {Token(END_OF_FILE), 4, 11},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testMultilineComments2) {
    string input =
"/*abcdefll\n\
\n\
\n\
;'.,:1";
    vector<Lexem> expected {
        {Token(ERROR_UNFINISHED_COMMENT, "/*abcdefll\n\n\n;'.,:1"), 1, 1},
        {Token(END_OF_FILE), 4, 7},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testIdentifiers2) {
    string input =
R"(abcd123
abcd
++==a__12311
)";
    vector<Lexem> expected {
        {Token(IDENTIFIER, "abcd123"), 1, 1},
        {Token(IDENTIFIER, "abcd"), 2, 1},
        {Token(PLUS), 3, 1},
        {Token(PLUS), 3, 2},
        {Token(EQUALS), 3, 3},
        {Token(IDENTIFIER, "a__12311"), 3, 5},
        {Token(END_OF_FILE), 4, 1},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testIntegerLiteralsEasier) {
    string input =
R"(123
111112 1230)";
    vector<Lexem> expected {
        {Token(INTEGER, 123), 1, 1},
        {Token(INTEGER, 111112), 2, 1},
        {Token(INTEGER, 1230), 2, 8},
        {Token(END_OF_FILE), 2, 12},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}
TEST(TestLexer, testIntegerLiterals) {
    string input =
R"(123
abcd12 1230
**789 a__12311)";
    vector<Lexem> expected {
        {Token(INTEGER, 123), 1, 1},
        {Token(IDENTIFIER, "abcd12"), 2, 1},
        {Token(INTEGER, 1230), 2, 8},
        {Token(STAR), 3, 1},
        {Token(STAR), 3, 2},
        {Token(INTEGER, 789), 3, 3},
        {Token(IDENTIFIER, "a__12311"), 3, 7},
        {Token(END_OF_FILE), 3, 15},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testIntegerMany) {
    string input =
R"(1 2 3 4 5 6 7 8 9 10 11 12+13)";
    vector<Lexem> expected {
        {Token(INTEGER, 1), 1, 1},
        {Token(INTEGER, 2), 1, 3},
        {Token(INTEGER, 3), 1, 5},
        {Token(INTEGER, 4), 1, 7},
        {Token(INTEGER, 5), 1, 9},
        {Token(INTEGER, 6), 1, 11},
        {Token(INTEGER, 7), 1, 13},
        {Token(INTEGER, 8), 1, 15},
        {Token(INTEGER, 9), 1, 17},
        {Token(INTEGER, 10), 1, 19},
        {Token(INTEGER, 11), 1, 22},
        {Token(INTEGER, 12), 1, 25},
        {Token(PLUS), 1, 27},
        {Token(INTEGER, 13), 1, 28},
        {Token(END_OF_FILE), 1, 30},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testIntegerLiteralsError) {
    string input =
R"(123_
abcd12 1230
**789a a__12311)";
    vector<Lexem> expected {
        {Token(ERROR_NUMBER_UNDEFINED_REPRESENTATION, "123_"), 1, 1},
        {Token(IDENTIFIER, "abcd12"), 2, 1},
        {Token(INTEGER, 1230), 2, 8},
        {Token(STAR), 3, 1},
        {Token(STAR), 3, 2},
        {Token(ERROR_NUMBER_UNDEFINED_REPRESENTATION, "789a"), 3, 3},
        {Token(IDENTIFIER, "a__12311"), 3, 8},
        {Token(END_OF_FILE), 3, 16},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testKeywords) {
    string input =
"123 if while";
    vector<Lexem> expected {
        {Token(INTEGER, 123), 1, 1},
        {Token(IF), 1, 5},
        {Token(WHILE), 1, 8},
        {Token(END_OF_FILE), 1, 13},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testStringsWithoutSpecialCharacters) {
    string input =
R"("kaczuszka"

"cos    ")";
    vector<Lexem> expected {
        {Token(STRING, "kaczuszka"), 1, 1},
        {Token(STRING, "cos    "), 3, 1},
        {Token(END_OF_FILE), 3, 10},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}
TEST(TestLexer, testStringsWithoutSpecialCharactersWithNewLine) {
    string input =
R"("kaczuszka

cos    ")";
    vector<Lexem> expected {
        {Token(STRING, "kaczuszka\n\ncos    "), 1, 1},
        {Token(END_OF_FILE), 3, 9},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testStringsWithDoubleQuotesInside) {
    string input =
R"("kaczuszka mowi \"Hau, Hau!\"")";
    vector<Lexem> expected {
        {Token(STRING, "kaczuszka mowi \"Hau, Hau!\""), 1, 1},
        {Token(END_OF_FILE), 1, 31},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testStringsWithNewLineCharacterInside) {
    string input =
R"("kaczuszka mowi:
- hello!
- hello!")";
    vector<Lexem> expected {
        {Token(STRING,
"kaczuszka mowi:\n\
- hello!\n\
- hello!"), 1, 1},
        {Token(END_OF_FILE), 3, 10},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testFloat) {
    string input =
R"(1.12345)";
    vector<Lexem> expected {
        {Token(FLOAT, 1.12345), 1, 1},
        {Token(END_OF_FILE), 1, 8},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testFloat2) {
    string input =
R"(1.12345 0.0)";
    vector<Lexem> expected {
        {Token(FLOAT, 1.12345), 1, 1},
        {Token(FLOAT, 0.0), 1, 9},
        {Token(END_OF_FILE), 1, 12},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testFloatWithoutNumbersAfterDot) {
    string input =
R"(1.12345 0.)";
    vector<Lexem> expected {
        {Token(FLOAT, 1.12345), 1, 1},
        {Token(FLOAT, 0.), 1, 9},
        {Token(END_OF_FILE), 1, 11},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

TEST(TestLexer, testAll1) {
    string input =
R"(let kaczka = if (a == 21) { 12 } else { "kaczka" };
while(x < 12) { x = x + 0.; }
let mut b = 123<-str;
)";
    vector<Lexem> expected {
        {Token(LET), 1, 1},
        {Token(IDENTIFIER, "kaczka"), 1, 5},
        {Token(ASSIGN), 1, 12},
        {Token(IF), 1, 14},
        {Token(LPARENT), 1, 17},
        {Token(IDENTIFIER, "a"), 1, 18},
        {Token(EQUALS), 1, 20},
        {Token(INTEGER, 21), 1, 23},
        {Token(RPARENT), 1, 25},

        {Token(LBRACE), 1, 27},
        {Token(INTEGER, 12), 1, 29},
        {Token(RBRACE), 1, 32},
        {Token(ELSE), 1, 34},
        {Token(LBRACE), 1, 39},
        {Token(STRING, "kaczka"), 1, 41},
        {Token(RBRACE), 1, 50},
        {Token(SEMICOLON), 1, 51},

        {Token(WHILE), 2, 1},
        {Token(LPARENT), 2, 6},
        {Token(IDENTIFIER, "x"), 2, 7},
        {Token(LESS), 2, 9},
        {Token(INTEGER, 12), 2, 11},
        {Token(RPARENT), 2, 13},
        {Token(LBRACE), 2, 15},
        {Token(IDENTIFIER, "x"), 2, 17},
        {Token(ASSIGN), 2, 19},
        {Token(IDENTIFIER, "x"), 2, 21},
        {Token(PLUS), 2, 23},
        {Token(FLOAT, 0.), 2, 25},
        {Token(SEMICOLON), 2, 27},
        {Token(RBRACE), 2, 29},

        {Token(LET), 3, 1},
        {Token(MUT), 3, 5},
        {Token(IDENTIFIER, "b"), 3, 9},
        {Token(ASSIGN), 3, 11},
        {Token(INTEGER, 123), 3, 13},
        {Token(LARROW), 3, 16},
        {Token(STR), 3,18},
        {Token(SEMICOLON), 3, 21},

        {Token(END_OF_FILE), 4, 1}
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
}
