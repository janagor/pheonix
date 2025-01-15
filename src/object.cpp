#include "object.hpp"

namespace pheonix::eval {

Function::Function() : body() {}

Function::Function(std::unique_ptr<node::Node> b) : body() {
  body.push_back(std::move(b));
}

Function::Function(const std::vector<Param> &args,
                   std::unique_ptr<node::Node> b)
    : args(args), body() {
  body.push_back(std::move(b));
}

Function::Function(const Function &other) {
  args = other.args;
  args2 = other.args2;
  for (const auto &node : other.body) {
    body.push_back(node->clone());
  }
}

Function::Function(const std::vector<Object> &arguments,
                   std::unique_ptr<node::Node> b)
    : args({}), body() {
  body.emplace_back(std::move(b));

  for (size_t i = 0; i < arguments.size(); ++i) {
    if (!std::holds_alternative<std::string>(arguments[i].value))
      throw std::runtime_error("");
    args.emplace_back(std::get<std::string>(arguments[i].value),
                      arguments[i].mut);
  }
}

bool Function::operator==(const Function &other) const {
  (void)(other);
  return true;
}

Function Function::clone() const {
  Function temp;
  temp.args = args;
  temp.args2 = args2;
  for (const auto &node : body) {
    temp.body.push_back(node->clone());
  }
  return temp;
}

Function &Function::operator=(const Function &other) {
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

std::ostream &operator<<(std::ostream &os, const Function &var) {
  auto args = std::string('_', var.args.size());
  os << "Function(" << args << ")";
  return os;
}

ObjectValue castVariants(const Primitive &p) {
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

std::ostream &operator<<(std::ostream &os, const ObjectValue &var) {
  std::visit(
      [&os](const auto &value) {
        if constexpr (std::is_same_v<std::decay_t<decltype(value)>, bool>) {
          os << (value ? "true" : "false");
        } else if constexpr (std::is_same_v<std::decay_t<decltype(value)>,
                                            std::reference_wrapper<Object>>) {
          os << value.get().value;
        } else {
          os << value;
        }
      },
      var);
  return os;
}

Object::Object() : value(std::monostate{}), mut(false) {}

Object::Object(const Primitive &p, bool mut)
    : value(castVariants(p)), mut(mut) {}

Object::Object(const ObjectValue &p, bool mut) : value(p), mut(mut) {}

Object Object::clone() const {
  if (std::holds_alternative<Function>(value)) {
    if (!std::get<Function>(value).body.empty()) {
      return Object(std::get<Function>(value).clone(), mut);
    }
    return Object(Function(), mut);
  }
  return Object(value, mut);
}

} // namespace pheonix::eval
