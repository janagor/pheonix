#pragma once
#include "node.hpp"

namespace pheonix::eval {

struct Object;
struct Function;
using ObjectValue =
    std::variant<std::monostate, types::Integer, types::Float, std::string,
                 bool, Function, std::reference_wrapper<Object>>;

struct Param {
  std::string name;
  bool mut;
};

struct Function {
  Function();
  Function(std::unique_ptr<node::Node> b);
  Function(const std::vector<Object> &args, std::unique_ptr<node::Node> body);
  Function(const std::vector<Param> &args, std::unique_ptr<node::Node> b);
  Function(const Function &other);
  bool operator==(const Function &other) const;
  Function clone() const;
  Function &operator=(const Function &other);

  // those are used for `normal` functions
  std::vector<Param> args;
  // those are used for compisition functions
  std::vector<Param> args2;
  std::vector<std::unique_ptr<node::Node>> body;
};

std::ostream &operator<<(std::ostream &os, const Function &var);
ObjectValue castVariants(const Primitive &p);
std::ostream &operator<<(std::ostream &os, const ObjectValue &var);

struct Object {
  Object();
  Object(const Primitive &p, bool mut = false);
  Object(const ObjectValue &p, bool mut = false);
  Object clone() const;

  ObjectValue value;
  bool mut;
};

} // namespace pheonix::eval
