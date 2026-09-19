// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>  // for find_if
#include <cassert>    // for assert
#include <cstddef>    // for size_t
#include <iterator>   // for begin, end
#include <memory>     // for unique_ptr, make_unique
#include <utility>    // for move
#include <vector>     // for vector, __alloc_traits<>::value_type

#include "ftxui/component/app.hpp"             // for Component, App
#include "ftxui/component/captured_mouse.hpp"  // for CapturedMouse, CapturedMouseInterface
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"  // for ComponentBase, Components
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/dom/elements.hpp"              // for text, Element
#include "ftxui/dom/node.hpp"                  // for Node, Elements
#include "ftxui/screen/box.hpp"                // for Box

namespace ftxui::animation {
class Params;
}  // namespace ftxui::animation

namespace ftxui {

namespace {
class CaptureMouseImpl : public CapturedMouseInterface {};
}  // namespace

struct ComponentBase::Impl {
  Components children;
  ComponentBase* parent = nullptr;
  bool in_render = false;
};

ComponentBase::ComponentBase() : impl_(std::make_unique<Impl>()) {}

ComponentBase::ComponentBase(Components children)
    : impl_(std::make_unique<Impl>()) {
  impl_->children = std::move(children);
}

ComponentBase::~ComponentBase() {
  DetachAllChildren();
}

Components& ComponentBase::children() {
  return impl_->children;
}

const Components& ComponentBase::children() const {
  return impl_->children;
}

/// @brief 返回父 ComponentBase，如果沒有則返回空。
/// @see Detach
/// @see Parent
ComponentBase* ComponentBase::Parent() const {
  return impl_->parent;
}

/// @brief 存取索引 `i` 處的子項。
Component& ComponentBase::ChildAt(size_t i) {
  assert(i < ChildCount());  // NOLINT
  return impl_->children[i];
}

/// @brief 返回子項的數量。
size_t ComponentBase::ChildCount() const {
  return impl_->children.size();
}

/// @brief 返回組件在其父項中的索引。如果沒有父項則返回 -1。
int ComponentBase::Index() const {
  if (impl_->parent == nullptr) {
    return -1;
  }
  int index = 0;
  for (const Component& child : impl_->parent->impl_->children) {
    if (child.get() == this) {
      return index;
    }
    index++;
  }
  return -1;  // 不會執行到此處。
}

/// @brief 新增一個子項目。
/// @param child 要附加的子項目。
void ComponentBase::Add(Component child) {
  child->Detach();
  child->impl_->parent = this;
  impl_->children.push_back(std::move(child));
}

/// @brief 將此子項從其父項中分離。
/// @see Detach
/// @see Parent
void ComponentBase::Detach() {
  if (impl_->parent == nullptr) {
    return;
  }
  auto it = std::find_if(std::begin(impl_->parent->impl_->children),  // NOLINT
                         std::end(impl_->parent->impl_->children),    //
                         [this](const Component& that) {              //
                           return this == that.get();
                         });
  ComponentBase* parent = impl_->parent;
  impl_->parent = nullptr;
  parent->impl_->children.erase(it);  // 可能會刪除 |this|。
}

/// @brief 移除所有子項。
void ComponentBase::DetachAllChildren() {
  while (!impl_->children.empty()) {
    impl_->children[0]->Detach();
  }
}

/// @brief 繪製組件。
/// 建構一個 ftxui::Element，用於在表示此 ftxui::ComponentBase 的 ftxui::Screen 上繪製。請覆寫 OnRender() 以修改渲染。
Element ComponentBase::Render() {
  // 有些使用者可能會在 `T::OnRender()` 中呼叫
  // `ComponentBase::Render()`。為了避免無限遞迴，我們使用一個旗標。
  if (impl_->in_render) {
    return ComponentBase::OnRender();
  }

  impl_->in_render = true;
  Element element = OnRender();
  impl_->in_render = false;

  class Wrapper : public Node {
   public:
    bool active_ = false;
    bool focused_ = false;

    Wrapper(Element child, bool active, bool focused)
        : Node({std::move(child)}), active_(active), focused_(focused) {}

    void SetBox(Box box) override {
      Node::SetBox(box);
      children_[0]->SetBox(box);
    }

    void ComputeRequirement() override {
      Node::ComputeRequirement();
      requirement_.focused.component_active = active_;
      requirement_.focused.component_focused = focused_;
    }
  };

  return std::make_shared<Wrapper>(std::move(element), Active(), Focused());
}

/// @brief 繪製組件。
/// 建構一個 ftxui::Element，用於在表示此 ftxui::ComponentBase 的 ftxi::Screen 上繪製。此函數旨在被覆寫。
Element ComponentBase::OnRender() {
  if (impl_->children.size() == 1) {
    return impl_->children.front()->Render();
  }

  return text("Not implemented component");
}

/// @brief 回應事件時呼叫。
/// @param event 事件。
/// @return 當事件已處理時返回 true。
/// 預設實作會在每個子項上呼叫 OnEvent，直到其中一個返回 true。如果都沒有返回 true，則返回 false。
bool ComponentBase::OnEvent(Event event) {    // NOLINT
  for (Component& child : impl_->children) {  // NOLINT
    if (child->OnEvent(event)) {
      return true;
    }
  }
  return false;
}

/// @brief 回應動畫事件時呼叫。
/// @param params 動畫的參數
/// 預設實作會將事件分派給每個子項。
void ComponentBase::OnAnimation(animation::Params& params) {
  for (const Component& child : impl_->children) {
    child->OnAnimation(params);
  }
}

/// @brief 返回目前活躍的子項。
/// @return 目前活躍的子項。
Component ComponentBase::ActiveChild() {
  for (auto& child : impl_->children) {
    if (child->Focusable()) {
      return child;
    }
  }
  return nullptr;
}

/// @brief 當組件包含可聚焦元素時返回 true。
/// 使用鍵盤導航時，不可聚焦的組件將被跳過。
bool ComponentBase::Focusable() const {
  for (const Component& child : impl_->children) {  // NOLINT
    if (child->Focusable()) {
      return true;
    }
  }
  return false;
}

/// @brief 返回該元素是否為其父項的目前活躍子項。
bool ComponentBase::Active() const {
  return impl_->parent == nullptr || impl_->parent->ActiveChild().get() == this;
}

/// @brief 返回元素是否被使用者聚焦。
/// 當 ComponentBase 被使用者聚焦時返回 true。當一個元素及其所有祖先都是其父項的 ActiveChild() 並且它是 Focusable() 時，該元素被聚焦。
bool ComponentBase::Focused() const {
  const auto* current = this;
  while (current && current->Active()) {
    current = current->impl_->parent;
  }
  return !current && Focusable();
}

/// @brief 使 |child| 成為「活躍」的子項。
/// @param child 要變為活躍的子項。
void ComponentBase::SetActiveChild([[maybe_unused]] ComponentBase* child) {}

/// @brief 使 |child| 成為「活躍」的子項。
/// @param child 要變為活躍的子項。
void ComponentBase::SetActiveChild(Component child) {  // NOLINT
  SetActiveChild(child.get());
}

/// @brief 配置所有祖先以將焦點給予此組件。
void ComponentBase::TakeFocus() {
  ComponentBase* child = this;
  while (ComponentBase* parent = child->impl_->parent) {
    parent->SetActiveChild(child);
    child = parent;
  }
}

/// @brief 如果可用，則取得 CapturedMouse。它們只有一個組件。它代表一個優先於其他組件的組件。
/// @param event 事件
CapturedMouse ComponentBase::CaptureMouse(const Event& event) {  // NOLINT
  if (event.screen_) {
    return event.screen_->CaptureMouse();
  }
  return std::make_unique<CaptureMouseImpl>();
}

void ComponentBase::Reserved1() {}
void ComponentBase::Reserved2() {}
void ComponentBase::Reserved3() {}
void ComponentBase::Reserved4() {}
void ComponentBase::Reserved5() {}
void ComponentBase::Reserved6() {}
void ComponentBase::Reserved7() {}
void ComponentBase::Reserved8() {}

}  // namespace ftxui
