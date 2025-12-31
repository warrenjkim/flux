#pragma once

#include <cstddef>
#include <string>

namespace flux {

class Buffer {
 public:
  struct Position {
    size_t row = 0;
    size_t col = 0;
  };

  virtual ~Buffer() = default;

  virtual size_t Lines() const = 0;

  virtual size_t Size() const = 0;

  virtual std::string GetLine(size_t row) const = 0;

  virtual size_t GetLineLength(size_t row) const = 0;

  virtual Position Insert(Position pos, char c) = 0;

  virtual Position BreakLine(Position pos) = 0;

  virtual Position Delete(Position pos) = 0;
};

inline std::string to_string(Buffer::Position position) {
  return "(" + std::to_string(position.row) + ", " +
         std::to_string(position.col) + ")";
}

inline std::ostream& operator<<(std::ostream& os, Buffer::Position position) {
  return os << to_string(position);
}

}  // namespace flux
