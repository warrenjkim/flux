#include "view/view.h"

#include <algorithm>

#include "buffer/buffer.h"
#include "terminal/raw_terminal.h"
#include "view/cursor.h"
#include "view/viewport.h"

namespace flux {

View::View(Buffer* buffer_) : buffer_(*buffer_) {}

void View::Draw(RawTerminal* terminal) {
  for (size_t i = 0; i < viewport_.rows; i++) {
    if (i >= buffer_.Lines()) {
      terminal->Write("~");
    } else {
      std::string line = buffer_.GetLine(i);
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
  return Buffer::Position{.row = cursor_.row, .col = cursor_.col};
}

void View::UpdateCursor(Buffer::Position pos) {
  cursor_.row = std::min(viewport_.rows, pos.row);
  cursor_.col = std::min(viewport_.cols, pos.col);
}

void View::MoveCursorUp() {
  if (cursor_.row == 0) {
    return;
  }
  if (size_t len = buffer_.GetLineLength(--cursor_.row); cursor_.col > len) {
    cursor_.col = len;
  }
}

void View::MoveCursorDown() {
  if (cursor_.row == buffer_.Lines() - 1) {
    return;
  }

  if (size_t len = buffer_.GetLineLength(++cursor_.row); cursor_.col > len) {
    cursor_.col = len;
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

}  // namespace flux
