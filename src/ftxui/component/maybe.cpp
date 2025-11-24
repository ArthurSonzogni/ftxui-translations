// Copyright 2021 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに準拠しています。
#include <functional>  // for function
#include <memory>  // for make_unique, __shared_ptr_access, __shared_ptr_access<>::element_type, shared_ptr
#include <utility>  // for move

#include "ftxui/component/component.hpp"  // for ComponentDecorator, Maybe, Make
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/dom/elements.hpp"              // for Element
#include "ftxui/dom/node.hpp"                  // for Node

namespace ftxui {

/// @brief コンポーネント|child|を装飾します。|show|がtrueを返す場合にのみ表示されます。
/// @param child 装飾するコンポーネント。
/// @param show |child|を表示するかどうかを返す関数。
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

/// @brief コンポーネントを装飾します。|show|関数がtrueを返す場合にのみ表示されます。
/// @param show 装飾されたコンポーネントを表示するかどうかを返す関数。
/// @ingroup component
///
/// ### 例
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

/// @brief コンポーネント|child|を装飾します。|show|がtrueの場合にのみ表示されます。
/// @param child 装飾するコンポーネント。
/// @param show 論理値。|show|がtrueの場合に|child|が表示されます。
/// @ingroup component
///
/// ### 例
///
/// ```cpp
/// auto component = Renderer([]{ return text("Hello World!"); });
/// auto maybe_component = Maybe(component, &show);
/// ```
Component Maybe(Component child, const bool* show) {
  return Maybe(std::move(child), [show] { return *show; });
}

/// @brief コンポーネントを装飾します。|show|がtrueの場合にのみ表示されます。
/// @param show 論理値。|show|がtrueの場合に|child|が表示されます。
/// @ingroup component
///
/// ### 例
///
/// ```cpp
/// auto component = Renderer([]{ return text("Hello World!"); });
/// auto maybe_component = component | Maybe(&show);
/// ```
ComponentDecorator Maybe(const bool* show) {
  return [show](Component child) { return Maybe(std::move(child), show); };
}

}  // namespace ftxui