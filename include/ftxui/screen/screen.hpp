// 版权所有 2020 Arthur Sonzogni. 保留所有权利。
// 此源代码的使用受 MIT 许可证的约束，该许可证可在
// LICENSE 文件中找到。
#ifndef FTXUI_SCREEN_SCREEN_HPP
#define FTXUI_SCREEN_SCREEN_HPP

#include <cstdint>     // for uint8_t
#include <functional>  // for function
#include <string>      // for string, basic_string, allocator
#include <vector>      // for vector

#include "ftxui/screen/image.hpp"     // for Pixel, Image
#include "ftxui/screen/terminal.hpp"  // for Dimensions

namespace ftxui {

/// @brief 定义屏幕的尺寸应如何呈现。
/// @ingroup screen
namespace Dimension {
Dimensions Fixed(int);
Dimensions Full();
}  // namespace Dimension

/// @brief 像素的矩形网格。
/// @ingroup screen
class Screen : public Image {
 public:
  // 构造函数：
  Screen(int dimx, int dimy);
  static Screen Create(Dimensions dimension);
  static Screen Create(Dimensions width, Dimensions height);

  // 析构函数：
  ~Screen() override = default;

  std::string ToString() const;

  // 将屏幕打印到终端。
  void Print() const;

  // 用空格填充屏幕并重置任何屏幕状态，例如超链接和
  // 光标
  void Clear();

  // 将终端光标向上移动 n 行，其中 n = dimy()。
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

  // 在屏幕中存储一个超链接。返回超链接的ID。当用户点击时，
  // 此ID用于识别超链接。
  uint8_t RegisterHyperlink(const std::string& link);
  const std::string& Hyperlink(uint8_t id) const;

  using SelectionStyle = std::function<void(Pixel&)>;
  const SelectionStyle& GetSelectionStyle() const;
  void SetSelectionStyle(SelectionStyle decorator);

 protected:
  Cursor cursor_;
  std::vector<std::string> hyperlinks_ = {""};

  // 当前选择样式。这将被各种DOM元素覆盖。
  SelectionStyle selection_style_ = [](Pixel& pixel) {
    pixel.inverted ^= true;
  };
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_SCREEN_HPP
