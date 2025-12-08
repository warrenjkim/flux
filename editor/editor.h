#pragma once

#include <memory>
#include <unordered_map>

#include "buffer/buffer.h"
#include "editor/key_handler.h"
#include "view/command_line.h"
#include "view/status_line.h"
#include "view/view.h"

namespace flux {

enum class Mode { kNormal, kInsert };

class Editor {
 public:
  explicit Editor();

  void Run(std::string_view path = "");

 private:
  void BindKeys();

  void BindNormalModeKeys();

  void BindInsertModeKeys();

  std::unique_ptr<Buffer> OpenFile(std::string_view path) const;

  size_t WriteFile(std::string_view path) const;

  std::unique_ptr<View> MakeView(Buffer* buffer, ViewPort vp) const;

  std::string path_;
  std::unique_ptr<Buffer> buffer_;
  std::unique_ptr<StatusLine> status_line_;
  std::unique_ptr<CommandLine> command_line_;
  std::unique_ptr<View> view_;
  std::unordered_map<Mode, KeyHandler> key_handler_;
  Mode mode_;
  bool running_;
};

}  // namespace flux
