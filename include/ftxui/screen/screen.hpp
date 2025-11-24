// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_SCREEN_HPP
#define FTXUI_SCREEN_SCREEN_HPP

#include <cstdint>     // for uint8_t
#include <functional>  // for function
#include <string>      // for string, basic_string, allocator
#include <vector>      // for vector

#include "ftxui/screen/image.hpp"     // for Pixel, Image
#include "ftxui/screen/terminal.hpp"  // for Dimensions

namespace ftxui {

/// @brief 定義螢幕的維度外觀。
/// @ingroup screen
namespace Dimension {
Dimensions Fixed(int);
Dimensions Full();
}  // namespace Dimension

/// @brief 像素的矩形網格。
/// @ingroup screen
class Screen : public Image {
 public:
  // 建構函式:
  Screen(int dimx, int dimy);
  static Screen Create(Dimensions dimension);
  static Screen Create(Dimensions width, Dimensions height);

  // 解構函式:
  ~Screen() override = default;

  std::string ToString() const;

  // 在終端機上印出螢幕。
  void Print() const;

  // 用空白填充螢幕並重置所有螢幕狀態，例如超連結和游標
  void Clear();

  // 將終端機游標向上移動 n 行，其中 n = dimy()。
  std::string ResetPosition(bool clear = false) const;

  void ApplyShader();

  struct Cursor {
    int x = 0;
    int y = 0;

    enum Shape {
      Hidden = 0,
      BlockBlinking = 1,
      Block = 2,
      UnderlineBlinking = 3,
      Underline = 4,
      BarBlinking = 5,
      Bar = 6,
    };
    Shape shape = Hidden;
  };

  Cursor cursor() const { return cursor_; }
  void SetCursor(Cursor cursor) { cursor_ = cursor; }

  // 在螢幕中儲存一個超連結。返回超連結的 ID。當使用者點擊超連結時，該 ID 用於識別超連結。
  uint8_t RegisterHyperlink(const std::string& link);
  const std::string& Hyperlink(uint8_t id) const;

  using SelectionStyle = std::function<void(Pixel&)>;
  const SelectionStyle& GetSelectionStyle() const;
  void SetSelectionStyle(SelectionStyle decorator);

 protected:
  Cursor cursor_;
  std::vector<std::string> hyperlinks_ = {""};

  // 當前的選擇樣式。這會被各種 DOM 元素覆寫。
  SelectionStyle selection_style_ = [](Pixel& pixel) {
    pixel.inverted ^= true;
  };
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_SCREEN_HPP
