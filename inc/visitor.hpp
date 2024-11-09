#pragma once

#include <iostream>
class IntegerLiteral;
class AdditiveExpression;
class MultiplicativeExpression;

class Visitor {
public:
    virtual void visit(IntegerLiteral&) = 0;
    virtual void visit(AdditiveExpression&) = 0;
    virtual void visit(MultiplicativeExpression&) = 0;
    virtual ~Visitor() = default;
};

class TreeGenVisitor: public Visitor {
public:
    TreeGenVisitor(): Visitor(), result() {};
    std::string getResult();

    void visit(IntegerLiteral &il) override;
    void visit(AdditiveExpression &ae) override;
    void visit(MultiplicativeExpression &me) override;

private:
        std::string result;
};
