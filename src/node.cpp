#include "node.hpp"
#include "visitor.hpp"

namespace pheonix::node {

void Program::accept(visitor::Visitor &v) { v.visit(*this); }
void Parameter::accept(visitor::Visitor &v) { v.visit(*this); }
void DeclarationArguments::accept(visitor::Visitor &v) { v.visit(*this); }
void Block::accept(visitor::Visitor &v) { v.visit(*this); }
void FunctionDeclaration::accept(visitor::Visitor &v) { v.visit(*this); }
void VariableDeclaration::accept(visitor::Visitor &v) { v.visit(*this); }
void WhileLoopStatement::accept(visitor::Visitor &v) { v.visit(*this); }
void IfStatement::accept(visitor::Visitor &v) { v.visit(*this); }
void ReturnStatement::accept(visitor::Visitor &v) { v.visit(*this); }
void ExpressionStatement::accept(visitor::Visitor &v) { v.visit(*this); }
void AssignementExpression::accept(visitor::Visitor &v) { v.visit(*this); }
void OrExpression::accept(visitor::Visitor &v) { v.visit(*this); }
void AndExpression::accept(visitor::Visitor &v) { v.visit(*this); }
void ComparisonExpression::accept(visitor::Visitor &v) { v.visit(*this); }
void RelationalExpression::accept(visitor::Visitor &v) { v.visit(*this); }
void AdditiveExpression::accept(visitor::Visitor &v) { v.visit(*this); }
void MultiplicativeExpression::accept(visitor::Visitor &v) { v.visit(*this); }
void CompositiveExpression::accept(visitor::Visitor &v) { v.visit(*this); }
void CastExpression::accept(visitor::Visitor &v) { v.visit(*this); }
void PrefixExpression::accept(visitor::Visitor &v) { v.visit(*this); }
void CallExpression::accept(visitor::Visitor &v) { v.visit(*this); }
void DebugExpression::accept(visitor::Visitor &v) { v.visit(*this); }
void CallArguments::accept(visitor::Visitor &v) { v.visit(*this); }
void LambdaExpression::accept(visitor::Visitor &v) { v.visit(*this); }
void Identifier::accept(visitor::Visitor &v) { v.visit(*this); }
void ParentExpression::accept(visitor::Visitor &v) { v.visit(*this); }
void IntegerLiteral::accept(visitor::Visitor &v) { v.visit(*this); }
void FloatLiteral::accept(visitor::Visitor &v) { v.visit(*this); }
void BoolLiteral::accept(visitor::Visitor &v) { v.visit(*this); }
void StringLiteral::accept(visitor::Visitor &v) { v.visit(*this); }
void TypeSpecifier::accept(visitor::Visitor &v) { v.visit(*this); }

} // namespace pheonix::node
