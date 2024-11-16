#pragma once

#include <iostream>
class AndExpression;
class ComparisonExpression;
class RelationalExpression;
class AdditiveExpression;
class MultiplicativeExpression;
class IntegerLiteral;

class Visitor {
public:
    virtual void visit(AndExpression&) = 0;
    virtual void visit(ComparisonExpression&) = 0;
    virtual void visit(RelationalExpression&) = 0;
    virtual void visit(AdditiveExpression&) = 0;
    virtual void visit(MultiplicativeExpression&) = 0;
    virtual void visit(IntegerLiteral&) = 0;
    virtual ~Visitor() = default;
};

class TreeGenVisitor: public Visitor {
public:
    TreeGenVisitor(): Visitor(), result() {};
    std::string getResult();

    void visit(AndExpression &me) override;
    void visit(ComparisonExpression &me) override;
    void visit(RelationalExpression &me) override;
    void visit(MultiplicativeExpression &me) override;
    void visit(AdditiveExpression &ae) override;
    void visit(IntegerLiteral &il) override;

private:
        std::string result;
};
