// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードはMITライセンスの下で利用可能です。
// 詳細はLICENSEファイルを参照してください。
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

/// @brief 何も行わないデコレーションです。
/// @ingroup dom
Element nothing(Element element) {
  return element;
}

/// @brief 2つのデコレーターを1つに合成します。
/// @ingroup dom
///
/// ### 例
///
/// ```cpp
/// auto decorator = bold | blink;
/// ```
Decorator operator|(Decorator a, Decorator b) {
  return compose(std::move(a),  //
                 std::move(b));
}

/// @brief 要素のセットから、すべての要素にデコレーターを適用します。
/// @return デコレートされた要素のセット。
/// @ingroup dom
Elements operator|(Elements elements, Decorator decorator) {  // NOLINT
  Elements output;
  output.reserve(elements.size());
  for (auto& it : elements) {
    output.push_back(std::move(it) | decorator);
  }
  return output;
}

/// @brief 要素にデコレーターを適用します。
/// @return デコレートされた要素。
/// @ingroup dom
///
/// ### 例
///
/// これらはどちらも同等です。
/// ```cpp
/// bold(text("Hello"));
/// ```
/// ```cpp
/// text("Hello") | bold;
/// ```
Element operator|(Element element, Decorator decorator) {  // NOLINT
  return decorator(std::move(element));
}

/// @brief 要素にデコレーターを適用します。
/// @return デコレートされた要素。
/// @ingroup dom
///
/// ### 例
///
/// これらはどちらも同等です。
/// ```cpp
/// auto element = text("Hello");
/// element |= bold;
/// ```
Element& operator|=(Element& e, Decorator d) {
  e = e | std::move(d);
  return e;
}

/// 与えられた要素に適合する最小の次元。
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

    // 要素に必要な以上のスペースを与えないでください:
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
    // ボックスが収まるまでサイズを増やします...
    box.x_max = std::min(e->requirement().min_x, fullsize.dimx);
    box.y_max = e->requirement().min_y;

    // ...ただし、画面サイズを超えないようにしてください:
    if (!extend_beyond_screen) {
      box.y_max = std::min(box.y_max, fullsize.dimy);
    }
  }

  return {
      box.x_max,
      box.y_max,
  };
}

/// 何も描画しない0x0サイズの要素。
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
