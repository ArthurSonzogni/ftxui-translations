// 版权所有 2020 Arthur Sonzogni. 保留所有权利。
// 此源代码的使用受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
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

// 将一组参数转换为一个向量。
template <class... Args>
Elements unpack(Args... args) {
  std::vector<Element> vec;
  (Merge(vec, std::move(args)), ...);
  return vec;
}

// 使 |container| 能够接受任意数量的参数。
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
    Elements elements;                                         \
    for (auto& child : children) {                             \
      elements.push_back(std::move(child));                    \
    }                                                          \
    return container(std::move(elements));                     \
  }                                                            \
  template <>                                                  \
  inline Element container(std::stack<Element>&& children) {   \
    Elements elements;                                         \
    while (!children.empty()) {                                \
      elements.push_back(std::move(children.top()));           \
      children.pop();                                          \
    }                                                          \
    return container(std::move(elements));                     \
  }                                                            \
  template <>                                                  \
  inline Element container(std::queue<Element>&& children) {   \
    Elements elements;                                         \
    while (!children.empty()) {                                \
      elements.push_back(std::move(children.front()));         \
      children.pop();                                          \
    }                                                          \
    return container(std::move(elements));                     \
  }                                                            \
  template <>                                                  \
  inline Element container(std::deque<Element>&& children) {   \
    Elements elements;                                         \
    for (auto& child : children) {                             \
      elements.push_back(std::move(child));                    \
    }                                                          \
    return container(std::move(elements));                     \
  }

TAKE_ANY_ARGS(vbox)
TAKE_ANY_ARGS(hbox)
TAKE_ANY_ARGS(dbox)
TAKE_ANY_ARGS(hflow)
}  // namespace ftxui

#endif  // FTXUI_DOM_TAKE_ANY_ARGS_HPP
