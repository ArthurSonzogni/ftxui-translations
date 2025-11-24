// Copyright 2021 Arthur Sonzogni. All rights reserved.
// 此原始碼的使用受 MIT 授權條款約束，該條款可在
// LICENSE 檔案中找到。
#include <functional>  // for function
#include <memory>  // for make_unique, __shared_ptr_access, __shared_ptr_access<>::element_type, shared_ptr
#include <utility>  // for move

#include "ftxui/component/component.hpp"  // for ComponentDecorator, Maybe, Make
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/dom/elements.hpp"              // for Element
#include "ftxui/dom/node.hpp"                  // for Node

namespace ftxui {

/// @brief 裝飾一個組件 |child|。它只在 |show| 返回 true 時顯示。
/// @param child 要裝飾的組件。
/// @param show 一個函數，返回是否應顯示 |child|。
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

/// @brief 裝飾一個組件。它只在 |show| 函數返回 true 時顯示。
/// @param show 一個函數，返回是否應顯示被裝飾的組件。
/// @ingroup component
///
/// ### 範例
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

/// @brief 裝飾一個組件 |child|。它只在 |show| 為 true 時顯示。
/// @param child 要裝飾的組件。
/// @param show 一個布林值。當 |show| 為 true 時，顯示 |child|。
/// @ingroup component
///
/// ### 範例
///
/// ```cpp
/// auto component = Renderer([]{ return text("Hello World!"); });
/// auto maybe_component = Maybe(component, &show);
/// ```
Component Maybe(Component child, const bool* show) {
  return Maybe(std::move(child), [show] { return *show; });
}

/// @brief 裝飾一個組件。它只在 |show| 為 true 時顯示。
/// @param show 一個布林值。當 |show| 為 true 時，顯示 |child|。
/// @ingroup component
///
/// ### 範例
///
/// ```cpp
/// auto component = Renderer([]{ return text("Hello World!"); });
/// auto maybe_component = component | Maybe(&show);
/// ```
ComponentDecorator Maybe(const bool* show) {
  return [show](Component child) { return Maybe(std::move(child), show); };
}

}  // namespace ftxui