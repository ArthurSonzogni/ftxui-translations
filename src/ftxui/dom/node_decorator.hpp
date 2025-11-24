// Copyright 2020 Arthur Sonzogni. 無断複写・転載を禁じます。
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスによって管理されます。
// the LICENSE file.
#ifndef FTXUI_DOM_NODE_DECORATOR_H_
#define FTXUI_DOM_NODE_DECORATOR_H_

#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Element, unpack
#include "ftxui/dom/node.hpp"      // for Node

namespace ftxui {
struct Box;

// ヘルパークラス。
class NodeDecorator : public Node {
 public:
  explicit NodeDecorator(Element child) : Node(unpack(std::move(child))) {}
  void ComputeRequirement() override;
  void SetBox(Box box) override;
};

}  // namespace ftxui

#endif /* インクルードガードの終わり: FTXUI_DOM_NODE_DECORATOR_H_ */