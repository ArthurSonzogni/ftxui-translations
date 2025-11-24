// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスによって管理されています。
#include <algorithm>  // for max, min
#include <cstddef>    // for size_t
#include <memory>  // for make_shared, __shared_ptr_access, allocator, shared_ptr, allocator_traits<>::value_type
#include <utility>  // for move

#include "ftxui/component/component.hpp"  // for Horizontal, Vertical, Tab
#include "ftxui/component/component_base.hpp"  // for Components, Component, ComponentBase
#include "ftxui/component/event.hpp"  // for Event, Event::Tab, Event::TabReverse, Event::ArrowDown, Event::ArrowLeft, Event::ArrowRight, Event::ArrowUp, Event::End, Event::Home, Event::PageDown, Event::PageUp
#include "ftxui/component/mouse.hpp"  // for Mouse, Mouse::WheelDown, Mouse::WheelUp
#include "ftxui/dom/elements.hpp"  // for text, Elements, operator|, reflect, Element, hbox, vbox
#include "ftxui/screen/box.hpp"  // for Box

namespace ftxui {

class ContainerBase : public ComponentBase {
 public:
  ContainerBase(Components children, int* selector)
      : selector_(selector ? selector : &selected_) {
    for (Component& child : children) {
      Add(std::move(child));
    }
  }

  // Component override.
  bool OnEvent(Event event) override {
    if (event.is_mouse()) {
      return OnMouseEvent(event);
    }

    if (!Focused()) {
      return false;
    }

    if (ActiveChild() && ActiveChild()->OnEvent(event)) {
      return true;
    }

    return EventHandler(event);
  }

  Component ActiveChild() override {
    if (children_.empty()) {
      return nullptr;
    }

    return children_[static_cast<size_t>(*selector_) % children_.size()];
  }

  void SetActiveChild(ComponentBase* child) override {
    for (size_t i = 0; i < children_.size(); ++i) {
      if (children_[i].get() == child) {
        *selector_ = static_cast<int>(i);
        return;
      }
    }
  }

 protected:
  // ハンドラ
  virtual bool EventHandler(Event /*unused*/) { return false; }  // NOLINT

  virtual bool OnMouseEvent(Event event) {
    return ComponentBase::OnEvent(std::move(event));
  }

  int selected_ = 0;
  int* selector_ = nullptr;

  void MoveSelector(int dir) {
    for (int i = *selector_ + dir; i >= 0 && i < int(children_.size());
         i += dir) {
      if (children_[i]->Focusable()) {
        *selector_ = i;
        return;
      }
    }
  }

  void MoveSelectorWrap(int dir) {
    if (children_.empty()) {
      return;
    }
    for (size_t offset = 1; offset < children_.size(); ++offset) {
      const size_t i =
          (*selector_ + offset * dir + children_.size()) % children_.size();
      if (children_[i]->Focusable()) {
        *selector_ = int(i);
        return;
      }
    }
  }
};

class VerticalContainer : public ContainerBase {
 public:
  using ContainerBase::ContainerBase;

  Element OnRender() override {
    Elements elements;
    elements.reserve(children_.size());
    for (auto& it : children_) {
      elements.push_back(it->Render());
    }
    if (elements.empty()) {
      return text("Empty container") | reflect(box_);
    }
    return vbox(std::move(elements)) | reflect(box_);
  }

  bool EventHandler(Event event) override {
    const int old_selected = *selector_;
    if (event == Event::ArrowUp || event == Event::Character('k')) {
      MoveSelector(-1);
    }
    if (event == Event::ArrowDown || event == Event::Character('j')) {
      MoveSelector(+1);
    }
    if (event == Event::PageUp) {
      for (int i = 0; i < box_.y_max - box_.y_min; ++i) {
        MoveSelector(-1);
      }
    }
    if (event == Event::PageDown) {
      for (int i = 0; i < box_.y_max - box_.y_min; ++i) {
        MoveSelector(1);
      }
    }
    if (event == Event::Home) {
      for (size_t i = 0; i < children_.size(); ++i) {
        MoveSelector(-1);
      }
    }
    if (event == Event::End) {
      for (size_t i = 0; i < children_.size(); ++i) {
        MoveSelector(1);
      }
    }
    if (event == Event::Tab) {
      MoveSelectorWrap(+1);
    }
    if (event == Event::TabReverse) {
      MoveSelectorWrap(-1);
    }

    *selector_ = std::max(0, std::min(int(children_.size()) - 1, *selector_));
    return old_selected != *selector_;
  }

  bool OnMouseEvent(Event event) override {
    if (ContainerBase::OnMouseEvent(event)) {
      return true;
    }

    if (event.mouse().button != Mouse::WheelUp &&
        event.mouse().button != Mouse::WheelDown) {
      return false;
    }

    if (!box_.Contain(event.mouse().x, event.mouse().y)) {
      return false;
    }

    const int old_selected = *selector_;
    if (event.mouse().button == Mouse::WheelUp) {
      MoveSelector(-1);
    }
    if (event.mouse().button == Mouse::WheelDown) {
      MoveSelector(+1);
    }
    *selector_ = std::max(0, std::min(int(children_.size()) - 1, *selector_));

    return old_selected != *selector_;
  }

  Box box_;
};

class HorizontalContainer : public ContainerBase {
 public:
  using ContainerBase::ContainerBase;

  Element OnRender() override {
    Elements elements;
    elements.reserve(children_.size());
    for (auto& it : children_) {
      elements.push_back(it->Render());
    }
    if (elements.empty()) {
      return text("Empty container");
    }
    return hbox(std::move(elements));
  }

  bool EventHandler(Event event) override {
    const int old_selected = *selector_;
    if (event == Event::ArrowLeft || event == Event::Character('h')) {
      MoveSelector(-1);
    }
    if (event == Event::ArrowRight || event == Event::Character('l')) {
      MoveSelector(+1);
    }
    if (event == Event::Tab) {
      MoveSelectorWrap(+1);
    }
    if (event == Event::TabReverse) {
      MoveSelectorWrap(-1);
    }

    *selector_ = std::max(0, std::min(int(children_.size()) - 1, *selector_));
    return old_selected != *selector_;
  }
};

class TabContainer : public ContainerBase {
 public:
  using ContainerBase::ContainerBase;

  Element OnRender() override {
    const Component active_child = ActiveChild();
    if (active_child) {
      return active_child->Render();
    }
    return text("Empty container");
  }

  bool Focusable() const override {
    if (children_.empty()) {
      return false;
    }
    return children_[size_t(*selector_) % children_.size()]->Focusable();
  }

  bool OnMouseEvent(Event event) override {
    return ActiveChild() && ActiveChild()->OnEvent(event);
  }
};

class StackedContainer : public ContainerBase {
 public:
  explicit StackedContainer(Components children)
      : ContainerBase(std::move(children), nullptr) {}

 private:
  Element OnRender() final {
    Elements elements;
    for (auto& child : children_) {
      elements.push_back(child->Render());
    }
    // 要素の順序を反転させる。
    std::reverse(elements.begin(), elements.end());
    return dbox(std::move(elements));
  }

  bool Focusable() const final {
    for (const auto& child : children_) {
      if (child->Focusable()) {
        return true;
      }
    }
    return false;
  }

  Component ActiveChild() final {
    if (children_.empty()) {
      return nullptr;
    }
    return children_[0];
  }

  void SetActiveChild(ComponentBase* child) final {
    if (children_.empty()) {
      return;
    }

    // `child`を見つけて、他の子の相対的な順序を変更せずに先頭に配置します。
    auto it =
        std::find_if(children_.begin(), children_.end(),
                     [child](const Component& c) { return c.get() == child; });
    if (it == children_.end()) {
      return;
    }
    std::rotate(children_.begin(), it, it + 1);
  }

  bool OnEvent(Event event) final {
    for (auto& child : children_) {
      if (child->OnEvent(event)) {
        return true;
      }
    }
    return false;
  }
};

namespace Container {

/// @brief コンポーネントのリスト。垂直方向に1つずつ描画され、上下の矢印キーまたは'j'/'k'キーを使用して垂直方向にナビゲートされます。
/// @param children コンポーネントのリスト。
/// @ingroup component
/// @see ContainerBase
///
/// ### Example
///
/// ```cpp
/// auto container = Container::Vertical({
///   children_1,
///   children_2,
///   children_3,
///   children_4,
/// });
/// ```
Component Vertical(Components children) {
  return Vertical(std::move(children), nullptr);
}

/// @brief コンポーネントのリスト。垂直方向に1つずつ描画され、上下の矢印キーまたは'j'/'k'キーを使用して垂直方向にナビゲートされます。
/// これは例えばMenuを実装するのに便利です。
/// @param children コンポーネントのリスト。
/// @param selector 選択された子のインデックスへの参照。
/// @ingroup component
/// @see ContainerBase
/// @ingroup component
/// @see ContainerBase
///
/// ### Example
///
/// ```cpp
/// auto container = Container::Vertical({
///   children_1,
///   children_2,
///   children_3,
///   children_4,
/// }, &selected_children);
/// ```
Component Vertical(Components children, int* selector) {
  return std::make_shared<VerticalContainer>(std::move(children), selector);
}

/// @brief コンポーネントのリスト。水平方向に1つずつ描画され、左右の矢印キーまたは'h'/'l'キーを使用して水平方向にナビゲートされます。
/// @param children コンポーネントのリスト。
/// @ingroup component
/// @see ContainerBase
///
/// ### Example
///
/// ```cpp
/// int selected_children = 2;
/// auto container = Container::Horizontal({
///   children_1,
///   children_2,
///   children_3,
///   children_4,
/// });
/// ```
Component Horizontal(Components children) {
  return Horizontal(std::move(children), nullptr);
}

/// @brief コンポーネントのリスト。水平方向に1つずつ描画され、左右の矢印キーまたは'h'/'l'キーを使用して水平方向にナビゲートされます。
/// @param children コンポーネントのリスト。
/// @param selector 選択された子のインデックスへの参照。
/// @ingroup component
/// @see ContainerBase
/// @ingroup component
/// @see ContainerBase
///
/// ### Example
///
/// ```cpp
/// int selected_children = 2;
/// auto container = Container::Horizontal({
///   children_1,
///   children_2,
///   children_3,
///   children_4,
/// }, selected_children);
/// ```
Component Horizontal(Components children, int* selector) {
  return std::make_shared<HorizontalContainer>(std::move(children), selector);
}

/// @brief コンポーネントのリスト。一度に1つだけ描画され、操作されます。|selector|は選択されたコンポーネントのインデックスを提供します。これはタブを実装するのに便利です。
/// @param children コンポーネントのリスト。
/// @param selector 描画される子のインデックス。
/// @ingroup component
/// @see ContainerBase
/// @ingroup component
/// @see ContainerBase
///
/// ### Example
///
/// ```cpp
/// int tab_drawn = 0;
/// auto container = Container::Tab({
///   children_1,
///   children_2,
///   children_3,
///   children_4,
/// }, &tab_drawn);
/// ```
Component Tab(Components children, int* selector) {
  return std::make_shared<TabContainer>(std::move(children), selector);
}

/// @brief 互いの上にスタックされるコンポーネントのリスト。
/// イベントは、最初のコンポーネントに伝播され、処理されない場合は2番目のコンポーネントに伝播されます。
/// コンポーネントは与えられた順序とは逆の順序で描画されます。
/// コンポーネントがフォーカスを取得すると、他の要素の相対的な順序を変更せずに前面に配置されます。
///
/// これは`Window`コンポーネントと一緒に使用する必要があります。
///
/// @param children コンポーネントのリスト。
/// @ingroup component
/// @see Window
/// @ingroup component
/// @see Window
///
/// ### Example
///
/// ```cpp
/// auto container = Container::Stacked({
///   children_1,
///   children_2,
///   children_3,
///   children_4,
/// });
/// ```
Component Stacked(Components children) {
  return std::make_shared<StackedContainer>(std::move(children));
}

}  // namespace Container

}  // namespace ftxui
