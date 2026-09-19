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
    // 覆寫 select 方法，使其不執行任何操作。
  }
};
}  // namespace

/// @brief 建立一個空的選取範圍。
Selection::Selection() = default;

/// @brief 建立一個選取範圍。
/// @param start_x 選取範圍起點的 x 座標。
/// @param start_y 選取範圍起點的 y 座標。
/// @param end_x 選取範圍終點的 x 座標。
/// @param end_y 選取範圍終點的 y 座標。
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

/// @brief 取得選取範圍的方塊。
/// @return 選取範圍的方塊。
const Box& Selection::GetBox() const {
  return box_;
}

/// @brief 將選取範圍飽和在方塊內。
/// 這是由 `hbox` 呼叫，用於將選取範圍傳播到其子元素。
/// @param box 用於飽和選取範圍的方塊。
/// @return 飽和後的選取範圍。
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

/// @brief 將選取範圍飽和在方塊內。
/// 這是由 `vbox` 呼叫，用於將選取範圍傳播到其子元素。
/// @param box 用於飽和選取範圍的方塊。
/// @return 飽和後的選取範圍。
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

    // 先前記錄的部分與此部分之間存在一段空白
    // 儲存格的水平間隙。這類間隙來自於使用空欄
    // （而非實際空格字元）來分隔可選取文字的版面配置
    // （例如 FlexboxConfig::SetGap 產生的 flexbox 間隙、
    // 填充物、間距裝飾器）。這些儲存格位於選取
    // 範圍之內，且在畫面上會顯示為空格，因此複製的文字
    // 也必須包含它們。僅有向前的間隙會被填補；重疊或
    // 順序不對的部分則會退回到單純的附加動作。
    for (int x = x_ + 1; x < left; ++x) {
      parts_ << ' ';
    }
    parts_ << part;
  }();
  y_ = y;
  x_ = right;
}

}  // namespace ftxui
