#pragma once

#include <cstddef>
#include <string>

namespace flux {

class Buffer {
 public:
  struct Position {
    size_t row;
    size_t col;
  };

  virtual ~Buffer() = default;

  virtual size_t Lines() const = 0;

  virtual size_t Size() const = 0;

  virtual std::string GetLine(size_t row) const = 0;

  virtual Position Insert(Position pos, char c) = 0;

  virtual Position BreakLine(Position pos) = 0;

  virtual Position Delete(Position pos) = 0;
};

}  // namespace flux
