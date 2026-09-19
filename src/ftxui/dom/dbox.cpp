// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>  // for max
#include <memory>     // for __shared_ptr_access, shared_ptr, make_shared
#include <utility>    // for move

#include "ftxui/dom/elements.hpp"  // for Element, dbox

#include "ftxui/dom/node.hpp"         // for Node, Elements
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box

namespace ftxui {

namespace {
class DBox : public Node {
 public:
  explicit DBox(Elements children) : Node(std::move(children)) {}

  void ComputeRequirement() override {
    requirement_ = Requirement{};
    for (auto& child : children_) {
      child->ComputeRequirement();

      // 擴展 min_x 和 min_y 使其包含所有子元素
      requirement_.min_x =
          std::max(requirement_.min_x, child->requirement().min_x);
      requirement_.min_y =
          std::max(requirement_.min_y, child->requirement().min_y);
    }

    // 傳遞聚焦需求。
    // 我們以反向順序迭代，因為子元素是依照從第一個到
    // 最後一個的順序渲染，意味著最後一個子元素會顯示在其他元素
    // 之上。我們希望優先讓最上層的子元素獲得聚焦。
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
      auto& child = *it;
      if (requirement_.focused.Prefer(child->requirement().focused)) {
        requirement_.focused = child->requirement().focused;
      }
    }
  }

  void SetBox(Box box) override {
    Node::SetBox(box);

    for (auto& child : children_) {
      child->SetBox(box);
    }
  }
};
}  // namespace

/// @brief 將多個元素堆疊在一起。
/// @param children_ 輸入的元素。
/// @return 右對齊的元素。
/// @ingroup dom
Element dbox(Elements children_) {
  return std::make_shared<DBox>(std::move(children_));
}

}  // namespace ftxui
