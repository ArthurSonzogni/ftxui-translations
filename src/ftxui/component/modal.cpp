// 版权所有 2022 Arthur Sonzogni. 保留所有权利。
// 此源代码的使用受 MIT 许可证的约束，您可以在
// LICENSE 文件中找到该许可证。
#include <ftxui/component/event.hpp>  // for Event
#include <ftxui/dom/elements.hpp>  // for operator|, Element, center, clear_under, dbox
#include <memory>                  // for __shared_ptr_access, shared_ptr
#include <utility>                 // for move

#include "ftxui/component/component.hpp"  // for Make, Tab, ComponentDecorator, Modal
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase

namespace ftxui {

// 在 |main| 组件顶部添加一个 |modal| 窗口。当 |show_modal| 为 true 时，它将显示在
// 其他组件的顶部。
/// @ingroup component
// NOLINTNEXTLINE
Component Modal(Component main, Component modal, const bool* show_modal) {
  class Impl : public ComponentBase {
   public:
    explicit Impl(Component main, Component modal, const bool* show_modal)
        : main_(std::move(main)),
          modal_(std::move(modal)),
          show_modal_(show_modal) {
      Add(Container::Tab({main_, modal_}, &selector_));
    }

   private:
    Element OnRender() override {
      selector_ = *show_modal_;
      auto document = main_->Render();
      if (*show_modal_) {
        document = dbox({
            document,
            modal_->Render() | clear_under | center,
        });
      }
      return document;
    }

    bool OnEvent(Event event) override {
      selector_ = *show_modal_;
      return ComponentBase::OnEvent(event);
    }

    Component main_;
    Component modal_;
    const bool* show_modal_;
    int selector_ = *show_modal_;
  };
  return Make<Impl>(main, modal, show_modal);
}

// 装饰一个组件。在其顶部添加一个 |modal| 窗口。当 |show_modal| 为 true 时，它将显示在
// 其他组件的顶部。
/// @ingroup component
// NOLINTNEXTLINE
ComponentDecorator Modal(Component modal, const bool* show_modal) {
  return [modal, show_modal](Component main) {
    return Modal(std::move(main), modal, show_modal);
  };
}

}  // namespace ftxui
