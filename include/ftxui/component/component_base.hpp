// Copyright 2020 Arthur Sonzogni. 保留所有权利。
// 此源代码的使用受可在 LICENSE 文件中找到的 MIT 许可的管辖。
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

/// @brief 它将自身实现为 ftxui::Element。它通过响应 ftxui::Event 来实现键盘导航。
/// @ingroup component
class ComponentBase {
 public:
  explicit ComponentBase(Components children)
      : children_(std::move(children)) {}
  virtual ~ComponentBase();
  ComponentBase() = default;

  // 组件不可复制/移动。
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

  // 渲染组件。
  Element Render();

  // 覆盖此函数以修改 `Render` 的工作方式。
  virtual Element OnRender();

  // 处理事件。
  // 默认情况下，使用惰性 OR 对子项进行归约。
  //
  // 返回事件是否已处理。
  virtual bool OnEvent(Event);

  // 处理动画步骤。
  virtual void OnAnimation(animation::Params& params);

  // 焦点管理 ----------------------------------------------------------
  //
  // 如果此组件包含子项，则此项指示哪个子项处于活动状态，
  // 如果没有活动子项，则为 nullptr。
  //
  // 如果从根组件的 ActiveChild() 链包含此对象，我们称元素具有焦点。
  virtual Component ActiveChild();

  // 当组件包含可聚焦元素时返回 true。
  // 使用键盘导航时将跳过不可聚焦的组件。
  virtual bool Focusable() const;

  // 这是否是其父级的活动子项。
  bool Active() const;
  // 所有祖先是否都处于活动状态。
  bool Focused() const;

  // 将 |child| 设置为“活动”子项。
  virtual void SetActiveChild(ComponentBase* child);
  void SetActiveChild(Component child);

  // 配置所有祖先以将焦点赋予此组件。
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
