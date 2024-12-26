#include "object.hpp"

namespace pheonix::eval {

Function::Function(const std::vector<Object> &arguments,
                   std::unique_ptr<node::Node> body)
    : args({}), body(std::move(body)) {

  for (size_t i = 0; i < arguments.size(); ++i) {
    if (!std::holds_alternative<std::string>(arguments[i].value))
      throw std::runtime_error("");
    args.push_back(std::get<std::string>(arguments[i].value));
  }
}

} // namespace pheonix::eval
