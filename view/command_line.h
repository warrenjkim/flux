#pragma once

#include "terminal/raw_terminal.h"

namespace flux {

class CommandLine {
 public:
  explicit CommandLine(size_t cols);

  void Resize(size_t cols);

  void SetMessage(std::string_view message);

  void Draw(RawTerminal* terminal);

 private:
  size_t cols_;
  std::string message_;
};

}  // namespace flux
