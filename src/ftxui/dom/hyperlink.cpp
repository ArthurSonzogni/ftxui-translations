// Copyright 2023 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <cstdint>  // for uint8_t
#include <memory>   // for make_shared
#include <string>   // for string
#include <string_view>
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"        // for Element, Decorator, hyperlink
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/screen/box.hpp"          // for Box
#include "ftxui/screen/screen.hpp"       // for Screen, Cell

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
        screen.CellAt(x, y).hyperlink = hyperlink_id;
      }
    }
    NodeDecorator::Render(screen);
  }

  std::string link_;
};
}  // namespace

/// @brief 使渲染區域可點擊並使用網路瀏覽器開啟。
///        當用戶點擊時，連結將被開啟。
///        此功能僅在有限的終端模擬器中受支援。
///        List: https://github.com/Alhadis/OSC8-Adoption/
/// @param link 連結
/// @param child 輸入元素。
/// @return 帶有連結的輸出元素。
/// @ingroup dom
///
/// ### 範例
///
/// ```cpp
/// Element document =
///   hyperlink("https://github.com/ArthurSonzogni/FTXUI", "link");
/// ```
Element hyperlink(std::string_view link, Element child) {
  return std::make_shared<Hyperlink>(std::move(child), std::string(link));
}

/// @brief 使用超連結裝飾。
///        當用戶點擊時，連結將被開啟。
///        此功能僅在有限的終端模擬器中受支援。
///        List: https://github.com/Alhadis/OSC8-Adoption/
/// @param link 用於將用戶重定向到的連結。
/// @return 應用超連結的裝飾器。
/// @ingroup dom
///
/// ### 範例
///
/// ```cpp
/// Element document =
///   text("red") | hyperlink("https://github.com/Arthursonzogni/FTXUI");
/// ```
// NOLINTNEXTLINE
Decorator hyperlink(std::string_view link) {
  return [link = std::string(link)](Element child) {
    return hyperlink(link, std::move(child));
  };
}

}  // namespace ftxui
