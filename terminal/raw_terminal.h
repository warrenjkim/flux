#pragma once

#include <sys/ioctl.h>
#include <sys/ttycom.h>
#include <termios.h>
#include <unistd.h>
#include <string_view>

#include "terminal/keyboard.h"
#include "view/viewport.h"

namespace flux {

class RawTerminal {
 public:
  RawTerminal();
  ~RawTerminal();

  Key GetKey() const;

  // strictly speaking, [Enable|Disable]RawMode is a const function, but it is
  // not marked const since it modifies global terminal state.
  void EnableRawMode();

  void DisableRawMode();

  ViewPort GetTerminalSize() const;

  void Write(std::string_view buffer);

 private:
  Key ReadKey() const;

  Key ResolveEscapeSequence() const;

  termios cooked_;
};

}  // namespace flux
