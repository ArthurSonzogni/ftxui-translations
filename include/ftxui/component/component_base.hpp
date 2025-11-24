// Copyright 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權約束，詳情請參閱 LICENSE 檔案。
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

/// @brief 它將自己實作為 ftxui::Element 進行渲染。它透過回應 ftxui::Event 來實現鍵盤導航。
/// @ingroup component
class ComponentBase {
 public:
  explicit ComponentBase(Components children)
      : children_(std::move(children)) {}
  virtual ~ComponentBase();
  ComponentBase() = default;

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

  // 渲染元件。
  Element Render();

  // 覆寫此函式以修改 `Render` 的運作方式。
  virtual Element OnRender();

  // 處理一個事件。
  // 預設情況下，使用惰性 OR 對子元件進行縮減。
  //
  // 返回事件是否已處理。
  virtual bool OnEvent(Event);

  // 處理動畫步驟。
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

  // 這是否是其父元件的活動子元件。
  bool Active() const;
  // 是否所有祖先都處於活動狀態。
  bool Focused() const;

  // 使 |child| 成為「活動」元件。
  virtual void SetActiveChild(ComponentBase* child);
  void SetActiveChild(Component child);

  // 配置所有祖先以將焦點賦予此元件。
  void TakeFocus();

 protected:
  CapturedMouse CaptureMouse(const Event& event);

  Components children_;

 private:
  ComponentBase* parent_ = nullptr;
  bool in_render = false;
};

}  // namespace ftxui

#endif /* 包含防護結束: FTXUI_COMPONENT_BASE_HPP */
