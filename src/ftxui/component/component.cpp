// Copyright 2020 Arthur Sonzogni. 版权所有。
// 本源代码的使用受 MIT 许可证的约束，该许可证可在
// LICENSE 文件中找到。
#include <algorithm>  // for find_if
#include <cassert>    // for assert
#include <cstddef>    // for size_t
#include <iterator>   // for begin, end
#include <memory>     // for unique_ptr, make_unique
#include <utility>    // for move
#include <vector>     // for vector, __alloc_traits<>::value_type

#include "ftxui/component/captured_mouse.hpp"  // for CapturedMouse, CapturedMouseInterface
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"  // for ComponentBase, Components
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"                  // for text, Element
#include "ftxui/dom/node.hpp"                      // for Node, Elements
#include "ftxui/screen/box.hpp"                    // for Box

namespace ftxui::animation {
class Params;
}  // namespace ftxui::animation

namespace ftxui {

namespace {
class CaptureMouseImpl : public CapturedMouseInterface {};
}  // namespace

ComponentBase::~ComponentBase() {
  DetachAllChildren();
}

/// @brief 返回父 ComponentBase，如果没有则返回 null。
/// @see Detach
/// @see Parent
ComponentBase* ComponentBase::Parent() const {
  return parent_;
}

/// @brief 访问索引 `i` 处的子项。
Component& ComponentBase::ChildAt(size_t i) {
  assert(i < ChildCount());  // NOLINT
  return children_[i];
}

/// @brief 返回子项的数量。
size_t ComponentBase::ChildCount() const {
  return children_.size();
}

/// @brief 返回组件在其父级中的索引。如果没有父级，则返回 -1。
int ComponentBase::Index() const {
  if (parent_ == nullptr) {
    return -1;
  }
  int index = 0;
  for (const Component& child : parent_->children_) {
    if (child.get() == this) {
      return index;
    }
    index++;
  }
  return -1;  // Not reached.
}

/// @brief 添加一个子项。
/// @@param child 要附加的子项。
void ComponentBase::Add(Component child) {
  child->Detach();
  child->parent_ = this;
  children_.push_back(std::move(child));
}

/// @brief 将此子项从其父级分离。
/// @see Detach
/// @see Parent
void ComponentBase::Detach() {
  if (parent_ == nullptr) {
    return;
  }
  auto it = std::find_if(std::begin(parent_->children_),  //
                         std::end(parent_->children_),    //
                         [this](const Component& that) {  //
                           return this == that.get();
                         });
  ComponentBase* parent = parent_;
  parent_ = nullptr;
  parent->children_.erase(it);  // Might delete |this|.
}

/// @brief 移除所有子项。
void ComponentBase::DetachAllChildren() {
  while (!children_.empty()) {
    children_[0]->Detach();
  }
}

/// @brief 绘制组件。
/// 构建一个 ftxui::Element，用于在表示此 ftxui::ComponentBase 的 ftxui::Screen 上绘制。
/// 请覆盖 OnRender() 以修改渲染。
Element ComponentBase::Render() {
// 某些用户可能会从 `T::OnRender()` 调用 `ComponentBase::Render()`。
// 为避免无限递归，我们使用一个标志。
  if (in_render) {
    return ComponentBase::OnRender();
  }

  in_render = true;
  Element element = OnRender();
  in_render = false;

  class Wrapper : public Node {
   public:
    bool active_ = false;

    Wrapper(Element child, bool active)
        : Node({std::move(child)}), active_(active) {}

    void SetBox(Box box) override {
      Node::SetBox(box);
      children_[0]->SetBox(box);
    }

    void ComputeRequirement() override {
      Node::ComputeRequirement();
      requirement_.focused.component_active = active_;
    }
  };

  return std::make_shared<Wrapper>(std::move(element), Active());
}

/// @brief 绘制组件。
/// 构建一个 ftxui::Element，用于在表示此 ftxui::ComponentBase 的 ftxui::Screen 上绘制。
/// 此函数旨在被覆盖。
Element ComponentBase::OnRender() {
  if (children_.size() == 1) {
    return children_.front()->Render();
  }

  return text("Not implemented component");
}

/// @brief 响应事件时调用。
/// @param event 事件。
/// @return 当事件已处理时返回 true。
/// 默认实现会在每个子项上调用 OnEvent，直到有一个返回 true。
/// 如果都没有返回 true，则返回 false。
bool ComponentBase::OnEvent(Event event) {  // NOLINT
  for (Component& child : children_) {      // NOLINT
    if (child->OnEvent(event)) {
      return true;
    }
  }
  return false;
}

/// @brief 响应动画事件时调用。
/// @param params 动画的参数
/// 默认实现将事件分派给每个子项。
void ComponentBase::OnAnimation(animation::Params& params) {
  for (const Component& child : children_) {
    child->OnAnimation(params);
  }
}

/// @brief 返回当前活动的子项。
/// @return 当前活动的子项。
Component ComponentBase::ActiveChild() {
  for (auto& child : children_) {
    if (child->Focusable()) {
      return child;
    }
  }
  return nullptr;
}

/// @brief 当组件包含可聚焦元素时返回 true。
/// 使用键盘导航时，不可聚焦的组件将被跳过。
bool ComponentBase::Focusable() const {
  for (const Component& child : children_) {  // NOLINT
    if (child->Focusable()) {
      return true;
    }
  }
  return false;
}

/// @brief 返回该元素是否是其父级的当前活动子项。
bool ComponentBase::Active() const {
  return parent_ == nullptr || parent_->ActiveChild().get() == this;
}

/// @brief 返回元素是否被用户聚焦。
/// 当 ComponentBase 被用户聚焦时返回 true。当一个元素及其所有祖先都是其父级的 ActiveChild() 并且它是 Focusable() 时，该元素被聚焦。
bool ComponentBase::Focused() const {
  const auto* current = this;
  while (current && current->Active()) {
    current = current->parent_;
  }
  return !current && Focusable();
}

/// @brief 使 |child| 成为“活动”子项。
/// @param child 将成为活动状态的子项。
void ComponentBase::SetActiveChild([[maybe_unused]] ComponentBase* child) {}

/// @brief 使 |child| 成为“活动”子项。
/// @param child 将成为活动状态的子项。
void ComponentBase::SetActiveChild(Component child) {  // NOLINT
  SetActiveChild(child.get());
}

/// @brief 配置所有祖先以将焦点赋予此组件。
void ComponentBase::TakeFocus() {
  ComponentBase* child = this;
  while (ComponentBase* parent = child->parent_) {
    parent->SetActiveChild(child);
    child = parent;
  }
}

/// @brief 如果可用，则捕获 CapturedMouse。只有一个组件可以捕获它。
/// 它表示一个优先于其他组件的组件。
/// @param event 事件
CapturedMouse ComponentBase::CaptureMouse(const Event& event) {  // NOLINT
  if (event.screen_) {
    return event.screen_->CaptureMouse();
  }
  return std::make_unique<CaptureMouseImpl>();
}

}  // namespace ftxui
