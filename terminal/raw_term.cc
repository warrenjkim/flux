#include "terminal/raw_term.h"

#include "terminal/keyboard.h"

namespace flux {

RawTerm::RawTerm() {
  tcgetattr(STDIN_FILENO, &cooked_);
  EnableRawMode();
}

RawTerm::~RawTerm() { DisableRawMode(); }

Key RawTerm::GetKey() const {
  switch (Key k = ReadKey()) {
    case Key::kEscape:
      return ResolveEscapeSequence();
    default:
      return k;
  }

  return Key::kNone;
}

Key RawTerm::ReadKey() const {
  char c;
  if (read(STDIN_FILENO, &c, 1) == 1) {
    return static_cast<Key>(c);
  }

  return Key::kNone;
}

Key RawTerm::ResolveEscapeSequence() const {
  if (ReadKey() == Key::kLeftBracket) {
    switch (ReadKey()) {
      case Key::kA:
        return Key::kArrowUp;
      case Key::kB:
        return Key::kArrowDown;
      case Key::kC:
        return Key::kArrowRight;
      case Key::kD:
        return Key::kArrowLeft;
      default:
        break;
    }
  }

  return Key::kEscape;
}

void RawTerm::EnableRawMode() {
  termios raw = cooked_;
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);

  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void RawTerm::DisableRawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &cooked_); }

}  // namespace flux
