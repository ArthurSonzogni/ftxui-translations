// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_NODE_DECORATOR_H_
#define FTXUI_DOM_NODE_DECORATOR_H_

#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Element, unpack
#include "ftxui/dom/node.hpp"      // for Node
#include "ftxui/util/export.hpp"   // for FTXUI_EXPORT

namespace ftxui {
struct Box;

// ヘルパークラス。
// 'component'ライブラリ用にエクスポートされた内部クラス。
class FTXUI_EXPORT(DOM) NodeDecorator : public Node {
 public:
  explicit NodeDecorator(Element child) : Node(unpack(std::move(child))) {}
  void ComputeRequirement() override;
  void SetBox(Box box) override;
};

}  // namespace ftxui

#endif /* インクルードガードの終わり: FTXUI_DOM_NODE_DECORATOR_H_ */
