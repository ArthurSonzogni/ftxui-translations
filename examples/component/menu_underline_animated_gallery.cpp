// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱
// LICENSE 文件。
#include <chrono>  // for operator""ms, literals
#include <memory>  // for allocator, shared_ptr, __shared_ptr_access
#include <string>  // for string, operator+, to_string, basic_string
#include <vector>  // for vector

#include "ftxui/component/animation.hpp"       // for BackOut, Duration
#include "ftxui/component/component.hpp"       // for Menu, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for MenuOption, UnderlineOption
#include "ftxui/component/mouse.hpp"              // for ftxui
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, Element, operator|, borderEmpty, inverted
#include "ftxui/screen/color.hpp"  // for Color, Color::Blue, Color::Red

using namespace ftxui;

Component DummyComponent(int id) {
  return Renderer([id](bool focused) {
    auto t = text("component " + std::to_string(id));
    if (focused) {
      t = t | inverted;
    }
    return t;
  });
}

Component Text(const std::string& t) {
  return Renderer([t] { return text(t) | borderEmpty; });
}

int main() {
  using namespace std::literals;
  std::vector<std::string> tab_values{
      "Tab 1", "Tab 2", "Tab 3", "A very very long tab", "탭",
  };
  int tab_selected = 0;

  auto container = Container::Vertical({});

  int frame_count = 0;
  container->Add(Renderer(
      [&] { return text("Frame count: " + std::to_string(frame_count++)); }));

  {
    auto option = MenuOption::HorizontalAnimated();
    container->Add(Text("這展示了 Menu 元件"));
    container->Add(Menu(&tab_values, &tab_selected, option));
  }

  {
    container->Add(Text("將底線顏色設為藍色"));
    auto option = MenuOption::HorizontalAnimated();
    option.underline.color_inactive = Color::Blue;
    container->Add(Menu(&tab_values, &tab_selected, option));
  }

  {
    container->Add(Text("將底線啟用顏色設為紅色"));
    auto option = MenuOption::HorizontalAnimated();
    option.underline.color_active = Color::Red;
    container->Add(Menu(&tab_values, &tab_selected, option));
  }

  {
    container->Add(Text("將動畫持續時間設為 0 毫秒"));
    auto option = MenuOption::HorizontalAnimated();
    option.underline.SetAnimationDuration(0ms);
    container->Add(Menu(&tab_values, &tab_selected, option));
  }

  {
    container->Add(Text("將動畫緩動函數設為 back-out"));
    auto option = MenuOption::HorizontalAnimated();
    option.underline.SetAnimationFunction(animation::easing::BackOut);
    option.underline.SetAnimationDuration(350ms);
    container->Add(Menu(&tab_values, &tab_selected, option));
  }

  // option.underline_animation_follower_delay = 250ms
  {
    container->Add(Text("新增延遲以使動畫不同步"));
    auto option = MenuOption::HorizontalAnimated();
    option.underline.follower_delay = 250ms;
    container->Add(Menu(&tab_values, &tab_selected, option));
  }

  container->SetActiveChild(container->ChildAt(2));

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(container);
}
