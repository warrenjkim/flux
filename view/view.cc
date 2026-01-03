#include "view/view.h"

#include <algorithm>
#include <cmath>

#include "buffer/buffer.h"
#include "terminal/raw_terminal.h"
#include "view/cursor.h"
#include "view/viewport.h"

namespace flux {

View::View(Buffer* buffer_, ViewPort vp) : buffer_(*buffer_), viewport_(vp) {}

void View::Draw(RawTerminal* terminal) {
  gutter_offset_ = static_cast<size_t>(floor(log10(buffer_.Lines()))) + 2;
  for (size_t i = 0; i < viewport_.rows; i++) {
    if (i + offset_.row >= buffer_.Lines()) {
      terminal->Write("~");
    } else {
      size_t row = offset_.row + i + 1;
      terminal->Write(std::to_string(row) +
                      std::string(gutter_offset_ - static_cast<size_t>(
                                                       floor(log10(row) + 1)),
                                  ' '));
      if (std::string line = buffer_.GetLine(i + offset_.row);
          offset_.col < line.size()) {
        for (size_t j = 0; j < std::min(viewport_.cols - gutter_offset_,
                                        line.size() - offset_.col);
             j++) {
          terminal->Write(line[offset_.col + j]);
        }
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

Cursor View::GetScreenCursor() const {
  return {.row = cursor_.row, .col = cursor_.col + gutter_offset_};
}

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
  } else if (size_t text_cols = viewport_.cols - gutter_offset_;
             pos.col >= offset_.col + text_cols) {
    offset_.col = pos.col - text_cols + 1;
  }

  cursor_.row = pos.row - offset_.row;
  cursor_.col = pos.col - offset_.col;
  if (update_preferred_col) {
    cursor_.preferred_col = pos.col;
  }
}

}  // namespace flux
