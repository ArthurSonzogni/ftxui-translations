// 版權所有 2020 Arthur Sonzogni. 保留所有權利。
// 此原始碼受 MIT 授權條款約束，詳情請參閱
// LICENSE 文件。
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

/// @brief 一個什麼都不做的裝飾器。
/// @ingroup dom
Element nothing(Element element) {
  return element;
}

/// @brief 將兩個裝飾器組合為一個。
/// @ingroup dom
///
/// ### 範例
///
/// ```cpp
/// auto decorator = bold | blink;
/// ```
Decorator operator|(Decorator a, Decorator b) {
  return compose(std::move(a),  //
                 std::move(b));
}

/// @brief 從一組元素中，對每個元素應用一個裝飾器。
/// @return 一組被裝飾的元素。
/// @ingroup dom
Elements operator|(Elements elements, Decorator decorator) {  // NOLINT
  Elements output;
  output.reserve(elements.size());
  for (auto& it : elements) {
    output.push_back(std::move(it) | decorator);
  }
  return output;
}

/// @brief 從一個元素中，應用一個裝飾器。
/// @return 被裝飾的元素。
/// @ingroup dom
///
/// ### 範例
///
/// 兩者是等效的：
/// ```cpp
/// bold(text("Hello"));
/// ```
/// ```cpp
/// text("Hello") | bold;
/// ```
Element operator|(Element element, Decorator decorator) {  // NOLINT
  return decorator(std::move(element));
}

/// @brief 對元素應用一個裝飾器。
/// @return 被裝飾的元素。
/// @ingroup dom
///
/// ### 範例
///
/// 兩者是等效的：
/// ```cpp
/// auto element = text("Hello");
/// element |= bold;
/// ```
Element& operator|=(Element& e, Decorator d) {
  e = e | std::move(d);
  return e;
}

/// 適合給定元素的最小尺寸。
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

    // 不要給元素超過它所需的空間：
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
    // 增加框的大小直到它適應...
    box.x_max = std::min(e->requirement().min_x, fullsize.dimx);
    box.y_max = e->requirement().min_y;

    // ... 但不要超出螢幕尺寸：
    if (!extend_beyond_screen) {
      box.y_max = std::min(box.y_max, fullsize.dimy);
    }
  }

  return {
      box.x_max,
      box.y_max,
  };
}

/// 一個 0x0 大小，不繪製任何東西的元素。
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