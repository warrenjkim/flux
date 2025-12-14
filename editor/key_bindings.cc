#include "editor/key_bindings.h"

#include "editor/commands.h"
#include "pulse/dsa/trie.h"

namespace flux {

bool KeyBindings::BindKey(Key key, Command::Function command, bool override) {
  return BindChord(Chord({key}), command, override);
}

bool KeyBindings::BindChord(const Chord& chord, Command::Function command,
                            bool override) {
  if (map_.match(chord) && !override) {
    return false;
  }

  map_[chord] = std::move(command);
  return true;
}

void KeyBindings::SetFallback(Command::Function fallback) {
  fallback_ = fallback;
}

Command::Function KeyBindings::GetCommand(Key key) const {
  return GetCommand(Chord({key}));
}

Command::Function KeyBindings::GetCommand(const Chord& chord) const {
  if (const Command::Function* cmd = map_.get(chord); cmd) {
    return *cmd;
  } else {
    return fallback_;
  }
}

}  // namespace flux
