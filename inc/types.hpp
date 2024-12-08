#pragma once
#include <iostream>
#include <limits>
#include <stdexcept>

namespace types {

struct Integer {
  Integer() : value(0) {}
  Integer(int v) : value(v) {}
  Integer(const Integer &other) : value(other.value) {}

  Integer &operator=(int val);
  Integer &operator=(const Integer &val);

  Integer &operator*=(int val);
  Integer &operator/=(int val);
  Integer &operator+=(int val);
  Integer &operator-=(int val);
  Integer &operator%=(int val);

  Integer &operator*=(const Integer &val);
  Integer &operator/=(const Integer &val);
  Integer &operator+=(const Integer &val);
  Integer &operator-=(const Integer &val);

  Integer operator*(int val) const;
  Integer operator/(int val) const;
  Integer operator+(int val) const;
  Integer operator-(int val) const;
  Integer operator%(int val) const;

  Integer operator*(const Integer &val) const;
  Integer operator/(const Integer &val) const;
  Integer operator+(const Integer &val) const;
  Integer operator-(const Integer &val) const;
  Integer operator%(const Integer &val) const;

  bool operator==(const Integer &other) const;
  bool operator!=(const Integer &other) const;
  bool operator<(const Integer &other) const;
  bool operator<=(const Integer &other) const;
  bool operator>(const Integer &other) const;
  bool operator>=(const Integer &other) const;

  int getValue() const;
  void setValue(long val);

  friend std::ostream &operator<<(std::ostream &os, const Integer &obj);

private:
  int value;
};

struct Float {
  Float() : value(0.0) {}
  Float(double v) : value(v) {}
  Float(const Float &other) : value(other.value) {}

  Float &operator=(double val);
  Float &operator=(const Float &val);

  Float &operator*=(double val);
  Float &operator/=(double val);
  Float &operator+=(double val);
  Float &operator-=(double val);

  Float &operator*=(const Float &val);
  Float &operator/=(const Float &val);
  Float &operator+=(const Float &val);
  Float &operator-=(const Float &val);

  Float operator*(double val) const;
  Float operator/(double val) const;
  Float operator+(double val) const;
  Float operator-(double val) const;

  Float operator*(const Float &val) const;
  Float operator/(const Float &val) const;
  Float operator+(const Float &val) const;
  Float operator-(const Float &val) const;

  bool operator==(const Float &other) const;
  bool operator!=(const Float &other) const;
  bool operator<(const Float &other) const;
  bool operator<=(const Float &other) const;
  bool operator>(const Float &other) const;
  bool operator>=(const Float &other) const;

  double getValue() const;
  void setValue(double val);

  friend std::ostream &operator<<(std::ostream &os, const Float &obj);

private:
  double value;
};

} // namespace types
