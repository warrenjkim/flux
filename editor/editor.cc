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

Editor::Editor() {}

void Editor::Run(std::string_view path) {
  flux::RawTerminal raw_term;
  raw_term.EnableRawMode();
  raw_term.EnterAlternateBuffer();
  raw_term.ClearScreen();

  buffer_ = OpenFile(path);
  ViewPort vp = raw_term.GetTerminalSize();
  vp.rows -= 2;
  view_ = MakeView(buffer_.get(), vp);
  status_line_ = std::make_unique<StatusLine>(vp.cols);
  command_line_ = std::make_unique<CommandLine>(vp.cols);

  bool exit = false;
  while (!exit) {
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

    if (flux::Key c = raw_term.GetKey(); c != flux::Key::kNone) {
      switch (c) {
        case flux::Key::kCtrlQ: {
          exit = true;
        } break;
        case flux::Key::kCtrlS: {
          WriteFile(path);
          command_line_->SetMessage(
              std::string("wrote to \"").append(path).append("\""));
        } break;
        case flux::Key::kBackspace: {
          view_->UpdateCursor(buffer_->Delete(view_->GetBufferPosition()));
        } break;
        case flux::Key::kReturn: {
          view_->UpdateCursor(buffer_->BreakLine(view_->GetBufferPosition()));
        } break;
        case flux::Key::kArrowUp: {
          view_->MoveCursorUp();
        } break;
        case flux::Key::kArrowDown: {
          view_->MoveCursorDown();
        } break;
        case flux::Key::kArrowLeft: {
          view_->MoveCursorLeft();
        } break;
        case flux::Key::kArrowRight: {
          view_->MoveCursorRight();
        } break;
        default: {
          view_->UpdateCursor(buffer_->Insert(view_->GetBufferPosition(),
                                              static_cast<char>(c)));
        } break;
      }
    }
  }

  raw_term.ExitAlternateBuffer();
  raw_term.DisableRawMode();
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
