// 版权所有 2020 Arthur Sonzogni。保留所有权利。
// 此源代码的使用受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
#include <ftxui/component/captured_mouse.hpp>  // for ftxui
#include <string>  // for allocator, operator+, char_traits, string

#include "ftxui/component/component.hpp"           // for Renderer, Vertical
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive, Component
#include "ftxui/dom/elements.hpp"  // for text, Decorator, focus, focusCursorBar, focusCursorBarBlinking, focusCursorBlock, focusCursorBlockBlinking, focusCursorUnderline, focusCursorUnderlineBlinking, hbox, Element

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
