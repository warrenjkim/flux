#include "view/view.h"

#include <algorithm>

#include "buffer/buffer.h"
#include "terminal/raw_terminal.h"
#include "view/cursor.h"
#include "view/viewport.h"

namespace flux {

View::View(Buffer* buffer_, ViewPort vp)
    : buffer_(*buffer_),
      viewport_(vp),
      cursor_(Cursor{.row = 0, .col = 0}),
      buffer_offset_(Offset{.row = 0}) {}

void View::Draw(RawTerminal* terminal) {
  for (size_t i = 0; i < viewport_.rows; i++) {
    if (i >= buffer_.Lines()) {
      terminal->Write("~");
    } else {
      std::string line = buffer_.GetLine(i + buffer_offset_.row);
      for (size_t j = 0; j < std::min(viewport_.cols, line.size()); j++) {
        terminal->Write(line[j]);
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
                          .col = cursor_.col};
}

void View::UpdateCursor(Buffer::Position pos) {
  cursor_.row = std::min(viewport_.rows, pos.row);
  cursor_.col = std::min(viewport_.cols, pos.col);
}

void View::MoveCursorUp() {
  if (cursor_.row > 0) {
    cursor_.col = std::min(cursor_.col, buffer_.GetLineLength(--cursor_.row));
    return;
  }

  if (buffer_offset_.row > 0) {
    cursor_.col =
        std::min(cursor_.col, buffer_.GetLineLength(--buffer_offset_.row));
  }
}

void View::MoveCursorDown() {
  if (cursor_.row < std::min(viewport_.rows, buffer_.Lines()) - 1) {
    cursor_.col = std::min(cursor_.col, buffer_.GetLineLength(++cursor_.row));
    return;
  }

  if (buffer_offset_.row <
      static_cast<size_t>(std::max(0, static_cast<int>(buffer_.Lines()) -
                                          static_cast<int>(viewport_.rows)))) {
    cursor_.col =
        std::min(cursor_.col,
                 buffer_.GetLineLength(cursor_.row + (++buffer_offset_.row)));
  }
}

void View::MoveCursorLeft() {
  if (cursor_.col == 0) {
    return;
  }

  cursor_.col--;
}

void View::MoveCursorRight() {
  if (cursor_.col >= buffer_.GetLineLength(cursor_.row)) {
    return;
  }

  cursor_.col++;
}

void View::MoveCursorStart() { cursor_.col = 0; }

void View::MoveCursorEnd() {
  cursor_.col = std::min(viewport_.cols, buffer_.GetLineLength(cursor_.row));
}

}  // namespace flux
