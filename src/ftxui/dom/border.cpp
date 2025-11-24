// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに準拠します。
#include <algorithm>               // for max
#include <array>                   // for array
#include <ftxui/screen/color.hpp>  // for Color
#include <memory>    // for allocator, make_shared, __shared_ptr_access
#include <optional>  // for optional, nullopt
#include <string>    // for basic_string, string
#include <utility>   // for move

#include "ftxui/dom/elements.hpp"  // for unpack, Element, Decorator, BorderStyle, ROUNDED, borderStyled, Elements, DASHED, DOUBLE, EMPTY, HEAVY, LIGHT, border, borderDashed, borderDouble, borderEmpty, borderHeavy, borderLight, borderRounded, borderWith, window
#include "ftxui/dom/node.hpp"      // for Node, Elements
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/pixel.hpp"     // for Pixel
#include "ftxui/screen/screen.hpp"    // for Pixel, Screen

namespace ftxui {

namespace {
using Charset = std::array<std::string, 6>;  // NOLINT
using Charsets = std::array<Charset, 6>;     // NOLINT
// NOLINTNEXTLINE
static Charsets simple_border_charset = {
    Charset{"┌", "┐", "└", "┘", "─", "│"},  // LIGHT
    Charset{"┏", "┓", "┗", "┛", "╍", "╏"},  // DASHED
    Charset{"┏", "┓", "┗", "┛", "━", "┃"},  // HEAVY
    Charset{"╔", "╗", "╚", "╝", "═", "║"},  // DOUBLE
    Charset{"╭", "╮", "╰", "╯", "─", "│"},  // ROUNDED
    Charset{" ", " ", " ", " ", " ", " "},  // EMPTY
};

// 参考までに、通常のボーダーの文字セットを以下に示します:
 public:
  Border(Elements children,
         BorderStyle style,
         std::optional<Color> foreground_color = std::nullopt)
      : Node(std::move(children)),
        charset_(simple_border_charset[style])  // NOLINT
        ,
        foreground_color_(foreground_color) {}  // NOLINT

  const Charset& charset_;  // NOLINT
  std::optional<Color> foreground_color_;

  void ComputeRequirement() override {
    Node::ComputeRequirement();
    requirement_ = children_[0]->requirement();
    requirement_.min_x += 2;
    requirement_.min_y += 2;
    if (children_.size() == 2) {
      requirement_.min_x =
          std::max(requirement_.min_x, children_[1]->requirement().min_x + 2);
    }
    requirement_.focused.box.x_min++;
    requirement_.focused.box.x_max++;
    requirement_.focused.box.y_min++;
    requirement_.focused.box.y_max++;
  }

  void SetBox(Box box) override {
    Node::SetBox(box);
    if (children_.size() == 2) {
      Box title_box;
      title_box.x_min = box.x_min + 1;
      title_box.x_max = std::min(box.x_max - 1,
                                 box.x_min + children_[1]->requirement().min_x);
      title_box.y_min = box.y_min;
      title_box.y_max = box.y_min;
      children_[1]->SetBox(title_box);
    }
    box.x_min++;
    box.x_max--;
    box.y_min++;
    box.y_max--;
    children_[0]->SetBox(box);
  }

  void Render(Screen& screen) override {
    // コンテンツを描画します。
    children_[0]->Render(screen);

    // ボーダーを描画します。
    if (box_.x_min >= box_.x_max || box_.y_min >= box_.y_max) {
      return;
    }

    screen.at(box_.x_min, box_.y_min) = charset_[0];  // NOLINT
    screen.at(box_.x_max, box_.y_min) = charset_[1];  // NOLINT
    screen.at(box_.x_min, box_.y_max) = charset_[2];  // NOLINT
    screen.at(box_.x_max, box_.y_max) = charset_[3];  // NOLINT

    for (int x = box_.x_min + 1; x < box_.x_max; ++x) {
      Pixel& p1 = screen.PixelAt(x, box_.y_min);
      Pixel& p2 = screen.PixelAt(x, box_.y_max);
      p1.character = charset_[4];  // NOLINT
      p2.character = charset_[4];  // NOLINT
      p1.automerge = true;
      p2.automerge = true;
    }
    for (int y = box_.y_min + 1; y < box_.y_max; ++y) {
      Pixel& p3 = screen.PixelAt(box_.x_min, y);
      Pixel& p4 = screen.PixelAt(box_.x_max, y);
      p3.character = charset_[5];  // NOLINT
      p4.character = charset_[5];  // NOLINT
      p3.automerge = true;
      p4.automerge = true;
    }

    // タイトルを描画します。
    if (children_.size() == 2) {
      children_[1]->Render(screen);
    }

    // ボーダーの色を描画します。
    if (foreground_color_) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        screen.PixelAt(x, box_.y_min).foreground_color = *foreground_color_;
        screen.PixelAt(x, box_.y_max).foreground_color = *foreground_color_;
      }
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        screen.PixelAt(box_.x_min, y).foreground_color = *foreground_color_;
        screen.PixelAt(box_.x_max, y).foreground_color = *foreground_color_;
      }
    }
  }
};

// 参考までに、通常のボーダーの文字セットを以下に示します:
class BorderPixel : public Node {
 public:
  BorderPixel(Elements children, Pixel pixel)
      : Node(std::move(children)), pixel_(std::move(pixel)) {}

 private:
  Pixel pixel_;

  void ComputeRequirement() override {
    Node::ComputeRequirement();
    requirement_ = children_[0]->requirement();
    requirement_.min_x += 2;
    requirement_.min_y += 2;
    if (children_.size() == 2) {
      requirement_.min_x =
          std::max(requirement_.min_x, children_[1]->requirement().min_x + 2);
    }

    requirement_.focused.box.Shift(1, 1);
  }

  void SetBox(Box box) override {
    Node::SetBox(box);
    if (children_.size() == 2) {
      Box title_box;
      title_box.x_min = box.x_min + 1;
      title_box.x_max = box.x_max - 1;
      title_box.y_min = box.y_min;
      title_box.y_max = box.y_min;
      children_[1]->SetBox(title_box);
    }
    box.x_min++;
    box.x_max--;
    box.y_min++;
    box.y_max--;
    children_[0]->SetBox(box);
  }

  void Render(Screen& screen) override {
    // コンテンツを描画します。
    children_[0]->Render(screen);

    // ボーダーを描画します。
    if (box_.x_min >= box_.x_max || box_.y_min >= box_.y_max) {
      return;
    }

    screen.PixelAt(box_.x_min, box_.y_min) = pixel_;
    screen.PixelAt(box_.x_max, box_.y_min) = pixel_;
    screen.PixelAt(box_.x_min, box_.y_max) = pixel_;
    screen.PixelAt(box_.x_max, box_.y_max) = pixel_;

    for (int x = box_.x_min + 1; x < box_.x_max; ++x) {
      screen.PixelAt(x, box_.y_min) = pixel_;
      screen.PixelAt(x, box_.y_max) = pixel_;
    }
    for (int y = box_.y_min + 1; y < box_.y_max; ++y) {
      screen.PixelAt(box_.x_min, y) = pixel_;
      screen.PixelAt(box_.x_max, y) = pixel_;
    }
  }
};
}  // namespace

/// @brief 要素の周囲にボーダーを描画します。
/// @ingroup dom
/// @see border
/// @see borderLight
/// @see borderDashed
/// @see borderDouble
/// @see borderHeavy
/// @see borderEmpty
/// @see borderRounded
/// @see borderStyled
/// @see borderWith
///
/// 要素の周囲にボーダーを追加します
///
/// ### 例
///
/// ```cpp
/// // 関数として'border'を使用する...
/// Element document = border(text("The element"));
///
/// // ...または'パイプ'として。
/// Element document = text("The element") | border;
/// ```
///
/// ### 出力
///
/// ```bash
/// ┌───────────┐
/// │The element│
/// └───────────┘
/// ```
Element border(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), ROUNDED);
}

/// @brief borderと同じですが、要素の周囲に一定のピクセルを使用します。
/// @ingroup dom
/// @see border
Decorator borderWith(const Pixel& pixel) {
  return [pixel](Element child) {
    return std::make_shared<BorderPixel>(unpack(std::move(child)), pixel);
  };
}

/// @brief borderと同じですが、異なるスタイルを使用します。
/// @ingroup dom
/// @see border
Decorator borderStyled(BorderStyle style) {
  return [style](Element child) {
    return std::make_shared<Border>(unpack(std::move(child)), style);
  };
}

/// @brief borderと同じですが、前景色を使用します。
/// @ingroup dom
/// @see border
Decorator borderStyled(Color foreground_color) {
  return [foreground_color](Element child) {
    return std::make_shared<Border>(unpack(std::move(child)), ROUNDED,
                                    foreground_color);
  };
}

/// @brief borderと同じですが、前景色と異なるスタイルを使用します
/// @ingroup dom
/// @see border
Decorator borderStyled(BorderStyle style, Color foreground_color) {
  return [style, foreground_color](Element child) {
    return std::make_shared<Border>(unpack(std::move(child)), style,
                                    foreground_color);
  };
}

/// @brief 要素の周囲に破線のボーダーを描画します。
/// @ingroup dom
/// @see border
/// @see borderLight
/// @see borderDashed
/// @see borderDouble
/// @see borderHeavy
/// @see borderRounded
/// @see borderEmpty
/// @see borderStyled
/// @see borderWith
///
/// 要素の周囲にボーダーを追加します
///
/// ### 例
///
/// ```cpp
/// // 関数として'borderDash'を使用する...
/// Element document = borderDash(text("The element"));
///
/// // ...または'パイプ'として。
/// Element document = text("The element") | borderDAsh;
/// ```
///
/// ### 出力
///
/// ```bash
/// ┏╍╍╍╍╍╍╍╍╍╍╍╍╍╍┓
/// ╏The element   ╏
/// ┗╍╍╍╍╍╍╍╍╍╍╍╍╍╍┛
/// ```
Element borderDashed(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), DASHED);
}

/// @brief 要素の周囲に細いボーダーを描画します。
/// @ingroup dom
/// @see border
/// @see borderLight
/// @see borderDashed
/// @see borderDouble
/// @see borderHeavy
/// @see borderRounded
/// @see borderEmpty
/// @see borderStyled
/// @see borderWith
///
/// 要素の周囲にボーダーを追加します
///
/// ### 例
///
/// ```cpp
/// // 関数として'borderLight'を使用する...
/// Element document = borderLight(text("The element"));
///
/// // ...または'パイプ'として。
/// Element document = text("The element") | borderLight;
/// ```
///
/// ### 出力
///
/// ```bash
/// ┌──────────────┐
/// │The element   │
/// └──────────────┘
/// ```
Element borderLight(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), LIGHT);
}

/// @brief 要素の周囲に太いボーダーを描画します。
/// @ingroup dom
/// @see border
/// @see borderLight
/// @see borderDashed
/// @see borderDouble
/// @see borderHeavy
/// @see borderRounded
/// @see borderEmpty
/// @see borderStyled
/// @see borderWith
///
/// 要素の周囲にボーダーを追加します
///
/// ### 例
///
/// ```cpp
/// // 関数として'borderHeavy'を使用する...
/// Element document = borderHeavy(text("The element"));
///
/// // ...または'パイプ'として。
/// Element document = text("The element") | borderHeavy;
/// ```
///
/// ### 出力
///
/// ```bash
/// ┏━━━━━━━━━━━━━━┓
/// ┃The element   ┃
/// ┗━━━━━━━━━━━━━━┛
/// ```
Element borderHeavy(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), HEAVY);
}

/// @brief 要素の周囲に二重線のボーダーを描画します。
/// @ingroup dom
/// @see border
/// @see borderLight
/// @see borderDashed
/// @see borderDouble
/// @see borderHeavy
/// @see borderRounded
/// @see borderEmpty
/// @see borderStyled
/// @see borderWith
///
/// 要素の周囲にボーダーを追加します
///
/// ### 例
///
/// ```cpp
/// // 関数として'borderDouble'を使用する...
/// Element document = borderDouble(text("The element"));
///
/// // ...または'パイプ'として。
/// Element document = text("The element") | borderDouble;
/// ```
///
/// ### 出力
///
/// ```bash
/// ╔══════════════╗
/// ║The element   ║
/// ╚══════════════╝
/// ```
Element borderDouble(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), DOUBLE);
}

/// @brief 要素の周囲に角の丸いボーダーを描画します。
/// @ingroup dom
/// @see border
/// @see borderLight
/// @see borderDashed
/// @see borderDouble
/// @see borderHeavy
/// @see borderRounded
/// @see borderEmpty
/// @see borderStyled
/// @see borderWith
///
/// 要素の周囲にボーダーを追加します
///
/// ### 例
///
/// ```cpp
/// // 関数として'borderRounded'を使用する...
/// Element document = borderRounded(text("The element"));
///
/// // ...または'パイプ'として。
/// Element document = text("The element") | borderRounded;
/// ```
///
/// ### 出力
///
/// ```bash
/// ╭──────────────╮
/// │The element   │
/// ╰──────────────╯
/// ```
Element borderRounded(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), ROUNDED);
}

/// @brief 要素の周囲に空のボーダーを描画します。
/// @ingroup dom
/// @see border
/// @see borderLight
/// @see borderDashed
/// @see borderDouble
/// @see borderHeavy
/// @see borderRounded
/// @see borderEmpty
/// @see borderStyled
/// @see borderWith
///
/// 要素の周囲にボーダーを追加します
///
/// ### 例
///
/// ```cpp
/// // 関数として'borderRounded'を使用する...
/// Element document = borderRounded(text("The element"));
///
/// // ...または'パイプ'として。
/// Element document = text("The element") | borderRounded;
/// ```
///
/// ### 出力
///
/// ```bash
///
///  The element
///
/// ```
Element borderEmpty(Element child) {
  return std::make_shared<Border>(unpack(std::move(child)), EMPTY);
}

/// @brief 要素の周囲にタイトルとボーダーを持つウィンドウを描画します。
/// @param title ウィンドウのタイトル。
/// @param content ラップする要素。
/// @param border ボーダーのスタイル。デフォルトはROUNDEDです。
/// @ingroup dom
/// @see border
///
/// ### 例
///
/// ```cpp
/// Element document = window(text("Title"),
///                           text("content")
///                    );
///
/// // ボーダーを指定する場合
/// Element document = window(text("Title"),
///                           text("content"),
///                           ROUNDED
///                    );
/// ```
///
/// ### 出力
///
/// ```bash
/// ┌Title──┐
/// │content│
/// └───────┘
/// ```
Element window(Element title, Element content, BorderStyle border) {
  return std::make_shared<Border>(unpack(std::move(content), std::move(title)),
                                  border);
}
}  // namespace ftxui
