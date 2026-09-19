// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_ELEMENTS_HPP
#define FTXUI_DOM_ELEMENTS_HPP

#include <cstdint>
#include <functional>
#include <memory>

#include <string_view>
#include "ftxui/dom/canvas.hpp"
#include "ftxui/dom/direction.hpp"
#include "ftxui/dom/flexbox_config.hpp"
#include "ftxui/dom/linear_gradient.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/box.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/screen/terminal.hpp"
#include "ftxui/util/export.hpp"
#include "ftxui/util/ref.hpp"

namespace ftxui {
class Node;
using Element = std::shared_ptr<Node>;
using Elements = std::vector<Element>;
using Decorator = std::function<Element(Element)>;
using GraphFunction = std::function<std::vector<int>(int, int)>;

/// @brief BorderStyle 是一個列舉，表示可以應用於終端機 UI 元素的不同邊框樣式。
///
/// BorderStyle 是一個列舉，表示可以應用於終端機 UI 元素的不同邊框樣式。
/// 它用於定義元素周圍邊框的視覺外觀，例如視窗、框架或分隔符。
/// @ingroup dom
enum BorderStyle : uint8_t {
  LIGHT,
  DASHED,
  HEAVY,
  DOUBLE,
  ROUNDED,
  EMPTY,
};

// 將元素以管線方式傳入裝飾器。
// 例如，以下幾行是等價的：
// -> text("ftxui") | bold | underlined
// -> underlined(bold(text("FTXUI")))
FTXUI_EXPORT(DOM) Element operator|(Element, Decorator);
FTXUI_EXPORT(DOM) Element& operator|=(Element&, Decorator);
FTXUI_EXPORT(DOM) Elements operator|(Elements, Decorator);
FTXUI_EXPORT(DOM) Decorator operator|(Decorator, Decorator);

// --- 元件 ---
FTXUI_EXPORT(DOM) Element text(std::string_view text);
FTXUI_EXPORT(DOM) Element vtext(std::string_view text);
FTXUI_EXPORT(DOM) Element separator();
FTXUI_EXPORT(DOM) Element separatorLight();
FTXUI_EXPORT(DOM) Element separatorDashed();
FTXUI_EXPORT(DOM) Element separatorHeavy();
FTXUI_EXPORT(DOM) Element separatorDouble();
FTXUI_EXPORT(DOM) Element separatorEmpty();
FTXUI_EXPORT(DOM) Element separatorStyled(BorderStyle);
FTXUI_EXPORT(DOM) Element separator(Cell);
FTXUI_EXPORT(DOM) Element separatorCharacter(std::string_view);
FTXUI_EXPORT(DOM)
Element separatorHSelector(float left,
                           float right,
                           Color unselected_color,
                           Color selected_color);
FTXUI_EXPORT(DOM)
Element separatorVSelector(float up,
                           float down,
                           Color unselected_color,
                           Color selected_color);
FTXUI_EXPORT(DOM) Element gauge(float progress);
FTXUI_EXPORT(DOM) Element gaugeLeft(float progress);
FTXUI_EXPORT(DOM) Element gaugeRight(float progress);
FTXUI_EXPORT(DOM) Element gaugeUp(float progress);
FTXUI_EXPORT(DOM) Element gaugeDown(float progress);
FTXUI_EXPORT(DOM) Element gaugeDirection(float progress, Direction direction);

// charset[0] 是「空」字元，charset.back() 是「滿」字元；
// 中間的項目則用於為邊界儲存格加上陰影。
FTXUI_EXPORT(DOM)
Element gaugeCharset(float progress,
                      std::vector<std::string> charset,
                      Direction direction = Direction::Right);
FTXUI_EXPORT(DOM) Element border(Element);
FTXUI_EXPORT(DOM) Element borderLight(Element);
FTXUI_EXPORT(DOM) Element borderDashed(Element);
FTXUI_EXPORT(DOM) Element borderHeavy(Element);
FTXUI_EXPORT(DOM) Element borderDouble(Element);
FTXUI_EXPORT(DOM) Element borderRounded(Element);
FTXUI_EXPORT(DOM) Element borderEmpty(Element);
FTXUI_EXPORT(DOM) Decorator borderStyled(BorderStyle);
FTXUI_EXPORT(DOM) Decorator borderStyled(BorderStyle, Color);
FTXUI_EXPORT(DOM) Decorator borderStyled(Color);
FTXUI_EXPORT(DOM) Decorator borderWith(const Cell&);
FTXUI_EXPORT(DOM)
Element window(Element title, Element content, BorderStyle border = ROUNDED);
FTXUI_EXPORT(DOM) Element spinner(int charset_index, size_t image_index);
FTXUI_EXPORT(DOM) Element paragraph(std::string_view text);
FTXUI_EXPORT(DOM) Element paragraphAlignLeft(std::string_view text);
FTXUI_EXPORT(DOM) Element paragraphAlignRight(std::string_view text);
FTXUI_EXPORT(DOM) Element paragraphAlignCenter(std::string_view text);
FTXUI_EXPORT(DOM) Element paragraphAlignJustify(std::string_view text);
FTXUI_EXPORT(DOM) Element graph(GraphFunction);
FTXUI_EXPORT(DOM) Element emptyElement();
FTXUI_EXPORT(DOM) Element canvas(ConstRef<Canvas>);
FTXUI_EXPORT(DOM)
Element canvas(int width, int height, std::function<void(Canvas&)>);
FTXUI_EXPORT(DOM) Element canvas(std::function<void(Canvas&)>);

// -- 裝飾器 ---
FTXUI_EXPORT(DOM) Element bold(Element);
FTXUI_EXPORT(DOM) Element dim(Element);
FTXUI_EXPORT(DOM) Element italic(Element);
FTXUI_EXPORT(DOM) Element inverted(Element);
FTXUI_EXPORT(DOM) Element underlined(Element);
FTXUI_EXPORT(DOM) Element underlinedDouble(Element);
FTXUI_EXPORT(DOM) Element blink(Element);
FTXUI_EXPORT(DOM) Element strikethrough(Element);
FTXUI_EXPORT(DOM) Decorator color(Color);
FTXUI_EXPORT(DOM) Decorator bgcolor(Color);
FTXUI_EXPORT(DOM) Decorator color(const LinearGradient&);
FTXUI_EXPORT(DOM) Decorator bgcolor(const LinearGradient&);
FTXUI_EXPORT(DOM) Element color(Color, Element);
FTXUI_EXPORT(DOM) Element bgcolor(Color, Element);
FTXUI_EXPORT(DOM) Element color(const LinearGradient&, Element);
FTXUI_EXPORT(DOM) Element bgcolor(const LinearGradient&, Element);
FTXUI_EXPORT(DOM) Decorator focusPosition(int x, int y);
FTXUI_EXPORT(DOM) Decorator focusPositionRelative(float x, float y);
FTXUI_EXPORT(DOM) Element automerge(Element child);
FTXUI_EXPORT(DOM) Decorator hyperlink(std::string_view link);
FTXUI_EXPORT(DOM) Element hyperlink(std::string_view link, Element child);
FTXUI_EXPORT(DOM) Element selectionStyleReset(Element);
FTXUI_EXPORT(DOM) Decorator selectionColor(Color foreground);
FTXUI_EXPORT(DOM) Decorator selectionBackgroundColor(Color foreground);
FTXUI_EXPORT(DOM) Decorator selectionForegroundColor(Color foreground);
FTXUI_EXPORT(DOM) Decorator selectionStyle(std::function<void(Cell&)> style);

// --- 布局是
// 水平、垂直或堆疊排列的元素集合。
FTXUI_EXPORT(DOM) Element hbox(Elements);
FTXUI_EXPORT(DOM) Element vbox(Elements);
FTXUI_EXPORT(DOM) Element dbox(Elements);
FTXUI_EXPORT(DOM)
Element flexbox(Elements, FlexboxConfig config = FlexboxConfig());
FTXUI_EXPORT(DOM) Element gridbox(std::vector<Elements> lines);

FTXUI_EXPORT(DOM)
Element hflow(Elements);  // 輔助函式：預設使用列方向的 flexbox。
FTXUI_EXPORT(DOM)
Element vflow(Elements);  // 輔助函式：預設使用欄方向的 flexbox。

// -- 彈性 ---
// 定義當容器內未使用全部空間時，如何分配容器內
// 剩餘空間。
FTXUI_EXPORT(DOM) Element flex(Element);  // 若可能/需要，展開/最小化。
FTXUI_EXPORT(DOM) Element flex_grow(Element);    // 若可能，展開元素。
FTXUI_EXPORT(DOM) Element flex_shrink(Element);  // 若需要，最小化元素。
FTXUI_EXPORT(DOM) Decorator flex_factor(int grow, int shrink);
FTXUI_EXPORT(DOM) Decorator flex_grow_factor(int grow);
FTXUI_EXPORT(DOM) Decorator flex_shrink_factor(int shrink);

FTXUI_EXPORT(DOM)
Element xflex(Element);  // 若可能/需要，在 X 軸上展開/最小化。
FTXUI_EXPORT(DOM)
Element xflex_grow(Element);  // 若可能，在 X 軸上展開元素。
FTXUI_EXPORT(DOM)
Element xflex_shrink(Element);  // 若需要，在 X 軸上最小化元素。
FTXUI_EXPORT(DOM) Decorator xflex_factor(int grow, int shrink);
FTXUI_EXPORT(DOM) Decorator xflex_grow_factor(int grow);
FTXUI_EXPORT(DOM) Decorator xflex_shrink_factor(int shrink);

FTXUI_EXPORT(DOM)
Element yflex(Element);  // 若可能/需要，在 Y 軸上展開/最小化。
FTXUI_EXPORT(DOM)
Element yflex_grow(Element);  // 若可能，在 Y 軸上展開元素。
FTXUI_EXPORT(DOM)
Element yflex_shrink(Element);  // 若需要，在 Y 軸上最小化元素。
FTXUI_EXPORT(DOM) Decorator yflex_factor(int grow, int shrink);
FTXUI_EXPORT(DOM) Decorator yflex_grow_factor(int grow);
FTXUI_EXPORT(DOM) Decorator yflex_shrink_factor(int shrink);

FTXUI_EXPORT(DOM) Element notflex(Element);  // 重設彈性屬性。
FTXUI_EXPORT(DOM) Element filler();          // 一個空白的可展開元素。

// -- 大小覆寫；
enum WidthOrHeight : uint8_t { WIDTH, HEIGHT };
enum Constraint : uint8_t { LESS_THAN, EQUAL, GREATER_THAN };
FTXUI_EXPORT(DOM) Decorator size(WidthOrHeight, Constraint, int value);

// --- 畫格 ---
// 畫格是一個可捲動的區域。內部區域可能比外部區域大得多。
// 內部區域會被捲動，以使目前聚焦的元素
// 保持可見。
FTXUI_EXPORT(DOM) Element frame(Element);
FTXUI_EXPORT(DOM) Element xframe(Element);
FTXUI_EXPORT(DOM) Element yframe(Element);
FTXUI_EXPORT(DOM) Element focus(Element);
FTXUI_EXPORT(DOM) Element select(Element e);  // 已棄用 - focus 的別名。

// --- 光標 ---
// 這些與 `focus` 相似，但也會改變光標的形狀。
FTXUI_EXPORT(DOM) Element focusCursorBlock(Element);
FTXUI_EXPORT(DOM) Element focusCursorBlockBlinking(Element);
FTXUI_EXPORT(DOM) Element focusCursorBar(Element);
FTXUI_EXPORT(DOM) Element focusCursorBarBlinking(Element);
FTXUI_EXPORT(DOM) Element focusCursorUnderline(Element);
FTXUI_EXPORT(DOM) Element focusCursorUnderlineBlinking(Element);

// --- 雜項 ---
FTXUI_EXPORT(DOM) Element vscroll_indicator(Element);
FTXUI_EXPORT(DOM) Element hscroll_indicator(Element);
FTXUI_EXPORT(DOM) Decorator reflect(Box& box);
// 在繪製 |element| 之前先清除下方的像素。這在
// 與 dbox 搭配使用時很有用。
FTXUI_EXPORT(DOM) Element clear_under(Element element);

// --- 工具 --------------------------------------------------------------------
FTXUI_EXPORT(DOM) Element hcenter(Element);
FTXUI_EXPORT(DOM) Element vcenter(Element);
FTXUI_EXPORT(DOM) Element center(Element);
FTXUI_EXPORT(DOM) Element align_right(Element);
FTXUI_EXPORT(DOM) Element nothing(Element element);

namespace Dimension {
FTXUI_EXPORT(DOM) Dimensions Fit(Element&, bool extend_beyond_screen = false);
}  // namespace Dimension

}  // namespace ftxui

// 使容器能夠接受任意數量的子元素作為輸入。
#include "ftxui/dom/take_any_args.hpp"

// 包含使用 wstring 的舊定義。
#include "ftxui/dom/deprecated.hpp"
#endif  // FTXUI_DOM_ELEMENTS_HPP
