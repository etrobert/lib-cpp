#pragma once

#include <vector>
#include <iostream>

template<typename T>
std::ostream& operator<<(std::ostream &out, const std::vector<T>& v) {
  out << "[";
  for (size_t i(0); i < v.size(); ++i) {
    out << v[i];
    if (i != v.size() - 1)
      out << ", ";
  }
  return out << "]";
}
