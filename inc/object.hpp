#pragma once
#include "node.hpp"

namespace pheonix::eval {
struct Object;

struct Function {
  Function() : body() {}
  bool operator==(const Function &other) const {
    (void)(other);
    return true; // Or any condition you want to define
  }

  inline Function clone() {
    Function temp;
    temp.args = args;
    temp.args2 = args2;
    for (const auto &node : body) {
      temp.body.push_back(node->clone());
    }
    return temp;
  }

  Function(const Function &other) {
    args = other.args;
    args2 = other.args2;
    for (const auto &node : other.body) {
      body.push_back(node->clone());
    }
  }

  // Function() : argNum(0), returnValue(std::monostate()), names({}) {}
  Function(std::unique_ptr<node::Node> b) : body() {
    body.push_back(std::move(b));
  }

  Function(const std::vector<Object> &args, std::unique_ptr<node::Node> body);

  Function(const std::vector<std::string> &args, std::unique_ptr<node::Node> b)
      : args(args), body() {
    body.push_back(std::move(b));
  }
  // // public
  // size_t argNum;
  // Primitive returnValue;
  // reference
  //
  // // private?
  // // NOTE: function declaration | lambda expression
  Function &operator=(const Function &other) {
    args = other.args;
    args2 = other.args2;
    if (this != &other) {
      body.clear();
      if (!other.body.empty()) {
        for (const auto &node : other.body)
          body.push_back(node->clone());
      }
    }
    return *this;
  }
  // those are used for `normal` functions
  std::vector<std::string> args;
  // those are used for compisition functions
  std::vector<std::string> args2;
  std::vector<std::unique_ptr<node::Node>> body;
};

inline std::ostream &operator<<(std::ostream &os, const Function &var) {
  auto args = std::string('_', var.args.size());
  os << "Function(" << args << ")";
  return os;
}

using ObjectValue = std::variant<std::monostate, types::Integer, types::Float,
                                 std::string, bool, Function>;

inline ObjectValue castVariants(const Primitive &p) {
  if (std::holds_alternative<types::Integer>(p))
    return std::get<types::Integer>(p);
  if (std::holds_alternative<types::Float>(p))
    return std::get<types::Float>(p);
  if (std::holds_alternative<std::string>(p))
    return std::get<std::string>(p);
  if (std::holds_alternative<bool>(p))
    return std::get<bool>(p);
  return std::monostate();
}

inline std::ostream &operator<<(std::ostream &os, const ObjectValue &var) {
  std::visit(
      [&os](const auto &value) {
        if constexpr (std::is_same_v<std::decay_t<decltype(value)>, bool>) {
          os << (value ? "true" : "false");
        } else {

          os << value;
        }
      },
      var);
  return os;
}

struct Object {
  Object() : value(std::monostate{}) {}
  Object(const Primitive &p) : value(castVariants(p)) {}
  Object(const ObjectValue &p) : value(p) {}
  Object clone() {
    if (std::holds_alternative<Function>(value)) {
      if (!std::get<Function>(value).body.empty()) {
        return Object(std::get<Function>(value).clone());
      }
      return Object(Function());
    }
    return Object(value);
  }
  ObjectValue value;
};
} // namespace pheonix::eval
