#pragma once

#include <iostream>
class OrExpression;
class AndExpression;
class ComparisonExpression;
class RelationalExpression;
class AdditiveExpression;
class MultiplicativeExpression;
class CastExpression;
class TypeSpecifier;
class IntegerLiteral;

class Visitor {
public:
    virtual void visit(OrExpression&) = 0;
    virtual void visit(AndExpression&) = 0;
    virtual void visit(ComparisonExpression&) = 0;
    virtual void visit(RelationalExpression&) = 0;
    virtual void visit(AdditiveExpression&) = 0;
    virtual void visit(MultiplicativeExpression&) = 0;
    virtual void visit(CastExpression&) = 0;
    virtual void visit(IntegerLiteral&) = 0;
    virtual void visit(TypeSpecifier&) = 0;
    virtual ~Visitor() = default;
};

class TreeGenVisitor: public Visitor {
public:
    TreeGenVisitor(): Visitor(), result() {};
    std::string getResult();

    void visit(OrExpression &oe) override;
    void visit(AndExpression &ae) override;
    void visit(ComparisonExpression &ce) override;
    void visit(RelationalExpression &re) override;
    void visit(MultiplicativeExpression &me) override;
    void visit(AdditiveExpression &ae) override;
    void visit(CastExpression &ce) override;
    void visit(TypeSpecifier &ts) override;
    void visit(IntegerLiteral &il) override;

private:
        std::string result;
};
