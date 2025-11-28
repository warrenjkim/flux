#pragma once

#include <cstddef>
#include <ostream>
#include <string>

namespace flux {

struct ViewPort {
  size_t rows;
  size_t cols;
};

inline std::string to_string(ViewPort vp) {
  return std::to_string(vp.cols) + " x " + std::to_string(vp.rows);
}

inline std::ostream& operator<<(std::ostream& os, ViewPort vp) {
  return os << to_string(vp);
}

}  // namespace flux
