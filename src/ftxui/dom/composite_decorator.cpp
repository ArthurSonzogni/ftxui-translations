// Copyright 2020 Arthur Sonzogni. 全著作権所有。
// このソースコードの使用は、以下に記載されているMITライセンスに準拠します。
// LICENSEファイル。

#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Element, filler, operator|, hbox, flex_grow, vbox, xflex_grow, yflex_grow, align_right, center, hcenter, vcenter

namespace ftxui {

/// @brief 要素を水平方向に中央揃えします。
/// @param child 装飾される要素。
/// @return 中央揃えされた要素。
/// @ingroup dom
Element hcenter(Element child) {
  return hbox(filler(), std::move(child), filler());
}

/// @brief 要素を垂直方向に中央揃えします。
/// @param child 装飾される要素。
/// @return 中央揃えされた要素。
/// @ingroup dom
Element vcenter(Element child) {
  return vbox(filler(), std::move(child), filler());
}

/// @brief 要素を水平方向および垂直方向に中央揃えします。
/// @param child 装飾される要素。
/// @return 中央揃えされた要素。
/// @ingroup dom
Element center(Element child) {
  return hcenter(vcenter(std::move(child)));
}

/// @brief 要素を右側に揃えます。
/// @param child 装飾される要素。
/// @return 右揃えされた要素。
/// @ingroup dom
Element align_right(Element child) {
  return hbox(filler(), std::move(child));
}

}  // namespace ftxui
