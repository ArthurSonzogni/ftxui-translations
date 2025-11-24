// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスによって管理されています。
#ifndef FTXUI_COMPONENT_BASE_HPP
#define FTXUI_COMPONENT_BASE_HPP

#include <memory>  // for unique_ptr
#include <vector>  // for vector

#include "ftxui/component/captured_mouse.hpp"  // for CaptureMouse
#include "ftxui/dom/elements.hpp"              // for Element

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
class ComponentBase {
 public:
  explicit ComponentBase(Components children)
      : children_(std::move(children)) {}
  virtual ~ComponentBase();
  ComponentBase() = default;

  // コンポーネントはコピー/ムーブできません。
  ComponentBase(const ComponentBase&) = delete;
  ComponentBase(ComponentBase&&) = delete;
  ComponentBase& operator=(const ComponentBase&) = delete;
  ComponentBase& operator=(ComponentBase&&) = delete;

  // コンポーネント階層:
  ComponentBase* Parent() const;
  Component& ChildAt(size_t i);
  size_t ChildCount() const;
  int Index() const;
  void Add(Component children);
  void Detach();
  void DetachAllChildren();

  // コンポーネントをレンダリングします。
  Element Render();

  // `Render`の動作を変更するためにこの関数をオーバーライドします。
  virtual Element OnRender();

  // イベントを処理します。
  // デフォルトでは、遅延ORで子要素を減らします。
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

  // これがその親のアクティブな子であるかどうか。
  bool Active() const;
  // すべての祖先がアクティブであるかどうか。
  bool Focused() const;

  // |child|を「アクティブ」にします。
  virtual void SetActiveChild(ComponentBase* child);
  void SetActiveChild(Component child);

  // すべての祖先を設定して、このコンポーネントにフォーカスを与えます。
  void TakeFocus();

 protected:
  CapturedMouse CaptureMouse(const Event& event);

  Components children_;

 private:
  ComponentBase* parent_ = nullptr;
  bool in_render = false;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_BASE_HPP */
