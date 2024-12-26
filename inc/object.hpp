#pragma once
#include "node.hpp"

namespace pheonix::eval {
struct Object;

struct Function {
  Function() : body(nullptr) {}
  bool operator==(const Function &other) const {
    (void)(other);
    return true; // Or any condition you want to define
  }
  inline Function clone() {
    Function temp;
    temp.args = args;
    temp.body = this->body->clone();
    return temp;
  }

  Function(const Function &other) {
    if (other.body) {
      body =
          other.body->clone(); // Clone the body using the clone method of Node
    }
  }

  // Function() : argNum(0), returnValue(std::monostate()), names({}) {}
  Function(std::unique_ptr<node::Node> body) : body(std::move(body)) {}

  Function(const std::vector<Object> &args, std::unique_ptr<node::Node> body);

  Function(const std::vector<std::string> &args,
           std::unique_ptr<node::Node> body)
      : args(args), body(std::move(body)) {}
  // // public
  // size_t argNum;
  // Primitive returnValue;
  std::vector<std::string> args;
  // reference
  //
  // // private?
  // // NOTE: function declaration | lambda expression
  Function &operator=(const Function &other) {
    if (this != &other) {
      if (other.body) {
        body = other.body->clone();
      } else {
        body.reset();
      }
    }
    return *this;
  }
  std::unique_ptr<node::Node> body;
};

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

struct Object {
  Object() : value(std::monostate{}) {}
  Object(const Primitive &p) : value(castVariants(p)) {}
  Object(const ObjectValue &p) : value(p) {}
  Object clone() {
    if (std::holds_alternative<Function>(value)) {
      if (std::get<Function>(value).body) {
        return Object(std::get<Function>(value).clone());
      }
      return Object(Function());
    }
    return Object(value);
  }
  ObjectValue value;
};
} // namespace pheonix::eval
