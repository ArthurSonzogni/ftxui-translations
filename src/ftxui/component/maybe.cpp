// 版权所有 2021 Arthur Sonzogni. 保留所有权利。
// 此源代码的使用受 MIT 许可证的约束，该许可证可在
// LICENSE 文件中找到。
#include <functional>  // for function
#include <memory>  // for make_unique, __shared_ptr_access, __shared_ptr_access<>::element_type, shared_ptr
#include <utility>  // for move

#include "ftxui/component/component.hpp"  // for ComponentDecorator, Maybe, Make
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/dom/elements.hpp"              // for Element
#include "ftxui/dom/node.hpp"                  // for Node

namespace ftxui {

/// @brief 装饰一个组件 |child|。它仅在 |show| 返回 true 时显示。
/// true。
/// @param child 要装饰的组件。
/// @param show 一个函数，返回 |child| 是否应该显示。
/// @ingroup component
Component Maybe(Component child, std::function<bool()> show) {
  class Impl : public ComponentBase {
   public:
    explicit Impl(std::function<bool()> show) : show_(std::move(show)) {}

   private:
    Element OnRender() override {
      return show_() ? ComponentBase::OnRender() : std::make_unique<Node>();
    }
    bool Focusable() const override {
      return show_() && ComponentBase::Focusable();
    }
    bool OnEvent(Event event) override {
      return show_() && ComponentBase::OnEvent(event);
    }

    std::function<bool()> show_;
  };

  auto maybe = Make<Impl>(std::move(show));
  maybe->Add(std::move(child));
  return maybe;
}

/// @brief 装饰一个组件。它仅在 |show| 函数返回 true 时显示。
/// 返回 true。
/// @param show 一个函数，返回被装饰的组件是否应该显示。
/// 显示。
/// @ingroup component
///
/// ### Example
///
/// ```cpp
/// auto component = Renderer([]{ return text("Hello World!"); });
/// auto maybe_component = component | Maybe([&]{ return counter == 42; });
/// ```
ComponentDecorator Maybe(std::function<bool()> show) {
  return [show = std::move(show)](Component child) mutable {
    return Maybe(std::move(child), std::move(show));
  };
}

/// @brief 装饰一个组件 |child|。它仅在 |show| 为 true 时显示。
/// @param child 要装饰的组件。
/// @param show 一个布尔值。当 |show| 为 true 时，|child| 显示。
/// @ingroup component
///
/// ### Example
///
/// ```cpp
/// auto component = Renderer([]{ return text("Hello World!"); });
/// auto maybe_component = Maybe(component, &show);
/// ```
Component Maybe(Component child, const bool* show) {
  return Maybe(std::move(child), [show] { return *show; });
}

/// @brief 装饰一个组件。它仅在 |show| 为 true 时显示。
/// @param show 一个布尔值。当 |show| 为 true 时，|child| 显示。
/// @ingroup component
///
/// ### Example
///
/// ```cpp
/// auto component = Renderer([]{ return text("Hello World!"); });
/// auto maybe_component = component | Maybe(&show);
/// ```
ComponentDecorator Maybe(const bool* show) {
  return [show](Component child) { return Maybe(std::move(child), show); };
}

}  // namespace ftxui
