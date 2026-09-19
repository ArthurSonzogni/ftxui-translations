// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <functional>  // for function
#include <utility>     // for move

#include "ftxui/component/component.hpp"       // for Make, Renderer
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/component/mouse.hpp"           // for Mouse
#include "ftxui/dom/elements.hpp"  // for Element, operator|, reflect
#include "ftxui/screen/box.hpp"    // for Box

namespace ftxui {

/// @brief |render| を使ってインターフェースを描画するコンポーネントを返します。
/// @param render インターフェースを描画する関数。
/// @ingroup component
///
/// ### Example
///
/// ```cpp
/// auto screen = App::TerminalOutput();
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

/// @brief |child| に似た、|render| をComponent::Render()イベントとして
/// 使用する新しいComponentを返します。
/// @param child イベントを転送するコンポーネント。
/// @param render インターフェースを描画する関数。
/// @ingroup component
///
/// ### Example
///
/// ```cpp
/// auto screen = App::TerminalOutput();
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

/// @brief |render| を使ってインターフェースを描画するフォーカス可能な
/// コンポーネントを返します。
/// @param render インターフェースを描画する関数で、コンポーネントが
/// フォーカスされているかどうかを示すブール値を受け取ります。
/// @ingroup component
///
/// ### Example
///
/// ```cpp
/// auto screen = App::TerminalOutput();
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

/// @brief コンポーネントが描画するものを装飾することで、コンポーネントを
/// 装飾します。
/// @param decorator コンポーネントが描画する要素を変更する関数。
/// @ingroup component
///
/// ### Example
///
/// ```cpp
/// auto screen = App::TerminalOutput();
/// auto renderer =
///   Renderer([] { return text("Hello"); })
///   | Renderer(bold)
///   | Renderer(inverted);
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
