#include "parser.hpp"

#include <gtest/gtest.h>
#include <map>

using namespace std;
using namespace pheonix::token;
using namespace pheonix::exception;
using namespace pheonix::node;
using namespace pheonix::visitor;
using namespace pheonix::lexer;
using namespace pheonix::parser;

// helper
void compareExpectedAndReceived(const string &input, const string &expected) {
  istringstream in(input);
  Parser p(in);
  unique_ptr<Node> output = p.generateParsingTree();
  TreeGenVisitor visitor;
  output->accept(visitor);
  string received = visitor.getResult();
  EXPECT_EQ(expected, received);
}

const map<string, string> TRIVIAL_CASES{
    // Empty
    {"", "(Program:)"},
    // just ";"
    {";", "(Program:\n\
    (NullStatement:))"},
    // IntegerLiteral
    {"1;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(Literal:\n\
            type=Integer,\n\
            value=1)))"},
    // StringLiteral
    {"\"duck\";", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(Literal:\n\
            type=String,\n\
            value=duck)))"},
    // BoolLiteral
    {"true;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(Literal:\n\
            type=Bool,\n\
            value=true)))"},
    {"false;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(Literal:\n\
            type=Bool,\n\
            value=false)))"},
    // FloatLiteral
    {"1.1;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(Literal:\n\
            type=Float,\n\
            value=1.100)))"},
    // Identifier
    {"duck;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(Identifier:\n\
            value=duck)))"},
    // CastExpression
    {"1<-int;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CastExpression:\n\
            expression=(Literal:\n\
                type=Integer,\n\
                value=1),\n\
            type=(TypeSpecifier:\n\
                value=int))))"},
    {"1<-str;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CastExpression:\n\
            expression=(Literal:\n\
                type=Integer,\n\
                value=1),\n\
            type=(TypeSpecifier:\n\
                value=str))))"},
    {"1<-flt;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CastExpression:\n\
            expression=(Literal:\n\
                type=Integer,\n\
                value=1),\n\
            type=(TypeSpecifier:\n\
                value=flt))))"},
    {"1<-bol;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CastExpression:\n\
            expression=(Literal:\n\
                type=Integer,\n\
                value=1),\n\
            type=(TypeSpecifier:\n\
                value=bol))))"},
    // PrefixExpression
    {"-2;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(PrefixExpression:\n\
            operator=[-],\n\
            expression=(Literal:\n\
                type=Integer,\n\
                value=2))))"},
    {"!2;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(PrefixExpression:\n\
            operator=[!],\n\
            expression=(Literal:\n\
                type=Integer,\n\
                value=2))))"},
    // AdditiveExpression
    {"1+2;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(AdditiveExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=1),\n\
            operator=[+],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=2))))"},
    {"100-100;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(AdditiveExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=100),\n\
            operator=[-],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=100))))"},
    // MultipliveExpression
    {"9*123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(MultiplicativeExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=9),\n\
            operator=[*],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    {"9/123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(MultiplicativeExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=9),\n\
            operator=[/],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    // CompositiveExpression
    {"9|123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CompositiveExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=9),\n\
            operator=[|],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    // RelationalExpression
    {"9<123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(RelationalExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=9),\n\
            operator=[<],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    {"9<=123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(RelationalExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=9),\n\
            operator=[<=],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    {"9>123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(RelationalExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=9),\n\
            operator=[>],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    {"9>=123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(RelationalExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=9),\n\
            operator=[>=],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    // ComparisonExpression
    {"9==123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(ComparisonExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=9),\n\
            operator=[==],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    {"9!=123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(ComparisonExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=9),\n\
            operator=[!=],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    // AndExpression
    {"9&&123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(AndExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=9),\n\
            operator=[&&],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    // OrExpression
    {"9||123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(OrExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=9),\n\
            operator=[||],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    // AssignementExpression
    {"$a=123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(AssignementExpression:\n\
            identifier=a,\n\
            expression=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    // VariableDeclaration
    {"let a = 123;", "(Program:\n\
    (VariableDeclaration:\n\
        isMutable=false,\n\
        identifier=a,\n\
        expression=(Literal:\n\
            type=Integer,\n\
            value=123)))"},
    // WhileLoopStatement
    {"while(1==1) { 1; }", "(Program:\n\
    (WhileLoopStatement:\n\
        expression=(ComparisonExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=1),\n\
            operator=[==],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=1)),\n\
        statements=(Block:\n\
            (ExpressionStatement:\n\
                expression=(Literal:\n\
                    type=Integer,\n\
                    value=1)))))"},
    // ReturnStatement
    {"return;", "(Program:\n\
    (ReturnStatement:\n\
        expression=))"},
    {"return 123;", "(Program:\n\
    (ReturnStatement:\n\
        expression=(Literal:\n\
            type=Integer,\n\
            value=123)))"},
    // function declaration
    {"fn returnOne() {\n\
    return 123;\n\
}",
     "(Program:\n\
    (FunctionDeclaration:\n\
        identifier=returnOne,\n\
        arguments=(DeclarationArguments:),\n\
        statements=(Block:\n\
            (ReturnStatement:\n\
                expression=(Literal:\n\
                    type=Integer,\n\
                    value=123)))))"},
    {"fn returnOne(mut num) {\n\
    return 123;\n\
}",
     "(Program:\n\
    (FunctionDeclaration:\n\
        identifier=returnOne,\n\
        arguments=(DeclarationArguments:\n\
            (Parameter:\n\
                isMutable=true,\n\
                identifier=num)),\n\
        statements=(Block:\n\
            (ReturnStatement:\n\
                expression=(Literal:\n\
                    type=Integer,\n\
                    value=123)))))"},
    // if statement without else
    {"if (1 == 1) {\n\
    return 123;\n\
}",
     "(Program:\n\
    (IfStatement:\n\
        predicate=(ComparisonExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=1),\n\
            operator=[==],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=1)),\n\
        ifBody=(Block:\n\
            (ReturnStatement:\n\
                expression=(Literal:\n\
                    type=Integer,\n\
                    value=123))),\n\
        elseBody=))"},
    // if statement with one else
    {"if (1 == 1) {\n\
    return 123;\n\
} else {\n\
    return 123;\n\
}",
     "(Program:\n\
    (IfStatement:\n\
        predicate=(ComparisonExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=1),\n\
            operator=[==],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=1)),\n\
        ifBody=(Block:\n\
            (ReturnStatement:\n\
                expression=(Literal:\n\
                    type=Integer,\n\
                    value=123))),\n\
        elseBody=(Block:\n\
            (ReturnStatement:\n\
                expression=(Literal:\n\
                    type=Integer,\n\
                    value=123)))))"},
    // if statement with else if
    {"if (1 == 1) {\n\
    return 123;\n\
} else if (1==1) {\n\
    return 123;\n\
}",
     "(Program:\n\
    (IfStatement:\n\
        predicate=(ComparisonExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=1),\n\
            operator=[==],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=1)),\n\
        ifBody=(Block:\n\
            (ReturnStatement:\n\
                expression=(Literal:\n\
                    type=Integer,\n\
                    value=123))),\n\
        elseBody=(IfStatement:\n\
            predicate=(ComparisonExpression:\n\
                left=(Literal:\n\
                    type=Integer,\n\
                    value=1),\n\
                operator=[==],\n\
                right=(Literal:\n\
                    type=Integer,\n\
                    value=1)),\n\
            ifBody=(Block:\n\
                (ReturnStatement:\n\
                    expression=(Literal:\n\
                        type=Integer,\n\
                        value=123))),\n\
            elseBody=)))"},
    // CallExpression
    {"call(1);", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CallExpression:\n\
            function=(Identifier:\n\
                value=call),\n\
            arguments=(CallArguments:\n\
                (Literal:\n\
                    type=Integer,\n\
                    value=1)))))"},
    // ParentExpression
    {"(1);", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(ParentExpression:\n\
            expression=(Literal:\n\
                type=Integer,\n\
                value=1))))"},
    // LambdaExpression
    {"#(){return 123;};", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(LambdaExpression:\n\
            arguments=(DeclarationArguments:),\n\
            statements=(Block:\n\
                (ReturnStatement:\n\
                    expression=(Literal:\n\
                        type=Integer,\n\
                        value=123))))))"},
    {"#(mut num){ return 123; };", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(LambdaExpression:\n\
            arguments=(DeclarationArguments:\n\
                (Parameter:\n\
                    isMutable=true,\n\
                    identifier=num)),\n\
            statements=(Block:\n\
                (ReturnStatement:\n\
                    expression=(Literal:\n\
                        type=Integer,\n\
                        value=123))))))"},
    // DebugExpression
    {"[call](1);", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(DebugExpression:\n\
            function=(Identifier:\n\
                value=call),\n\
            arguments=(CallArguments:\n\
                (Literal:\n\
                    type=Integer,\n\
                    value=1)))))"},
};

TEST(TestParser, testTrivialCases) {
  for (const auto &[i, e] : TRIVIAL_CASES) {
    compareExpectedAndReceived(i, e);
  }
}
///////////////////////////////////////////////////////////////////////////////
const map<string, string> FUNCTION_CALLS{
    {"(call)(1);", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CallExpression:\n\
            function=(ParentExpression:\n\
                expression=(Identifier:\n\
                    value=call)),\n\
            arguments=(CallArguments:\n\
                (Literal:\n\
                    type=Integer,\n\
                    value=1)))))"},
    {"((call))(1);", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CallExpression:\n\
            function=(ParentExpression:\n\
                expression=(ParentExpression:\n\
                    expression=(Identifier:\n\
                        value=call))),\n\
            arguments=(CallArguments:\n\
                (Literal:\n\
                    type=Integer,\n\
                    value=1)))))"},
    {"((call))((1));", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CallExpression:\n\
            function=(ParentExpression:\n\
                expression=(ParentExpression:\n\
                    expression=(Identifier:\n\
                        value=call))),\n\
            arguments=(CallArguments:\n\
                (ParentExpression:\n\
                    expression=(Literal:\n\
                        type=Integer,\n\
                        value=1))))))"},
    {"((call))((1), 12);", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CallExpression:\n\
            function=(ParentExpression:\n\
                expression=(ParentExpression:\n\
                    expression=(Identifier:\n\
                        value=call))),\n\
            arguments=(CallArguments:\n\
                (ParentExpression:\n\
                    expression=(Literal:\n\
                        type=Integer,\n\
                        value=1)),\n\
                (Literal:\n\
                    type=Integer,\n\
                    value=12)))))"},
    {"(call(12))((1), 12);", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CallExpression:\n\
            function=(ParentExpression:\n\
                expression=(CallExpression:\n\
                    function=(Identifier:\n\
                        value=call),\n\
                    arguments=(CallArguments:\n\
                        (Literal:\n\
                            type=Integer,\n\
                            value=12)))),\n\
            arguments=(CallArguments:\n\
                (ParentExpression:\n\
                    expression=(Literal:\n\
                        type=Integer,\n\
                        value=1)),\n\
                (Literal:\n\
                    type=Integer,\n\
                    value=12)))))"},
    {"call()();", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CallExpression:\n\
            function=(CallExpression:\n\
                function=(Identifier:\n\
                    value=call),\n\
                arguments=(CallArguments:)),\n\
            arguments=(CallArguments:))))"},
    {"call(12)(12);", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CallExpression:\n\
            function=(CallExpression:\n\
                function=(Identifier:\n\
                    value=call),\n\
                arguments=(CallArguments:\n\
                    (Literal:\n\
                        type=Integer,\n\
                        value=12))),\n\
            arguments=(CallArguments:\n\
                (Literal:\n\
                    type=Integer,\n\
                    value=12)))))"},
    {"call(12)((1), 12)();", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CallExpression:\n\
            function=(CallExpression:\n\
                function=(CallExpression:\n\
                    function=(Identifier:\n\
                        value=call),\n\
                    arguments=(CallArguments:\n\
                        (Literal:\n\
                            type=Integer,\n\
                            value=12))),\n\
                arguments=(CallArguments:\n\
                    (ParentExpression:\n\
                        expression=(Literal:\n\
                            type=Integer,\n\
                            value=1)),\n\
                    (Literal:\n\
                        type=Integer,\n\
                        value=12))),\n\
            arguments=(CallArguments:))))"},
    {"call1(call2((1), 12))();", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CallExpression:\n\
            function=(CallExpression:\n\
                function=(Identifier:\n\
                    value=call1),\n\
                arguments=(CallArguments:\n\
                    (CallExpression:\n\
                        function=(Identifier:\n\
                            value=call2),\n\
                        arguments=(CallArguments:\n\
                            (ParentExpression:\n\
                                expression=(Literal:\n\
                                    type=Integer,\n\
                                    value=1)),\n\
                            (Literal:\n\
                                type=Integer,\n\
                                value=12))))),\n\
            arguments=(CallArguments:))))"},
    // with lambda expressions
    {"#(){return 123;}();", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CallExpression:\n\
            function=(LambdaExpression:\n\
                arguments=(DeclarationArguments:),\n\
                statements=(Block:\n\
                    (ReturnStatement:\n\
                        expression=(Literal:\n\
                            type=Integer,\n\
                            value=123)))),\n\
            arguments=(CallArguments:))))"},
    {"#(mut num){ return 123; }();", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(CallExpression:\n\
            function=(LambdaExpression:\n\
                arguments=(DeclarationArguments:\n\
                    (Parameter:\n\
                        isMutable=true,\n\
                        identifier=num)),\n\
                statements=(Block:\n\
                    (ReturnStatement:\n\
                        expression=(Literal:\n\
                            type=Integer,\n\
                            value=123)))),\n\
            arguments=(CallArguments:))))"},
};

TEST(TestParser, testFunctionCalls) {
  for (const auto &[i, e] : FUNCTION_CALLS) {
    compareExpectedAndReceived(i, e);
  }
}
///////////////////////////////////////////////////////////////////////////////
const map<string, string> TWO_STATEMENTS{
    // IntegerLiteral
    {"1; 1;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(Literal:\n\
            type=Integer,\n\
            value=1)),\n\
    (ExpressionStatement:\n\
        expression=(Literal:\n\
            type=Integer,\n\
            value=1)))"},
    {"-2 + 12;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(AdditiveExpression:\n\
            left=(PrefixExpression:\n\
                operator=[-],\n\
                expression=(Literal:\n\
                    type=Integer,\n\
                    value=2)),\n\
            operator=[+],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=12))))"},
    {"12 + -2;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(AdditiveExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=12),\n\
            operator=[+],\n\
            right=(PrefixExpression:\n\
                operator=[-],\n\
                expression=(Literal:\n\
                    type=Integer,\n\
                    value=2)))))"},
    {"9*123+12;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(AdditiveExpression:\n\
            left=(MultiplicativeExpression:\n\
                left=(Literal:\n\
                    type=Integer,\n\
                    value=9),\n\
                operator=[*],\n\
                right=(Literal:\n\
                    type=Integer,\n\
                    value=123)),\n\
            operator=[+],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=12))))"},
    {"12+9*123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(AdditiveExpression:\n\
            left=(Literal:\n\
                type=Integer,\n\
                value=12),\n\
            operator=[+],\n\
            right=(MultiplicativeExpression:\n\
                left=(Literal:\n\
                    type=Integer,\n\
                    value=9),\n\
                operator=[*],\n\
                right=(Literal:\n\
                    type=Integer,\n\
                    value=123)))))"},
    // AdditiveExpressions
    {"12+9+123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(AdditiveExpression:\n\
            left=(AdditiveExpression:\n\
                left=(Literal:\n\
                    type=Integer,\n\
                    value=12),\n\
                operator=[+],\n\
                right=(Literal:\n\
                    type=Integer,\n\
                    value=9)),\n\
            operator=[+],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    // MultiplicativeExpressions
    {"12*9*123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(MultiplicativeExpression:\n\
            left=(MultiplicativeExpression:\n\
                left=(Literal:\n\
                    type=Integer,\n\
                    value=12),\n\
                operator=[*],\n\
                right=(Literal:\n\
                    type=Integer,\n\
                    value=9)),\n\
            operator=[*],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    // RelationalExpression
    {"12<9<123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(RelationalExpression:\n\
            left=(RelationalExpression:\n\
                left=(Literal:\n\
                    type=Integer,\n\
                    value=12),\n\
                operator=[<],\n\
                right=(Literal:\n\
                    type=Integer,\n\
                    value=9)),\n\
            operator=[<],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    // ComparisonExpressions
    {"12==9==123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(ComparisonExpression:\n\
            left=(ComparisonExpression:\n\
                left=(Literal:\n\
                    type=Integer,\n\
                    value=12),\n\
                operator=[==],\n\
                right=(Literal:\n\
                    type=Integer,\n\
                    value=9)),\n\
            operator=[==],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    // AndExpressions
    {"12&&9&&123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(AndExpression:\n\
            left=(AndExpression:\n\
                left=(Literal:\n\
                    type=Integer,\n\
                    value=12),\n\
                operator=[&&],\n\
                right=(Literal:\n\
                    type=Integer,\n\
                    value=9)),\n\
            operator=[&&],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
    // OrExpressions
    {"12||9||123;", "(Program:\n\
    (ExpressionStatement:\n\
        expression=(OrExpression:\n\
            left=(OrExpression:\n\
                left=(Literal:\n\
                    type=Integer,\n\
                    value=12),\n\
                operator=[||],\n\
                right=(Literal:\n\
                    type=Integer,\n\
                    value=9)),\n\
            operator=[||],\n\
            right=(Literal:\n\
                type=Integer,\n\
                value=123))))"},
};

TEST(TestParser, testTwoStatements) {
  for (const auto &[i, e] : TWO_STATEMENTS) {
    compareExpectedAndReceived(i, e);
  }
}
///////////////////////////////////////////////////////////////////////////////

const map<string, string> SOME_FROM_DOCUMENTATION{
    {"fn returnOne(arg1, arg2) {\n\
    \n\
}",
     "(Program:\n\
    (FunctionDeclaration:\n\
        identifier=returnOne,\n\
        arguments=(DeclarationArguments:\n\
            (Parameter:\n\
                isMutable=false,\n\
                identifier=arg1),\n\
            (Parameter:\n\
                isMutable=false,\n\
                identifier=arg2)),\n\
        statements=(Block:\n\
            )))"},
};

TEST(TestParser, some_from_documentation) {
  for (const auto &[i, e] : SOME_FROM_DOCUMENTATION) {
    compareExpectedAndReceived(i, e);
  }
}
////////////////////////////////////////////////////////////////////////////////
// testing errors

TEST(TestParser, testErrorsNoSemicolon) {
  string input = "kaczka";

  istringstream in(input);
  try {
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    // TreeGenVisitor visitor;
    // output->accept(visitor);
    FAIL() << "Expected ParserException";
  } catch (const ParserException &e) {
    EXPECT_STREQ(e.what(), "Expected: SEMICOLON, Got: END_OF_FILE.");
    EXPECT_EQ(e.getLine(), 1);
    EXPECT_EQ(e.getColumn(), 7);
  } catch (...) {
    FAIL() << "Unexpected exception type thrown";
  }
}

TEST(TestParser, testErrorsNoSemicolon2) {
  string input = "#(){}";

  istringstream in(input);
  try {
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    // TreeGenVisitor visitor;
    // output->accept(visitor);
    FAIL() << "Expected ParserException";
  } catch (const ParserException &e) {
    EXPECT_STREQ(e.what(), "Expected: SEMICOLON, Got: END_OF_FILE.");
    EXPECT_EQ(e.getLine(), 1);
    EXPECT_EQ(e.getColumn(), 6);
  } catch (...) {
    FAIL() << "Unexpected exception type thrown";
  }
}
TEST(TestParser, notFinishedParenthesis1) {
  string input = "#(){";

  istringstream in(input);
  try {
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    // TreeGenVisitor visitor;
    // output->accept(visitor);
    FAIL() << "Expected ParserException";
  } catch (const ParserException &e) {
    EXPECT_STREQ(e.what(), "Expected: RBRACE, Got: END_OF_FILE.");
    EXPECT_EQ(e.getLine(), 1);
    EXPECT_EQ(e.getColumn(), 5);
  } catch (...) {
    FAIL() << "Unexpected exception type thrown";
  }
}
TEST(TestParser, notFinishedParenthesis2) {
  string input = "#({}";

  istringstream in(input);
  try {
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    // TreeGenVisitor visitor;
    // output->accept(visitor);
    FAIL() << "Expected ParserException";
  } catch (const ParserException &e) {
    EXPECT_STREQ(e.what(), "Expected: RPARENT, Got: LBRACE.");
    EXPECT_EQ(e.getLine(), 1);
    EXPECT_EQ(e.getColumn(), 3);
  } catch (...) {
    FAIL() << "Unexpected exception type thrown";
  }
}

TEST(TestParser, NoRValue) {
  string input = "let kaczka = ;";

  istringstream in(input);
  try {
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    // TreeGenVisitor visitor;
    // output->accept(visitor);
    FAIL() << "Expected ParserException";
  } catch (const ParserException &e) {
    EXPECT_STREQ(e.what(), "Expected expression.");
    EXPECT_EQ(e.getLine(), 1);
    EXPECT_EQ(e.getColumn(), 14);
  } catch (...) {
    FAIL() << "Unexpected exception type thrown";
  }
}

TEST(TestParser, wrongUsageOfKeyword) {
  string input = "leT kaczka = 12 ;";

  istringstream in(input);
  try {
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    // TreeGenVisitor visitor;
    // output->accept(visitor);
    FAIL() << "Expected ParserException";
  } catch (const ParserException &e) {
    EXPECT_STREQ(e.what(), "Expected: SEMICOLON, Got: IDENTIFIER.");
    EXPECT_EQ(e.getLine(), 1);
    EXPECT_EQ(e.getColumn(), 5);
  } catch (...) {
    FAIL() << "Unexpected exception type thrown";
  }
}

TEST(TestParser, WrongEQSign) {
  string input = "if (kaczka = 12) {}";

  istringstream in(input);
  try {
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    // TreeGenVisitor visitor;
    // output->accept(visitor);
    FAIL() << "Expected ParserException";
  } catch (const ParserException &e) {
    EXPECT_STREQ(e.what(), "Expected: RPARENT, Got: ASSIGN.");
    EXPECT_EQ(e.getLine(), 1);
    EXPECT_EQ(e.getColumn(), 12);
  } catch (...) {
    FAIL() << "Unexpected exception type thrown";
  }
}

TEST(TestParser, NoPredicate) {
  string input = "if () {};";

  istringstream in(input);
  try {
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    // TreeGenVisitor visitor;
    // output->accept(visitor);
    FAIL() << "Expected ParserException";
  } catch (const ParserException &e) {
    EXPECT_STREQ(e.what(), "Expected predicate in if statement");
    EXPECT_EQ(e.getLine(), 1);
    EXPECT_EQ(e.getColumn(), 5);
  } catch (...) {
    FAIL() << "Unexpected exception type thrown";
  }
}

TEST(TestParser, NoPredicate2) {
  string input = "while () {};";

  istringstream in(input);
  try {
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    // TreeGenVisitor visitor;
    // output->accept(visitor);
    FAIL() << "Expected ParserException";
  } catch (const ParserException &e) {
    EXPECT_STREQ(e.what(), "Expected predicate in while statement");
    EXPECT_EQ(e.getLine(), 1);
    EXPECT_EQ(e.getColumn(), 8);
  } catch (...) {
    FAIL() << "Unexpected exception type thrown";
  }
}

TEST(TestParser, ArgumentsListWithCommaAtTheEnd) {
  string input = "fn kaczka(kaczka,){}";

  istringstream in(input);
  try {
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    // TreeGenVisitor visitor;
    // output->accept(visitor);
    FAIL() << "Expected ParserException";
  } catch (const ParserException &e) {
    EXPECT_STREQ(e.what(), "Error parsing parameter list.");
    EXPECT_EQ(e.getLine(), 1);
    EXPECT_EQ(e.getColumn(), 18);
  } catch (...) {
    FAIL() << "Unexpected exception type thrown";
  }
}

TEST(TestParser, LetWithoutLvalue) {
  string input = "let =12;";

  istringstream in(input);
  try {
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    // TreeGenVisitor visitor;
    // output->accept(visitor);
    FAIL() << "Expected ParserException";
  } catch (const ParserException &e) {
    EXPECT_STREQ(e.what(), "Expected: IDENTIFIER, Got: ASSIGN.");
    EXPECT_EQ(e.getLine(), 1);
    EXPECT_EQ(e.getColumn(), 5);
  } catch (...) {
    FAIL() << "Unexpected exception type thrown";
  }
}

TEST(TestParser, AssignmentWithoutRvalue) {
  string input = "$kaczka = ;";

  istringstream in(input);
  try {
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    // TreeGenVisitor visitor;
    // output->accept(visitor);
    FAIL() << "Expected ParserException";
  } catch (const ParserException &e) {
    EXPECT_STREQ(e.what(), "Expected expression.");
    EXPECT_EQ(e.getLine(), 1);
    EXPECT_EQ(e.getColumn(), 11);
  } catch (...) {
    FAIL() << "Unexpected exception type thrown";
  }
}
