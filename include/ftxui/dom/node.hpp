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

  // 步驟 1：計算布局需求。告知父元件此
  //         元素想要的尺寸。
  //         從子元件傳遞到父元件。
  virtual void ComputeRequirement();
  Requirement requirement() { return requirement_; }

  // 步驟 2：指派此元素最終的尺寸。
  //         從父元件傳遞到子元件。
  virtual void SetBox(Box box);

  // 步驟 3：（選用）選取
  //         從父元件傳遞到子元件。
  virtual void Select(Selection& selection);

  // 步驟 4：繪製此元素。
  virtual void Render(Screen& screen);

  virtual std::string GetSelectedContent(Selection& selection);

  // 對某些元素而言，布局可能無法在單一次迭代中完成。這
  // 讓它們可以要求額外的迭代。此訊號必須
  // 至少向子元件轉發一次。
  struct Status {
    int iteration = 0;
    bool need_iteration = false;
  };
  virtual void Check(Status* status);

  // ABI 保留欄位：
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
