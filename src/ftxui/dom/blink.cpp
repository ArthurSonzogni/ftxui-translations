// Copyright 2020 Arthur Sonzogni. 保留所有权利。
// 此源代码受 MIT 许可证的约束，可在
// LICENSE 文件中找到。
#include <memory>   // for make_shared
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"        // for Element, blink
#include "ftxui/dom/node.hpp"            // for Node
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/box.hpp"          // for Box
#include "ftxui/screen/screen.hpp"       // for Pixel, Screen

namespace ftxui {

namespace {
class Blink : public NodeDecorator {
 public:
  using NodeDecorator::NodeDecorator;

  void Render(Screen& screen) override {
    Node::Render(screen);
    for (int y = box_.y_min; y <= box_.y_max; ++y) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        screen.PixelAt(x, y).blink = true;
      }
    }
  }
};
}  // namespace

/// @brief 绘制的文本在可见和隐藏之间交替。
/// @ingroup dom
Element blink(Element child) {
  return std::make_shared<Blink>(std::move(child));
}

}  // namespace ftxui
