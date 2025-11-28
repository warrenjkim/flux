#include <fstream>
#include <iostream>
#include <utility>

#include "buffer/buffer.h"
#include "buffer/vector_buffer.h"
#include "terminal/keyboard.h"
#include "terminal/raw_terminal.h"
#include "view/view.h"

int main(int argc, char** argv) {
  std::vector<std::string> lines;
  std::ifstream file;
  if (argc > 1) {
    file.open(argv[1]);
    if (!file) {
      std::cout << "read failed\n";
      return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
      lines.push_back(std::move(line));
    }

    file.close();
  }

  if (lines.empty()) {
    lines.push_back("");
  }

  flux::RawTerminal raw_term;
  raw_term.EnableRawMode();
  raw_term.EnterAlternateBuffer();
  {
    flux::VectorBuffer buffer(std::move(lines));
    flux::View view(&buffer);
    flux::ViewPort vp;
    view.Resize(vp = raw_term.GetTerminalSize());
    raw_term.ClearScreen();
    while (true) {
      raw_term.ResetCursor();
      raw_term.HideCursor();
      view.Draw(&raw_term);
      raw_term.MoveCursor(view.GetCursor());
      raw_term.ShowCursor();
      raw_term.Flush();

      if (flux::Key c = raw_term.GetKey(); c != flux::Key::kNone) {
        switch (c) {
          case flux::Key::kCtrlQ: {
            raw_term.ExitAlternateBuffer();
            raw_term.DisableRawMode();
            return 0;
          };
          case flux::Key::kBackspace: {
            view.UpdateCursor(buffer.Delete(view.GetBufferPosition()));
          } break;
          case flux::Key::kReturn: {
            view.UpdateCursor(buffer.BreakLine(view.GetBufferPosition()));
          } break;
          case flux::Key::kArrowUp: {
            view.MoveCursorUp();
          } break;
          case flux::Key::kArrowDown: {
            view.MoveCursorDown();
          } break;
          case flux::Key::kArrowLeft: {
            view.MoveCursorLeft();
          } break;
          case flux::Key::kArrowRight: {
            view.MoveCursorRight();
          } break;
          default: {
            view.UpdateCursor(
                buffer.Insert(view.GetBufferPosition(), static_cast<char>(c)));
          } break;
        }
      }
    }

    raw_term.ExitAlternateBuffer();
    raw_term.DisableRawMode();
  }

  return 0;
}
