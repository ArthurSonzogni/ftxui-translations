// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#include "ftxui/dom/selection.hpp"  // for Selection
#include <algorithm>                // for max, min
#include <string_view>
#include <tuple>  // for ignore

#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator

namespace ftxui {

namespace {
class Unselectable : public NodeDecorator {
 public:
  using NodeDecorator::NodeDecorator;

  void Select(Selection& ignored) override {
    std::ignore = ignored;
    // select メソッドを上書きして何も行わない。
  }
};
}  // namespace

/// @brief 空の選択を作成します。
Selection::Selection() = default;

/// @brief 選択を作成します。
/// @param start_x 選択範囲の開始X座標。
/// @param start_y 選択範囲の開始Y座標。
/// @param end_x 選択範囲の終了X座標。
/// @param end_y 選択範囲の終了Y座標。
Selection::Selection(int start_x, int start_y, int end_x, int end_y)
    : start_x_(start_x),
      start_y_(start_y),
      end_x_(end_x),
      end_y_(end_y),
      box_{
          std::min(start_x, end_x),
          std::max(start_x, end_x),
          std::min(start_y, end_y),
          std::max(start_y, end_y),
      },
      empty_(false) {}

Selection::Selection(int start_x,
                     int start_y,
                     int end_x,
                     int end_y,
                     Selection* parent)
    : start_x_(start_x),
      start_y_(start_y),
      end_x_(end_x),
      end_y_(end_y),
      box_{
          std::min(start_x, end_x),
          std::max(start_x, end_x),
          std::min(start_y, end_y),
          std::max(start_y, end_y),
      },
      parent_(parent),
      empty_(false) {}

/// @brief 選択範囲のボックスを取得します。
/// @return 選択範囲のボックス。
const Box& Selection::GetBox() const {
  return box_;
}

/// @brief 選択範囲をボックス内に収めます。
/// これは`hbox`によって、選択範囲をその子に伝播するために呼び出されます。
/// @param box 選択範囲を収めるボックス。
/// @return 収められた選択範囲。
Selection Selection::SaturateHorizontal(Box box) {
  int start_x = start_x_;
  int start_y = start_y_;
  int end_x = end_x_;
  int end_y = end_y_;

  const bool start_outside = !box.Contain(start_x, start_y);
  const bool end_outside = !box.Contain(end_x, end_y);
  const bool properly_ordered =
      start_y < end_y || (start_y == end_y && start_x <= end_x);
  if (properly_ordered) {
    if (start_outside) {
      start_x = box.x_min;
      start_y = box.y_min;
    }
    if (end_outside) {
      end_x = box.x_max;
      end_y = box.y_max;
    }
  } else {
    if (start_outside) {
      start_x = box.x_max;
      start_y = box.y_max;
    }
    if (end_outside) {
      end_x = box.x_min;
      end_y = box.y_min;
    }
  }
  return {
      start_x, start_y, end_x, end_y, parent_,
  };
}

/// @brief 選択範囲をボックス内に収めます。
/// これは`vbox`によって、選択範囲をその子に伝播するために呼び出されます。
/// @param box 選択範囲を収めるボックス。
/// @return 収められた選択範囲。
Selection Selection::SaturateVertical(Box box) {
  int start_x = start_x_;
  int start_y = start_y_;
  int end_x = end_x_;
  int end_y = end_y_;

  const bool start_outside = !box.Contain(start_x, start_y);
  const bool end_outside = !box.Contain(end_x, end_y);
  const bool properly_ordered =
      start_y < end_y || (start_y == end_y && start_x <= end_x);

  if (properly_ordered) {
    if (start_outside) {
      start_x = box.x_min;
      start_y = box.y_min;
    }
    if (end_outside) {
      end_x = box.x_max;
      end_y = box.y_max;
    }
  } else {
    if (start_outside) {
      start_x = box.x_max;
      start_y = box.y_max;
    }
    if (end_outside) {
      end_x = box.x_min;
      end_y = box.y_min;
    }
  }
  return {start_x, start_y, end_x, end_y, parent_};
}

void Selection::AddPart(std::string_view part, int y, int left, int right) {
  if (parent_ != this) {
    parent_->AddPart(part, y, left, right);
    return;
  }
  [&] {
    if (parts_.str().empty()) {
      parts_ << part;
      return;
    }

    if (y_ != y) {
      parts_ << '\n' << part;
      return;
    }

    if (x_ == left + 1) {
      parts_ << part;
      return;
    }

    // 以前に記録された部分とこの部分の間に、空白セルの水平方向の隙間が
    // ある。このような隙間は、選択可能なテキストを実際のスペース文字
    // ではなく空の列で区切るレイアウト(例: FlexboxConfig::SetGapによる
    // flexboxのgap、filler、spacingデコレータ)から生じる。これらのセルは
    // 選択範囲内にあり、画面上ではスペースとして読み取られるため、
    // コピーされるテキストにもそれらを含める必要がある。順方向の隙間
    // のみが埋められる。重複または順序が乱れた部分は単純な追加に
    // フォールバックする。
    for (int x = x_ + 1; x < left; ++x) {
      parts_ << ' ';
    }
    parts_ << part;
  }();
  y_ = y;
  x_ = right;
}

}  // namespace ftxui
