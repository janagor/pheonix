#pragma once

#include <iostream>

namespace pheonix {

class Int {

public:
  constexpr Int() noexcept : m_value(0) {}
  constexpr Int(int v) noexcept : m_value(std::move(v)) {}

  [[nodiscard]] constexpr Int operator*(Int const &other) const noexcept {
    return Int{m_value * other.value()};
  }
  [[nodiscard]] constexpr Int operator/(Int const &other) const {
    return Int{m_value / other.value()};
  }
  [[nodiscard]] constexpr Int operator+(Int const &other) const noexcept {
    return Int{m_value + other.value()};
  }
  [[nodiscard]] constexpr Int operator-(Int const &other) const noexcept {
    return Int{m_value - other.value()};
  }
  [[nodiscard]] constexpr Int operator%(Int const &other) const {
    return Int{m_value % other.value()};
  }
  [[nodiscard]] constexpr Int operator-() const noexcept {
    return Int{-m_value};
  }
  [[nodiscard]] constexpr auto operator<=>(Int const &) const = default;

  [[nodiscard]] constexpr int const &value() const noexcept { return m_value; }
  [[nodiscard]] constexpr int &value() noexcept { return m_value; }

  friend std::ostream &operator<<(std::ostream &os, Int const &obj) {
    return os << obj.m_value;
  }

private:
  int m_value;
};

} // namespace pheonix
