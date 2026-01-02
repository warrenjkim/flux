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

bool KeyBindings::PrefixCommand(const Chord& chord) const {
  return map_.prefix(chord);
}

const Command::Function& KeyBindings::GetFallback() const { return fallback_; }

const Command::Function* KeyBindings::MatchCommand(const Chord& chord) const {
  return map_.get(chord);
}

}  // namespace flux
