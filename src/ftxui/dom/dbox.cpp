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

      // 全ての子要素を内包するようにmin_xとmin_yを拡張する。
      requirement_.min_x =
          std::max(requirement_.min_x, child->requirement().min_x);
      requirement_.min_y =
          std::max(requirement_.min_y, child->requirement().min_y);
    }

    // フォーカス要求を伝播する。
    // 子は最初から最後まで順に描画される、つまり最後の子が他の子の上に
    // 表示されるため、逆順で反復する。最も上にある子を優先的にフォーカス
    // させたい。
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

/// @brief 複数の要素を互いに重ねて表示する。
/// @param children_ 入力要素。
/// @return 右揃えされた要素。
/// @ingroup dom
Element dbox(Elements children_) {
  return std::make_shared<DBox>(std::move(children_));
}

}  // namespace ftxui
