// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <memory>   // for make_shared
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Decorator, Element, focusPosition, focusPositionRelative
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/dom/requirement.hpp"  // for Requirement, Requirement::NORMAL, Requirement::Selection
#include "ftxui/screen/box.hpp"  // for Box

namespace ftxui {

/// @brief 在 `frame` 內部使用，這會強制視圖滾動到給定位置。該位置以請求大小的比例表示。
///
/// 例如：
/// - (0, 0) 表示視圖滾動到左上方。
/// - (1, 0) 表示視圖滾動到右上方。
/// - (0, 1) 表示視圖滾動到左下方。
/// @ingroup dom
///
/// ### 範例
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

/// @brief 在 `frame` 內部使用，這會強制視圖滾動到給定位置。該位置以單元格數表示。
///
/// @ingroup dom
///
/// ### 範例
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