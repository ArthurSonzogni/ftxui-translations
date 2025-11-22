#include "ftxui/component/captured_mouse.hpp"      // 适用于 ftxui
#include "ftxui/component/component.hpp"           // 适用于 滑块
#include "ftxui/component/screen_interactive.hpp"  // 适用于 ScreenInteractive

using namespace ftxui;

int main() {
  auto screen = ScreenInteractive::TerminalOutput();
  int value = 50;
  auto slider = Slider("Value:", &value, 0, 100, 1);
  screen.Loop(slider);
}

// 版权所有 2020 Arthur Sonzogni. 保留所有权利。
// 此源代码的使用受 MIT 许可的约束，MIT 许可可在
// LICENSE 文件中找到。
