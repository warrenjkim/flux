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

Editor::Editor(Config config_)
    : config_(std::move(config_)), mode_(Mode::kNormal), running_(true) {
  BindKeys();
}

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
                            .left_padding = 2,
                            .right_padding = 2,
                            .buffer_position = view_->GetBufferPosition()});
    raw_term.WriteLine();

    command_line_->Draw(&raw_term);

    raw_term.MoveCursor(view_->GetCursor());
    raw_term.ShowCursor();
    raw_term.Flush();

    if (flux::Key key = raw_term.GetKey(); key != flux::Key::kNone) {
      const KeyBindings& bindings = config_.key_bindings[mode_];
      chord_.push_back(key);
      if (const Command::Function* function = bindings.MatchCommand(chord_);
          function) {
        (*function)(this, key);
      } else {
        bindings.GetFallback()(this, key);
      }

      if (!bindings.PrefixCommand(chord_) || bindings.MatchCommand(chord_)) {
        chord_.clear();
      }
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
  config_.key_bindings[Mode::kNormal].BindKey(Key::kArrowUp,
                                              &Command::MoveCursorUp);
  config_.key_bindings[Mode::kNormal].BindKey(Key::kArrowDown,
                                              &Command::MoveCursorDown);
  config_.key_bindings[Mode::kNormal].BindKey(Key::kArrowLeft,
                                              &Command::MoveCursorLeft);
  config_.key_bindings[Mode::kNormal].BindKey(Key::kArrowRight,
                                              &Command::MoveCursorRight);

  config_.key_bindings[Mode::kNormal].BindKey(Key::kk, &Command::MoveCursorUp);
  config_.key_bindings[Mode::kNormal].BindKey(Key::kj,
                                              &Command::MoveCursorDown);
  config_.key_bindings[Mode::kNormal].BindKey(Key::kh,
                                              &Command::MoveCursorLeft);
  config_.key_bindings[Mode::kNormal].BindKey(Key::kl,
                                              &Command::MoveCursorRight);
  config_.key_bindings[Mode::kNormal].BindKey(Key::kUnderscore,
                                              &Command::MoveCursorStart);
  config_.key_bindings[Mode::kNormal].BindKey(Key::kDollar,
                                              &Command::MoveCursorEnd);

  config_.key_bindings[Mode::kNormal].BindKey(Key::ki,
                                              &Command::EnterInsertMode);
  config_.key_bindings[Mode::kNormal].BindKey(Key::ka,
                                              [](Editor* e, Key) -> void {
                                                Command::EnterInsertMode(e);
                                                Command::MoveCursorRight(e);
                                              });

  config_.key_bindings[Mode::kNormal].BindKey(Key::kI,
                                              [](Editor* e, Key) -> void {
                                                Command::EnterInsertMode(e);
                                                Command::MoveCursorStart(e);
                                              });
  config_.key_bindings[Mode::kNormal].BindKey(Key::kA,
                                              [](Editor* e, Key) -> void {
                                                Command::EnterInsertMode(e);
                                                Command::MoveCursorEnd(e);
                                              });

  config_.key_bindings[Mode::kNormal].BindChord({Key::kd, Key::kd},
                                                &Command::DeleteLine);

  config_.key_bindings[Mode::kNormal].SetFallback([](Editor*, Key) -> void {});
}

void Editor::BindInsertModeKeys() {
  config_.key_bindings[Mode::kInsert].BindKey(Key::kArrowUp,
                                              &Command::MoveCursorUp);
  config_.key_bindings[Mode::kInsert].BindKey(Key::kArrowDown,
                                              &Command::MoveCursorDown);
  config_.key_bindings[Mode::kInsert].BindKey(Key::kArrowLeft,
                                              &Command::MoveCursorLeft);
  config_.key_bindings[Mode::kInsert].BindKey(Key::kArrowRight,
                                              &Command::MoveCursorRight);

  config_.key_bindings[Mode::kInsert].BindKey(Key::kCtrlP,
                                              &Command::MoveCursorUp);
  config_.key_bindings[Mode::kInsert].BindKey(Key::kCtrlN,
                                              &Command::MoveCursorDown);
  config_.key_bindings[Mode::kInsert].BindKey(Key::kCtrlB,
                                              &Command::MoveCursorLeft);
  config_.key_bindings[Mode::kInsert].BindKey(Key::kCtrlF,
                                              &Command::MoveCursorRight);
  config_.key_bindings[Mode::kInsert].BindKey(Key::kCtrlA,
                                              &Command::MoveCursorStart);
  config_.key_bindings[Mode::kInsert].BindKey(Key::kCtrlE,
                                              &Command::MoveCursorEnd);

  config_.key_bindings[Mode::kInsert].BindKey(Key::kCtrlQ, &Command::Quit);
  config_.key_bindings[Mode::kInsert].BindKey(Key::kCtrlS, &Command::Save);
  config_.key_bindings[Mode::kInsert].BindKey(Key::kBackspace,
                                              &Command::Backspace);
  config_.key_bindings[Mode::kInsert].BindKey(Key::kReturn, &Command::Return);

  config_.key_bindings[Mode::kInsert].BindKey(Key::kEscape,
                                              &Command::EnterNormalMode);

  config_.key_bindings[Mode::kInsert].SetFallback(&Command::InsertChar);
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
