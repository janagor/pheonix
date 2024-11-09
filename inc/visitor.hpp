#pragma once

#include <iostream>
class IntegerLiteral;
class AdditiveExpression;
class MultiplicativeExpression;

class Visitor {
public:
    virtual void visit(IntegerLiteral& il) = 0;
    virtual void visit(AdditiveExpression&il) = 0;
    virtual void visit(MultiplicativeExpression&il) = 0;
    virtual ~Visitor() = default;
};

class PrintVisitor: public Visitor {
    void visit(IntegerLiteral &il) override;
    void visit(AdditiveExpression &il) override;
    void visit(MultiplicativeExpression &il) override;
};
