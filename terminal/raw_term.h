#pragma once

#include <termios.h>
#include <unistd.h>

#include "terminal/keyboard.h"

namespace flux {

class RawTerm {
 public:
  RawTerm();
  ~RawTerm();

  Key GetKey() const;

 private:
  Key ReadKey() const;

  Key ResolveEscapeSequence() const;

  // strictly speaking, [Enable|Disable]RawMode is a const function, but it is
  // not marked const since it modifies global terminal state.
  void EnableRawMode();

  void DisableRawMode();

  termios cooked_;
};

}  // namespace flux
