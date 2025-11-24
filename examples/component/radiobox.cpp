// Copyright 2020 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに準拠します。
#include <string>  // for string, allocator, basic_string
#include <vector>  // for vector

#include "ftxui/component/captured_mouse.hpp"      // for ftxui
#include "ftxui/component/component.hpp"           // for Radiobox
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive

using namespace ftxui;

int main() {
  std::vector<std::string> radiobox_list = {
      "Use gcc",
      "Use clang",
      "Use emscripten",
      "Use tcc",
  };
  int selected = 0;

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(Radiobox(&radiobox_list, &selected));
  return 0;
}
