#pragma once

#include <iostream>
class Program;
class Parameter;
class DeclarationArguments;
class Block;
class FunctionDeclaration;
class VariableDeclaration;
class WhileLoopStatement;
class IfStatement;
class ReturnStatement;
class ExpressionStatement;
class AssignementExpression;
class OrExpression;
class AndExpression;
class ComparisonExpression;
class RelationalExpression;
class AdditiveExpression;
class MultiplicativeExpression;
class CastExpression;
class PrefixExpression;
class CallExpression;
class DebugExpression;
class CallArguments;
class LambdaExpression;
class Identifier;
class ParentExpression;
class IntegerLiteral;
class FloatLiteral;
class BoolLiteral;
class StringLiteral;
class TypeSpecifier;

class Visitor {
public:
    virtual void visit(Program&) = 0;
    virtual void visit(Parameter&) = 0;
    virtual void visit(DeclarationArguments&) = 0;
    virtual void visit(Block&) = 0;
    virtual void visit(FunctionDeclaration&) = 0;
    virtual void visit(VariableDeclaration&) = 0;
    virtual void visit(WhileLoopStatement&) = 0;
    virtual void visit(IfStatement&) = 0;
    virtual void visit(ReturnStatement&) = 0;
    virtual void visit(ExpressionStatement&) = 0;
    virtual void visit(AssignementExpression&) = 0;
    virtual void visit(OrExpression&) = 0;
    virtual void visit(AndExpression&) = 0;
    virtual void visit(ComparisonExpression&) = 0;
    virtual void visit(RelationalExpression&) = 0;
    virtual void visit(AdditiveExpression&) = 0;
    virtual void visit(MultiplicativeExpression&) = 0;
    virtual void visit(CastExpression&) = 0;
    virtual void visit(PrefixExpression&) = 0;
    virtual void visit(CallExpression&) = 0;
    virtual void visit(DebugExpression&) = 0;
    virtual void visit(CallArguments&) = 0;
    virtual void visit(LambdaExpression&) = 0;
    virtual void visit(Identifier&) = 0;
    virtual void visit(ParentExpression&) = 0;
    virtual void visit(IntegerLiteral&) = 0;
    virtual void visit(FloatLiteral&) = 0;
    virtual void visit(BoolLiteral&) = 0;
    virtual void visit(StringLiteral&) = 0;
    virtual void visit(TypeSpecifier&) = 0;
    virtual ~Visitor() = default;
};

class TreeGenVisitor: public Visitor {
public:
    TreeGenVisitor(): Visitor(), result() {};
    std::string getResult();

    void visit(Program &p) override;
    void visit(Parameter &p) override;
    void visit(DeclarationArguments &p) override;
    void visit(Block &p) override;
    void visit(FunctionDeclaration &fd) override;
    void visit(VariableDeclaration &vd) override;
    void visit(WhileLoopStatement &wls) override;
    void visit(IfStatement &wls) override;
    void visit(ReturnStatement &rs) override;
    void visit(ExpressionStatement &es) override;
    void visit(AssignementExpression &ae) override;
    void visit(OrExpression &oe) override;
    void visit(AndExpression &ae) override;
    void visit(ComparisonExpression &ce) override;
    void visit(RelationalExpression &re) override;
    void visit(MultiplicativeExpression &me) override;
    void visit(AdditiveExpression &ae) override;
    void visit(CastExpression &ce) override;
    void visit(PrefixExpression &pe) override;
    void visit(CallExpression &ce) override;
    void visit(DebugExpression &de) override;
    void visit(CallArguments &ca) override;
    void visit(LambdaExpression &le) override;
    void visit(Identifier &i) override;
    void visit(ParentExpression &pe) override;
    void visit(IntegerLiteral &il) override;
    void visit(FloatLiteral &fl) override;
    void visit(BoolLiteral &il) override;
    void visit(StringLiteral &sl) override;
    void visit(TypeSpecifier &ts) override;

private:
        std::string result;
};
