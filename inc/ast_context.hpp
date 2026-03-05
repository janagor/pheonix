#pragma once

#include "node_def.hpp"
#include <memory_resource>
#include <vector>

namespace pheonix {

class ASTContext {
public:
  ASTContext()
      : m_arena(m_buffer, sizeof(m_buffer), std::pmr::new_delete_resource()),
        m_allocator(&m_arena) {}
  ASTContext(const ASTContext &) = delete;
  ASTContext &operator=(const ASTContext &) = delete;

  template <typename NodeType, typename... Args>
  node2::Node *make(Args &&...args) {
    void *mem = m_allocator.allocate(1);
    return new (mem) node2::Node(NodeType(std::forward<Args>(args)...));
  }

private:
  alignas(std::max_align_t) std::byte m_buffer[65536];
  std::pmr::monotonic_buffer_resource m_arena;
  std::pmr::polymorphic_allocator<node2::Node> m_allocator;
};

} // namespace pheonix
