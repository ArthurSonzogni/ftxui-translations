// Copyright 2021 Arthur Sonzogni. All rights reserved.
// 本原始碼的使用受 MIT 授權約束，詳見 LICENSE 檔案。
#include <functional>  // for function
#include <utility>     // for move

#include "ftxui/component/component.hpp"       // for Make, Renderer
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/component/mouse.hpp"           // for Mouse
#include "ftxui/dom/elements.hpp"  // for Element, operator|, reflect
#include "ftxui/screen/box.hpp"    // for Box

namespace ftxui {

/// @brief 建立一個元件，使用 |render| 來繪製其介面。
/// @param render 繪製介面的函式。
/// @ingroup component
///
/// ### 範例
///
/// ```cpp
/// auto screen = ScreenInteractive::TerminalOutput();
/// auto renderer = Renderer([] {
///   return text("My interface");
/// });
/// screen.Loop(renderer);
/// ```
Component Renderer(std::function<Element()> render) {
  class Impl : public ComponentBase {
   public:
    explicit Impl(std::function<Element()> render)
        : render_(std::move(render)) {}
    Element OnRender() override { return render_(); }
    std::function<Element()> render_;
  };

  return Make<Impl>(std::move(render));
}

/// @brief 回傳一個新的元件，類似於 |child|，但使用 |render| 作為 Component::Render() 事件。
/// @param child 用於轉發事件的元件。
/// @param render 繪製介面的函式。
/// @ingroup component
///
/// ### 範例
///
/// ```cpp
/// auto screen = ScreenInteractive::TerminalOutput();
/// std::string label = "Click to quit";
/// auto button = Button(&label, screen.ExitLoopClosure());
/// auto renderer = Renderer(button, [&] {
///   return hbox({
///     text("A button:"),
///     button->Render(),
///   });
/// });
/// screen.Loop(renderer);
/// ```
Component Renderer(Component child, std::function<Element()> render) {
  Component renderer = Renderer(std::move(render));
  renderer->Add(std::move(child));
  return renderer;
}

/// @brief 建立一個可聚焦的元件，使用 |render| 來繪製其介面。
/// @param render 繪製介面的函式，它接受一個布林值，表示該元件是否被聚焦。
/// @ingroup component
///
/// ### 範例
///
/// ```cpp
/// auto screen = ScreenInteractive::TerminalOutput();
/// auto renderer = Renderer([] (bool focused) {
///   if (focused)
///     return text("My interface") | inverted;
///   else
///     return text("My interface");
/// });
/// screen.Loop(renderer);
/// ```
Component Renderer(std::function<Element(bool)> render) {
  class Impl : public ComponentBase {
   public:
    explicit Impl(std::function<Element(bool)> render)
        : render_(std::move(render)) {}

   private:
    Element OnRender() override { return render_(Focused()) | reflect(box_); }
    bool Focusable() const override { return true; }
    bool OnEvent(Event event) override {
      if (event.is_mouse() && box_.Contain(event.mouse().x, event.mouse().y)) {
        if (!CaptureMouse(event)) {
          return false;
        }

        TakeFocus();
      }

      return false;
    }
    Box box_;

    std::function<Element(bool)> render_;
  };
  return Make<Impl>(std::move(render));
}

/// @brief 裝飾一個元件，透過裝飾它所呈現的內容。
/// @param decorator 修改其呈現元素的函式。
/// @ingroup component
///
/// ### 範例
///
/// ```cpp
/// auto screen = ScreenInteractive::TerminalOutput();
/// auto renderer =
//     Renderer([] { return text("Hello");)
///  | Renderer(bold)
///  | Renderer(inverted);
/// screen.Loop(renderer);
/// ```
ComponentDecorator Renderer(ElementDecorator decorator) {  // NOLINT
  return [decorator](Component component) {                // NOLINT
    return Renderer(component, [component, decorator] {
      return component->Render() | decorator;
    });
  };
}

}  // namespace ftxui
