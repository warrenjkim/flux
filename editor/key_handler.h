#pragma once

#include <unordered_map>

#include "editor/commands.h"
#include "terminal/keyboard.h"

namespace flux {

class KeyHandler {
 public:
  explicit KeyHandler() = default;

  bool Bind(Key key, Command::Function command, bool override = true);

  void SetFallback(Command::Function command);

  Command::Function GetCommand(Key key) const;

 private:
  std::unordered_map<Key, Command::Function> map_;

  Command::Function fallback_;
};

}  // namespace flux
