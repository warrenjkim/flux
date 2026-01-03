#include "editor/commands.h"

#include "editor/editor.h"
#include "editor/mode.h"

namespace flux {

void Command::Quit(Editor* e, Key) { e->running_ = false; }

void Command::Save(Editor* e, Key) {
  if (e->path_.empty()) {
    e->command_line_->SetMessage("path is empty. not saving.");
    return;
  }

  e->WriteFile(e->path_);
  e->command_line_->SetMessage(
      std::string("wrote to \"").append(e->path_).append("\""));
}

void Command::InsertChar(Editor* e, Key k) {
  e->view_->UpdateCursor(
      e->buffer_->Insert(e->view_->GetBufferPosition(), static_cast<char>(k)));
}

void Command::Backspace(Editor* e, Key) {
  e->view_->UpdateCursor(e->buffer_->Delete(e->view_->GetBufferPosition()));
}

void Command::Return(Editor* e, Key) {
  e->view_->UpdateCursor(e->buffer_->BreakLine(e->view_->GetBufferPosition()));
}

void Command::EnterNormalMode(Editor* e, Key) {
  e->mode_ = Mode::kNormal;
  MoveCursorLeft(e);
}

void Command::EnterInsertMode(Editor* e, Key) { e->mode_ = Mode::kInsert; }

void Command::MoveCursorUp(Editor* e, Key) {
  Buffer::Position pos = e->view_->GetBufferPosition();
  if (pos.row == 0) {
    return;
  }

  pos.row--;
  size_t len = e->buffer_->GetLineLength(pos.row);
  pos.col = std::min(e->view_->GetCursor().preferred_col,
                     len - (e->mode_ == Mode::kNormal && len > 0));
  e->view_->UpdateCursor(pos);
}

void Command::MoveCursorDown(Editor* e, Key) {
  Buffer::Position pos = e->view_->GetBufferPosition();
  if (pos.row >= e->buffer_->Lines() - 1) {
    return;
  }

  pos.row++;
  size_t len = e->buffer_->GetLineLength(pos.row);
  pos.col = std::min(e->view_->GetCursor().preferred_col,
                     len - (e->mode_ == Mode::kNormal && len > 0));
  e->view_->UpdateCursor(pos);
}

void Command::MoveCursorLeft(Editor* e, Key) {
  Buffer::Position pos = e->view_->GetBufferPosition();
  if (pos.col == 0) {
    return;
  }

  pos.col--;
  e->view_->UpdateCursor(pos, /*update_preferred_cols=*/true);
}

void Command::MoveCursorRight(Editor* e, Key) {
  Buffer::Position pos = e->view_->GetBufferPosition();
  size_t len = e->buffer_->GetLineLength(pos.row);
  size_t max_col = len - (e->mode_ == Mode::kNormal && len > 0);
  if (pos.col < max_col) {
    pos.col++;
    e->view_->UpdateCursor(pos, /*update_preferred_cols=*/true);
  }
}

void Command::MoveCursorStart(Editor* e, Key) {
  Buffer::Position pos = e->view_->GetBufferPosition();
  pos.col = 0;
  e->view_->UpdateCursor(pos, /*update_preferred_col=*/true);
}

void Command::MoveCursorEnd(Editor* e, Key) {
  Buffer::Position pos = e->view_->GetBufferPosition();
  size_t len = e->buffer_->GetLineLength(pos.row);
  pos.col = len - (e->mode_ == Mode::kNormal && len > 0);
  e->view_->UpdateCursor(pos, /*update_preferred_col=*/true);
}

void Command::DeleteLine(Editor* e, Key) {
  Buffer::Position pos = e->buffer_->DeleteLine(e->view_->GetBufferPosition());
  size_t len = e->buffer_->GetLineLength(pos.row);
  pos.col = std::min(pos.col, len - (e->mode_ == Mode::kNormal && len > 0));
  e->view_->UpdateCursor(pos, /*update_preferred_col=*/true);
}

}  // namespace flux
