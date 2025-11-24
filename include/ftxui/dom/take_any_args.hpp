// 版權所有 2020 Arthur Sonzogni。保留所有權利。
// 此原始碼的使用受 MIT 授權條款約束，詳情請參閱
// LICENSE 檔案。
#ifndef FTXUI_DOM_TAKE_ANY_ARGS_HPP
#define FTXUI_DOM_TAKE_ANY_ARGS_HPP

// IWYU pragma: private, include "ftxui/dom/elements.hpp"
#include <deque>
#include <ftxui/dom/node.hpp>
#include <queue>
#include <stack>
#include <vector>

namespace ftxui {

inline void Merge(Elements& container, Element element) {
  container.push_back(std::move(element));
}

// 將一組參數轉換為向量。
template <class... Args>
Elements unpack(Args... args) {
  std::vector<Element> vec;
  (Merge(vec, std::move(args)), ...);
  return vec;
}

// 使 |container| 能夠接受任意數量的參數。
#define TAKE_ANY_ARGS(container)                               \
  inline Element container(Element child) {                    \
    return container(unpack(std::move(child)));                \
  }                                                            \
                                                               \
  template <class... Args>                                     \
  inline Element container(Args... children) {                 \
    return container(unpack(std::forward<Args>(children)...)); \
  }                                                            \
                                                               \
  template <class Container>                                   \
  inline Element container(Container&& children) {             \
    Elements elements;
    for (auto& child : children) {
      elements.push_back(std::move(child));                    \
    }
    return container(std::move(elements));                     \
  }                                                            \
  template <>
  inline Element container(std::stack<Element>&& children) {   \
    Elements elements;
    while (!children.empty()) {
      elements.push_back(std::move(children.top()));           \
      children.pop();                                          \
    }
    return container(std::move(elements));                     \
  }                                                            \
  template <>
  inline Element container(std::queue<Element>&& children) {   \
    Elements elements;
    while (!children.empty()) {
      elements.push_back(std::move(children.front()));         \
      children.pop();                                          \
    }
    return container(std::move(elements));                     \
  }                                                            \
  template <>
  inline Element container(std::deque<Element>&& children) {   \
    Elements elements;
    for (auto& child : children) {
      elements.push_back(std::move(child));                    \
    }
    return container(std::move(elements));                     \
  }

TAKE_ANY_ARGS(vbox)
TAKE_ANY_ARGS(hbox)
TAKE_ANY_ARGS(dbox)
TAKE_ANY_ARGS(hflow)
}  // namespace ftxui

#endif  // FTXUI_DOM_TAKE_ANY_ARGS_HPP