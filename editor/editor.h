#pragma once

#include <memory>
#include <unordered_map>

#include "buffer/buffer.h"
#include "editor/key_bindings.h"
#include "editor/mode.h"
#include "view/command_line.h"
#include "view/status_line.h"
#include "view/view.h"

namespace flux {

class Editor {
 public:
  struct Config {
    std::unordered_map<Mode, KeyBindings> key_bindings;
  };

  explicit Editor(Config config = {});

  void Run(std::string_view path = "");

 private:
  void BindKeys();

  void BindNormalModeKeys();

  void BindInsertModeKeys();

  std::unique_ptr<Buffer> OpenFile(std::string_view path) const;

  size_t WriteFile(std::string_view path) const;

  std::string path_;
  std::unique_ptr<Buffer> buffer_;
  std::unique_ptr<StatusLine> status_line_;
  std::unique_ptr<CommandLine> command_line_;
  std::unique_ptr<View> view_;
  Config config_;
  Mode mode_;
  bool running_;

  friend struct Command;
};

}  // namespace flux
