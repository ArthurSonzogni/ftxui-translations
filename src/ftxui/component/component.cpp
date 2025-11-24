// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに準拠しています。
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

/// @brief 親のComponentBaseを返します。存在しない場合はnullを返します。
/// @see Detach
/// @see Parent
ComponentBase* ComponentBase::Parent() const {
  return parent_;
}

/// @brief インデックス`i`の子にアクセスします。
Component& ComponentBase::ChildAt(size_t i) {
  assert(i < ChildCount());  // NOLINT
  return children_[i];
}

/// @brief 子の数を返します。
size_t ComponentBase::ChildCount() const {
  return children_.size();
}

/// @brief 親におけるコンポーネントのインデックスを返します。親がない場合は-1を返します。
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

/// @brief 子を追加します。
/// @@param child 添付する子。
void ComponentBase::Add(Component child) {
  child->Detach();
  child->parent_ = this;
  children_.push_back(std::move(child));
}

/// @brief この子を親からデタッチします。
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

/// @brief すべての子を削除します。
void ComponentBase::DetachAllChildren() {
  while (!children_.empty()) {
    children_[0]->Detach();
  }
}

/// @brief コンポーネントを描画します。
/// このftxui::ComponentBaseを表すftxui::Screen上に描画されるftxui::Elementを構築します。レンダリングを変更するにはOnRender()をオーバーライドしてください。
Element ComponentBase::Render() {
  // Some users might call `ComponentBase::Render()` from
  // `T::OnRender()`. To avoid infinite recursion, we use a flag.
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

/// @brief コンポーネントを描画します。
/// このftxui::ComponentBaseを表すftxui::Screen上に描画されるftxui::Elementを構築します。この関数はオーバーライドされることを意図しています。
Element ComponentBase::OnRender() {
  if (children_.size() == 1) {
    return children_.front()->Render();
  }

  return text("Not implemented component");
}

/// @brief イベントに応じて呼び出されます。
/// @param event イベント。
/// @return イベントが処理された場合はtrue。
/// デフォルトの実装では、いずれかの子がtrueを返すまで、すべての子でOnEventを呼び出します。どれもtrueを返さない場合は、falseを返します。
bool ComponentBase::OnEvent(Event event) {  // NOLINT
  for (Component& child : children_) {      // NOLINT
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
  for (const Component& child : children_) {
    child->OnAnimation(params);
  }
}

/// @brief 現在アクティブな子を返します。
/// @return 現在アクティブな子。
Component ComponentBase::ActiveChild() {
  for (auto& child : children_) {
    if (child->Focusable()) {
      return child;
    }
  }
  return nullptr;
}

/// @brief コンポーネントがフォーカス可能な要素を含んでいる場合にtrueを返します。
/// フォーカス不可能なコンポーネントは、キーボードでナビゲートする際にスキップされます。
bool ComponentBase::Focusable() const {
  for (const Component& child : children_) {  // NOLINT
    if (child->Focusable()) {
      return true;
    }
  }
  return false;
}

/// @brief 要素が現在親のアクティブな子であるかどうかを返します。
bool ComponentBase::Active() const {
  return parent_ == nullptr || parent_->ActiveChild().get() == this;
}

/// @brief 要素がユーザーによってフォーカスされているかどうかを返します。
/// ComponentBaseがユーザーによってフォーカスされている場合にtrueを返します。要素は、そのすべての子孫が親のActiveChild()であり、かつFocusable()である場合にフォーカスされます。
bool ComponentBase::Focused() const {
  const auto* current = this;
  while (current && current->Active()) {
    current = current->parent_;
  }
  return !current && Focusable();
}

/// @brief |child|を「アクティブ」にします。
/// @param child アクティブにする子。

/// @brief |child|を「アクティブ」にします。
/// @param child アクティブにする子。
void ComponentBase::SetActiveChild(Component child) {  // NOLINT
  SetActiveChild(child.get());
}

/// @brief このコンポーネントにフォーカスを与えるように、すべての祖先を設定します。
void ComponentBase::TakeFocus() {
  ComponentBase* child = this;
  while (ComponentBase* parent = child->parent_) {
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

}  // namespace ftxui
