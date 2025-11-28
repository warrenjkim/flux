#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "buffer/buffer.h"

namespace flux {

class VectorBuffer : public Buffer {
 public:
  explicit VectorBuffer(std::vector<std::string> buffer);

  ~VectorBuffer() = default;

  virtual size_t Lines() const override;

  virtual size_t Size() const override;

  virtual std::string GetLine(size_t row) const override;

  virtual size_t GetLineLength(size_t row) const override;

  virtual Position Insert(Position pos, char c) override;

  virtual Position BreakLine(Position pos) override;

  virtual Position Delete(Position pos) override;

 private:
  std::vector<std::string> buffer_;
};

}  // namespace flux
