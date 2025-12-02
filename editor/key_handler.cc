#include "editor/key_handler.h"

namespace flux {

bool KeyHandler::Bind(Key key, Handler handler, bool override) {
  if (override) {
    map_[key] = std::move(handler);
    return true;
  }

  return map_.insert({key, std::move(handler)}).second;
}

void KeyHandler::SetFallback(Fallback fallback) {
  fallback_ = std::move(fallback);
}

void KeyHandler::Handle(Key key) {
  if (auto it = map_.find(key); it != map_.end()) {
    it->second();
  } else {
    fallback_(key);
  }
}

}  // namespace flux
