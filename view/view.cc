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
    if (i + offset_.row >= buffer_.Lines()) {
      terminal->Write("~");
    } else if (std::string line = buffer_.GetLine(i + offset_.row);
               offset_.col < line.size()) {
      for (size_t j = 0;
           j < std::min(viewport_.cols, line.size() - offset_.col); j++) {
        terminal->Write(line[offset_.col + j]);
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
  return Buffer::Position{.row = cursor_.row + offset_.row,
                          .col = cursor_.col + offset_.col};
}

void View::UpdateCursor(Buffer::Position pos, bool update_preferred_col) {
  if (pos.row < offset_.row) {
    offset_.row = pos.row;
  } else if (pos.row >= offset_.row + viewport_.rows) {
    offset_.row = pos.row - viewport_.rows + 1;
  }

  if (pos.col < offset_.col) {
    offset_.col = pos.col;
  } else if (pos.col >= offset_.col + viewport_.cols) {
    offset_.col = pos.col - viewport_.cols + 1;
  }

  cursor_.row = pos.row - offset_.row;
  cursor_.col = pos.col - offset_.col;
  if (update_preferred_col) {
    cursor_.preferred_col = pos.col;
  }
}

}  // namespace flux
