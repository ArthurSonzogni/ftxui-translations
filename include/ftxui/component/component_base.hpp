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

  // コンポーネントはコピー/ムーブ不可です。
  ComponentBase(const ComponentBase&) = delete;
  ComponentBase(ComponentBase&&) = delete;
  ComponentBase& operator=(const ComponentBase&) = delete;
  ComponentBase& operator=(ComponentBase&&) = delete;

  // コンポーネントの階層:
  ComponentBase* Parent() const;
  Component& ChildAt(size_t i);
  size_t ChildCount() const;
  int Index() const;
  void Add(Component children);
  void Detach();
  void DetachAllChildren();

  // コンポーネントを描画します。
  Element Render();

  // `Render`の動作を変更するためにこの関数をオーバーライドします。
  virtual Element OnRender();

  // イベントを処理します。
  // デフォルトでは、子に対して遅延ORで削減します。
  //
  // イベントが処理されたかどうかを返します。
  virtual bool OnEvent(Event);

  // アニメーションステップを処理します。
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

  // これが親のアクティブな子であるかどうか。
  bool Active() const;
  // すべての祖先がアクティブかどうか。
  bool Focused() const;

  // |child|を「アクティブ」にします。
  virtual void SetActiveChild(ComponentBase* child);
  void SetActiveChild(Component child);

  // すべての祖先を構成して、このコンポーネントにフォーカスを与えます。
  void TakeFocus();

  // ABI予約:
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
