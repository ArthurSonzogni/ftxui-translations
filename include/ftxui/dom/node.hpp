// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_NODE_HPP
#define FTXUI_DOM_NODE_HPP

#include <memory>  // for shared_ptr
#include <vector>  // for vector

#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/dom/selection.hpp"    // for Selection
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/screen.hpp"
#include "ftxui/util/export.hpp"

namespace ftxui {

class Node;
class Screen;

using Element = std::shared_ptr<Node>;
using Elements = std::vector<Element>;

/// @brief 節點是DOM樹中所有元素的基底類別。
///
/// 它代表文件物件模型 (DOM) 中的單一節點，並提供佈局和渲染的基本結構。
/// 它包含用於計算佈局需求、設定框尺寸、選取內容、渲染到螢幕以及檢查佈局狀態的方法。
/// 它通常包含子元素，這些子元素也是節點的實例。
///
/// 鼓勵使用者從這個類別派生以建立自訂元素。
///
/// 內建元素的清單可以在 `elements.hpp` 檔案中找到。
///
/// @ingroup dom
class FTXUI_EXPORT(DOM) Node {
 public:
  Node();
  explicit Node(Elements children);
  Node(const Node&) = delete;
  Node(const Node&&) = delete;
  Node& operator=(const Node&) = delete;
  Node& operator=(const Node&&) = delete;

  virtual ~Node();

  // Step 1: Compute layout requirement. Tell parent what dimensions this
  //         element wants to be.
  //         Propagated from Children to Parents.
  virtual void ComputeRequirement();
  Requirement requirement() { return requirement_; }

  // Step 2: Assign this element its final dimensions.
  //         Propagated from Parents to Children.
  virtual void SetBox(Box box);

  // Step 3: (optional) Selection
  //         Propagated from Parents to Children.
  virtual void Select(Selection& selection);

  // Step 4: Draw this element.
  virtual void Render(Screen& screen);

  virtual std::string GetSelectedContent(Selection& selection);

  // Layout may not resolve within a single iteration for some elements. This
  // allows them to request additional iterations. This signal must be
  // forwarded to children at least once.
  struct Status {
    int iteration = 0;
    bool need_iteration = false;
  };
  virtual void Check(Status* status);

  // ABI Reserve:
  virtual void Reserved1();
  virtual void Reserved2();
  virtual void Reserved3();
  virtual void Reserved4();
  virtual void Reserved5();
  virtual void Reserved6();
  virtual void Reserved7();
  virtual void Reserved8();

  friend FTXUI_EXPORT(DOM) void Render(Screen& screen, Node* node, Selection& selection);

 protected:
  Elements children_;
  Requirement requirement_;
  Box box_;
};

FTXUI_EXPORT(DOM) void Render(Screen& screen, const Element& element);
FTXUI_EXPORT(DOM) void Render(Screen& screen, Node* node);
FTXUI_EXPORT(DOM)
void Render(Screen& screen, Node* node, Selection& selection);
FTXUI_EXPORT(DOM)
std::string GetNodeSelectedContent(Screen& screen,
                                   Node* node,
                                   Selection& selection);

}  // namespace ftxui

#endif  // FTXUI_DOM_NODE_HPP
