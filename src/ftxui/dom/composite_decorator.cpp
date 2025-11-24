// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Element, filler, operator|, hbox, flex_grow, vbox, xflex_grow, yflex_grow, align_right, center, hcenter, vcenter

namespace ftxui {

/// @brief 水平置中一個元素。
/// @param child 被裝飾的元素。
/// @return 水平置中的元素。
/// @ingroup dom
Element hcenter(Element child) {
  return hbox(filler(), std::move(child), filler());
}

/// @brief 垂直置中一個元素。
/// @param child 被裝飾的元素。
/// @return 垂直置中的元素。
/// @ingroup dom
Element vcenter(Element child) {
  return vbox(filler(), std::move(child), filler());
}

/// @brief 水平與垂直置中一個元素。
/// @param child 被裝飾的元素。
/// @return 置中的元素。
/// @ingroup dom
Element center(Element child) {
  return hcenter(vcenter(std::move(child)));
}

/// @brief 將元素靠右對齊。
/// @param child 被裝飾的元素。
/// @return 靠右對齊的元素。
/// @ingroup dom
Element align_right(Element child) {
  return hbox(filler(), std::move(child));
}

}  // namespace ftxui