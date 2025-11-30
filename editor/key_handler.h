#pragma once

#include <functional>
#include <unordered_map>

#include "terminal/keyboard.h"

namespace flux {

class KeyHandler {
 public:
  using Handler = std::function<void()>;
  using Fallback = std::function<void(Key)>;

  explicit KeyHandler() = default;

  bool Bind(Key key, Handler handler, bool override = true);

  void SetFallback(Fallback fallback);

  void Handle(Key key);

 private:
  std::unordered_map<Key, Handler> map_;

  Fallback fallback_;
};

}  // namespace flux
