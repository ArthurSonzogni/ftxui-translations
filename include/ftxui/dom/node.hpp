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
class Node {
 public:
  Node();
  explicit Node(Elements children);
  Node(const Node&) = delete;
  Node(const Node&&) = delete;
  Node& operator=(const Node&) = delete;
  Node& operator=(const Node&&) = delete;

  virtual ~Node();

  // 步驟 1: 計算佈局需求。告知父元素此元素想要的尺寸。
  //         從子元素傳播到父元素。
  virtual void ComputeRequirement();
  Requirement requirement() { return requirement_; }

  // 步驟 2: 為此元素指定其最終尺寸。
  //         從父元素傳播到子元素。
  virtual void SetBox(Box box);

  // 步驟 3: (可選) 選取
  //         從父元素傳播到子元素。
  virtual void Select(Selection& selection);

  // 步驟 4: 繪製此元素。
  virtual void Render(Screen& screen);

  virtual std::string GetSelectedContent(Selection& selection);

  // 對於某些元素，佈局可能無法在單次迭代中解決。
  // 這允許它們請求額外的迭代。此信號必須至少轉發給子元素一次。
  struct Status {
    int iteration = 0;
    bool need_iteration = false;
  };
  virtual void Check(Status* status);

  friend void Render(Screen& screen, Node* node, Selection& selection);

 protected:
  Elements children_;
  Requirement requirement_;
  Box box_;
};

void Render(Screen& screen, const Element& element);
void Render(Screen& screen, Node* node);
void Render(Screen& screen, Node* node, Selection& selection);
std::string GetNodeSelectedContent(Screen& screen,
                                   Node* node,
                                   Selection& selection);

}  // namespace ftxui

#endif  // FTXUI_DOM_NODE_HPP
