#pragma once

#include <cstddef>
#include <ostream>
#include <string>

namespace flux {

struct Cursor {
  size_t row;
  size_t col;
};

inline std::string to_string(Cursor cursor) {
  return "(" + std::to_string(cursor.row) + ", " + std::to_string(cursor.col) +
         ")";
}

inline std::ostream& operator<<(std::ostream& os, Cursor cursor) {
  return os << to_string(cursor);
}

}  // namespace flux
