#include <fstream>
#include <iostream>

#include "terminal/keyboard.h"
#include "terminal/raw_term.h"

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

  {
    flux::RawTerm raw_term;
    while (true) {
      std::cout << "\033[2J\033[H";
      for (size_t i = 0; i < lines.size(); i++) {
        for (size_t j = 0; j < lines[i].size(); j++) {
          if (cursor.r == i && cursor.c == j) {
            std::cout << '~';
          } else {
            std::cout << lines[i][j];
          }
        }

        std::cout << "\r\n";
      }

      if (flux::Key c = raw_term.GetKey(); c != flux::Key::kNone) {
        switch (c) {
          case flux::Key::kCtrlQ:
            std::cout << "\033[2J\033[H";
            return 0;
          case flux::Key::kBackspace:
            if (cursor.c > 0) {
              lines[cursor.r].erase(cursor.c--, 1);
            }

            break;
          default:
            lines[cursor.r].insert(lines[cursor.r].begin() + cursor.c++, 1,
                                   static_cast<char>(c));

            break;
        }
      }

      std::cout.flush();
    }
  }

  return 0;
}
