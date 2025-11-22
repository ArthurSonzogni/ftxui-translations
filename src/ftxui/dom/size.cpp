// 版权所有 2020 Arthur Sonzogni。保留所有权利。
// 本源代码受 MIT 许可证的约束，该许可证可在以下文件中找到：
// LICENSE 文件。
#include <algorithm>  // for min, max
#include <memory>     // for make_shared, __shared_ptr_access
#include <utility>    // for move

#include "ftxui/dom/elements.hpp"  // for Constraint, WidthOrHeight, EQUAL, GREATER_THAN, LESS_THAN, WIDTH, unpack, Decorator, Element, size
#include "ftxui/dom/node.hpp"      // for Node, Elements
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box

namespace ftxui {

namespace {
class Size : public Node {
 public:
  Size(Element child, WidthOrHeight direction, Constraint constraint, int value)
      : Node(unpack(std::move(child))),
        direction_(direction),
        constraint_(constraint),
        value_(std::max(0, value)) {}

  void ComputeRequirement() override {
    Node::ComputeRequirement();
    requirement_ = children_[0]->requirement();

    auto& value = direction_ == WIDTH ? requirement_.min_x : requirement_.min_y;

    switch (constraint_) {
      case LESS_THAN:
        value = std::min(value, value_);
        break;
      case EQUAL:
        value = value_;
        break;
      case GREATER_THAN:
        value = std::max(value, value_);
        break;
    }

    if (direction_ == WIDTH) {
      requirement_.flex_grow_x = 0;
      requirement_.flex_shrink_x = 0;
    } else {
      requirement_.flex_grow_y = 0;
      requirement_.flex_shrink_y = 0;
    }
  }

  void SetBox(Box box) override {
    Node::SetBox(box);

    if (direction_ == WIDTH) {
      switch (constraint_) {
        case LESS_THAN:
        case EQUAL:
          box.x_max = std::min(box.x_min + value_ + 1, box.x_max);
          break;
        case GREATER_THAN:
          break;
      }
    } else {
      switch (constraint_) {
        case LESS_THAN:
        case EQUAL:
          box.y_max = std::min(box.y_min + value_ + 1, box.y_max);
          break;
        case GREATER_THAN:
          break;
      }
    }
    children_[0]->SetBox(box);
  }

 private:
  WidthOrHeight direction_;
  Constraint constraint_;
  int value_;
};
}  // namespace

/// @brief 对元素大小应用约束。
/// @param direction 元素的宽度 WIDTH 或高度 HEIGHT 是否必须受到约束。
///                  约束。
/// @param constraint 约束的类型。
/// @param value 值。
/// @ingroup dom
Decorator size(WidthOrHeight direction, Constraint constraint, int value) {
  return [=](Element e) {
    return std::make_shared<Size>(std::move(e), direction, constraint, value);
  };
}

}  // namespace ftxui
