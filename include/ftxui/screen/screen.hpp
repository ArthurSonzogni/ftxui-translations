// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_SCREEN_HPP
#define FTXUI_SCREEN_SCREEN_HPP

#include <cstdint>     // for uint8_t
#include <functional>  // for function
#include <string>      // for string, basic_string, allocator
#include <vector>      // for vector

#include "ftxui/screen/surface.hpp"   // for Surface
#include "ftxui/screen/terminal.hpp"  // for Dimensions
#include "ftxui/util/export.hpp"      // for FTXUI_EXPORT

namespace ftxui {

/// @brief スクリーンがどのように見えるべきかを定義します。
/// @ingroup screen
namespace Dimension {
FTXUI_EXPORT(SCREEN) Dimensions Fixed(int);
FTXUI_EXPORT(SCREEN) Dimensions Full();
}  // namespace Dimension

/// @brief Cellの長方形グリッド。
/// @ingroup screen
class FTXUI_EXPORT(SCREEN) Screen : public Surface {
 public:
  // コンストラクタ:
  Screen(int dimx, int dimy);
  static Screen Create(Dimensions dimension);
  static Screen Create(Dimensions width, Dimensions height);

  // デストラクタ:
  ~Screen() override = default;

  // コピー:
  Screen(const Screen&) = default;
  Screen& operator=(const Screen&) = default;

  std::string ToString() const;
  void ToString(std::string& ss) const;

  // Screenをターミナルに出力します。
  void Print() const;

  // 画面をスペースで埋め、ハイパーリンクやカーソルなどの画面状態を
  // リセットします。
  void Clear();

  // ターミナルカーソルをn行上に移動します。n = dimy()です。
  std::string ResetPosition(bool clear = false) const;
  void ResetPosition(std::string& ss, bool clear = false) const;

  void ApplyShader();

  struct Cursor {
    int x = 0;
    int y = 0;

    enum Shape : uint8_t {
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

  // ABI予約:
  void Reserved1() override;
  void Reserved2() override;
  void Reserved3() override;
  void Reserved4() override;
  void Reserved5() override;
  void Reserved6() override;
  void Reserved7() override;
  void Reserved8() override;

  // スクリーンにハイパーリンクを保存します。ハイパーリンクのIDを返します。このIDは、ユーザーがクリックしたときにハイパーリンクを識別するために使用されます。
  uint8_t RegisterHyperlink(std::string_view link);
  const std::string& Hyperlink(uint8_t id) const;

  using SelectionStyle = std::function<void(Cell&)>;
  const SelectionStyle& GetSelectionStyle() const;
  void SetSelectionStyle(SelectionStyle decorator);

 protected:
  Cursor cursor_;
  std::vector<std::string> hyperlinks_ = {""};

  // 現在の選択スタイル。これは様々なDOM要素によって上書きされます。
  SelectionStyle selection_style_ = [](Cell& cell) { cell.inverted ^= true; };
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_SCREEN_HPP
