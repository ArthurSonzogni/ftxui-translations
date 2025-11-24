// Copyright 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，該條款可在
// LICENSE 檔案中找到。
#include <memory>   // for make_shared
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"        // for Element, inverted
#include "ftxui/dom/node.hpp"            // for Node
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/box.hpp"          // for Box
#include "ftxui/screen/screen.hpp"       // for Pixel, Screen

namespace ftxui {

namespace {
class Inverted : public NodeDecorator {
 public:
  using NodeDecorator::NodeDecorator;

  void Render(Screen& screen) override {
    Node::Render(screen);
    for (int y = box_.y_min; y <= box_.y_max; ++y) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        screen.PixelAt(x, y).inverted ^= true;
      }
    }
  }
};
}  // namespace

/// @brief 添加一個濾鏡，它將反轉前景和背景
/// 顏色。
/// @ingroup dom
Element inverted(Element child) {
  return std::make_shared<Inverted>(std::move(child));
}

}  // namespace ftxui
