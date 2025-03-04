#include "operator_visitor.hpp"

namespace pheonix::eval {

// infix operators
ObjectValue OperatorVisitor::operator()(const types::Integer &lhs,
                                        const types::Integer &rhs,
                                        const std::string &op) const {
  // arithmetic
  if (op == "+")
    return lhs + rhs;
  else if (op == "-")
    return lhs - rhs;
  else if (op == "*")
    return lhs * rhs;
  else if (op == "/")
    return lhs / rhs;
  else if (op == "%")
    return lhs % rhs;
  // comparison
  else if (op == "==")
    return bool(lhs == rhs);
  else if (op == "!=")
    return bool(lhs != rhs);
  else if (op == "<")
    return lhs < rhs;
  else if (op == ">")
    return (lhs > rhs);
  else if (op == "<=")
    return (lhs <= rhs);
  else if (op == ">=")
    return bool(lhs >= rhs);
  else
    throw std::runtime_error("Invalid transition");
}

ObjectValue OperatorVisitor::operator()(const types::Float &lhs,
                                        const types::Float &rhs,
                                        const std::string &op) const {
  // arithmetic
  if (op == "+")
    return lhs + rhs;
  else if (op == "-")
    return lhs - rhs;
  else if (op == "*")
    return lhs * rhs;
  else if (op == "/")
    return lhs / rhs;
  // comparison
  else if (op == "==")
    return lhs == rhs;
  else if (op == "!=")
    return lhs != rhs;
  else if (op == "<")
    return lhs < rhs;
  else if (op == ">")
    return lhs > rhs;
  else if (op == "<=")
    return lhs <= rhs;
  else if (op == ">=")
    return lhs >= rhs;
  else
    throw std::runtime_error("Invalid transition");
}

ObjectValue OperatorVisitor::operator()(bool lhs, bool rhs,
                                        const std::string &op) const {
  if (op == "&&")
    return lhs && rhs;
  else if (op == "||")
    return lhs || rhs;
  else
    throw std::runtime_error("Invalid transition");
}

ObjectValue OperatorVisitor::operator()(const std::string &lhs,
                                        const std::string &rhs,
                                        const std::string &op) const {
  if (op == "+")
    return lhs + rhs;
  // cast operator
  if ((op == "<-") && (rhs == "STR"))
    return lhs;
  else
    throw std::runtime_error("Invalid transition");
}

// cast operator
ObjectValue OperatorVisitor::operator()(types::Integer lhs,
                                        const std::string &rhs,
                                        const std::string &op) const {
  if ((op == "<-") && (rhs == "INT"))
    return lhs;
  if ((op == "<-") && (rhs == "STR"))
    return std::to_string(lhs.getValue());
  if ((op == "<-") && (rhs == "FLT"))
    return types::Float(lhs.getValue());
  if ((op == "<-") && (rhs == "BOL"))
    return lhs.getValue() != 0;
  else
    throw std::runtime_error("Invalid transition");
}

ObjectValue OperatorVisitor::operator()(types::Float lhs,
                                        const std::string &rhs,
                                        const std::string &op) const {
  if ((op == "<-") && (rhs == "INT"))
    return types::Integer(static_cast<int>(lhs.getValue()));
  if ((op == "<-") && (rhs == "STR"))
    return std::to_string(lhs.getValue());
  if ((op == "<-") && (rhs == "FLT"))
    return lhs;
  else
    throw std::runtime_error("Invalid transition");
}
ObjectValue OperatorVisitor::operator()(bool lhs, const std::string &rhs,
                                        const std::string &op) const {
  if ((op == "<-") && (rhs == "INT"))
    return lhs ? types::Integer(1) : types::Integer(0);
  if ((op == "<-") && (rhs == "STR"))
    return lhs ? "true" : "false";
  if ((op == "<-") && (rhs == "FLT"))
    return lhs ? types::Float(1) : types::Float(0);
  if ((op == "<-") && (rhs == "BOL"))
    return lhs;
  else
    throw std::runtime_error("Invalid transition");
}
ObjectValue OperatorVisitor::operator()(const Function &lhs,
                                        const Function &rhs,
                                        const std::string &op) const {
  if (op == "|") {
    auto res = lhs;
    if (rhs.args.size() != 1) {
      throw std::runtime_error("Rhs function wrong number of parameters.");
    }
    res.args2.push_back(rhs.args.at(0));
    for (const auto &arg2 : rhs.args2)
      res.args2.push_back(arg2);

    for (const auto &rhs_body : rhs.body)
      res.body.push_back(rhs_body->clone());
    assert(res.body.size() - 1 == res.args2.size());
    return res;
  } else
    throw std::runtime_error("Invalid transition");
}

// prefix operators
ObjectValue OperatorVisitor::operator()(const types::Integer &exp,
                                        const std::string &op) const {
  if (op == "-")
    return -exp;
  else
    return types::Integer(0);
}

ObjectValue OperatorVisitor::operator()(const types::Float &exp,
                                        const std::string &op) const {
  if (op == "-")
    return -exp;
  else
    return types::Integer(0);
}

ObjectValue OperatorVisitor::operator()(bool exp, const std::string &op) const {
  if (op == "!")
    return !exp;
  else
    return types::Integer(0);
}

// NOTE: Just handling references
ObjectValue
OperatorVisitor::operator()(const std::reference_wrapper<Object> &lhs,
                            const std::reference_wrapper<Object> &rhs,
                            const std::string &op) const {
  return std::visit(*this, lhs.get().value, rhs.get().value,
                    std::variant<std::string>(op));
}

} // namespace pheonix::eval
