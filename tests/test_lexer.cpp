#include "lexer.hpp"
#include "types.hpp"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <map>

using namespace std;
using namespace pheonix::token;
using namespace pheonix::exception;
using namespace pheonix::types;
using namespace pheonix::lexer;

// helpers
void compareLexemVectors(const vector<Lexem> &expected,
                         const vector<Lexem> &received) {
  EXPECT_EQ(expected.size(), received.size());

  for (size_t i = 0; i < min(expected.size(), received.size()); ++i) {
    EXPECT_EQ(expected[i], received[i]);
  }
}
vector<Lexem> lexerize(Lexer &lexer) {
  vector<Lexem> result;
  while (true) {
    Lexem l = lexer.nextLexem();
    result.emplace_back(l);
    if (l.token.getTokenType() == TokenType::END_OF_FILE) {
      return result;
    }
  }
}

// tests
TEST(TestLexer, testEmptyInput) {
  string input = R"()";
  vector<Lexem> expected{
      {Token(TokenType::END_OF_FILE), 1, 1},
  };
  istringstream in(input);
  Lexer l(in);
  vector<Lexem> result = lexerize(l);

  EXPECT_EQ(expected.size(), result.size());

  compareLexemVectors(expected, result);
}

const map<const string, const Token> SPECIAL_CHARS_AND_KEYWORDS{
    {"=", Token(TokenType::ASSIGN)},
    {"!", Token(TokenType::BANG)},
    {"+", Token(TokenType::PLUS)},
    {"-", Token(TokenType::MINUS)},
    {"*", Token(TokenType::STAR)},
    {"%", Token(TokenType::PERCENT)},
    {"#", Token(TokenType::HASH)},
    {"|", Token(TokenType::PIPE)},
    {"$", Token(TokenType::DOLAR)},
    {";", Token(TokenType::SEMICOLON)},
    {",", Token(TokenType::COMMA)},
    {"(", Token(TokenType::LPARENT)},
    {")", Token(TokenType::RPARENT)},
    {"{", Token(TokenType::LBRACE)},
    {"}", Token(TokenType::RBRACE)},
    {"[", Token(TokenType::LBRACKET)},
    {"]", Token(TokenType::RBRACKET)},
    {"==", Token(TokenType::EQUALS)},
    {"&&", Token(TokenType::AND)},
    {"||", Token(TokenType::OR)},
    {"<=", Token(TokenType::LEQ)},
    {">=", Token(TokenType::GEQ)},
    {"<-", Token(TokenType::LARROW)},
    {"fn", Token(TokenType::FN)},
    {"let", Token(TokenType::LET)},
    {"mut", Token(TokenType::MUT)},
    {"return", Token(TokenType::RETURN)},
    {"if", Token(TokenType::IF)},
    {"else", Token(TokenType::ELSE)},
    {"while", Token(TokenType::WHILE)},
    {"int", Token(TokenType::INT)},
    {"str", Token(TokenType::STR)},
    {"flt", Token(TokenType::FLT)},
    {"bol", Token(TokenType::BOL)},
    {"true", Token(TokenType::TRUE, true)},
    {"false", Token(TokenType::FALSE, false)},
};

TEST(TestLexer, specialCharsAndKeywords) {
  for (const auto &[key, value] : SPECIAL_CHARS_AND_KEYWORDS) {
    string input = key;
    // NOTE: first character is at the indexes (1, 1)
    size_t shift = input.length() + 1;
    vector<Lexem> expected{
        {value, 1, 1},
        {Token(TokenType::END_OF_FILE), 1, shift},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
  }
}

const map<string, Token> ONE_LINE_COMMENTS{
    {R"(//)", Token(TokenType::ONE_LINE_COMMENT, "//")},
    {R"(//aa)", Token(TokenType::ONE_LINE_COMMENT, "//aa")},
    {R"(//  )", Token(TokenType::ONE_LINE_COMMENT, "//  ")},
    {R"(// c)", Token(TokenType::ONE_LINE_COMMENT, "// c")},
    {R"(//////)", Token(TokenType::ONE_LINE_COMMENT, "//////")},
    {R"(//for while let 1 2 &&)",
     Token(TokenType::ONE_LINE_COMMENT, "//for while let 1 2 &&")},
    {"//" + string(COMMENT_MAX_SIZE - 2, 'a'),
     Token(TokenType::ONE_LINE_COMMENT,
           "//" + string(COMMENT_MAX_SIZE - 2, 'a'))},
};

TEST(TestLexer, testOneLineCommentError) {
  string input = "//" + string(COMMENT_MAX_SIZE, 'a');
  istringstream in(input);
  Lexer l(in);
  try {
    lexerize(l);
    FAIL() << "Expected LexerException";
  } catch (const LexerException &e) {
    EXPECT_STREQ(e.what(), "1:1: error: Oneline comment too long.");
  } catch (...) {
    FAIL() << "Unexpected exception type thrown";
  }
}

TEST(TestLexer, testOneLineComments) {
  for (const auto &[key, value] : ONE_LINE_COMMENTS) {
    string input = key;
    size_t shift = input.length() + 1;
    vector<Lexem> expected{
        {value, 1, 1},
        {Token(TokenType::END_OF_FILE), 1, shift},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
  }
}

const map<string, Token> MULTILINE_COMMENTS{
    {R"(/**/)", Token(TokenType::MULTILINE_COMMENT, "//")},
    {R"(/*aa*/)", Token(TokenType::MULTILINE_COMMENT, "/*aa*/")},
    {R"(/*  */)", Token(TokenType::MULTILINE_COMMENT, "/*  */")},
    {R"(/* c*/)", Token(TokenType::MULTILINE_COMMENT, "/* c*/")},
    {R"(/*////*/)", Token(TokenType::MULTILINE_COMMENT, "/*////*/")},
    {R"(/*for while let 1 2 &&*/)",
     Token(TokenType::ONE_LINE_COMMENT, "/*for while let 1 2 &&*/")},
    {"/*" + string(COMMENT_MAX_SIZE - 4, 'a') + "*/",
     Token(TokenType::ONE_LINE_COMMENT,
           "/*" + string(COMMENT_MAX_SIZE - 4, 'a') + "*/")},
};

const map<string, Token> MULTILINE_COMMENTS_ERRORS{
    {"/*" + string(COMMENT_MAX_SIZE, 'a') + "*/",
     Token(TokenType::ONE_LINE_COMMENT,
           "/*" + string(COMMENT_MAX_SIZE - 2, 'a'))},
};
TEST(TestLexer, testMultilineCommentError) {
  string input = "/*" + string(COMMENT_MAX_SIZE, 'a') + "*/";
  istringstream in(input);
  Lexer l(in);
  try {
    lexerize(l);
    FAIL() << "Expected LexerException";
  } catch (const LexerException &e) {
    EXPECT_STREQ(e.what(), "1:1: error: Multiline comment too long.");
  } catch (...) {
    FAIL() << "Unexpected exception type thrown";
  }
}

const map<const string, const Token> STRINGS{
    {R"("")", Token(TokenType::STRING, "")},
    {R"("1")", Token(TokenType::STRING, "1")},
    {R"("&")", Token(TokenType::STRING, "&")},
    {R"("a")", Token(TokenType::STRING, "a")},
    {R"("\n")", Token(TokenType::STRING, "\n")},
    {R"("\r")", Token(TokenType::STRING, "\r")},
    {R"("\\")", Token(TokenType::STRING, "\\")},
    {R"("\t")", Token(TokenType::STRING, "\t")},
    {R"(" ")", Token(TokenType::STRING, " ")},
    {R"("\"")", Token(TokenType::STRING, "\"")},
    {"\"" + string(STRING_MAX_SIZE, 'a') + "\"",
     Token(TokenType::STRING, string(STRING_MAX_SIZE, 'a'))},
};

const map<const string, string> STRINGS_ERRORS{
    // errors
    {R"("\")", "1:1: error: Unfinished string literal."},
    {R"("\t)", "1:1: error: Unfinished string literal."},
    {R"("\n)", "1:1: error: Unfinished string literal."},
    {R"("\\)", "1:1: error: Unfinished string literal."},
    {R"("\ ")", "1:1: error: Wrong usage of \\ character in string literal"},
    {R"("\a")", "1:1: error: Wrong usage of \\ character in string literal"},
    {"\"" + string(STRING_MAX_SIZE + 1, 'a') + "\"",
     "1:1: error: String literal to long."},
    {"\"" + string(STRING_MAX_SIZE * 2, 'a') + "\"",
     "1:1: error: String literal to long."},
};

TEST(TestLexer, testStringErrors) {
  for (const auto &[key, value] : STRINGS_ERRORS) {
    string input = key;
    istringstream in(input);
    Lexer l(in);
    try {
      lexerize(l);
      FAIL() << "Expected LexerException";
    } catch (const LexerException &e) {
      EXPECT_STREQ(e.what(), value.c_str());
    } catch (...) {
      FAIL() << "Unexpected exception type thrown";
    }
  }
}

const map<const string, const Token> IDENTIFIERS{
    {"normal", Token(TokenType::IDENTIFIER, "normal")},
    {"normal123", Token(TokenType::IDENTIFIER, "normal123")},
    {"normal__123", Token(TokenType::IDENTIFIER, "normal__123")},
    {"NORMAL", Token(TokenType::IDENTIFIER, "NORMAL")},
    {string(IDENTIFIER_MAX_SIZE, 'a'),
     Token(TokenType::IDENTIFIER, string(IDENTIFIER_MAX_SIZE, 'a'))},
    {string(IDENTIFIER_MAX_SIZE - 1, 'a'),
     Token(TokenType::IDENTIFIER, string(IDENTIFIER_MAX_SIZE - 1, 'a'))},
};

TEST(TestLexer, testIdentifiers) {
  for (const auto &[key, value] : IDENTIFIERS) {
    string input = key;
    size_t shift = input.length() + 1;
    vector<Lexem> expected{
        {value, 1, 1},
        {Token(TokenType::END_OF_FILE), 1, shift},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);
    EXPECT_EQ(expected.size(), result.size());
    compareLexemVectors(expected, result);
  }
}

const map<const string, const string> IDENTIFIERS_ERRORS{
    {string(IDENTIFIER_MAX_SIZE + 1, 'a'), "1:1: error: Identifier too long."},
    {string(2 * IDENTIFIER_MAX_SIZE, 'a'), "1:1: error: Identifier too long."},
    {string(10 * IDENTIFIER_MAX_SIZE, 'a'), "1:1: error: Identifier too long."},
};

TEST(TestLexer, testIdentifiersErrors) {
  for (const auto &[key, value] : IDENTIFIERS_ERRORS) {
    string input = key;
    istringstream in(input);
    Lexer l(in);
    try {
      lexerize(l);
      FAIL() << "Expected LexerException";
    } catch (const LexerException &e) {
      EXPECT_STREQ(e.what(), value.c_str());
    } catch (...) {
      FAIL() << "Unexpected exception type thrown";
    }
  }
}

const map<const string, const Token> INTEGERS{
    {"0", Token(TokenType::INTEGER, 0)},
    {"1", Token(TokenType::INTEGER, 1)},
    {"213", Token(TokenType::INTEGER, 213)},
    {"21323234", Token(TokenType::INTEGER, 21323234)},
    {to_string(numeric_limits<int>::max()),
     Token(TokenType::INTEGER, numeric_limits<int>::max())},
};
TEST(TestLexer, testIntegers) {
  for (const auto &[key, value] : INTEGERS) {
    string input = key;
    size_t shift = input.length() + 1;
    vector<Lexem> expected{
        {value, 1, 1},
        {Token(TokenType::END_OF_FILE), 1, shift},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);
    EXPECT_EQ(expected.size(), result.size());
    compareLexemVectors(expected, result);
  }
}
// TODO: make errors of integers and floats mention lines
const map<const string, const string> INTEGERS_ERRORS{
    {to_string(static_cast<long>(numeric_limits<int>::max()) + 1),
     "Integer literal out of range."},
    {to_string(2 * static_cast<long>(numeric_limits<int>::max())),
     "Integer literal out of range."},
    // {
    //     to_string(21321) + "a",
    //     "Undefined value."
    // },
};

TEST(TestLexer, testIntegersErrors) {
  for (const auto &[key, value] : INTEGERS_ERRORS) {
    string input = key;
    istringstream in(input);
    Lexer l(in);
    try {
      lexerize(l);
      FAIL() << "Expected runtime_error";
    } catch (const runtime_error &e) {
      EXPECT_STREQ(e.what(), value.c_str());
    } catch (...) {
      FAIL() << "Unexpected exception type thrown";
    }
  }
}

const map<const string, const Token> FLOATS{
    {"0.", Token(TokenType::FLOAT, stod("0."))},
    {"1.", Token(TokenType::FLOAT, stod("1."))},
    {"133.12", Token(TokenType::FLOAT, stod("133.12"))},
    {"13333.12", Token(TokenType::FLOAT, stod("13333.12"))},
    {"133.12", Token(TokenType::FLOAT, stod("133.12"))},
    {"13.12", Token(TokenType::FLOAT, stod("13.12"))},
    {"213.", Token(TokenType::FLOAT, stod("213."))},
    {"213.11111", Token(TokenType::FLOAT, stod("213.11111"))},
};

TEST(TestLexer, testFloats) {
  for (const auto &[key, value] : FLOATS) {
    string input = key;
    size_t shift = input.length() + 1;
    vector<Lexem> expected{
        {value, 1, 1},
        {Token(TokenType::END_OF_FILE), 1, shift},
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
    vector<Lexem> expected{
        {Token(TokenType::INTEGER, i), 1, 1},
        {Token(TokenType::END_OF_FILE), 1, shift},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);

    EXPECT_EQ(expected.size(), result.size());

    compareLexemVectors(expected, result);
  }
}

const vector<string> NEW_LINE_CHARACTERS{
    "\n",
    "\r",
    "\r\n",
};

TEST(TestLexer, testNewLineCharacters) {
  for (const auto &input : NEW_LINE_CHARACTERS) {
    vector<Lexem> expected{
        {Token(TokenType::END_OF_FILE), 2, 1},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);
    EXPECT_EQ(expected.size(), result.size());
    compareLexemVectors(expected, result);
  }
}

const vector<string> NEW_LINE_CHARACTERS_WITH_SOMETHING_AFTER{
    "\na",
    "\ra",
    "\r\na",
};

TEST(TestLexer, testNewLineCharactersWithSomethingAfter) {
  for (const auto &input : NEW_LINE_CHARACTERS_WITH_SOMETHING_AFTER) {
    vector<Lexem> expected{
        {Token(TokenType::IDENTIFIER, "a"), 2, 1},
        {Token(TokenType::END_OF_FILE), 2, 2},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = lexerize(l);
    EXPECT_EQ(expected.size(), result.size());
    compareLexemVectors(expected, result);
  }
}

const vector<string> NOT_A_TOKENS{".", "`", "~", "@", "^", "&", "?", "\\"};

TEST(TestLexer, testNotATokens) {
  for (const auto &input : NOT_A_TOKENS) {
    istringstream in(input);
    Lexer l(in);
    try {
      lexerize(l);
      cout << input << endl;
      FAIL() << "Expected LexerException";
    } catch (const LexerException &e) {
      EXPECT_STREQ(e.what(), "1:1: error: Not a token.");
    } catch (...) {
      FAIL() << "Unexpected exception type thrown";
    }
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
  vector<Lexem> expected{
      {Token(TokenType::ONE_LINE_COMMENT, "// These are one line comments."), 1,
       1},
      {Token(TokenType::MULTILINE_COMMENT, R"(/* these
are
multiline
comments */)"),
       2, 1},
      {Token(TokenType::END_OF_FILE), 5, 12},
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
  vector<Lexem> expected{
      {Token(TokenType::LET), 1, 1},
      {Token(TokenType::IDENTIFIER, "a"), 1, 5},
      {Token(TokenType::ASSIGN), 1, 7},
      {Token(TokenType::INTEGER, 12), 1, 9},
      {Token(TokenType::SEMICOLON), 1, 11},
      {Token(TokenType::ONE_LINE_COMMENT, "// int - const"), 1, 13},

      {Token(TokenType::LET), 2, 1},
      {Token(TokenType::MUT), 2, 5},
      {Token(TokenType::IDENTIFIER, "b"), 2, 9},
      {Token(TokenType::ASSIGN), 2, 11},
      {Token(TokenType::FLOAT, 12.12), 2, 13},
      {Token(TokenType::SEMICOLON), 2, 18},
      {Token(TokenType::ONE_LINE_COMMENT, "// flt - mutowalny"), 2, 20},

      {Token(TokenType::LET), 3, 1},
      {Token(TokenType::IDENTIFIER, "c"), 3, 5},
      {Token(TokenType::ASSIGN), 3, 7},
      {Token(TokenType::STRING, "duck"), 3, 9},
      {Token(TokenType::SEMICOLON), 3, 15},
      {Token(TokenType::ONE_LINE_COMMENT, "// str - const"), 3, 17},

      {Token(TokenType::LET), 4, 1},
      {Token(TokenType::MUT), 4, 5},
      {Token(TokenType::IDENTIFIER, "d"), 4, 9},
      {Token(TokenType::ASSIGN), 4, 11},
      {Token(TokenType::TRUE, true), 4, 13},
      {Token(TokenType::SEMICOLON), 4, 17},
      {Token(TokenType::ONE_LINE_COMMENT, "// bol - mutowalny"), 4, 19},

      {Token(TokenType::END_OF_FILE), 4, 37},
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
  ofstream file("test.txt");
  if (file.is_open()) {
    file << input;
    file.close();
  } else {
    FAIL() << "Failed to open the file!";
  }
  vector<Lexem> expected{
      {Token(TokenType::LET), 1, 1},
      {Token(TokenType::IDENTIFIER, "kaczka"), 1, 5},
      {Token(TokenType::ASSIGN), 1, 12},
      {Token(TokenType::IF), 1, 14},
      {Token(TokenType::LPARENT), 1, 17},
      {Token(TokenType::IDENTIFIER, "a"), 1, 18},
      {Token(TokenType::EQUALS), 1, 20},
      {Token(TokenType::INTEGER, 21), 1, 23},
      {Token(TokenType::RPARENT), 1, 25},

      {Token(TokenType::LBRACE), 1, 27},
      {Token(TokenType::INTEGER, 12), 1, 29},
      {Token(TokenType::RBRACE), 1, 32},
      {Token(TokenType::ELSE), 1, 34},
      {Token(TokenType::LBRACE), 1, 39},
      {Token(TokenType::STRING, "kaczka"), 1, 41},
      {Token(TokenType::RBRACE), 1, 50},
      {Token(TokenType::SEMICOLON), 1, 51},

      {Token(TokenType::WHILE), 2, 1},
      {Token(TokenType::LPARENT), 2, 6},
      {Token(TokenType::IDENTIFIER, "x"), 2, 7},
      {Token(TokenType::LESS), 2, 9},
      {Token(TokenType::INTEGER, 12), 2, 11},
      {Token(TokenType::RPARENT), 2, 13},
      {Token(TokenType::LBRACE), 2, 15},
      {Token(TokenType::IDENTIFIER, "x"), 2, 17},
      {Token(TokenType::ASSIGN), 2, 19},
      {Token(TokenType::IDENTIFIER, "x"), 2, 21},
      {Token(TokenType::PLUS), 2, 23},
      {Token(TokenType::FLOAT, 0.), 2, 25},
      {Token(TokenType::SEMICOLON), 2, 27},
      {Token(TokenType::RBRACE), 2, 29},

      {Token(TokenType::LET), 3, 1},
      {Token(TokenType::MUT), 3, 5},
      {Token(TokenType::IDENTIFIER, "b"), 3, 9},
      {Token(TokenType::ASSIGN), 3, 11},
      {Token(TokenType::INTEGER, 123), 3, 13},
      {Token(TokenType::LARROW), 3, 16},
      {Token(TokenType::STR), 3, 18},
      {Token(TokenType::SEMICOLON), 3, 21},

      {Token(TokenType::END_OF_FILE), 4, 1},
  };

  ifstream file_input("test.txt");
  Lexer l(file_input);
  vector<Lexem> result = lexerize(l);

  EXPECT_EQ(expected.size(), result.size());

  compareLexemVectors(expected, result);
  remove("test.txt");
}

///////////////////////////////////////////////////////////////////////////////
// some less ordered tests
TEST(TestLexer, testOperators) {
  string input = R"(+=*/-)";
  vector<Lexem> expected{
      {Token(TokenType::PLUS), 1, 1},  {Token(TokenType::ASSIGN), 1, 2},
      {Token(TokenType::STAR), 1, 3},  {Token(TokenType::SLASH), 1, 4},
      {Token(TokenType::MINUS), 1, 5}, {Token(TokenType::END_OF_FILE), 1, 6},
  };
  istringstream in(input);
  Lexer l(in);
  vector<Lexem> result = lexerize(l);

  EXPECT_EQ(expected.size(), result.size());

  compareLexemVectors(expected, result);
}

TEST(TestLexer, testBrackets) {
  string input = R"((){}[])";
  vector<Lexem> expected{
      {Token(TokenType::LPARENT), 1, 1},     {Token(TokenType::RPARENT), 1, 2},
      {Token(TokenType::LBRACE), 1, 3},      {Token(TokenType::RBRACE), 1, 4},
      {Token(TokenType::LBRACKET), 1, 5},    {Token(TokenType::RBRACKET), 1, 6},
      {Token(TokenType::END_OF_FILE), 1, 7},
  };
  istringstream in(input);
  Lexer l(in);
  vector<Lexem> result = lexerize(l);

  EXPECT_EQ(expected.size(), result.size());

  compareLexemVectors(expected, result);
}

TEST(TestLexer, testOneLineComments2) {
  string input = "//abcdefg1123\n\
++//3kaczuszka";
  vector<Lexem> expected{
      {Token(TokenType::ONE_LINE_COMMENT, "//abcdefg1123"), 1, 1},
      {Token(TokenType::PLUS), 2, 1},
      {Token(TokenType::PLUS), 2, 2},
      {Token(TokenType::ONE_LINE_COMMENT, "//3kaczuszka"), 2, 3},
      {Token(TokenType::END_OF_FILE), 2, 15},
  };
  istringstream in(input);
  Lexer l(in);
  vector<Lexem> result = lexerize(l);

  EXPECT_EQ(expected.size(), result.size());

  compareLexemVectors(expected, result);
}

TEST(TestLexer, testOneLineComments3) {
  string input = "//abcdefg    \t\t!@@#$\n//";
  vector<Lexem> expected{
      {Token(TokenType::ONE_LINE_COMMENT, "//abcdefg    \t\t!@@#$"), 1, 1},
      {Token(TokenType::ONE_LINE_COMMENT, "//"), 2, 1},
      {Token(TokenType::END_OF_FILE), 2, 3},
  };
  istringstream in(input);
  Lexer l(in);
  vector<Lexem> result = lexerize(l);

  EXPECT_EQ(expected.size(), result.size());

  compareLexemVectors(expected, result);
}

TEST(TestLexer, testMultilineComments) {
  string input = "/*abcdefll\n\
\n\
\n\
;'.,:1*/+=";
  vector<Lexem> expected{
      {Token(TokenType::MULTILINE_COMMENT, "/*abcdefll\n\n\n;'.,:1*/"), 1, 1},
      {Token(TokenType::PLUS), 4, 9},
      {Token(TokenType::ASSIGN), 4, 10},
      {Token(TokenType::END_OF_FILE), 4, 11},
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
  vector<Lexem> expected{
      {Token(TokenType::IDENTIFIER, "abcd123"), 1, 1},
      {Token(TokenType::IDENTIFIER, "abcd"), 2, 1},
      {Token(TokenType::PLUS), 3, 1},
      {Token(TokenType::PLUS), 3, 2},
      {Token(TokenType::EQUALS), 3, 3},
      {Token(TokenType::IDENTIFIER, "a__12311"), 3, 5},
      {Token(TokenType::END_OF_FILE), 4, 1},
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
  vector<Lexem> expected{
      {Token(TokenType::INTEGER, 123), 1, 1},
      {Token(TokenType::INTEGER, 111112), 2, 1},
      {Token(TokenType::INTEGER, 1230), 2, 8},
      {Token(TokenType::END_OF_FILE), 2, 12},
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
  vector<Lexem> expected{
      {Token(TokenType::INTEGER, 123), 1, 1},
      {Token(TokenType::IDENTIFIER, "abcd12"), 2, 1},
      {Token(TokenType::INTEGER, 1230), 2, 8},
      {Token(TokenType::STAR), 3, 1},
      {Token(TokenType::STAR), 3, 2},
      {Token(TokenType::INTEGER, 789), 3, 3},
      {Token(TokenType::IDENTIFIER, "a__12311"), 3, 7},
      {Token(TokenType::END_OF_FILE), 3, 15},
  };
  istringstream in(input);
  Lexer l(in);
  vector<Lexem> result = lexerize(l);

  EXPECT_EQ(expected.size(), result.size());

  compareLexemVectors(expected, result);
}

TEST(TestLexer, testIntegerMany) {
  string input = R"(1 2 3 4 5 6 7 8 9 10 11 12+13)";
  vector<Lexem> expected{
      {Token(TokenType::INTEGER, 1), 1, 1},
      {Token(TokenType::INTEGER, 2), 1, 3},
      {Token(TokenType::INTEGER, 3), 1, 5},
      {Token(TokenType::INTEGER, 4), 1, 7},
      {Token(TokenType::INTEGER, 5), 1, 9},
      {Token(TokenType::INTEGER, 6), 1, 11},
      {Token(TokenType::INTEGER, 7), 1, 13},
      {Token(TokenType::INTEGER, 8), 1, 15},
      {Token(TokenType::INTEGER, 9), 1, 17},
      {Token(TokenType::INTEGER, 10), 1, 19},
      {Token(TokenType::INTEGER, 11), 1, 22},
      {Token(TokenType::INTEGER, 12), 1, 25},
      {Token(TokenType::PLUS), 1, 27},
      {Token(TokenType::INTEGER, 13), 1, 28},
      {Token(TokenType::END_OF_FILE), 1, 30},
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
  istringstream in(input);
  Lexer l(in);
  try {
    lexerize(l);
    FAIL() << "Expected LexerException";
  } catch (const LexerException &e) {
    EXPECT_STREQ(e.what(), "1:4: error: Not a token.");
  } catch (...) {
    FAIL() << "Unexpected exception type thrown";
  }
}

TEST(TestLexer, testKeywords) {
  string input = "123 if while";
  vector<Lexem> expected{
      {Token(TokenType::INTEGER, 123), 1, 1},
      {Token(TokenType::IF), 1, 5},
      {Token(TokenType::WHILE), 1, 8},
      {Token(TokenType::END_OF_FILE), 1, 13},
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
  vector<Lexem> expected{
      {Token(TokenType::STRING, "kaczuszka"), 1, 1},
      {Token(TokenType::STRING, "cos    "), 3, 1},
      {Token(TokenType::END_OF_FILE), 3, 10},
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
  vector<Lexem> expected{
      {Token(TokenType::STRING, "kaczuszka\n\ncos    "), 1, 1},
      {Token(TokenType::END_OF_FILE), 3, 9},
  };
  istringstream in(input);
  Lexer l(in);
  vector<Lexem> result = lexerize(l);

  EXPECT_EQ(expected.size(), result.size());

  compareLexemVectors(expected, result);
}

TEST(TestLexer, testStringsWithDoubleQuotesInside) {
  string input = R"("kaczuszka mowi \"Hau, Hau!\"")";
  vector<Lexem> expected{
      {Token(TokenType::STRING, "kaczuszka mowi \"Hau, Hau!\""), 1, 1},
      {Token(TokenType::END_OF_FILE), 1, 31},
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
  vector<Lexem> expected{
      {Token(TokenType::STRING, "kaczuszka mowi:\n\
- hello!\n\
- hello!"),
       1, 1},
      {Token(TokenType::END_OF_FILE), 3, 10},
  };
  istringstream in(input);
  Lexer l(in);
  vector<Lexem> result = lexerize(l);

  EXPECT_EQ(expected.size(), result.size());

  compareLexemVectors(expected, result);
}

TEST(TestLexer, testFloat) {
  string input = R"(1.12345)";
  vector<Lexem> expected{
      {Token(TokenType::FLOAT, 1.12345), 1, 1},
      {Token(TokenType::END_OF_FILE), 1, 8},
  };
  istringstream in(input);
  Lexer l(in);
  vector<Lexem> result = lexerize(l);

  EXPECT_EQ(expected.size(), result.size());

  compareLexemVectors(expected, result);
}

TEST(TestLexer, testFloat2) {
  string input = R"(1.12345 0.0)";
  vector<Lexem> expected{
      {Token(TokenType::FLOAT, 1.12345), 1, 1},
      {Token(TokenType::FLOAT, 0.0), 1, 9},
      {Token(TokenType::END_OF_FILE), 1, 12},
  };
  istringstream in(input);
  Lexer l(in);
  vector<Lexem> result = lexerize(l);

  EXPECT_EQ(expected.size(), result.size());

  compareLexemVectors(expected, result);
}

TEST(TestLexer, testFloatWithoutNumbersAfterDot) {
  string input = R"(1.12345 0.)";
  vector<Lexem> expected{
      {Token(TokenType::FLOAT, 1.12345), 1, 1},
      {Token(TokenType::FLOAT, 0.), 1, 9},
      {Token(TokenType::END_OF_FILE), 1, 11},
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
  vector<Lexem> expected{{Token(TokenType::LET), 1, 1},
                         {Token(TokenType::IDENTIFIER, "kaczka"), 1, 5},
                         {Token(TokenType::ASSIGN), 1, 12},
                         {Token(TokenType::IF), 1, 14},
                         {Token(TokenType::LPARENT), 1, 17},
                         {Token(TokenType::IDENTIFIER, "a"), 1, 18},
                         {Token(TokenType::EQUALS), 1, 20},
                         {Token(TokenType::INTEGER, 21), 1, 23},
                         {Token(TokenType::RPARENT), 1, 25},

                         {Token(TokenType::LBRACE), 1, 27},
                         {Token(TokenType::INTEGER, 12), 1, 29},
                         {Token(TokenType::RBRACE), 1, 32},
                         {Token(TokenType::ELSE), 1, 34},
                         {Token(TokenType::LBRACE), 1, 39},
                         {Token(TokenType::STRING, "kaczka"), 1, 41},
                         {Token(TokenType::RBRACE), 1, 50},
                         {Token(TokenType::SEMICOLON), 1, 51},

                         {Token(TokenType::WHILE), 2, 1},
                         {Token(TokenType::LPARENT), 2, 6},
                         {Token(TokenType::IDENTIFIER, "x"), 2, 7},
                         {Token(TokenType::LESS), 2, 9},
                         {Token(TokenType::INTEGER, 12), 2, 11},
                         {Token(TokenType::RPARENT), 2, 13},
                         {Token(TokenType::LBRACE), 2, 15},
                         {Token(TokenType::IDENTIFIER, "x"), 2, 17},
                         {Token(TokenType::ASSIGN), 2, 19},
                         {Token(TokenType::IDENTIFIER, "x"), 2, 21},
                         {Token(TokenType::PLUS), 2, 23},
                         {Token(TokenType::FLOAT, 0.), 2, 25},
                         {Token(TokenType::SEMICOLON), 2, 27},
                         {Token(TokenType::RBRACE), 2, 29},

                         {Token(TokenType::LET), 3, 1},
                         {Token(TokenType::MUT), 3, 5},
                         {Token(TokenType::IDENTIFIER, "b"), 3, 9},
                         {Token(TokenType::ASSIGN), 3, 11},
                         {Token(TokenType::INTEGER, 123), 3, 13},
                         {Token(TokenType::LARROW), 3, 16},
                         {Token(TokenType::STR), 3, 18},
                         {Token(TokenType::SEMICOLON), 3, 21},

                         {Token(TokenType::END_OF_FILE), 4, 1}};
  istringstream in(input);
  Lexer l(in);
  vector<Lexem> result = lexerize(l);

  EXPECT_EQ(expected.size(), result.size());

  compareLexemVectors(expected, result);
}
