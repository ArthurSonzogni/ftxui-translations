// Copyright 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼受 MIT 授權條款約束，詳情請參閱
// LICENSE 檔案。
#include <memory>   // for make_shared
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"        // for Element, dim
#include "ftxui/dom/node.hpp"            // for Node
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/box.hpp"          // for Box
#include "ftxui/screen/screen.hpp"       // for Pixel, Screen

namespace ftxui {

namespace {
class Dim : public NodeDecorator {
 public:
  using NodeDecorator::NodeDecorator;

  void Render(Screen& screen) override {
    Node::Render(screen);
    for (int y = box_.y_min; y <= box_.y_max; ++y) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        screen.PixelAt(x, y).dim = true;
      }
    }
  }
};
}  // namespace

/// @brief 使用淺色字體，適用於不那麼強調的元素。
/// @ingroup dom
Element dim(Element child) {
  return std::make_shared<Dim>(std::move(child));
}

}  // namespace ftxui
