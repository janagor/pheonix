#pragma once

#include "node.hpp"
#include <map>
#include <string>
#include <vector>

namespace pheonix {
using Primitive = std::variant<bool, types::Integer, types::Float, std::string>;
}
namespace pheonix::context {

struct Context {
public:
  Context() : context() {}
  // void pop_scope();
  // void push_scope();
  // void find(std::string ident);
  // void insert_unique();
  // void insert();

  // private:
  // std::vector<std::map<std::string, types::Integer>> context{};
  std::map<std::string, Primitive> context{};
};

} // namespace pheonix::context
