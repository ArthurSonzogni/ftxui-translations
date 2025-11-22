// 版权所有 2021 Arthur Sonzogni. 保留所有权利。
// 本源代码的使用受 MIT 许可证的约束，该许可证可在以下文件中找到：
// LICENSE 文件。
#include <functional>  // for function
#include <utility>     // for move

#include "ftxui/component/component.hpp"       // for Make, Renderer
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/component/mouse.hpp"           // for Mouse
#include "ftxui/dom/elements.hpp"  // for Element, operator|, reflect
#include "ftxui/screen/box.hpp"    // for Box

namespace ftxui {

/// @brief 返回一个组件，使用 |render| 来渲染其界面。
/// @param render 绘制界面的函数。
/// @ingroup component
///
/// ### 示例
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

/// @brief 返回一个新组件，类似于 |child|，但使用 |render| 作为
/// Component::Render() 事件。
/// @param child 用于转发事件的组件。
/// @param render 绘制界面的函数。
/// @ingroup component
///
/// ### 示例
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

/// @brief 返回一个可聚焦的组件，使用 |render| 来渲染其界面。
/// @param render 绘制界面的函数，接受一个布尔值，指示
/// 该组件是否被聚焦。
/// @ingroup component
///
/// ### 示例
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

/// @brief 装饰一个组件，通过装饰其渲染内容。
/// @param decorator 修改其渲染元素的函数。
/// @ingroup component
///
/// ### 示例
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
