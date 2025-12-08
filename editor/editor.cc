#include "editor/editor.h"

#include <fstream>
#include <memory>
#include <string>
#include <string_view>

#include "buffer/vector_buffer.h"
#include "terminal/raw_terminal.h"
#include "view/command_line.h"
#include "view/status_line.h"
#include "view/view.h"

namespace flux {

Editor::Editor() : running_(true) { BindKeys(); }

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
  view_ = MakeView(buffer_.get(), vp);
  status_line_ = std::make_unique<StatusLine>(vp.cols);
  command_line_ = std::make_unique<CommandLine>(vp.cols);

  while (running_) {
    raw_term.ResetCursor();
    raw_term.HideCursor();

    view_->Draw(&raw_term);
    raw_term.WriteLine();

    status_line_->Draw(
        &raw_term, StatusLine::Options{
                       .file_name = path, .delimiter = '=', .left_padding = 2});
    raw_term.WriteLine();

    command_line_->Draw(&raw_term);

    raw_term.MoveCursor(view_->GetCursor());
    raw_term.ShowCursor();
    raw_term.Flush();

    if (flux::Key key = raw_term.GetKey(); key != flux::Key::kNone) {
      key_handler_.Handle(key);
    }
  }

  raw_term.ExitAlternateBuffer();
  raw_term.DisableRawMode();
}

void Editor::BindKeys() {
  key_handler_.Bind(Key::kCtrlQ, [this]() -> void { running_ = false; });

  key_handler_.Bind(Key::kCtrlS, [this]() -> void {
    if (path_.empty()) {
      command_line_->SetMessage("path is empty. not saving.");
      return;
    }

    WriteFile(path_);
    command_line_->SetMessage(
        std::string("wrote to \"").append(path_).append("\""));
  });

  key_handler_.Bind(Key::kBackspace, [this]() -> void {
    view_->UpdateCursor(buffer_->Delete(view_->GetBufferPosition()));
  });

  key_handler_.Bind(Key::kReturn, [this]() -> void {
    view_->UpdateCursor(buffer_->BreakLine(view_->GetBufferPosition()));
  });

  key_handler_.Bind(Key::kArrowUp, [this]() -> void { view_->MoveCursorUp(); });

  key_handler_.Bind(Key::kArrowDown,
                    [this]() -> void { view_->MoveCursorDown(); });

  key_handler_.Bind(Key::kArrowLeft,
                    [this]() -> void { view_->MoveCursorLeft(); });

  key_handler_.Bind(Key::kArrowRight,
                    [this]() -> void { view_->MoveCursorRight(); });

  key_handler_.Bind(Key::kCtrlP, [this]() -> void { view_->MoveCursorUp(); });

  key_handler_.Bind(Key::kCtrlN, [this]() -> void { view_->MoveCursorDown(); });

  key_handler_.Bind(Key::kCtrlH, [this]() -> void { view_->MoveCursorLeft(); });

  key_handler_.Bind(Key::kCtrlF,
                    [this]() -> void { view_->MoveCursorRight(); });

  key_handler_.Bind(Key::kCtrlA, [this]() -> void {
    view_->UpdateCursor(
        Buffer::Position{.row = view_->GetBufferPosition().row, .col = 0});
  });

  key_handler_.Bind(Key::kCtrlE, [this]() -> void {
    Buffer::Position pos = view_->GetBufferPosition();
    view_->UpdateCursor(Buffer::Position{
        .row = pos.row, .col = buffer_->GetLineLength(pos.row)});
  });

  key_handler_.SetFallback([this](Key key) -> void {
    view_->UpdateCursor(
        buffer_->Insert(view_->GetBufferPosition(), static_cast<char>(key)));
  });
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

std::unique_ptr<View> Editor::MakeView(Buffer* buffer, ViewPort vp) const {
  return std::make_unique<View>(buffer, vp);
}

}  // namespace flux
