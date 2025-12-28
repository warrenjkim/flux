#pragma once

#include "buffer/buffer.h"
#include "terminal/raw_terminal.h"
#include "view/cursor.h"
#include "view/viewport.h"

namespace flux {

class View {
 public:
  explicit View(Buffer* buffer, ViewPort vp);

  void Draw(RawTerminal* terminal);

  void Resize(ViewPort viewport);

  Cursor GetCursor() const;

  Cursor GetOffset() const;

  Buffer::Position GetBufferPosition() const;

  void UpdateCursor(Buffer::Position pos);

  void MoveCursorUp();

  void MoveCursorDown();

  void MoveCursorLeft();

  void MoveCursorRight();

  void MoveCursorStart();

  void MoveCursorEnd();

 private:
  struct Offset {
    size_t row = 0;
    size_t col = 0;
  };

  Buffer& buffer_;
  ViewPort viewport_;
  Cursor cursor_;
  Offset buffer_offset_;
};

}  // namespace flux
