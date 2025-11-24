#include <fstream>
#include <iostream>
#include <utility>

#include "buffer/buffer.h"
#include "buffer/vector_buffer.h"
#include "terminal/keyboard.h"
#include "terminal/raw_terminal.h"

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

  struct {
    size_t r = 0;
    size_t c = 0;
  } cursor;

  flux::RawTerminal raw_term;
  raw_term.EnableRawMode();
  {
    flux::VectorBuffer buffer(std::move(lines));
    while (true) {
      std::cout << "\033[2J\033[H";
      for (size_t i = 0; i < buffer.Lines(); i++) {
        std::string line = buffer.GetLine(i);
        if (i == cursor.r) {
          if (cursor.c == line.length()) {
            line.push_back('~');
          } else {
            line[cursor.c] = '~';
          }
        }

        std::cout << line << "\r\n";
      }

      if (flux::Key c = raw_term.GetKey(); c != flux::Key::kNone) {
        switch (c) {
          case flux::Key::kCtrlQ: {
            std::cout << "\033[2J\033[H";
            raw_term.DisableRawMode();
            return 0;
          }
          case flux::Key::kBackspace: {
            flux::Buffer::Position pos = buffer.Delete(
                flux::Buffer::Position{.row = cursor.r, .col = cursor.c});
            cursor.r = pos.row;
            cursor.c = pos.col;
          } break;
          case flux::Key::kReturn: {
            flux::Buffer::Position pos = buffer.BreakLine(
                flux::Buffer::Position{.row = cursor.r, .col = cursor.c});
            cursor.r = pos.row;
            cursor.c = pos.col;
          } break;
          default: {
            flux::Buffer::Position pos = buffer.Insert(
                flux::Buffer::Position{.row = cursor.r, .col = cursor.c},
                static_cast<char>(c));
            cursor.r = pos.row;
            cursor.c = pos.col;
          } break;
        }
      }

      std::cout.flush();
    }

    raw_term.DisableRawMode();
  }

  return 0;
}
