// Copyright 2020 Arthur Sonzogni. All rights reserved.
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱 LICENSE 檔案。
#include <ftxui/screen/box.hpp>  // for Box
#include <string>
#include <utility>  // for move

#include <cstddef>
#include "ftxui/dom/node.hpp"
#include "ftxui/dom/selection.hpp"  // for Selection
#include "ftxui/screen/screen.hpp"  // for Screen

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

  // 預設情況下，需求是第一個子節點的需求。
  requirement_ = children_[0]->requirement();

  // 傳播焦點需求。
  for (size_t i = 1; i < children_.size(); ++i) {
    if (!requirement_.focused.enabled &&
        children_[i]->requirement().focused.enabled) {
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

  // 預設情況下，我們將選取延遲到子節點。
  for (auto& child : children_) {
    child->Select(selection);
  }
}

/// @brief 在 ftxui::Screen 上顯示元素。
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
    // 步驟 1：找出此元素想要的尺寸。
    node->ComputeRequirement();

    // 步驟 2：為元素分配尺寸。
    node->SetBox(box);

    // 檢查元素是否需要佈局演算法的另一次迭代。
    status.need_iteration = false;
    status.iteration++;
    node->Check(&status);
  }

  // 步驟 3：選取
  if (!selection.IsEmpty()) {
    node->Select(selection);
  }

  if (node->requirement().focused.enabled
#if defined(FTXUI_MICROSOFT_TERMINAL_FALLBACK)
      // 將游標設定到正確的位置，允許使用 CJK（中文、日文、韓文等）字元的使用者
      // 在正確的位置看到他們的 [輸入法編輯器]。請參閱 [問題]。
      //
      // [輸入法編輯器]：
      // https://zh.wikipedia.org/wiki/輸入法編輯器
      //
      // [問題]：
      // https://github.com/ArthurSonzogni/FTXUI/issues/2#issuecomment-505282355
      //
      // 遺憾的是，Microsoft 終端機無法正確處理隱藏游標。相反地，
      // 游標下方的字元會被隱藏，這是一個大問題。因此，我們無法啟用
      // 將游標設定到正確的位置。它將顯示在右下角。
      // 請參閱：
      // https://github.com/microsoft/terminal/issues/1203
      // https://github.com/microsoft/terminal/issues/3093
      &&
      node->requirement().focused.cursor_shape != Screen::Cursor::Shape::Hidden
#endif
  ) {
    screen.SetCursor(Screen::Cursor{
        node->requirement().focused.node->box_.x_max,
        node->requirement().focused.node->box_.y_max,
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

  // 步驟 5：應用著色器
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
    // 步驟 1：找出此元素想要的尺寸。
    node->ComputeRequirement();

    // 步驟 2：為元素分配尺寸。
    node->SetBox(box);

    // 檢查元素是否需要佈局演算法的另一次迭代。
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
