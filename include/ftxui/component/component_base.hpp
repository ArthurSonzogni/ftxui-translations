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

/// @brief 它將自己實作為 ftxui::Element 進行渲染。它透過回應 ftxui::Event 來實現鍵盤導航。
/// @ingroup component
class FTXUI_EXPORT(COMPONENT) ComponentBase {
 public:
  explicit ComponentBase(Components children);
  virtual ~ComponentBase();
  ComponentBase();

  // 元件不可複製/移動。
  ComponentBase(const ComponentBase&) = delete;
  ComponentBase(ComponentBase&&) = delete;
  ComponentBase& operator=(const ComponentBase&) = delete;
  ComponentBase& operator=(ComponentBase&&) = delete;

  // 元件階層：
  ComponentBase* Parent() const;
  Component& ChildAt(size_t i);
  size_t ChildCount() const;
  int Index() const;
  void Add(Component children);
  void Detach();
  void DetachAllChildren();

  // 繪製此元件。
  Element Render();

  // 覆寫此函式以修改 `Render` 的運作方式。
  virtual Element OnRender();

  // 處理一個事件。
  // 預設情況下，對子元件以惰性 OR 進行歸約。
  //
  // 回傳事件是否已被處理。
  virtual bool OnEvent(Event);

  // 處理一個動畫步驟。
  virtual void OnAnimation(animation::Params& params);

  // 焦點管理 ----------------------------------------------------------
  //
  // 如果此元件包含子元件，這表示哪個是活動的，
  // 如果沒有活動的，則為 nullptr。
  //
  // 如果從根元件的 ActiveChild() 鏈包含此物件，我們稱一個元素具有焦點。
  virtual Component ActiveChild();

  // 當元件包含可聚焦元素時返回 true。
  // 使用鍵盤導航時將跳過不可聚焦的元件。
  virtual bool Focusable() const;

  // 是否為其父元件目前作用中的子元件。
  bool Active() const;
  // 是否所有祖先都是作用中的。
  bool Focused() const;

  // 使 |child| 成為「活動」元件。
  virtual void SetActiveChild(ComponentBase* child);
  void SetActiveChild(Component child);

  // 設定所有祖先，將焦點交給此元件。
  void TakeFocus();

  // ABI 保留欄位：
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

#endif /* 包含防護結束: FTXUI_COMPONENT_BASE_HPP */
