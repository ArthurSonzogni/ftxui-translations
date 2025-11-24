// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 許可證的約束，該許可證可在
// LICENSE 文件中找到。
#include <functional>  // 用於 function
#include <memory>      // 用於 allocator, __shared_ptr_access
#include <string>      // 用於 string, basic_string, operator+, to_string
#include <vector>      // 用於 vector

#include "ftxui/component/captured_mouse.hpp"  // 用於 ftxui
#include "ftxui/component/component.hpp"       // 用於 Menu, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"  // 用於 ComponentBase
#include "ftxui/component/component_options.hpp"  // 用於 MenuOption
#include "ftxui/component/screen_interactive.hpp"  // 用於 Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // 用於 text, separator, bold, hcenter, vbox, hbox, gauge, Element, operator|, border

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
               // -------- 頂部面板 --------------
               hbox({
                   // -------- 左側菜單 --------------
                   vbox({
                       hcenter(bold(text("以 10% 為增量的百分比"))),
                       separator(),
                       left_menu_->Render(),
                   }),
                   separator(),
                   // -------- 右側菜單 --------------
                   vbox({
                       hcenter(bold(text("以 1% 為增量的百分比"))),
                       separator(),
                       right_menu_->Render(),
                   }),
                   separator(),
               }),
               separator(),
               // -------- 底部面板 --------------
               vbox({
                   hbox({
                       text(" 量表 : "),
                       gauge(sum / 100.0),
                   }),
                   hbox({
                       text("  文字 : "),
                       text(std::to_string(sum) + " %"),
                   }),
               }),
           }) |
           border;
  });

  screen.Loop(renderer);
}
