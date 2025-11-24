// Copyright 2020 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、MITライセンスに従います。
// LICENSEファイルで確認できます。
#include <functional>  // 関数用
#include <memory>      // アロケーター、__shared_ptr_access用
#include <string>      // string、basic_string、operator+、to_string用
#include <vector>      // vector用

#include "ftxui/component/captured_mouse.hpp"  // ftxui用
#include "ftxui/component/component.hpp"       // Menu、Horizontal、Renderer用
#include "ftxui/component/component_base.hpp"  // ComponentBase用
#include "ftxui/component/component_options.hpp"  // MenuOption用
#include "ftxui/component/screen_interactive.hpp"  // Component、ScreenInteractive用
#include "ftxui/dom/elements.hpp"  // text、separator、bold、hcenter、vbox、hbox、gauge、Element、operator|、border用

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
               // -------- 上部パネル --------------
               hbox({
                   // -------- 左メニュー --------------
                   vbox({
                       hcenter(bold(text("Percentage by 10%"))),
                       separator(),
                       left_menu_->Render(),
                   }),
                   separator(),
                   // -------- 右メニュー --------------
                   vbox({
                       hcenter(bold(text("Percentage by 1%"))),
                       separator(),
                       right_menu_->Render(),
                   }),
                   separator(),
               }),
               separator(),
               // -------- 下部パネル --------------
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
