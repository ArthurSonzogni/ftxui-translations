// Copyright 2023 Arthur Sonzogni. All rights reserved.
// 本源代码的使用受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
#include <cstdint>  // for uint8_t
#include <memory>   // for make_shared
#include <string>   // for string
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"        // for Element, Decorator, hyperlink
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/box.hpp"          // for Box
#include "ftxui/screen/screen.hpp"       // for Screen, Pixel

namespace ftxui {

namespace {
class Hyperlink : public NodeDecorator {
 public:
  Hyperlink(Element child, std::string link)
      : NodeDecorator(std::move(child)), link_(std::move(link)) {}

  void Render(Screen& screen) override {
    const uint8_t hyperlink_id = screen.RegisterHyperlink(link_);
    for (int y = box_.y_min; y <= box_.y_max; ++y) {
      for (int x = box_.x_min; x <= box_.x_max; ++x) {
        screen.PixelAt(x, y).hyperlink = hyperlink_id;
      }
    }
    NodeDecorator::Render(screen);
  }

  std::string link_;
};
}  // namespace

/// @brief 使渲染区域可通过网络浏览器点击。
///        当用户点击时，链接将被打开。
///        这仅在有限的终端模拟器中受支持。
///        列表：https://github.com/Alhadis/OSC8-Adoption/
/// @param link 链接
/// @param child 输入元素。
/// @return 带有链接的输出元素。
/// @ingroup dom
///
/// ### 示例
///
/// ```cpp
/// Element document =
///   hyperlink("https://github.com/ArthurSonzogni/FTXUI", "link");
/// ```
Element hyperlink(std::string link, Element child) {
  return std::make_shared<Hyperlink>(std::move(child), std::move(link));
}

/// @brief 使用超链接进行装饰。
///        当用户点击时，链接将被打开。
///        这仅在有限的终端模拟器中受支持。
///        列表：https://github.com/Alhadis/OSC8-Adoption/
/// @param link 重定向用户到的链接。
/// @return 应用超链接的 Decorator。
/// @ingroup dom
///
/// ### 示例
///
/// ```cpp
/// Element document =
///   text("red") | hyperlink("https://github.com/Arthursonzogni/FTXUI");
/// ```
// NOLINTNEXTLINE
Decorator hyperlink(std::string link) {
  return [link](Element child) { return hyperlink(link, std::move(child)); };
}

}  // namespace ftxui
