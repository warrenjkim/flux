#include "view/view.h"

#include <algorithm>

#include "buffer/buffer.h"
#include "terminal/raw_terminal.h"
#include "view/cursor.h"
#include "view/viewport.h"

namespace flux {

View::View(Buffer* buffer_, ViewPort vp) : buffer_(*buffer_), viewport_(vp) {}

void View::Draw(RawTerminal* terminal) {
  for (size_t i = 0; i < viewport_.rows; i++) {
    if (i + buffer_offset_.row >= buffer_.Lines()) {
      terminal->Write("~");
    } else if (std::string line = buffer_.GetLine(i + buffer_offset_.row);
               buffer_offset_.col < line.size()) {
      for (size_t j = 0;
           j < std::min(viewport_.cols, line.size() - buffer_offset_.col);
           j++) {
        terminal->Write(line[buffer_offset_.col + j]);
      }
    }

    terminal->ClearLine();
    if (i < viewport_.rows - 1) {
      terminal->WriteLine();
    }
  }
}

void View::Resize(ViewPort viewport) { viewport_ = viewport; }

Cursor View::GetCursor() const { return cursor_; }

Buffer::Position View::GetBufferPosition() const {
  return Buffer::Position{.row = cursor_.row + buffer_offset_.row,
                          .col = cursor_.col + buffer_offset_.col};
}

void View::UpdateCursor(Buffer::Position pos) {
  if (pos.row < buffer_offset_.row) {
    buffer_offset_.row = pos.row;
  } else if (pos.row >= buffer_offset_.row + viewport_.rows) {
    buffer_offset_.row = pos.row - viewport_.rows + 1;
  }

  if (pos.col < buffer_offset_.col) {
    buffer_offset_.col = pos.col;
  } else if (pos.col >= buffer_offset_.col + viewport_.cols) {
    buffer_offset_.col = pos.col - viewport_.cols + 1;
  }

  cursor_.row = pos.row - buffer_offset_.row;
  cursor_.col = pos.col - buffer_offset_.col;
}

void View::MoveCursorUp() {
  if (GetBufferPosition().row == 0) {
    return;
  }

  if (cursor_.row > 0) {
    cursor_.row--;
  } else {
    buffer_offset_.row--;
  }

  cursor_.col =
      std::min(cursor_.col, buffer_.GetLineLength(GetBufferPosition().row));

  size_t len = buffer_.GetLineLength(GetBufferPosition().row);
  if (buffer_offset_.col > len) {
    buffer_offset_.col = len;
  }

  cursor_.col = std::min(cursor_.col, len - buffer_offset_.col);
}

void View::MoveCursorDown() {
  if (GetBufferPosition().row + 1 >= buffer_.Lines()) {
    return;
  }

  if (cursor_.row + 1 < viewport_.rows) {
    cursor_.row++;
  } else {
    buffer_offset_.row++;
  }

  cursor_.col =
      std::min(cursor_.col, buffer_.GetLineLength(GetBufferPosition().row));

  size_t len = buffer_.GetLineLength(GetBufferPosition().row);
  if (buffer_offset_.col > len) {
    buffer_offset_.col = len;
  }

  cursor_.col = std::min(cursor_.col, len - buffer_offset_.col);
}

void View::MoveCursorLeft() {
  if (GetBufferPosition().col == 0) return;

  if (cursor_.col > 0) {
    cursor_.col--;
  } else {
    buffer_offset_.col--;
  }
}

void View::MoveCursorRight() {
  if (Buffer::Position pos = GetBufferPosition();
      pos.col >= buffer_.GetLineLength(pos.row)) {
    return;
  }

  if (cursor_.col + 1 < viewport_.cols) {
    cursor_.col++;
  } else {
    buffer_offset_.col++;
  }
}

void View::MoveCursorStart() { cursor_.col = 0; }

void View::MoveCursorEnd() {
  cursor_.col = std::min(viewport_.cols, buffer_.GetLineLength(cursor_.row));
}

}  // namespace flux
