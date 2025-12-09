#include "editor/editor.h"

#include <fstream>
#include <memory>
#include <string>
#include <string_view>

#include "buffer/vector_buffer.h"
#include "editor/commands.h"
#include "terminal/raw_terminal.h"
#include "view/command_line.h"
#include "view/status_line.h"
#include "view/view.h"

namespace flux {

constexpr std::string_view kUnnamedFile = "[unnamed file]";

Editor::Editor() : mode_(Mode::kNormal), running_(true) { BindKeys(); }

void Editor::Run(std::string_view path) {
  flux::RawTerminal raw_term;
  raw_term.EnableRawMode();
  raw_term.EnterAlternateBuffer();
  raw_term.ClearScreen();

  path_ = std::string(path);
  if (path_.empty()) {
    buffer_ = std::make_unique<VectorBuffer>(std::vector<std::string>{""});
  } else {
    buffer_ = OpenFile(path_);
  }

  ViewPort vp = raw_term.GetTerminalSize();
  vp.rows -= 2;
  view_ = std::make_unique<View>(buffer_.get(), vp);
  status_line_ = std::make_unique<StatusLine>(vp.cols);
  command_line_ = std::make_unique<CommandLine>(vp.cols);

  while (running_) {
    raw_term.ResetCursor();
    raw_term.HideCursor();

    view_->Draw(&raw_term);
    raw_term.WriteLine();

    status_line_->Draw(
        &raw_term,
        StatusLine::Options{.file_name = path.empty() ? kUnnamedFile : path,
                            .delimiter = '=',
                            .left_padding = 2});
    raw_term.WriteLine();

    command_line_->Draw(&raw_term);

    raw_term.MoveCursor(view_->GetCursor());
    raw_term.ShowCursor();
    raw_term.Flush();

    if (flux::Key key = raw_term.GetKey(); key != flux::Key::kNone) {
      key_bindings_[mode_].GetCommand(key)(this, key);
    }
  }

  raw_term.ExitAlternateBuffer();
  raw_term.DisableRawMode();
}

void Editor::BindKeys() {
  BindNormalModeKeys();
  BindInsertModeKeys();
}

void Editor::BindNormalModeKeys() {
  key_bindings_[Mode::kNormal].Bind(Key::kArrowUp, &Command::MoveCursorUp);
  key_bindings_[Mode::kNormal].Bind(Key::kArrowDown, &Command::MoveCursorDown);
  key_bindings_[Mode::kNormal].Bind(Key::kArrowLeft, &Command::MoveCursorLeft);
  key_bindings_[Mode::kNormal].Bind(Key::kArrowRight,
                                    &Command::MoveCursorRight);

  key_bindings_[Mode::kNormal].Bind(Key::kk, &Command::MoveCursorUp);
  key_bindings_[Mode::kNormal].Bind(Key::kj, &Command::MoveCursorDown);
  key_bindings_[Mode::kNormal].Bind(Key::kh, &Command::MoveCursorLeft);
  key_bindings_[Mode::kNormal].Bind(Key::kl, &Command::MoveCursorRight);
  key_bindings_[Mode::kNormal].Bind(Key::kUnderscore,
                                    &Command::MoveCursorStart);
  key_bindings_[Mode::kNormal].Bind(Key::kDollar, &Command::MoveCursorEnd);

  key_bindings_[Mode::kNormal].Bind(Key::ki, &Command::EnterInsertMode);

  key_bindings_[Mode::kNormal].SetFallback([](Editor*, Key) -> void {});
}

void Editor::BindInsertModeKeys() {
  key_bindings_[Mode::kInsert].Bind(Key::kArrowUp, &Command::MoveCursorUp);
  key_bindings_[Mode::kInsert].Bind(Key::kArrowDown, &Command::MoveCursorDown);
  key_bindings_[Mode::kInsert].Bind(Key::kArrowLeft, &Command::MoveCursorLeft);
  key_bindings_[Mode::kInsert].Bind(Key::kArrowRight,
                                    &Command::MoveCursorRight);

  key_bindings_[Mode::kInsert].Bind(Key::kCtrlP, &Command::MoveCursorUp);
  key_bindings_[Mode::kInsert].Bind(Key::kCtrlN, &Command::MoveCursorDown);
  key_bindings_[Mode::kInsert].Bind(Key::kCtrlH, &Command::MoveCursorLeft);
  key_bindings_[Mode::kInsert].Bind(Key::kCtrlF, &Command::MoveCursorRight);
  key_bindings_[Mode::kInsert].Bind(Key::kCtrlA, &Command::MoveCursorStart);
  key_bindings_[Mode::kInsert].Bind(Key::kCtrlE, &Command::MoveCursorEnd);

  key_bindings_[Mode::kInsert].Bind(Key::kCtrlQ, &Command::Quit);
  key_bindings_[Mode::kInsert].Bind(Key::kCtrlS, &Command::Save);
  key_bindings_[Mode::kInsert].Bind(Key::kBackspace, &Command::Backspace);
  key_bindings_[Mode::kInsert].Bind(Key::kReturn, &Command::Return);

  key_bindings_[Mode::kInsert].Bind(Key::kEscape, &Command::EnterNormalMode);

  key_bindings_[Mode::kInsert].SetFallback(&Command::InsertChar);
}

std::unique_ptr<Buffer> Editor::OpenFile(std::string_view path) const {
  std::vector<std::string> lines;

  std::ifstream file(path.data());
  std::string line;
  while (std::getline(file, line)) {
    lines.push_back(std::move(line));
  }

  file.close();
  if (lines.empty()) {
    lines.push_back("");
  }

  return std::make_unique<VectorBuffer>(std::move(lines));
}

size_t Editor::WriteFile(std::string_view path) const {
  std::ofstream file(path.data());
  size_t size = 0;
  for (size_t i = 0; i < buffer_->Lines(); i++) {
    std::string line = buffer_->GetLine(i);
    file.write(line.data(), line.size());
    file.put('\n');
    size += line.size() + 1;
  }

  file.close();

  return size;
}

}  // namespace flux
