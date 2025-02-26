#pragma once

#include <cassert>

#include "object.hpp"
#include "types.hpp"

namespace pheonix::eval {

struct OperatorVisitor {
  // infix operators
  ObjectValue operator()(const types::Integer &lhs, const types::Integer &rhs,
                         const std::string &op) const;
  ObjectValue operator()(const types::Float &lhs, const types::Float &rhs,
                         const std::string &op) const;
  ObjectValue operator()(bool lhs, bool rhs, const std::string &op) const;
  ObjectValue operator()(const std::string &lhs, const std::string &rhs,
                         const std::string &op) const;
  // cast operator
  ObjectValue operator()(types::Integer lhs, const std::string &rhs,
                         const std::string &op) const;
  ObjectValue operator()(types::Float lhs, const std::string &rhs,
                         const std::string &op) const;
  ObjectValue operator()(bool lhs, const std::string &rhs,
                         const std::string &op) const;
  ObjectValue operator()(const Function &lhs, const Function &rhs,
                         const std::string &op) const;
  // prefix operators
  ObjectValue operator()(const types::Integer &exp,
                         const std::string &op) const;
  ObjectValue operator()(const types::Float &exp, const std::string &op) const;
  ObjectValue operator()(bool exp, const std::string &op) const;
  // NOTE: Just handling references
  ObjectValue operator()(const std::reference_wrapper<Object> &lhs,
                         const std::reference_wrapper<Object> &rhs,
                         const std::string &op) const;

  template <typename T>
  ObjectValue operator()(const std::reference_wrapper<Object> &lhs,
                         const T &rhs, const std::string &op) const {
    return std::visit(*this, lhs.get().value, std::variant<T>(rhs),
                      std::variant<std::string>(op));
  }

  template <typename T>
  ObjectValue operator()(const T &lhs,
                         const std::reference_wrapper<Object> &rhs,
                         const std::string &op) const {
    return std::visit(*this, std::variant<T>(lhs), rhs.get().value,
                      std::variant<std::string>(op));
  }

  template <typename T>
  ObjectValue operator()(const std::reference_wrapper<Object> &left,
                         const T &right) const {
    return std::visit(OperatorVisitor{}, left.get().value,
                      std::variant<T>(right));
  }

  template <typename T>
  ObjectValue operator()(const T &left,
                         const std::reference_wrapper<Object> &right) const {
    return std::visit(OperatorVisitor{}, right.get().value,
                      std::variant<T>(left));
  }

  ObjectValue operator()(const auto &, const auto &, const auto &) const {
    throw std::runtime_error("Invalid transition");
  }

  ObjectValue operator()(const auto &, const auto &) const {
    throw std::runtime_error("Invalid transition");
  }
};

} // namespace pheonix::eval
