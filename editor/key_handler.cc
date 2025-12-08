#include "editor/key_handler.h"

#include "editor/commands.h"

namespace flux {

bool KeyHandler::Bind(Key key, Command::Function command, bool override) {
  if (override) {
    map_[key] = command;
    return true;
  }

  return map_.insert({key, command}).second;
}

void KeyHandler::SetFallback(Command::Function fallback) {
  fallback_ = fallback;
}

Command::Function KeyHandler::GetCommand(Key key) const {
  if (auto it = map_.find(key); it != map_.end()) {
    return it->second;
  } else {
    return fallback_;
  }
}

}  // namespace flux
