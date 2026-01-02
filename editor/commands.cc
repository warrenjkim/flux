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
  e->view_->MoveCursorUp();

  Buffer::Position pos = e->view_->GetBufferPosition();
  size_t len = e->buffer_->GetLineLength(pos.row);
  if (pos.col > len - (e->mode_ == Mode::kNormal && len > 0)) {
    e->view_->MoveCursorLeft();
  }
}

void Command::MoveCursorDown(Editor* e, Key) {
  e->view_->MoveCursorDown();

  Buffer::Position pos = e->view_->GetBufferPosition();
  size_t len = e->buffer_->GetLineLength(pos.row);
  if (pos.col > len - (e->mode_ == Mode::kNormal && len > 0)) {
    e->view_->MoveCursorLeft();
  }
}

void Command::MoveCursorLeft(Editor* e, Key) { e->view_->MoveCursorLeft(); }

void Command::MoveCursorRight(Editor* e, Key) {
  Buffer::Position pos = e->view_->GetBufferPosition();
  size_t len = e->buffer_->GetLineLength(pos.row);
  if (pos.col < len - (e->mode_ == Mode::kNormal && len > 0)) {
    e->view_->MoveCursorRight();
  }
}

void Command::MoveCursorStart(Editor* e, Key) { e->view_->MoveCursorStart(); }

void Command::MoveCursorEnd(Editor* e, Key) {
  e->view_->MoveCursorEnd();

  if (e->mode_ == Mode::kNormal) {
    e->view_->MoveCursorLeft();
  }
}

void Command::DeleteLine(Editor* e, Key) {
  e->view_->UpdateCursor(e->buffer_->DeleteLine(e->view_->GetBufferPosition()));
}

}  // namespace flux
