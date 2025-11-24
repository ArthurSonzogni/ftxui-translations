// Copyright 2020 Arthur Sonzogni. 無断複写・転載を禁じます。
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに従います。
#include <memory>  // アロケータ、__shared_ptr_access、shared_ptr用
#include <string>  // 文字列用

#include "ftxui/component/captured_mouse.hpp"  // ftxui用
#include "ftxui/component/component.hpp"  // Input、Renderer、ResizableSplitLeft用
#include "ftxui/component/component_base.hpp"  // ComponentBase、Component用
#include "ftxui/component/screen_interactive.hpp"  // ScreenInteractive用
#include "ftxui/dom/elements.hpp"  // operator|、separator、text、Element、flex、vbox、border用

int main() {
  using namespace ftxui;

  std::string content_1;
  std::string content_2;
  auto textarea_1 = Input(&content_1);
  auto textarea_2 = Input(&content_2);
  int size = 50;
  auto layout = ResizableSplitLeft(textarea_1, textarea_2, &size);

  auto component = Renderer(layout, [&] {
    return vbox({
               text("Input:"),
               separator(),
               layout->Render() | flex,
           }) |
           border;
  });

  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(component);
}
