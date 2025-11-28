#pragma once

#include <sys/ioctl.h>
#include <sys/ttycom.h>
#include <termios.h>
#include <unistd.h>

#include <string_view>

#include "terminal/keyboard.h"
#include "view/cursor.h"
#include "view/viewport.h"

namespace flux {

class RawTerminal {
 public:
  RawTerminal();
  ~RawTerminal();

  // strictly speaking, most of these are const functions, but the are not
  // marked const since they modify global terminal state. only functions that
  // truly don't modify anything will be marked const.
  Key GetKey();

  void EnableRawMode();

  void DisableRawMode();

  void EnterAlternateBuffer();

  void ExitAlternateBuffer();

  ViewPort GetTerminalSize() const;

  void ResetCursor();

  void ShowCursor();

  void HideCursor();

  void MoveCursor(Cursor cursor);

  void Write(char c);

  void Write(std::string_view buffer);

  void WriteLine(std::string_view buffer = "");

  void Flush();

  void ClearLine();

  void ClearScreen();

 private:
  Key ReadKey();

  Key ResolveEscapeSequence();

  termios cooked_;
};

}  // namespace flux
