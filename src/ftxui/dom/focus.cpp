// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスに従って管理されます。
#include <memory>   // for make_shared
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Decorator, Element, focusPosition, focusPositionRelative
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/dom/requirement.hpp"  // for Requirement, Requirement::NORMAL, Requirement::Selection
#include "ftxui/screen/box.hpp"  // for Box

namespace ftxui {

/// @brief `frame`内で使用され、ビューを特定の位置にスクロールさせます。位置は要求されたサイズの割合で表されます。
///
/// 例えば：
/// - (0, 0) は、ビューが左上隅にスクロールされることを意味します。
/// - (1, 0) は、ビューが右上隅にスクロールされることを意味します。
/// - (0, 1) は、ビューが左下隅にスクロールされることを意味します。
/// @ingroup dom
///
/// ### Example
///
/// ```cpp
/// Element document = huge_document()
///   | focusPositionRelative(0.f, 1.f)
///   | frame;
/// ```
Decorator focusPositionRelative(float x, float y) {
  class Impl : public NodeDecorator {
   public:
    Impl(Element child, float x, float y)
        : NodeDecorator(std::move(child)), x_(x), y_(y) {}

    void ComputeRequirement() override {
      NodeDecorator::ComputeRequirement();
      requirement_.focused.enabled = true;
      requirement_.focused.node = this;
      requirement_.focused.box.x_min = int(float(requirement_.min_x) * x_);
      requirement_.focused.box.y_min = int(float(requirement_.min_y) * y_);
      requirement_.focused.box.x_max = int(float(requirement_.min_x) * x_);
      requirement_.focused.box.y_max = int(float(requirement_.min_y) * y_);
    }

   private:
    const float x_;
    const float y_;
  };

  return [x, y](Element child) {
    return std::make_shared<Impl>(std::move(child), x, y);
  };
}

/// @brief `frame`内で使用され、ビューを特定の位置にスクロールさせます。位置はセル数で表されます。
///
/// @ingroup dom
///
/// ### Example
///
/// ```cpp
/// Element document = huge_document()
///   | focusPosition(10, 10)
///   | frame;
/// ```
Decorator focusPosition(int x, int y) {
  class Impl : public NodeDecorator {
   public:
    Impl(Element child, int x, int y)
        : NodeDecorator(std::move(child)), x_(x), y_(y) {}

    void ComputeRequirement() override {
      NodeDecorator::ComputeRequirement();
      requirement_.focused.enabled = false;

      Box& box = requirement_.focused.box;
      box.x_min = x_;
      box.y_min = y_;
      box.x_max = x_;
      box.y_max = y_;
    }

   private:
    const int x_;
    const int y_;
  };

  return [x, y](Element child) {
    return std::make_shared<Impl>(std::move(child), x, y);
  };
}

}  // namespace ftxui
