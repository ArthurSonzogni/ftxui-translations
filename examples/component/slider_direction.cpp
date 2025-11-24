// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權約束，可在 LICENSE 檔案中找到。
#include <array>                                  // for array
#include <cmath>                                  // for sin
#include <ftxui/component/component_base.hpp>     // for ComponentBase
#include <ftxui/component/component_options.hpp>  // for SliderOption
#include <ftxui/dom/direction.hpp>  // for Direction, Direction::Up
#include <ftxui/dom/elements.hpp>   // for size, GREATER_THAN, HEIGHT
#include <ftxui/util/ref.hpp>       // for ConstRef, Ref
#include <memory>                   // for shared_ptr, __shared_ptr_access

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Horizontal, Slider, operator|=
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive

using namespace ftxui;

int main() {
  auto screen = ScreenInteractive::TerminalOutput();
  std::array<int, 30> values;
  for (size_t i = 0; i < values.size(); ++i) {
    values[i] = 50 + 20 * std::sin(i * 0.3);
  }

  auto layout_horizontal = Container::Horizontal({});
  for (auto& value : values) {
    // 在 C++17 中：
    SliderOption<int> option;
    option.value = &value;
    option.max = 100;
    option.increment = 5;
    option.direction = Direction::Up;
    layout_horizontal->Add(Slider<int>(option));

    /* 在 C++20 中：
    layout_horizontal->Add(Slider<int>({
        .value = &values[i],
        .max = 100,
        .increment = 5,
        .direction = Direction::Up,
    }));
    */
  }

  layout_horizontal |= size(HEIGHT, GREATER_THAN, 20);

  screen.Loop(layout_horizontal);
}
