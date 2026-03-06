#pragma once

#include "Node.hpp"
#include "NodeDef.hpp"
#include "Value.hpp"

#include <memory_resource>

namespace pheonix {

class ASTContext {
public:
  ASTContext()
      : m_arena(m_buffer, sizeof(m_buffer), std::pmr::new_delete_resource()),
        m_allocator(&m_arena) {}
  ASTContext(const ASTContext &) = delete;
  ASTContext &operator=(const ASTContext &) = delete;

  template <typename NodeType, typename... Args> Node *make(Args &&...args) {
    void *mem = m_allocator.allocate(1);
    return new (mem) Node(NodeType(std::forward<Args>(args)...));
  }

private:
  alignas(std::max_align_t) std::byte m_buffer[65536];
  std::pmr::monotonic_buffer_resource m_arena;
  std::pmr::polymorphic_allocator<Node> m_allocator;
};

} // namespace pheonix
