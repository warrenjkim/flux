#include "editor/key_bindings.h"

#include "editor/commands.h"

namespace flux {

bool KeyBindings::Bind(Key key, Command::Function command, bool override) {
  if (override) {
    map_[key] = command;
    return true;
  }

  return map_.insert({key, command}).second;
}

void KeyBindings::SetFallback(Command::Function fallback) {
  fallback_ = fallback;
}

Command::Function KeyBindings::GetCommand(Key key) const {
  if (auto it = map_.find(key); it != map_.end()) {
    return it->second;
  } else {
    return fallback_;
  }
}

}  // namespace flux
