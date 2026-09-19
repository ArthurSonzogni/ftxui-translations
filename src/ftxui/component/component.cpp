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

/// @brief 親のComponentBaseを返します。存在しない場合はnullを返します。
/// @see Detach
/// @see Parent
ComponentBase* ComponentBase::Parent() const {
  return impl_->parent;
}

/// @brief インデックス`i`の子にアクセスします。
Component& ComponentBase::ChildAt(size_t i) {
  assert(i < ChildCount());  // NOLINT
  return impl_->children[i];
}

/// @brief 子の数を返します。
size_t ComponentBase::ChildCount() const {
  return impl_->children.size();
}

/// @brief 親におけるコンポーネントのインデックスを返します。親がない場合は-1を返します。
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
  return -1;  // Not reached.
}

/// @brief Add a child.
/// @param child The child to be attached.
void ComponentBase::Add(Component child) {
  child->Detach();
  child->impl_->parent = this;
  impl_->children.push_back(std::move(child));
}

/// @brief この子を親からデタッチします。
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
  parent->impl_->children.erase(it);  // Might delete |this|.
}

/// @brief すべての子を削除します。
void ComponentBase::DetachAllChildren() {
  while (!impl_->children.empty()) {
    impl_->children[0]->Detach();
  }
}

/// @brief コンポーネントを描画します。
/// このftxui::ComponentBaseを表すftxui::Screen上に描画されるftxui::Elementを構築します。レンダリングを変更するにはOnRender()をオーバーライドしてください。
Element ComponentBase::Render() {
  // Some users might call `ComponentBase::Render()` from
  // `T::OnRender()`. To avoid infinite recursion, we use a flag.
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

/// @brief コンポーネントを描画します。
/// このftxui::ComponentBaseを表すftxui::Screen上に描画されるftxui::Elementを構築します。この関数はオーバーライドされることを意図しています。
Element ComponentBase::OnRender() {
  if (impl_->children.size() == 1) {
    return impl_->children.front()->Render();
  }

  return text("Not implemented component");
}

/// @brief イベントに応じて呼び出されます。
/// @param event イベント。
/// @return イベントが処理された場合はtrue。
/// デフォルトの実装では、いずれかの子がtrueを返すまで、すべての子でOnEventを呼び出します。どれもtrueを返さない場合は、falseを返します。
bool ComponentBase::OnEvent(Event event) {    // NOLINT
  for (Component& child : impl_->children) {  // NOLINT
    if (child->OnEvent(event)) {
      return true;
    }
  }
  return false;
}

/// @brief アニメーションイベントに応じて呼び出されます。
/// @param params アニメーションのパラメータ
/// デフォルトの実装では、イベントをすべての子にディスパッチします。
void ComponentBase::OnAnimation(animation::Params& params) {
  for (const Component& child : impl_->children) {
    child->OnAnimation(params);
  }
}

/// @brief 現在アクティブな子を返します。
/// @return 現在アクティブな子。
Component ComponentBase::ActiveChild() {
  for (auto& child : impl_->children) {
    if (child->Focusable()) {
      return child;
    }
  }
  return nullptr;
}

/// @brief コンポーネントがフォーカス可能な要素を含んでいる場合にtrueを返します。
/// フォーカス不可能なコンポーネントは、キーボードでナビゲートする際にスキップされます。
bool ComponentBase::Focusable() const {
  for (const Component& child : impl_->children) {  // NOLINT
    if (child->Focusable()) {
      return true;
    }
  }
  return false;
}

/// @brief 要素が現在親のアクティブな子であるかどうかを返します。
bool ComponentBase::Active() const {
  return impl_->parent == nullptr || impl_->parent->ActiveChild().get() == this;
}

/// @brief 要素がユーザーによってフォーカスされているかどうかを返します。
/// ComponentBaseがユーザーによってフォーカスされている場合にtrueを返します。要素は、そのすべての子孫が親のActiveChild()であり、かつFocusable()である場合にフォーカスされます。
bool ComponentBase::Focused() const {
  const auto* current = this;
  while (current && current->Active()) {
    current = current->impl_->parent;
  }
  return !current && Focusable();
}

/// @brief |child|を「アクティブ」にします。
/// @param child アクティブにする子。
void ComponentBase::SetActiveChild([[maybe_unused]] ComponentBase* child) {}

/// @brief |child|を「アクティブ」にします。
/// @param child アクティブにする子。
void ComponentBase::SetActiveChild(Component child) {  // NOLINT
  SetActiveChild(child.get());
}

/// @brief このコンポーネントにフォーカスを与えるように、すべての祖先を設定します。
void ComponentBase::TakeFocus() {
  ComponentBase* child = this;
  while (ComponentBase* parent = child->impl_->parent) {
    parent->SetActiveChild(child);
    child = parent;
  }
}

/// @brief 利用可能であればCapturedMouseを取得します。コンポーネントは1つしかありません。これは他のコンポーネントよりも優先されるコンポーネントを表します。
/// @param event イベント
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
