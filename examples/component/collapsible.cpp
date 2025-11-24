// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 此原始碼受 MIT 授權條款約束，詳情請參閱
// LICENSE 文件。
#include <memory>   // for allocator, make_shared, __shared_ptr_access
#include <utility>  // for move
#include <vector>   // for vector

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Collapsible, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"                  // for text, hbox, Element

using namespace ftxui;

// 接收一個元件列表，垂直顯示它們，每一列向右偏移一格。
Component Inner(std::vector<Component> children) {
  Component vlist = Container::Vertical(std::move(children));
  return Renderer(vlist, [vlist] {
    return hbox({
        text(" "),
        vlist->Render(),
    });
  });
}

Component Empty() {
  return std::make_shared<ComponentBase>();
}

int main() {
  auto component =
      Collapsible("Collapsible 1",
                  Inner({
                      Collapsible("Collapsible 1.1",
                                  Inner({
                                      Collapsible("Collapsible 1.1.1", Empty()),
                                      Collapsible("Collapsible 1.1.2", Empty()),
                                      Collapsible("Collapsible 1.1.3", Empty()),
                                  })),
                      Collapsible("Collapsible 1.2",
                                  Inner({
                                      Collapsible("Collapsible 1.2.1", Empty()),
                                      Collapsible("Collapsible 1.2.2", Empty()),
                                      Collapsible("Collapsible 1.2.3", Empty()),
                                  })),
                      Collapsible("Collapsible 1.3",
                                  Inner({
                                      Collapsible("Collapsible 1.3.1", Empty()),
                                      Collapsible("Collapsible 1.3.2", Empty()),
                                      Collapsible("Collapsible 1.3.3", Empty()),
                                  })),
                  }));

  ScreenInteractive::FitComponent().Loop(component);
}