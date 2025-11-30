#pragma once

#include "terminal/raw_terminal.h"

namespace flux {

class StatusLine {
 public:
  struct Options {
    std::string_view file_name;
    char delimiter;
    size_t left_padding;
  };

  explicit StatusLine(size_t cols);

  void Resize(size_t cols);

  void Draw(RawTerminal* terminal, Options opts = {});

 private:
  size_t cols_;
};

}  // namespace flux
