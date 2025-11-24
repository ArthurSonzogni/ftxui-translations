// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに準拠します。
#ifndef FTXUI_SCREEN_SCREEN_HPP
#define FTXUI_SCREEN_SCREEN_HPP

#include <cstdint>     // for uint8_t
#include <functional>  // for function
#include <string>      // for string, basic_string, allocator
#include <vector>      // for vector

#include "ftxui/screen/image.hpp"     // for Pixel, Image
#include "ftxui/screen/terminal.hpp"  // for Dimensions

namespace ftxui {

/// @brief スクリーンがどのように見えるべきかを定義します。
/// @ingroup screen
namespace Dimension {
Dimensions Fixed(int);
Dimensions Full();
}  // namespace Dimension

/// @brief ピクセルの長方形グリッド。
/// @ingroup screen
class Screen : public Image {
 public:
  // コンストラクタ:
  Screen(int dimx, int dimy);
  static Screen Create(Dimensions dimension);
  static Screen Create(Dimensions width, Dimensions height);

  // デストラクタ:
  ~Screen() override = default;

  std::string ToString() const;

  // スクリーンをターミナルに表示します。
  void Print() const;

  // スクリーンをスペースで埋め、ハイパーリンクやカーソルなどのスクリーン状態をリセットします。
  void Clear();

  // ターミナルカーソルをn行上に移動します。n = dimy()です。
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

  // スクリーンにハイパーリンクを保存します。ハイパーリンクのIDを返します。このIDは、ユーザーがクリックしたときにハイパーリンクを識別するために使用されます。
  uint8_t RegisterHyperlink(const std::string& link);
  const std::string& Hyperlink(uint8_t id) const;

  using SelectionStyle = std::function<void(Pixel&)>;
  const SelectionStyle& GetSelectionStyle() const;
  void SetSelectionStyle(SelectionStyle decorator);

 protected:
  Cursor cursor_;
  std::vector<std::string> hyperlinks_ = {""};

  // 現在の選択スタイル。これは様々なDOM要素によって上書きされます。
  SelectionStyle selection_style_ = [](Pixel& pixel) {
    pixel.inverted ^= true;
  };
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_SCREEN_HPP