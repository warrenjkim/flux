#pragma once

#include "terminal/keyboard.h"

namespace flux {

class Editor;

struct Command {
  using Function = void (*)(Editor*, Key);

  static void Quit(Editor* e, Key);

  static void Save(Editor* e, Key);

  static void InsertChar(Editor* e, Key k);

  static void Backspace(Editor* e, Key);

  static void Return(Editor* e, Key);

  static void EnterNormalMode(Editor* e, Key);

  static void EnterInsertMode(Editor* e, Key);

  static void MoveCursorUp(Editor* e, Key);

  static void MoveCursorDown(Editor* e, Key);

  static void MoveCursorLeft(Editor* e, Key);

  static void MoveCursorRight(Editor* e, Key);

  static void MoveCursorStart(Editor* e, Key);

  static void MoveCursorEnd(Editor* e, Key);
};

}  // namespace flux
