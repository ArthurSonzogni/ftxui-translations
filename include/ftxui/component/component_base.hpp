// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_COMPONENT_BASE_HPP
#define FTXUI_COMPONENT_BASE_HPP

#include <memory>  // for unique_ptr
#include <vector>  // for vector

#include "ftxui/component/captured_mouse.hpp"  // for CaptureMouse
#include "ftxui/dom/elements.hpp"              // for Element
#include "ftxui/util/export.hpp"

namespace ftxui {

class Delegate;
class Focus;
struct Event;

namespace animation {
class Params;
}  // namespace animation

class ComponentBase;
using Component = std::shared_ptr<ComponentBase>;
using Components = std::vector<Component>;

/// @brief ftxui::Elementとして自身のレンダリングを実装します。ftxui::Eventに応答してキーボードナビゲーションを実装します。
/// @ingroup component
class FTXUI_EXPORT(COMPONENT) ComponentBase {
 public:
  explicit ComponentBase(Components children);
  virtual ~ComponentBase();
  ComponentBase();

  // A component is not copyable/movable.
  ComponentBase(const ComponentBase&) = delete;
  ComponentBase(ComponentBase&&) = delete;
  ComponentBase& operator=(const ComponentBase&) = delete;
  ComponentBase& operator=(ComponentBase&&) = delete;

  // Component hierarchy:
  ComponentBase* Parent() const;
  Component& ChildAt(size_t i);
  size_t ChildCount() const;
  int Index() const;
  void Add(Component children);
  void Detach();
  void DetachAllChildren();

  // Renders the component.
  Element Render();

  // `Render`の動作を変更するためにこの関数をオーバーライドします。
  virtual Element OnRender();

  // Handles an event.
  // By default, reduce on children with a lazy OR.
  //
  // Returns whether the event was handled or not.
  virtual bool OnEvent(Event);

  // Handle an animation step.
  virtual void OnAnimation(animation::Params& params);

  // フォーカス管理 ----------------------------------------------------------
  //
  // このコンポーネントに子要素が含まれている場合、これはどれがアクティブであるかを示します。
  // アクティブな子要素がない場合はnullptrです。
  //
  // ルートコンポーネントからのActiveChild()のチェーンにこのオブジェクトが含まれている場合、要素がフォーカスを持っていると言います。
  virtual Component ActiveChild();

  // コンポーネントがフォーカス可能な要素を含んでいる場合にtrueを返します。
  // フォーカス不可能なコンポーネントは、キーボードを使用してナビゲートする際にスキップされます。
  virtual bool Focusable() const;

  // Whether this is the active child of its parent.
  bool Active() const;
  // Whether all the ancestors are active.
  bool Focused() const;

  // |child|を「アクティブ」にします。
  virtual void SetActiveChild(ComponentBase* child);
  void SetActiveChild(Component child);

  // Configure all the ancestors to give focus to this component.
  void TakeFocus();

  // ABI Reserve:
  virtual void Reserved1();
  virtual void Reserved2();
  virtual void Reserved3();
  virtual void Reserved4();
  virtual void Reserved5();
  virtual void Reserved6();
  virtual void Reserved7();
  virtual void Reserved8();

 protected:
  CapturedMouse CaptureMouse(const Event& event);

  Components& children();
  const Components& children() const;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_BASE_HPP */
