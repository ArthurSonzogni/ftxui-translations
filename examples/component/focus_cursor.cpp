// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼受 MIT 授權條款約束，詳情請參閱
// LICENSE 文件。
#include <ftxui/component/captured_mouse.hpp>  // 用於 ftxui
#include <string>  // 用於 allocator, operator+, char_traits, string

#include "ftxui/component/component.hpp"           // 用於 Renderer, Vertical
#include "ftxui/component/screen_interactive.hpp"  // 用於 ScreenInteractive, Component
#include "ftxui/dom/elements.hpp"  // 用於 text, Decorator, focus, focusCursorBar, focusCursorBarBlinking, focusCursorBlock, focusCursorBlockBlinking, focusCursorUnderline, focusCursorUnderlineBlinking, hbox, Element

using namespace ftxui;

Component Instance(std::string label, Decorator focusCursor) {
  return Renderer([=](bool focused) {
    if (focused) {
      return hbox({
          text("> " + label + " "),
          focusCursor(text(" ")),
      });
    }
    return text("  " + label + " ");
  });
};

int main() {
  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(Container::Vertical({
      Instance("focus", focus),
      Instance("focusCursorBlock", focusCursorBlock),
      Instance("focusCursorBlockBlinking", focusCursorBlockBlinking),
      Instance("focusCursorBar", focusCursorBar),
      Instance("focusCursorBarBlinking", focusCursorBarBlinking),
      Instance("focusCursorUnderline", focusCursorUnderline),
      Instance("focusCursorUnderlineBlinking", focusCursorUnderlineBlinking),
  }));
  return 0;
}
