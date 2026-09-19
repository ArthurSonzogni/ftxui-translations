// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <ftxui/screen/box.hpp>  // for Box
#include <string>
#include <utility>  // for move

#include <cstddef>
#include "ftxui/dom/node.hpp"
#include "ftxui/dom/selection.hpp"    // for Selection
#include "ftxui/screen/screen.hpp"    // for Screen
#include "ftxui/screen/terminal.hpp"  // for GetQuirks

namespace ftxui {

Node::Node() = default;
Node::Node(Elements children) : children_(std::move(children)) {}
Node::~Node() = default;

/// @brief 計算元素所需的空間大小。
void Node::ComputeRequirement() {
  if (children_.empty()) {
    return;
  }
  for (auto& child : children_) {
    child->ComputeRequirement();
  }

  // 預設情況下，需求是第一個子元素的需求。
  requirement_ = children_[0]->requirement();

  // 傳遞聚焦需求。
  for (size_t i = 1; i < children_.size(); ++i) {
    if (requirement_.focused.Prefer(children_[i]->requirement().focused)) {
      requirement_.focused = children_[i]->requirement().focused;
    }
  }
}

/// @brief 為元素分配繪圖位置和尺寸。
void Node::SetBox(Box box) {
  box_ = box;
}

/// @brief 計算元素的選取範圍。
void Node::Select(Selection& selection) {
  // 如果此節點的 box_ 與選取範圍沒有交集，則不進行選取。
  if (Box::Intersection(selection.GetBox(), box_).IsEmpty()) {
    return;
  }

  // 預設情況下，我們將選取交由子元素處理。
  for (auto& child : children_) {
    child->Select(selection);
  }
}

/// @brief 在 ftxui::Screen 上顯示元素。
void Node::Render(Screen& screen) {
  for (auto& child : children_) {
    child->Render(screen);
  }
}

void Node::Check(Status* status) {
  for (auto& child : children_) {
    child->Check(status);
  }
  status->need_iteration |= (status->iteration == 0);
}

std::string Node::GetSelectedContent(Selection& selection) {
  std::string content;

  for (auto& child : children_) {
    content += child->GetSelectedContent(selection);
  }

  return content;
}

void Node::Reserved1() {}
void Node::Reserved2() {}
void Node::Reserved3() {}
void Node::Reserved4() {}
void Node::Reserved5() {}
void Node::Reserved6() {}
void Node::Reserved7() {}
void Node::Reserved8() {}

/// @brief 在 ftxui::Screen 上顯示元素。
/// @ingroup dom
void Render(Screen& screen, const Element& element) {
  Selection selection;
  Render(screen, element.get(), selection);
}

/// @brief 在 ftxui::Screen 上顯示元素。
/// @ingroup dom
void Render(Screen& screen, Node* node) {
  Selection selection;
  Render(screen, node, selection);
}

void Render(Screen& screen, Node* node, Selection& selection) {
  Box box;
  box.x_min = 0;
  box.y_min = 0;
  box.x_max = screen.dimx() - 1;
  box.y_max = screen.dimy() - 1;

  Node::Status status;
  node->Check(&status);
  const int max_iterations = 20;
  while (status.need_iteration && status.iteration < max_iterations) {
    // 步驟 1：找出這個元素想要的維度。
    node->ComputeRequirement();

    // 步驟 2：為元素指定一個維度。
    node->SetBox(box);

    // 檢查此元素是否需要另一輪配置演算法的迭代。
    status.need_iteration = false;
    status.iteration++;
    node->Check(&status);
  }

  // 步驟 3：選取
  if (!selection.IsEmpty()) {
    node->Select(selection);
  }

  bool use_cursor = node->requirement().focused.enabled;
  if (!Terminal::GetQuirks().CursorHiding() &&
      node->requirement().focused.cursor_shape ==
          Screen::Cursor::Shape::Hidden) {
    // 將游標設定在正確的位置，可以讓使用 CJK（中文、
    // 日文、韓文……）字元的使用者，在正確的位置看到他們的
    // [輸入法編輯器]。詳見 [issue]。
    //
    // [輸入法編輯器]:
    // https://en.wikipedia.org/wiki/Input_method
    //
    // [issue]:
    // https://github.com/ArthurSonzogni/FTXUI/issues/2#issuecomment-505282355
    //
    // 不幸的是，Microsoft 終端機並未正確處理隱藏游標。
    // 反而是游標所在的字元被隱藏，這是個
    // 大問題。因此，我們無法啟用將游標設定到正確
    // 位置的功能。它會顯示在右下角。
    // 詳見：
    // https://github.com/microsoft/terminal/issues/1203
    // https://github.com/microsoft/terminal/issues/3093
    use_cursor = false;
  }

  if (use_cursor) {
    screen.SetCursor(Screen::Cursor{
        node->requirement().focused.node->box_.x_min,
        node->requirement().focused.node->box_.y_min,
        node->requirement().focused.cursor_shape,
    });
  } else {
    screen.SetCursor(Screen::Cursor{
        screen.dimx() - 1,
        screen.dimy() - 1,
        Screen::Cursor::Shape::Hidden,
    });
  }

  // 步驟 4：繪製元素。
  screen.stencil = box;
  node->Render(screen);

  // 步驟 5：套用 shaders
  screen.ApplyShader();
}

std::string GetNodeSelectedContent(Screen& screen,
                                   Node* node,
                                   Selection& selection) {
  Box box;
  box.x_min = 0;
  box.y_min = 0;
  box.x_max = screen.dimx() - 1;
  box.y_max = screen.dimy() - 1;

  Node::Status status;
  node->Check(&status);
  const int max_iterations = 20;
  while (status.need_iteration && status.iteration < max_iterations) {
    // 步驟 1：找出這個元素想要的維度。
    node->ComputeRequirement();

    // 步驟 2：為元素指定一個維度。
    node->SetBox(box);

    // 檢查此元素是否需要另一輪配置演算法的迭代。
    status.need_iteration = false;
    status.iteration++;
    node->Check(&status);
  }

  // 步驟 3：選取
  node->Select(selection);

  // 步驟 4：取得選取的內容。
  return node->GetSelectedContent(selection);
}

}  // namespace ftxui
