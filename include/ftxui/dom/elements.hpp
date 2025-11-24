// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスによって管理されています。
#ifndef FTXUI_DOM_ELEMENTS_HPP
#define FTXUI_DOM_ELEMENTS_HPP

#include <functional>
#include <memory>

#include "ftxui/dom/canvas.hpp"
#include "ftxui/dom/direction.hpp"
#include "ftxui/dom/flexbox_config.hpp"
#include "ftxui/dom/linear_gradient.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/box.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/screen/terminal.hpp"
#include "ftxui/util/ref.hpp"

namespace ftxui {
class Node;
using Element = std::shared_ptr<Node>;
using Elements = std::vector<Element>;
using Decorator = std::function<Element(Element)>;
using GraphFunction = std::function<std::vector<int>(int, int)>;

/// @brief BorderStyleは、ターミナルUIの要素に適用できる様々なボーダースタイルを表す列挙型です。
///
/// BorderStyleは、ターミナルUIの要素に適用できる様々なボーダースタイルを表す列挙型です。
/// ウィンドウ、フレーム、セパレータなどの要素の周囲の境界線の視覚的な外観を定義するために使用されます。
/// @ingroup dom
enum BorderStyle {
  LIGHT,
  DASHED,
  HEAVY,
  DOUBLE,
  ROUNDED,
  EMPTY,
};

// 要素をデコレータにまとめてパイプします。
// 例えば、次の行は同等です。
// -> text("ftxui") | bold | underlined
// -> underlined(bold(text("FTXUI")))
Element operator|(Element, Decorator);
Element& operator|=(Element&, Decorator);
Elements operator|(Elements, Decorator);
Decorator operator|(Decorator, Decorator);

// --- ウィジェット ---
Element text(std::string text);
Element vtext(std::string text);
Element separator();
Element separatorLight();
Element separatorDashed();
Element separatorHeavy();
Element separatorDouble();
Element separatorEmpty();
Element separatorStyled(BorderStyle);
Element separator(Pixel);
Element separatorCharacter(std::string);
Element separatorHSelector(float left,
                           float right,
                           Color unselected_color,
                           Color selected_color);
Element separatorVSelector(float up,
                           float down,
                           Color unselected_color,
                           Color selected_color);
Element gauge(float progress);
Element gaugeLeft(float progress);
Element gaugeRight(float progress);
Element gaugeUp(float progress);
Element gaugeDown(float progress);
Element gaugeDirection(float progress, Direction direction);
Element border(Element);
Element borderLight(Element);
Element borderDashed(Element);
Element borderHeavy(Element);
Element borderDouble(Element);
Element borderRounded(Element);
Element borderEmpty(Element);
Decorator borderStyled(BorderStyle);
Decorator borderStyled(BorderStyle, Color);
Decorator borderStyled(Color);
Decorator borderWith(const Pixel&);
Element window(Element title, Element content, BorderStyle border = ROUNDED);
Element spinner(int charset_index, size_t image_index);
Element paragraph(const std::string& text);
Element paragraphAlignLeft(const std::string& text);
Element paragraphAlignRight(const std::string& text);
Element paragraphAlignCenter(const std::string& text);
Element paragraphAlignJustify(const std::string& text);
Element graph(GraphFunction);
Element emptyElement();
Element canvas(ConstRef<Canvas>);
Element canvas(int width, int height, std::function<void(Canvas&)>);
Element canvas(std::function<void(Canvas&)>);

// -- デコレーター ---
Element bold(Element);
Element dim(Element);
Element italic(Element);
Element inverted(Element);
Element underlined(Element);
Element underlinedDouble(Element);
Element blink(Element);
Element strikethrough(Element);
Decorator color(Color);
Decorator bgcolor(Color);
Decorator color(const LinearGradient&);
Decorator bgcolor(const LinearGradient&);
Element color(Color, Element);
Element bgcolor(Color, Element);
Element color(const LinearGradient&, Element);
Element bgcolor(const LinearGradient&, Element);
Decorator focusPosition(int x, int y);
Decorator focusPositionRelative(float x, float y);
Element automerge(Element child);
Decorator hyperlink(std::string link);
Element hyperlink(std::string link, Element child);
Element selectionStyleReset(Element);
Decorator selectionColor(Color foreground);
Decorator selectionBackgroundColor(Color foreground);
Decorator selectionForegroundColor(Color foreground);
Decorator selectionStyle(std::function<void(Pixel&)> style);

// --- レイアウト
// 水平、垂直、または積み重ねられた要素のセット。
Element hbox(Elements);
Element vbox(Elements);
Element dbox(Elements);
Element flexbox(Elements, FlexboxConfig config = FlexboxConfig());
Element gridbox(std::vector<Elements> lines);

Element hflow(Elements);  // Helper: default flexbox with row direction.
Element vflow(Elements);  // Helper: default flexbox with column direction.

// -- 柔軟性 ---
// コンテナ内で全てのスペースが使用されない場合に、残りのスペースをどのように共有するかを定義します。
Element flex(Element);         // 可能/必要に応じて拡大/縮小します。
Element flex_grow(Element);    // 可能であれば要素を拡大します。
Element flex_shrink(Element);  // 必要であれば要素を最小化します。

Element xflex(Element);         // X軸上で可能/必要に応じて拡大/縮小します。
Element xflex_grow(Element);    // X軸上で可能であれば要素を拡大します。
Element xflex_shrink(Element);  // X軸上で必要であれば要素を最小化します。

Element yflex(Element);         // Y軸上で可能/必要に応じて拡大/縮小します。
Element yflex_grow(Element);    // Y軸上で可能であれば要素を拡大します。
Element yflex_shrink(Element);  // Y軸上で必要であれば要素を最小化します。

Element notflex(Element);  // flex属性をリセットします。
Element filler();          // 空白の拡張可能な要素。

// -- サイズのオーバーライド
enum WidthOrHeight { WIDTH, HEIGHT };
enum Constraint { LESS_THAN, EQUAL, GREATER_THAN };
Decorator size(WidthOrHeight, Constraint, int value);

// --- フレーム ---
// フレームはスクロール可能な領域です。内部領域は外部領域よりも大きくなる可能性があります。
// フォーカスされた要素を見えるようにするために、内部領域がスクロールされます。
Element frame(Element);
Element xframe(Element);
Element yframe(Element);
Element focus(Element);
Element select(Element e);  // 非推奨 - focusのエイリアス。

// --- カーソル ---
// これらは`focus`に似ていますが、カーソルの形状も変更します。
Element focusCursorBlock(Element);
Element focusCursorBlockBlinking(Element);
Element focusCursorBar(Element);
Element focusCursorBarBlinking(Element);
Element focusCursorUnderline(Element);
Element focusCursorUnderlineBlinking(Element);

// --- その他 ---
Element vscroll_indicator(Element);
Element hscroll_indicator(Element);
Decorator reflect(Box& box);
// |element|を描画する前に、下のピクセルをクリアします。これはdboxとの組み合わせで役立ちます。
Element clear_under(Element element);

// --- ユーティリティ --------------------------------------------------------------------
Element hcenter(Element);
Element vcenter(Element);
Element center(Element);
Element align_right(Element);
Element nothing(Element element);

namespace Dimension {
Dimensions Fit(Element&, bool extend_beyond_screen = false);
}  // namespace Dimension

}  // namespace ftxui

// Make container able to take any number of children as input.
#include "ftxui/dom/take_any_args.hpp"

// Include old definitions using wstring.
#include "ftxui/dom/deprecated.hpp"
#endif  // FTXUI_DOM_ELEMENTS_HPP
