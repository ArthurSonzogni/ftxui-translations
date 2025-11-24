// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>  // for max, min
#include <memory>     // for make_shared, __shared_ptr_access
#include <utility>    // for move

#include "ftxui/dom/elements.hpp"  // for Element, unpack, Elements, focus, frame, select, xframe, yframe
#include "ftxui/dom/node.hpp"         // for Node, Elements
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/screen.hpp"    // for Screen, Screen::Cursor
#include "ftxui/util/autoreset.hpp"   // for AutoReset

namespace ftxui {

namespace {
class Focus : public Node {
 public:
  explicit Focus(Elements children) : Node(std::move(children)) {}

  void ComputeRequirement() override {
    Node::ComputeRequirement();
    requirement_ = children_[0]->requirement();
    requirement_.focused.enabled = true;
    requirement_.focused.node = this;
    requirement_.focused.box.x_min = 0;
    requirement_.focused.box.y_min = 0;
    requirement_.focused.box.x_max = requirement_.min_x - 1;
    requirement_.focused.box.y_max = requirement_.min_y - 1;
  }

  void SetBox(Box box) override {
    Node::SetBox(box);
    children_[0]->SetBox(box);
  }
};

class Frame : public Node {
 public:
  Frame(Elements children, bool x_frame, bool y_frame)
      : Node(std::move(children)), x_frame_(x_frame), y_frame_(y_frame) {}

  void SetBox(Box box) override {
    Node::SetBox(box);
    auto& focused_box = requirement_.focused.box;
    Box children_box = box;

    if (x_frame_) {
      const int external_dimx = box.x_max - box.x.min;
      const int internal_dimx = std::max(requirement_.min_x, external_dimx);
      const int focused_dimx = focused_box.x_max - focused_box.x_min;
      int dx = focused_box.x_min - external_dimx / 2 + focused_dimx / 2;
      dx = std::max(0, std::min(internal_dimx - external_dimx - 1, dx));
      children_box.x_min = box.x_min - dx;
      children_box.x_max = box.x_min + internal_dimx - dx;
    }

    if (y_frame_) {
      const int external_dimy = box.y_max - box.y_min;
      const int internal_dimy = std::max(requirement_.min_y, external_dimy);
      const int focused_dimy = focused_box.y_max - focused_box.y_min;
      int dy = focused_box.y_min - external_dimy / 2 + focused_dimy / 2;
      dy = std::max(0, std::min(internal_dimy - external_dimy - 1, dy));
      children_box.y_min = box.y_min - dy;
      children_box.y_max = box.y_min + internal_dimy - dy;
    }

    children_[0]->SetBox(children_box);
  }

  void Render(Screen& screen) override {
    const AutoReset<Box> stencil(&screen.stencil,
                                 Box::Intersection(box_, screen.stencil));
    children_[0]->Render(screen);
  }

 private:
  bool x_frame_;
  bool y_frame_;
};

class FocusCursor : public Focus {
 public:
  FocusCursor(Elements children, Screen::Cursor::Shape shape)
      : Focus(std::move(children)), shape_(shape) {}

 private:
  void ComputeRequirement() override {
    Focus::ComputeRequirement();  // NOLINT
    requirement_.focused.cursor_shape = shape_;
  }
  Screen::Cursor::Shape shape_;
};

}  // namespace

/// @brief 子要素を兄弟要素の中でフォーカスされたものとして設定します。
/// @param child フォーカスされる要素。
/// @ingroup dom
Element focus(Element child) {
  return std::make_shared<Focus>(unpack(std::move(child)));
}

/// これは非推奨です。代わりに `focus` を使用してください。
/// @brief 子要素を兄弟要素の中でフォーカスされたものとして設定します。
/// @param child フォーカスされる要素。
Element select(Element child) {
  return focus(std::move(child));
}

/// @brief 要素を「仮想」領域内に表示できるようにします。そのサイズはコンテナよりも大きくすることができます。
/// この場合、より小さい部分のみが表示されます。フォーカスされた要素を表示するためにビューはスクロール可能です。
/// @see frame
/// @see xframe
/// @see yframe
Element frame(Element child) {
  return std::make_shared<Frame>(unpack(std::move(child)), true, true);
}

/// @brief `frame` と同じですが、x軸のみに適用されます。
/// @see frame
/// @see xframe
/// @see yframe
Element xframe(Element child) {
  return std::make_shared<Frame>(unpack(std::move(child)), true, false);
}

/// @brief `frame` と同じですが、y軸のみに適用されます。
/// @see frame
/// @see xframe
/// @see yframe
Element yframe(Element child) {
  return std::make_shared<Frame>(unpack(std::move(child)), false, true);
}

/// @brief `focus` と同じですが、カーソル形状を静止ブロックに設定します。
/// @see focus
/// @see focusCursorBlock
/// @see focusCursorBlockBlinking
/// @see focusCursorBar
/// @see focusCursorBarBlinking
/// @see focusCursorUnderline
/// @see focusCursorUnderlineBlinking
/// @ingroup dom
Element focusCursorBlock(Element child) {
  return std::make_shared<FocusCursor>(unpack(std::move(child)),
                                       Screen::Cursor::Block);
}

/// @brief `focus` と同じですが、カーソル形状を点滅ブロックに設定します。
/// @see focus
/// @see focusCursorBlock
/// @see focusCursorBlockBlinking
/// @see focusCursorBar
/// @see focusCursorBarBlinking
/// @see focusCursorUnderline
/// @see focusCursorUnderlineBlinking
/// @ingroup dom
Element focusCursorBlockBlinking(Element child) {
  return std::make_shared<FocusCursor>(unpack(std::move(child)),
                                       Screen::Cursor::BlockBlinking);
}

/// @brief `focus` と同じですが、カーソル形状を静止ブロックに設定します。
/// @see focus
/// @see focusCursorBlock
/// @see focusCursorBlockBlinking
/// @see focusCursorBar
/// @see focusCursorBarBlinking
/// @see focusCursorUnderline
/// @see focusCursorUnderlineBlinking
/// @ingroup dom
Element focusCursorBar(Element child) {
  return std::make_shared<FocusCursor>(unpack(std::move(child)),
                                       Screen::Cursor::Bar);
}

/// @brief `focus` と同じですが、カーソル形状を点滅バーに設定します。
/// @see focus
/// @see focusCursorBlock
/// @see focusCursorBlockBlinking
/// @see focusCursorBar
/// @see focusCursorBarBlinking
/// @see focusCursorUnderline
/// @see focusCursorUnderlineBlinking
/// @ingroup dom
Element focusCursorBarBlinking(Element child) {
  return std::make_shared<FocusCursor>(unpack(std::move(child)),
                                       Screen::Cursor::BarBlinking);
}

/// @brief `focus` と同じですが、カーソル形状を静止下線に設定します。
/// @see focus
/// @see focusCursorBlock
/// @see focusCursorBlockBlinking
/// @see focusCursorBar
/// @see focusCursorBarBlinking
/// @see focusCursorUnderline
/// @see focusCursorUnderlineBlinking
/// @ingroup dom
Element focusCursorUnderline(Element child) {
  return std::make_shared<FocusCursor>(unpack(std::move(child)),
                                       Screen::Cursor::Underline);
}

/// @brief `focus` と同じですが、カーソル形状を点滅下線に設定します。
/// @see focus
/// @see focusCursorBlock
/// @see focusCursorBlockBlinking
/// @see focusCursorBar
/// @see focusCursorBarBlinking
/// @see focusCursorUnderline
/// @see focusCursorUnderlineBlinking
/// @ingroup dom
Element focusCursorUnderlineBlinking(Element child) {
  return std::make_shared<FocusCursor>(unpack(std::move(child)),
                                       Screen::Cursor::UnderlineBlinking);
}

}  // namespace ftxui