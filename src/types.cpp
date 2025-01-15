#include "types.hpp"

#include <limits>
#include <stdexcept>

namespace pheonix::types {

Integer &Integer::operator=(int val) {
  value = val;
  return *this;
}

Integer &Integer::operator=(const Integer &val) {
  value = val.value;
  return *this;
}

Integer &Integer::operator*=(int val) {
  setValue(static_cast<long>(value) * static_cast<long>(val));
  return *this;
}
Integer &Integer::operator/=(int val) {
  if (val == 0)
    throw std::runtime_error("Division by zero");
  setValue(static_cast<long>(value) / static_cast<long>(val));
  return *this;
}
Integer &Integer::operator+=(int val) {
  setValue(static_cast<long>(value) + static_cast<long>(val));
  return *this;
}
Integer &Integer::operator-=(int val) {
  setValue(static_cast<long>(value) - static_cast<long>(val));
  return *this;
}
Integer &Integer::operator%=(int val) {
  if (val == 0)
    throw std::runtime_error("Modulo by zero");
  setValue(static_cast<long>(value) % static_cast<long>(val));
  return *this;
}

Integer &Integer::operator*=(const Integer &val) {
  return (*this) *= val.value;
}
Integer &Integer::operator/=(const Integer &val) {
  return (*this) /= val.value;
}
Integer &Integer::operator+=(const Integer &val) {
  return (*this) += val.value;
}
Integer &Integer::operator-=(const Integer &val) {
  return (*this) -= val.value;
}

Integer Integer::operator*(int val) const {
  Integer res(*this);
  return res *= val;
}
Integer Integer::operator/(int val) const {
  Integer res(*this);
  return res /= val;
}
Integer Integer::operator+(int val) const {
  Integer res(*this);
  return res += val;
}
Integer Integer::operator-(int val) const {
  Integer res(*this);
  return res -= val;
}
Integer Integer::operator%(int val) const {
  Integer res(*this);
  return res %= val;
}

Integer Integer::operator*(const Integer &val) const {
  return (*this) * val.value;
}
Integer Integer::operator/(const Integer &val) const {
  return (*this) / val.value;
}
Integer Integer::operator+(const Integer &val) const {
  return (*this) + val.value;
}
Integer Integer::operator-(const Integer &val) const {
  return (*this) - val.value;
}
Integer Integer::operator%(const Integer &val) const {
  return (*this) % val.value;
}

bool Integer::operator==(const Integer &other) const {
  return value == other.value;
}

bool Integer::operator!=(const Integer &other) const {
  return value != other.value;
}

bool Integer::operator<(const Integer &other) const {
  return value < other.value;
}

bool Integer::operator<=(const Integer &other) const {
  return value <= other.value;
}

bool Integer::operator>(const Integer &other) const {
  return value > other.value;
}

bool Integer::operator>=(const Integer &other) const {
  return value >= other.value;
}

Integer Integer::operator-() const { return Integer(-value); }

int Integer::getValue() const { return value; }

void Integer::setValue(long val) {
  if (val > std::numeric_limits<int>::max() ||
      val < std::numeric_limits<int>::min()) {
    throw std::runtime_error("Integer literal out of range.");
  }
  value = static_cast<int>(val);
}

std::ostream &operator<<(std::ostream &os, const Integer &obj) {
  os << obj.value;
  return os;
}

Float &Float::operator=(double val) {
  value = val;
  return *this;
}

Float &Float::operator=(const Float &val) {
  value = val.value;
  return *this;
}

Float &Float::operator*=(double val) {
  setValue(value * val);
  return *this;
}
Float &Float::operator/=(double val) {
  if (val == 0.0)
    throw std::runtime_error("Division by zero");
  setValue(value / val);
  return *this;
}
Float &Float::operator+=(double val) {
  setValue(value + val);
  return *this;
}
Float &Float::operator-=(double val) {
  setValue(value - val);
  return *this;
}

Float &Float::operator*=(const Float &val) { return (*this) *= val.value; }
Float &Float::operator/=(const Float &val) { return (*this) /= val.value; }
Float &Float::operator+=(const Float &val) { return (*this) += val.value; }
Float &Float::operator-=(const Float &val) { return (*this) -= val.value; }

Float Float::operator*(double val) const {
  Float res(*this);
  return res *= val;
}
Float Float::operator/(double val) const {
  Float res(*this);
  return res /= val;
}
Float Float::operator+(double val) const {
  Float res(*this);
  return res += val;
}
Float Float::operator-(double val) const {
  Float res(*this);
  return res -= val;
}

Float Float::operator*(const Float &val) const { return (*this) * val.value; }
Float Float::operator/(const Float &val) const { return (*this) / val.value; }
Float Float::operator+(const Float &val) const { return (*this) + val.value; }
Float Float::operator-(const Float &val) const { return (*this) - val.value; }

bool Float::operator==(const Float &other) const {
  return value == other.value;
}

bool Float::operator!=(const Float &other) const {
  return value != other.value;
}

bool Float::operator<(const Float &other) const { return value < other.value; }

bool Float::operator<=(const Float &other) const {
  return value <= other.value;
}

bool Float::operator>(const Float &other) const { return value > other.value; }

bool Float::operator>=(const Float &other) const {
  return value >= other.value;
}

Float Float::operator-() const { return Float(-value); }

double Float::getValue() const { return value; }

void Float::setValue(double val) { value = val; }

std::ostream &operator<<(std::ostream &os, const Float &obj) {
  os << obj.getValue();
  return os;
}

} // namespace pheonix::types
