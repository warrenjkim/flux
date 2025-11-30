#include "view/status_line.h"

#include <string_view>

#include "terminal/raw_terminal.h"

namespace flux {

StatusLine::StatusLine(size_t cols) : cols_(cols) {}

void StatusLine::Resize(size_t cols) { cols_ = cols; }

void StatusLine::Draw(RawTerminal* terminal, Options opts) {
  for (size_t i = 0; i < std::min(cols_, opts.left_padding); i++) {
    terminal->Write(opts.delimiter);
  }

  terminal->Write(
      opts.file_name.substr(0, std::min(cols_, opts.file_name.length())));

  size_t i = opts.left_padding + opts.file_name.length();
  while (++i < cols_) {
    terminal->Write(opts.delimiter);
  }
}

}  // namespace flux
