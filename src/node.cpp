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
void NullStatement::accept(visitor::Visitor &v) { v.visit(*this); }
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
void Literal::accept(visitor::Visitor &v) { v.visit(*this); }
void TypeSpecifier::accept(visitor::Visitor &v) { v.visit(*this); }
void PrintFunction::accept(visitor::Visitor &v) { v.visit(*this); }

std::unique_ptr<Node> Program::clone() const {
  auto program = std::make_unique<Program>();
  for (size_t i = 0; i < this->statements.size(); ++i)
    program->statements.push_back(this->statements[i]->clone());
  return program;
}

std::unique_ptr<Node> Parameter::clone() const {
  return std::make_unique<Parameter>(this->isMutable, this->identifier);
}

std::unique_ptr<Node> DeclarationArguments::clone() const {
  auto declArgs = std::make_unique<DeclarationArguments>();
  for (size_t i = 0; i < this->arguments.size(); ++i)
    declArgs->arguments.push_back(this->arguments[i]->clone());
  return declArgs;
}

std::unique_ptr<Node> Block::clone() const {
  auto block = std::make_unique<Block>();
  for (size_t i = 0; i < this->statements.size(); ++i)
    block->statements.push_back(this->statements[i]->clone());
  return block;
}

std::unique_ptr<Node> FunctionDeclaration::clone() const {
  auto fn = std::make_unique<FunctionDeclaration>(this->identifier);
  fn->statements = this->statements->clone();
  fn->arguments = this->arguments->clone();
  return fn;
}

std::unique_ptr<Node> VariableDeclaration::clone() const {
  return std::make_unique<VariableDeclaration>(
      this->isMutable, this->identifier, this->expression->clone());
}

std::unique_ptr<Node> WhileLoopStatement::clone() const {
  return std::make_unique<WhileLoopStatement>(this->expression->clone(),
                                              this->statements->clone());
}

std::unique_ptr<Node> IfStatement::clone() const {
  auto ifStmt = std::make_unique<IfStatement>(this->predicate->clone());
  ifStmt->ifBody = this->ifBody->clone();
  if (this->elseBody)
    ifStmt->elseBody = this->elseBody->clone();
  return ifStmt;
}

std::unique_ptr<Node> ReturnStatement::clone() const {
  return std::make_unique<ReturnStatement>(this->expression->clone());
}

std::unique_ptr<Node> ExpressionStatement::clone() const {
  return std::make_unique<ExpressionStatement>(this->expression->clone());
}

std::unique_ptr<Node> NullStatement::clone() const {
  return std::make_unique<NullStatement>();
}

std::unique_ptr<Node> AssignementExpression::clone() const {
  return std::make_unique<AssignementExpression>(this->identifier,
                                                 this->expression->clone());
}

std::unique_ptr<Node> OrExpression::clone() const {
  return std::make_unique<OrExpression>(
      this->left->clone(), (this->right ? this->right->clone() : nullptr),
      this->op);
}

std::unique_ptr<Node> AndExpression::clone() const {
  return std::make_unique<AndExpression>(
      this->left->clone(), (this->right ? this->right->clone() : nullptr),
      this->op);
}

std::unique_ptr<Node> ComparisonExpression::clone() const {
  return std::make_unique<ComparisonExpression>(
      this->left->clone(), (this->right ? this->right->clone() : nullptr),
      this->op);
}

std::unique_ptr<Node> RelationalExpression::clone() const {
  return std::make_unique<RelationalExpression>(
      this->left->clone(), (this->right ? this->right->clone() : nullptr),
      this->op);
}

std::unique_ptr<Node> AdditiveExpression::clone() const {
  return std::make_unique<AdditiveExpression>(
      this->left->clone(), (this->right ? this->right->clone() : nullptr),
      this->op);
}

std::unique_ptr<Node> MultiplicativeExpression::clone() const {
  return std::make_unique<MultiplicativeExpression>(
      this->left->clone(), (this->right ? this->right->clone() : nullptr),
      this->op);
}

std::unique_ptr<Node> CompositiveExpression::clone() const {
  return std::make_unique<CompositiveExpression>(
      this->left->clone(), (this->right ? this->right->clone() : nullptr));
}

std::unique_ptr<Node> CastExpression::clone() const {
  return std::make_unique<CastExpression>(this->expression->clone(),
                                          this->type->clone());
}

std::unique_ptr<Node> PrefixExpression::clone() const {
  return std::make_unique<PrefixExpression>(this->op,
                                            this->expression->clone());
}

std::unique_ptr<Node> CallExpression::clone() const {
  return std::make_unique<CallExpression>(this->function->clone(),
                                          this->arguments->clone());
}

std::unique_ptr<Node> DebugExpression::clone() const {
  return std::make_unique<DebugExpression>(this->function->clone(),
                                           this->arguments->clone());
}

std::unique_ptr<Node> CallArguments::clone() const {
  auto arguments = std::make_unique<CallArguments>();
  for (size_t i = 0; i < this->arguments.size(); ++i)
    arguments->arguments.push_back(this->arguments[i]->clone());
  return arguments;
}

std::unique_ptr<Node> LambdaExpression::clone() const {
  return std::make_unique<LambdaExpression>(this->arguments->clone(),
                                            this->statements->clone());
}

std::unique_ptr<Node> Identifier::clone() const {
  return std::make_unique<Identifier>(this->value);
}

std::unique_ptr<Node> ParentExpression::clone() const {
  return std::make_unique<ParentExpression>(this->expression->clone());
}

std::unique_ptr<Node> Literal::clone() const {
  return std::make_unique<Literal>(this->value);
}

std::unique_ptr<Node> TypeSpecifier::clone() const {
  return std::make_unique<TypeSpecifier>(this->typeName);
}

std::unique_ptr<Node> PrintFunction::clone() const {
  return std::make_unique<PrintFunction>();
}

} // namespace pheonix::node
