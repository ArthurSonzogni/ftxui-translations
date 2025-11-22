// Copyright 2021 Arthur Sonzogni. 保留所有权利。
// 此源代码的使用受 MIT 许可协议的约束，该协议可在以下文件中找到：
// LICENSE 文件。
#ifndef FTXUI_DOM_CANVAS_HPP
#define FTXUI_DOM_CANVAS_HPP

#include <cstddef>        // for size_t
#include <functional>     // for function
#include <string>         // for string
#include <unordered_map>  // for unordered_map

#include "ftxui/screen/color.hpp"  // for Color
#include "ftxui/screen/image.hpp"  // for Pixel, Image

#ifdef DrawText
// 解决 WinUsr.h（通过 Windows.h）定义破坏性宏的问题。
// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-drawtext
#undef DrawText
#endif

namespace ftxui {

/// @brief Canvas 是一个与绘图操作相关的可绘制缓冲区。
///
/// Canvas 是一个可绘制区域，可用于创建复杂的图形。它
/// 支持使用点、线、圆、椭圆、文本和图像进行绘制，使用
/// 盲文、块或普通字符。
///
/// 注意：终端包含单元格。一个单元格是以下单位：
/// - 2x4 盲文字符（1x1 像素）
/// - 2x2 块字符（2x2 像素）
/// - 2x4 普通字符（2x4 像素）
///
/// 您需要将 x 坐标乘以 2，将 y 坐标乘以 4，以
/// 获取终端中的正确位置。
///
/// @ingroup dom
struct Canvas {
 public:
  Canvas() = default;
  Canvas(int width, int height);

  // 获取器：
  int width() const { return width_; }
  int height() const { return height_; }
  Pixel GetPixel(int x, int y) const;

  using Stylizer = std::function<void(Pixel&)>;

  // 使用盲文字符绘图 --------------------------------------------
  void DrawPointOn(int x, int y);
  void DrawPointOff(int x, int y);
  void DrawPointToggle(int x, int y);
  void DrawPoint(int x, int y, bool value);
  void DrawPoint(int x, int y, bool value, const Stylizer& s);
  void DrawPoint(int x, int y, bool value, const Color& color);
  void DrawPointLine(int x1, int y1, int x2, int y2);
  void DrawPointLine(int x1, int y1, int x2, int y2, const Stylizer& s);
  void DrawPointLine(int x1, int y1, int x2, int y2, const Color& color);
  void DrawPointCircle(int x, int y, int radius);
  void DrawPointCircle(int x, int y, int radius, const Stylizer& s);
  void DrawPointCircle(int x, int y, int radius, const Color& color);
  void DrawPointCircleFilled(int x, int y, int radius);
  void DrawPointCircleFilled(int x, int y, int radius, const Stylizer& s);
  void DrawPointCircleFilled(int x, int y, int radius, const Color& color);
  void DrawPointEllipse(int x, int y, int r1, int r2);
  void DrawPointEllipse(int x, int y, int r1, int r2, const Color& color);
  void DrawPointEllipse(int x, int y, int r1, int r2, const Stylizer& s);
  void DrawPointEllipseFilled(int x, int y, int r1, int r2);
  void DrawPointEllipseFilled(int x, int y, int r1, int r2, const Color& color);
  void DrawPointEllipseFilled(int x, int y, int r1, int r2, const Stylizer& s);

  // 使用方块字符绘图 -------------------------------------------------
  // 块的大小为 1x2。y 被认为是 2 的倍数。
  void DrawBlockOn(int x, int y);
  void DrawBlockOff(int x, int y);
  void DrawBlockToggle(int x, int y);
  void DrawBlock(int x, int y, bool value);
  void DrawBlock(int x, int y, bool value, const Stylizer& s);
  void DrawBlock(int x, int y, bool value, const Color& color);
  void DrawBlockLine(int x1, int y1, int x2, int y2);
  void DrawBlockLine(int x1, int y1, int x2, int y2, const Stylizer& s);
  void DrawBlockLine(int x1, int y1, int x2, int y2, const Color& color);
  void DrawBlockCircle(int x1, int y1, int radius);
  void DrawBlockCircle(int x1, int y1, int radius, const Stylizer& s);
  void DrawBlockCircle(int x1, int y1, int radius, const Color& color);
  void DrawBlockCircleFilled(int x1, int y1, int radius);
  void DrawBlockCircleFilled(int x1, int y1, int radius, const Stylizer& s);
  void DrawBlockCircleFilled(int x1, int y1, int radius, const Color& color);
  void DrawBlockEllipse(int x1, int y1, int r1, int r2);
  void DrawBlockEllipse(int x1, int y1, int r1, int r2, const Stylizer& s);
  void DrawBlockEllipse(int x1, int y1, int r1, int r2, const Color& color);
  void DrawBlockEllipseFilled(int x1, int y1, int r1, int r2);
  void DrawBlockEllipseFilled(int x1,
                              int y1,
                              int r1,
                              int r2,
                              const Stylizer& s);
  void DrawBlockEllipseFilled(int x1,
                              int y1,
                              int r1,
                              int r2,
                              const Color& color);

  // 使用普通字符绘图 ----------------------------------------------
  // 在位置 (x,y) 处绘制大小为 2x4 的字符
  // x 被认为是 2 的倍数。
  // y 被认为是 4 的倍数。
  void DrawText(int x, int y, const std::string& value);
  void DrawText(int x, int y, const std::string& value, const Color& color);
  void DrawText(int x, int y, const std::string& value, const Stylizer& style);

  // 直接使用像素或图像绘图 --------------------------------------
  // x 被认为是 2 的倍数。
  // y 被认为是 4 的倍数。
  void DrawPixel(int x, int y, const Pixel&);
  void DrawImage(int x, int y, const Image&);

  // 装饰器：
  // x 被认为是 2 的倍数。
  // y 被认为是 4 的倍数。
  void Style(int x, int y, const Stylizer& style);

 private:
  bool IsIn(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
  }

  enum CellType {
    kCell,     // 大小为 2x4 的单位
    kBlock,    // 大小为 2x2 的单位
    kBraille,  // 大小为 1x1 的单位
  };

  struct Cell {
    CellType type = kCell;
    Pixel content;
  };

  struct XY {
    int x;
    int y;
    bool operator==(const XY& other) const {
      return x == other.x && y == other.y;
    }
  };

  struct XYHash {
    size_t operator()(const XY& xy) const {
      constexpr size_t shift = 1024;
      return size_t(xy.x) * shift + size_t(xy.y);
    }
  };

  int width_ = 0;
  int height_ = 0;
  std::unordered_map<XY, Cell, XYHash> storage_;
};

}  // namespace ftxui

#endif  // FTXUI_DOM_CANVAS_HPP
