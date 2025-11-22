// 版权所有 2020 Arthur Sonzogni。保留所有权利。
// 本源代码的使用受 MIT 许可证管辖，该许可证可在
// LICENSE 文件中找到。
#include <functional>  // for function
#include <memory>      // for allocator, __shared_ptr_access
#include <string>      // for string, basic_string, operator+, to_string
#include <vector>      // for vector

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Menu, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for MenuOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, separator, bold, hcenter, vbox, hbox, gauge, Element, operator|, border

int main() {
  using namespace ftxui;
  auto screen = ScreenInteractive::TerminalOutput();

  std::vector<std::string> left_menu_entries = {
      "0%", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%",
  };
  std::vector<std::string> right_menu_entries = {
      "0%", "1%", "2%", "3%", "4%", "5%", "6%", "7%", "8%", "9%", "10%",
  };

  auto menu_option = MenuOption();
  menu_option.on_enter = screen.ExitLoopClosure();

  int left_menu_selected = 0;
  int right_menu_selected = 0;
  Component left_menu_ =
      Menu(&left_menu_entries, &left_menu_selected, menu_option);
  Component right_menu_ =
      Menu(&right_menu_entries, &right_menu_selected, menu_option);

  Component container = Container::Horizontal({
      left_menu_,
      right_menu_,
  });

  auto renderer = Renderer(container, [&] {
    int sum = left_menu_selected * 10 + right_menu_selected;
    return vbox({
               // -------- 顶部面板 --------------
               hbox({
                   // -------- 左侧菜单 --------------
                   vbox({
                       hcenter(bold(text("Percentage by 10%"))),
                       separator(),
                       left_menu_->Render(),
                   }),
                   separator(),
                   // -------- 右侧菜单 --------------
                   vbox({
                       hcenter(bold(text("Percentage by 1%"))),
                       separator(),
                       right_menu_->Render(),
                   }),
                   separator(),
               }),
               separator(),
               // -------- 底部面板 --------------
               vbox({
                   hbox({
                       text(" gauge : "),
                       gauge(sum / 100.0),
                   }),
                   hbox({
                       text("  text : "),
                       text(std::to_string(sum) + " %"),
                   }),
               }),
           }) |
           border;
  });

  screen.Loop(renderer);
}
