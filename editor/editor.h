#pragma once

#include <memory>

#include "buffer/buffer.h"
#include "view/command_line.h"
#include "view/status_line.h"
#include "view/view.h"

namespace flux {

class Editor {
 public:
  explicit Editor();

  void Run(std::string_view path = "");

 private:
  std::unique_ptr<Buffer> OpenFile(std::string_view path) const;

  size_t WriteFile(std::string_view path) const;

  std::unique_ptr<View> MakeView(Buffer* buffer, ViewPort vp) const;

  std::unique_ptr<Buffer> buffer_;
  std::unique_ptr<StatusLine> status_line_;
  std::unique_ptr<CommandLine> command_line_;
  std::unique_ptr<View> view_;
};

}  // namespace flux
