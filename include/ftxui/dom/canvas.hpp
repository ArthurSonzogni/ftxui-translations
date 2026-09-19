// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_CANVAS_HPP
#define FTXUI_DOM_CANVAS_HPP

#include <cstddef>        // for size_t
#include <functional>     // for function
#include <string>         // for string
#include <unordered_map>  // for unordered_map

#include "ftxui/screen/color.hpp"    // for Color
#include "ftxui/screen/surface.hpp"  // for Cell, Surface
#include "ftxui/util/export.hpp"

#ifdef DrawText
// 解決 WinUsr.h (透過 Windows.h) 定義會導致問題的宏。
// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-drawtext
#undef DrawText
#endif

namespace ftxui {

/// @brief 畫布是與繪圖操作相關聯的可繪製緩衝區。
///
/// 畫布是一個可繪製區域，可用於創建複雜的圖形。它支援使用
/// 盲文、塊狀或普通字符繪製點、線條、圓形、橢圓、文本和圖像。
///
/// 注意：終端包含單元格。一個單元格的單位是：
/// - 2x4 盲文字符 (1x1 像素)
/// - 2x2 塊狀字符 (2x2 像素)
/// - 2x4 普通字符 (2x4 像素)
///
/// 您需要將 x 座標乘以 2，將 y 座標乘以 4，才能在終端中獲得正確的位置。
///
/// @ingroup dom
struct FTXUI_EXPORT(DOM) Canvas {
 public:
  Canvas() = default;
  Canvas(int width, int height);

  // 取值器：
  int width() const { return width_; }
  int height() const { return height_; }
  Cell GetCell(int x, int y) const;
  // [已棄用] GetCell 的別名。
  Cell GetPixel(int x, int y) const { return GetCell(x, y); }

  using Stylizer = std::function<void(Cell&)>;

  // 使用點字字元繪製 --------------------------------------------
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

  // 使用方塊字符繪製 -------------------------------------------------
  // 塊狀字符的大小為 1x2。y 被認為是 2 的倍數。  void DrawBlockOn(int x, int y);
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

  // 使用普通字符繪製 ----------------------------------------------
  // 在 (x,y) 位置使用 2x4 大小的字符繪製
  // x 被認為是 2 的倍數。
  // y 被認為是 4 的倍數。  void DrawText(int x, int y, const std::string& value);
  void DrawText(int x, int y, std::string_view value);
  void DrawText(int x, int y, std::string_view value, const Color& color);
  void DrawText(int x, int y, std::string_view value, const Stylizer& style);

  // 直接使用像素或圖像繪製 --------------------------------------
  // x 被認為是 2 的倍數。
  // y 被認為是 4 的倍數。  void DrawPixel(int x, int y, const Pixel&);
  void DrawCell(int x, int y, const Cell&);
  void DrawSurface(int x, int y, const Surface&);

  // [已棄用] DrawCell 的別名。
  void DrawPixel(int x, int y, const Cell& cell) { DrawCell(x, y, cell); }
  // [已棄用] DrawSurface 的別名。
  void DrawImage(int x, int y, const Surface& s) { DrawSurface(x, y, s); }

  // 裝飾器：
  // x 被認為是 2 的倍數。
  // y 被認為是 4 的倍數。  void Style(int x, int y, const Stylizer& style);
  void Style(int x, int y, const Stylizer& style);

 private:
  bool IsIn(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
  }

  enum CellType {
    kCell,     // 大小為 2x4 的單位
    kBlock,    // 大小為 2x2 的單位
    kBraille,  // 大小為 1x1 的單位
  };

  struct CanvasCell {
    CellType type = kCell;
    Cell content;
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
  std::unordered_map<XY, CanvasCell, XYHash> storage_;
};

}  // namespace ftxui

#endif  // FTXUI_DOM_CANVAS_HPP
