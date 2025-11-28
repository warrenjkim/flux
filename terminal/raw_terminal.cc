#include "terminal/raw_terminal.h"

#include <iostream>
#include <string_view>

#include "terminal/keyboard.h"
#include "view/viewport.h"

namespace flux {

RawTerminal::RawTerminal() { tcgetattr(STDIN_FILENO, &cooked_); }

RawTerminal::~RawTerminal() {}

Key RawTerminal::GetKey() {
  switch (Key k = ReadKey()) {
    case Key::kEscape:
      return ResolveEscapeSequence();
    default:
      return k;
  }

  return Key::kNone;
}

void RawTerminal::EnableRawMode() {
  termios raw = cooked_;
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);

  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void RawTerminal::DisableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &cooked_);
}

ViewPort RawTerminal::GetTerminalSize() const {
  winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
    return ViewPort{.rows = size_t{ws.ws_row}, .cols = size_t{ws.ws_col}};
  }

  return ViewPort{};
}

void RawTerminal::EnterAlternateBuffer() {
  std::cout << "\x1b[?1049h";
  Flush();
}

void RawTerminal::ExitAlternateBuffer() {
  std::cout << "\x1b[?1049l";
  Flush();
}

void RawTerminal::ResetCursor() { std::cout << "\x1b[H"; }

void RawTerminal::ShowCursor() { std::cout << "\x1b[?25h"; }

void RawTerminal::HideCursor() { std::cout << "\x1b[?25l"; }

void RawTerminal::MoveCursor(Cursor cursor) {
  std::cout << "\x1b[" << (cursor.row + 1) << ";" << (cursor.col + 1) << "H";
}

void RawTerminal::Write(char c) { std::cout << c; }

void RawTerminal::Write(std::string_view buffer) { std::cout << buffer; }

void RawTerminal::WriteLine(std::string_view buffer) {
  std::cout << buffer << "\r\n";
}

void RawTerminal::Flush() { std::cout.flush(); }

void RawTerminal::ClearLine() { std::cout << "\x1b[K"; }

void RawTerminal::ClearScreen() { std::cout << "\x1b[2J\x1b[H"; }

Key RawTerminal::ReadKey() {
  char c;
  if (read(STDIN_FILENO, &c, 1) == 1) {
    return static_cast<Key>(c);
  }

  return Key::kNone;
}

Key RawTerminal::ResolveEscapeSequence() {
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

}  // namespace flux
