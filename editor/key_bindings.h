#pragma once

#include <vector>

#include "editor/commands.h"
#include "pulse/dsa/trie.h"
#include "terminal/keyboard.h"

namespace flux {

class KeyBindings {
 public:
  using Chord = std::vector<Key>;

  explicit KeyBindings() = default;

  bool BindKey(Key key, Command::Function command, bool override = true);

  bool BindChord(Chord chord, Command::Function command, bool override = true);

  void SetFallback(Command::Function command);

  Command::Function GetCommand(Key key) const;

  Command::Function GetCommand(Chord key) const;

 private:
  pulse::Trie<Chord, Command::Function> map_;

  Command::Function fallback_;
};

}  // namespace flux
