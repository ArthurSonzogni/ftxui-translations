// Copyright 2021 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに準拠します。
#ifndef FTXUI_DOM_CANVAS_HPP
#define FTXUI_DOM_CANVAS_HPP

#include <cstddef>        // for size_t
#include <functional>     // for function
#include <string>         // for string
#include <unordered_map>  // for unordered_map

#include "ftxui/screen/color.hpp"  // for Color
#include "ftxui/screen/image.hpp"  // for Pixel, Image

#ifdef DrawText
// WinUsr.h (via Windows.h)が、問題を発生させるマクロを定義していることへの回避策。
// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-drawtext
#undef DrawText
#endif

namespace ftxui {

/// @brief Canvasは、描画操作に関連付けられた描画可能なバッファです。
///
/// Canvasは、複雑なグラフィックを作成するために使用できる描画領域です。
/// 点、線、円、楕円、テキスト、および画像を点字、ブロック、または通常の文字を使用して描画する
/// ことができます。
///
/// 注: ターミナルにはセルが含まれています。セルは以下の単位です。
/// - 2x4点字文字 (1x1ピクセル)
/// - 2x2ブロック文字 (2x2ピクセル)
/// - 2x4通常文字 (2x4ピクセル)
///
/// ターミナルで正しい位置を取得するには、x座標を2倍し、y座標を4倍する必要があります。
///
/// @ingroup dom
struct Canvas {
 public:
  Canvas() = default;
  Canvas(int width, int height);

  // Getters:
  int width() const { return width_; }
  int height() const { return height_; }
  Pixel GetPixel(int x, int y) const;

  using Stylizer = std::function<void(Pixel&)>;

  // 点字文字を使用した描画 --------------------------------------------
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

  // ボックス文字を使用した描画 -------------------------------------------------
  // ブロックは1x2のサイズです。yは2の倍数と見なされます。  void DrawBlockOn(int x, int y);
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

  // 通常文字を使用した描画 ----------------------------------------------
  // (x,y)の位置に2x4サイズの文字を使用して描画します。
  // xは2の倍数と見なされます。
  // yは4の倍数と見なされます。  void DrawText(int x, int y, const std::string& value);
  void DrawText(int x, int y, const std::string& value, const Color& color);
  void DrawText(int x, int y, const std::string& value, const Stylizer& style);

  // ピクセルまたは画像を直接使用した描画 --------------------------------------
  // xは2の倍数と見なされます。
  // yは4の倍数と見なされます。  void DrawPixel(int x, int y, const Pixel&);
  void DrawImage(int x, int y, const Image&);

  // デコレータ:
  // xは2の倍数と見なされます。
  // yは4の倍数と見なされます。  void Style(int x, int y, const Stylizer& style);

 private:
  bool IsIn(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
  }

  enum CellType {
    kCell,     // Units of size 2x4
    kBlock,    // Units of size 2x2
    kBraille,  // Units of size 1x1
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
