// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスに従います。
#include <ftxui/dom/node.hpp>  // for Node, Elements

#include "ftxui/dom/node_decorator.hpp"
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box

namespace ftxui {

void NodeDecorator::ComputeRequirement() {
  Node::ComputeRequirement();
  requirement_ = children_[0]->requirement();
}

void NodeDecorator::SetBox(Box box) {
  Node::SetBox(box);
  children_[0]->SetBox(box);
}

}  // namespace ftxui
