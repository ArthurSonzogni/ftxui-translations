// Copyright 2020 Arthur Sonzogni. 保留所有权利。
// 本源代码的使用受可在 LICENSE 文件中找到的 MIT 许可协议的约束。
#ifndef FTXUI_DOM_NODE_DECORATOR_H_
#define FTXUI_DOM_NODE_DECORATOR_H_

#include <utility>  // 用于移动

#include "ftxui/dom/elements.hpp"  // 用于 Element，解包
#include "ftxui/dom/node.hpp"      // 用于 Node

namespace ftxui {
struct Box;

// 辅助类。
class NodeDecorator : public Node {
 public:
  explicit NodeDecorator(Element child) : Node(unpack(std::move(child))) {}
  void ComputeRequirement() override;
  void SetBox(Box box) override;
};

}  // namespace ftxui

#endif /* include guard 结束: FTXUI_DOM_NODE_DECORATOR_H_ */
