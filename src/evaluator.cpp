#include "evaluator.hpp"

#include <iomanip>
#include <sstream>

// NOTE: remove this until it is done
#define UNUSED(x) (void)(x)

namespace pheonix::eval {

void Evaluator::visit(node::Parameter &p) { UNUSED(p); }

void Evaluator::visit(node::DeclarationArguments &da) { UNUSED(da); }

void Evaluator::visit(node::Block &b) { UNUSED(b); }

void Evaluator::visit(node::FunctionDeclaration &fd) { UNUSED(fd); }

void Evaluator::visit(node::WhileLoopStatement &wls) { UNUSED(wls); }

void Evaluator::visit(node::VariableDeclaration &vd) { UNUSED(vd); }

void Evaluator::visit(node::IfStatement &is) { UNUSED(is); }

void Evaluator::visit(node::ReturnStatement &rs) { UNUSED(rs); }

void Evaluator::visit(node::ExpressionStatement &es) { UNUSED(es); }

void Evaluator::visit(node::NullStatement &) {}

void Evaluator::visit(node::AssignementExpression &ae) { UNUSED(ae); }

void Evaluator::visit(node::OrExpression &oe) { UNUSED(oe); }

void Evaluator::visit(node::AndExpression &ae) { UNUSED(ae); }

void Evaluator::visit(node::ComparisonExpression &ce) { UNUSED(ce); }

void Evaluator::visit(node::RelationalExpression &re) { UNUSED(re); }

void Evaluator::visit(node::MultiplicativeExpression &me) { UNUSED(me); }

void Evaluator::visit(node::CompositiveExpression &me) { UNUSED(me); }

void Evaluator::visit(node::AdditiveExpression &ae) { UNUSED(ae); }

void Evaluator::visit(node::CastExpression &ce) { UNUSED(ce); }

void Evaluator::visit(node::PrefixExpression &pe) { UNUSED(pe); }

void Evaluator::visit(node::CallExpression &ce) { UNUSED(ce); }

void Evaluator::visit(node::DebugExpression &de) { UNUSED(de); }

void Evaluator::visit(node::CallArguments &ca) { UNUSED(ca); }

void Evaluator::visit(node::LambdaExpression &le) { UNUSED(le); }

void Evaluator::visit(node::Identifier &i) { UNUSED(i); }

void Evaluator::visit(node::ParentExpression &pe) { UNUSED(pe); }

void Evaluator::visit(node::Literal &il) { UNUSED(il); }

void Evaluator::visit(node::TypeSpecifier &ts) { UNUSED(ts); }

} // namespace pheonix::eval
