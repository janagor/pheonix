#include "../inc/parser.hpp"
#include <gtest/gtest.h>
#include <cassert>
#include <map>
using namespace std;
using namespace token;
using namespace lexer;
using namespace parser;

// helper
void compareExpectedAndReceived(const string& input, const string& expected) {
    istringstream in(input);
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    TreeGenVisitor visitor;
    output->accept(visitor);
    string received = visitor.getResult();
    EXPECT_EQ(expected, received);
}

const map<string, string> TRIVIAL_CASES {
    //IntegerLiteral
    {
"1;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(IntegerLiteral:\n\
            value=1)))"
    },
    // CastExpression
    {
"1<-int;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(CastExpression:\n\
            expression=(IntegerLiteral:\n\
                value=1),\n\
            type=(TypeSpecifier:\n\
                value=int))))"
    },
    {
"1<-str;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(CastExpression:\n\
            expression=(IntegerLiteral:\n\
                value=1),\n\
            type=(TypeSpecifier:\n\
                value=str))))"
    },
    {
"1<-flt;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(CastExpression:\n\
            expression=(IntegerLiteral:\n\
                value=1),\n\
            type=(TypeSpecifier:\n\
                value=flt))))"
    },
    {
"1<-bol;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(CastExpression:\n\
            expression=(IntegerLiteral:\n\
                value=1),\n\
            type=(TypeSpecifier:\n\
                value=bol))))"
    },
    // PrefixExpression
    {
"-2;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(PrefixExpression:\n\
            operator=[-],\n\
            expression=(IntegerLiteral:\n\
                value=2))))"
    },
    {
"!2;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(PrefixExpression:\n\
            operator=[!],\n\
            expression=(IntegerLiteral:\n\
                value=2))))"
    },
    // AdditiveExpression
    {
"1+2;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(AdditiveExpression:\n\
            left=(IntegerLiteral:\n\
                value=1),\n\
            operator=[+],\n\
            right=(IntegerLiteral:\n\
                value=2))))"
    },
    {
"100-100;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(AdditiveExpression:\n\
            left=(IntegerLiteral:\n\
                value=100),\n\
            operator=[-],\n\
            right=(IntegerLiteral:\n\
                value=100))))"
    },
    // MultipliveExpression
    {
"9*123;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(MultiplicativeExpression:\n\
            left=(IntegerLiteral:\n\
                value=9),\n\
            operator=[*],\n\
            right=(IntegerLiteral:\n\
                value=123))))"
    },
    {
"9/123;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(MultiplicativeExpression:\n\
            left=(IntegerLiteral:\n\
                value=9),\n\
            operator=[/],\n\
            right=(IntegerLiteral:\n\
                value=123))))"
    },
    // RelationalExpression
    {
"9<123;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(RelationalExpression:\n\
            left=(IntegerLiteral:\n\
                value=9),\n\
            operator=[<],\n\
            right=(IntegerLiteral:\n\
                value=123))))"
    },
    {
"9<=123;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(RelationalExpression:\n\
            left=(IntegerLiteral:\n\
                value=9),\n\
            operator=[<=],\n\
            right=(IntegerLiteral:\n\
                value=123))))"
    },
    {
"9>123;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(RelationalExpression:\n\
            left=(IntegerLiteral:\n\
                value=9),\n\
            operator=[>],\n\
            right=(IntegerLiteral:\n\
                value=123))))"
    },
    {
"9>=123;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(RelationalExpression:\n\
            left=(IntegerLiteral:\n\
                value=9),\n\
            operator=[>=],\n\
            right=(IntegerLiteral:\n\
                value=123))))"
    },
    // ComparisonExpression
    {
"9==123;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(ComparisonExpression:\n\
            left=(IntegerLiteral:\n\
                value=9),\n\
            operator=[==],\n\
            right=(IntegerLiteral:\n\
                value=123))))"
    },
    {
"9!=123;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(ComparisonExpression:\n\
            left=(IntegerLiteral:\n\
                value=9),\n\
            operator=[!=],\n\
            right=(IntegerLiteral:\n\
                value=123))))"
    },
    // AndExpression
    {
"9&&123;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(AndExpression:\n\
            left=(IntegerLiteral:\n\
                value=9),\n\
            operator=[&&],\n\
            right=(IntegerLiteral:\n\
                value=123))))"
    },
    // OrExpression
    {
"9||123;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(OrExpression:\n\
            left=(IntegerLiteral:\n\
                value=9),\n\
            operator=[||],\n\
            right=(IntegerLiteral:\n\
                value=123))))"
    },
    // AssignementExpression
    {
"$a=123;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(AssignementExpression:\n\
            identifier=a,\n\
            expression=(IntegerLiteral:\n\
                value=123))))"
    },
    // VariableDeclaration
    {
"let a = 123;",
"(Program:\n\
    (VariableDeclaration:\n\
        isMutable=false,\n\
        identifier=a,\n\
        expression=(ExpressionStatement:\n\
            expression=(IntegerLiteral:\n\
                value=123))))"
    },
    // WhileLoopStatement
    {
"while(1==1) { 1; }",
"(Program:\n\
    (WhileLoopStatement:\n\
        expression=(ComparisonExpression:\n\
            left=(IntegerLiteral:\n\
                value=1),\n\
            operator=[==],\n\
            right=(IntegerLiteral:\n\
                value=1)),\n\
        statements=(Block:\n\
            (ExpressionStatement:\n\
                expression=(IntegerLiteral:\n\
                    value=1)))))"
    },
    // ReturnStatement
    /*
    {
"return 123;",
"(Program:\n\
    (ReturnStatement:\n\
        expression=(ExpressionStatement:\n\
            expression=(IntegerLiteral:\n\
                value=123))))"
    },
    // function declaration
    {
"fn returnOne(mut num) {\n\
    return 123;\n\
}",
"(Program:\n\
    (FunctionDeclaration:\n\
        identifier=returnOne,\n\
        parameterList=(\n\
            (parameter=(\n\
                isMutable=true,\n\
                identifier=num))),\n\
        statements=(\n\
            (ReturnStatement:\n\
                expression=(ExpressionStatement:\n\
                    expression=(IntegerLiteral:\n\
                        value=123))))))"
    },
    // if statement without else
    {
"if (1 == 1) {\n\
    return 123;\n\
}",
"(Program:\n\
    (IfStatement:\n\
        predicate=(ComparisonExpression:\n\
            left=(IntegerLiteral:\n\
                value=1),\n\
            operator=[==],\n\
            right=(IntegerLiteral:\n\
                value=1)),\n\
        ifBody=(\n\
            (ReturnStatement:\n\
                expression=(ExpressionStatement:\n\
                    expression=(IntegerLiteral:\n\
                        value=123)))),\n\
        elseBody=(\n\
            )))"
    },
    */
    // if statement with one else
    /*
    {
"if (1 == 1) {\n\
    return 123;\n\
} else {\n\
    return 123;\n\
}",
"(Program:\n\
    (IfStatement:\n\
        predicate=(ComparisonExpression:\n\
            left=(IntegerLiteral:\n\
                value=1),\n\
            operator=[==],\n\
            right=(IntegerLiteral:\n\
                value=1)),\n\
        ifBody=(\n\
            (ReturnStatement:\n\
                expression=(ExpressionStatement:\n\
                    expression=(IntegerLiteral:\n\
                        value=123)))),\n\
        elseBody=(\n\
            (ReturnStatement:\n\
                expression=(ExpressionStatement:\n\
                    expression=(IntegerLiteral:\n\
                        value=123))))))"
    },
    */
};

TEST(TestParser, testTrivialCases) {
    for (const auto& [i, e] : TRIVIAL_CASES) {
        compareExpectedAndReceived(i, e);
    }
}
///////////////////////////////////////////////////////////////////////////////

const map<string, string> TWO_STATEMENTS {
    //IntegerLiteral
    {
"1; 1;",
"(Program:\n\
    (ExpressionStatement:\n\
        expression=(IntegerLiteral:\n\
            value=1)),\n\
    (ExpressionStatement:\n\
        expression=(IntegerLiteral:\n\
            value=1)))"
    },
};

TEST(TestParser, testTwoStatements) {
    for (const auto& [i, e] : TWO_STATEMENTS) {
        compareExpectedAndReceived(i, e);
    }
}
///////////////////////////////////////////////////////////////////////////////
const map<string, string> NON_TRIVIAL_CASES {
    //IntegerLiteral
    {
"1;",
"(ExpressionStatement:\n\
    expression=(IntegerLiteral:\n\
        value=1))"
    },
};

