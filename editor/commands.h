#pragma once

#include "terminal/keyboard.h"

namespace flux {

class Editor;

struct Command {
  using Function = void (*)(Editor*, Key);

  static void Quit(Editor* e, Key = Key::kNone);

  static void Save(Editor* e, Key = Key::kNone);

  static void InsertChar(Editor* e, Key k);

  static void Backspace(Editor* e, Key = Key::kNone);

  static void Return(Editor* e, Key = Key::kNone);

  static void EnterNormalMode(Editor* e, Key = Key::kNone);

  static void EnterInsertMode(Editor* e, Key = Key::kNone);

  static void MoveCursorUp(Editor* e, Key = Key::kNone);

  static void MoveCursorDown(Editor* e, Key = Key::kNone);

  static void MoveCursorLeft(Editor* e, Key = Key::kNone);

  static void MoveCursorRight(Editor* e, Key = Key::kNone);

  static void MoveCursorStart(Editor* e, Key = Key::kNone);

  static void MoveCursorEnd(Editor* e, Key = Key::kNone);

  static void DeleteLine(Editor* e, Key = Key::kNone);
};

}  // namespace flux
