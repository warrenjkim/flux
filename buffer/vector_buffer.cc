#include "buffer/vector_buffer.h"

#include <utility>
#include <vector>

namespace flux {

VectorBuffer::VectorBuffer(std::vector<std::string> buffer)
    : buffer_(std::move(buffer)) {}

size_t VectorBuffer::Lines() const { return buffer_.size(); }

size_t VectorBuffer::Size() const {
  size_t size = 0;
  for (const std::string& line : buffer_) {
    size += line.size();
  }

  return size;
}

std::string VectorBuffer::GetLine(size_t row) const { return buffer_[row]; }

size_t VectorBuffer::GetLineLength(size_t row) const {
  return buffer_[row].size();
}

Buffer::Position VectorBuffer::Insert(Buffer::Position pos, char c) {
  std::string& line = buffer_[pos.row];
  if (pos.col == line.length()) {
    line.push_back(c);
  } else {
    line.insert(buffer_[pos.row].begin() + pos.col, c);
  }

  pos.col++;

  return pos;
}

Buffer::Position VectorBuffer::BreakLine(Buffer::Position pos) {
  std::string new_line = buffer_[pos.row].substr(pos.col);
  buffer_[pos.row++].erase(pos.col);
  buffer_.insert(buffer_.begin() + pos.row, std::move(new_line));
  pos.col = 0;

  return pos;
}

Buffer::Position VectorBuffer::Delete(Buffer::Position pos) {
  if (pos.row == 0 && pos.col == 0) {
    return pos;
  }

  if (pos.col == 0) {
    std::string line = buffer_[pos.row];
    buffer_.erase(buffer_.begin() + pos.row);
    pos.row--;
    pos.col = buffer_[pos.row].size();
    buffer_[pos.row].append(std::move(line));

    return pos;
  }

  buffer_[pos.row].erase(--pos.col, 1);

  return pos;
}

}  // namespace flux
