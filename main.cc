#include <string_view>

#include "editor/editor.h"

int main(int argc, char** argv) {
  std::string_view path;
  if (argc > 1) {
    path = argv[1];
  }

  flux::Editor().Run(path);

  return 0;
}
