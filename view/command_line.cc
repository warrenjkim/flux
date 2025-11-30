#include "view/command_line.h"

namespace flux {

CommandLine::CommandLine(size_t cols) : cols_(cols), message_("") {}

void CommandLine::Resize(size_t cols) { cols_ = cols; }

void CommandLine::SetMessage(std::string_view message) {
  message_ = std::string(message);
}

void CommandLine::Draw(RawTerminal* terminal) { terminal->Write(message_); }

}  // namespace flux
