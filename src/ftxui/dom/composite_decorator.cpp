// Copyright 2020 Arthur Sonzogni. All rights reserved.
// 本源代码的使用受可在 LICENSE 文件中找到的 MIT 许可的约束。

#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Element, filler, operator|, hbox, flex_grow, vbox, xflex_grow, yflex_grow, align_right, center, hcenter, vcenter

namespace ftxui {

/// @brief 水平居中一个元素。
/// @param child 被装饰的元素。
/// @return 居中的元素。
/// @ingroup dom
Element hcenter(Element child) {
  return hbox(filler(), std::move(child), filler());
}

/// @brief 垂直居中一个元素。
/// @param child 被装饰的元素。
/// @return 居中的元素。
/// @ingroup dom
Element vcenter(Element child) {
  return vbox(filler(), std::move(child), filler());
}

/// @brief 水平并垂直居中一个元素。
/// @param child 被装饰的元素。
/// @return 居中的元素。
/// @ingroup dom
Element center(Element child) {
  return hcenter(vcenter(std::move(child)));
}

/// @brief 将元素右对齐。
/// @param child 被装饰的元素。
/// @return 右对齐的元素。
/// @ingroup dom
Element align_right(Element child) {
  return hbox(filler(), std::move(child));
}

}  // namespace ftxui
