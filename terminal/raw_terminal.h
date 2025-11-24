#pragma once

#include <termios.h>
#include <unistd.h>

#include "terminal/keyboard.h"

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

 private:
  Key ReadKey() const;

  Key ResolveEscapeSequence() const;

  termios cooked_;
};

}  // namespace flux
