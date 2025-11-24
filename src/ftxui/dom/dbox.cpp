// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードは、MITライセンスの下で利用可能です。
// 詳細については、LICENSEファイルを参照してください。
#include <algorithm>  // for max
#include <cstddef>    // for size_t
#include <memory>     // for __shared_ptr_access, shared_ptr, make_shared
#include <utility>    // for move
#include <vector>

#include "ftxui/dom/elements.hpp"     // for Element, Elements, dbox
#include "ftxui/dom/node.hpp"         // for Node, Elements
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/pixel.hpp"     // for Pixel

namespace ftxui {

namespace {
class DBox : public Node {
 public:
  explicit DBox(Elements children) : Node(std::move(children)) {}

  void ComputeRequirement() override {
    requirement_ = Requirement{};
    for (auto& child : children_) {
      child->ComputeRequirement();

      // Propagate the focused requirement.
      if (requirement_.focused.Prefer(child->requirement().focused)) {
        requirement_.focused = child->requirement().focused;
      }

      // Extend the min_x and min_y to contain all the children
      requirement_.min_x =
          std::max(requirement_.min_x, child->requirement().min_x);
      requirement_.min_y =
          std::max(requirement_.min_y, child->requirement().min_y);
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

/// @brief 複数の要素を重ねて表示します。
/// @param children_ 入力要素。
/// @return 右寄せされた要素。
/// @ingroup dom
Element dbox(Elements children_) {
  return std::make_shared<DBox>(std::move(children_));
}

}  // namespace ftxui