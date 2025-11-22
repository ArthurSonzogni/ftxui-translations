// Copyright 2020 Arthur Sonzogni. All rights reserved.
// 本源代码的使用受 MIT 许可证的约束，该许可证可在
// LICENSE 文件中找到。
#include <algorithm>   // for min
#include <functional>  // for function
#include <memory>      // for __shared_ptr_access, make_unique
#include <utility>     // for move

#include "ftxui/dom/elements.hpp"  // for Element, Decorator, Elements, operator|, Fit, emptyElement, nothing, operator|=
#include "ftxui/dom/node.hpp"      // for Node, Node::Status
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/screen.hpp"    // for Full
#include "ftxui/screen/terminal.hpp"  // for Dimensions

namespace ftxui {

namespace {
Decorator compose(Decorator a, Decorator b) {
  return [a = std::move(a), b = std::move(b)](Element element) {
    return b(a(std::move(element)));
  };
}
}  // namespace

/// @brief 一个什么都不做的装饰器。
/// @ingroup dom
Element nothing(Element element) {
  return element;
}

/// @brief 将两个装饰器组合成一个。
/// @ingroup dom
///
/// ### 示例
///
/// ```cpp
/// auto decorator = bold | blink;
/// ```
Decorator operator|(Decorator a, Decorator b) {
  return compose(std::move(a),  //
                 std::move(b));
}

/// @brief 从一组元素中，对每个元素应用一个装饰器。
/// @return 装饰后的元素集。
/// @ingroup dom
Elements operator|(Elements elements, Decorator decorator) {  // NOLINT
  Elements output;
  output.reserve(elements.size());
  for (auto& it : elements) {
    output.push_back(std::move(it) | decorator);
  }
  return output;
}

/// @brief 从一个元素中，应用一个装饰器。
/// @return 装饰后的元素。
/// @ingroup dom
///
/// ### 示例
///
/// 以下两者等效：
/// ```cpp
/// bold(text("Hello"));
/// ```
/// ```cpp
/// text("Hello") | bold;
/// ```
Element operator|(Element element, Decorator decorator) {  // NOLINT
  return decorator(std::move(element));
}

/// @brief 对元素应用一个装饰器。
/// @return 装饰后的元素。
/// @ingroup dom
///
/// ### 示例
///
/// 以下两者等效：
/// ```cpp
/// auto element = text("Hello");
/// element |= bold;
/// ```
Element& operator|=(Element& e, Decorator d) {
  e = e | std::move(d);
  return e;
}

/// 适应给定元素的最小尺寸。
/// @see Fixed
/// @see Full
Dimensions Dimension::Fit(Element& e, bool extend_beyond_screen) {
  const Dimensions fullsize = Dimension::Full();
  Box box;
  box.x_min = 0;
  box.y_min = 0;
  box.x_max = fullsize.dimx;
  box.y_max = fullsize.dimy;

  Node::Status status;
  e->Check(&status);
  const int max_iteration = 20;
  while (status.need_iteration && status.iteration < max_iteration) {
    e->ComputeRequirement();

    // Don't give the element more space than it needs:
    box.x_max = std::min(box.x_max, e->requirement().min_x);
    box.y_max = e->requirement().min_y;
    if (!extend_beyond_screen) {
      box.y_max = std::min(box.y_max, fullsize.dimy);
    }

    e->SetBox(box);
    status.need_iteration = false;
    status.iteration++;
    e->Check(&status);

    if (!status.need_iteration) {
      break;
    }
    // Increase the size of the box until it fits...
    box.x_max = std::min(e->requirement().min_x, fullsize.dimx);
    box.y_max = e->requirement().min_y;

    // ... but don't go beyond the screen size:
    if (!extend_beyond_screen) {
      box.y_max = std::min(box.y_max, fullsize.dimy);
    }
  }

  return {
      box.x_max,
      box.y_max,
  };
}

/// 一个大小为 0x0 且不绘制任何内容的元素。
/// @ingroup dom
Element emptyElement() {
  class Impl : public Node {
    void ComputeRequirement() override {
      requirement_.min_x = 0;
      requirement_.min_y = 0;
    }
  };
  return std::make_unique<Impl>();
}

}  // namespace ftxui
