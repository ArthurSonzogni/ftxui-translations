// Copyright 2020 Arthur Sonzogni. All rights reserved.
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱 LICENSE 檔案。
#include <memory>   // 用於 make_shared
#include <utility>  // 用於 move

#include "ftxui/dom/elements.hpp"        // 用於 Element, underlined
#include "ftxui/dom/node.hpp"            // 用於 Node
#include "ftxui/dom/node_decorator.hpp"  // 用於 NodeDecorator
#include "ftxui/screen/box.hpp"          // 用於 Box
#include "ftxui/screen/screen.hpp"       // 用於 Pixel, Screen

namespace ftxui {

namespace {
class Underlined : public NodeDecorator {
 public:
  using NodeDecorator::NodeDecorator;

  void Render(Screen& screen) override {
    Node::Render(screen);
    for (int y = box_.y_min; y <= box_.y_max; ++y) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        screen.PixelAt(x, y).underlined = true;
      }
    }
  }
};
}  // namespace

/// @brief 為給定元素加上底線。
/// @ingroup dom
Element underlined(Element child) {
  return std::make_shared<Underlined>(std::move(child));
}

}  // namespace ftxui