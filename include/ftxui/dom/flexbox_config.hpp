// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_FLEXBOX_CONFIG_HPP
#define FTXUI_DOM_FLEXBOX_CONFIG_HPP

#include <cstdint>

#include "ftxui/util/export.hpp"  // for FTXUI_EXPORT

/*
  這複製了 CSS flexbox 模型。
  請參閱指南文件：
  https://css-tricks.com/snippets/css/a-guide-to-flexbox/
*/

namespace ftxui {

/// @brief FlexboxConfig 是一個配置結構，用於定義彈性盒容器的佈局屬性。
//
/// 它允許您指定彈性項目的方向、是否應該換行、它們在主軸上的對齊方式，以及
/// 它們在交叉軸上的對齊方式。
/// 它還包括彈性項目在主軸和交叉軸之間間距的屬性。
/// 此結構用於配置終端使用者介面中彈性盒容器的佈局行為。
///
/// @ingroup dom
struct FTXUI_EXPORT(DOM) FlexboxConfig {
  /// 這決定了主軸，也就是彈性項目在彈性容器中
  /// 排列的方向。除了換行外，Flexbox 是單方向的
  /// 布局概念。可以將彈性項目想成主要以
  /// 水平列或垂直欄的方式排列。
  enum class Direction : uint8_t {
    Row,            ///< 彈性項目以列的方式排列。
    RowInversed,    ///< 彈性項目以列的方式排列，但順序相反。
    Column,         ///< 彈性項目以欄的方式排列。
    ColumnInversed  ///< 彈性項目以欄的方式排列，但順序
                    ///< 相反。
  };
  Direction direction = Direction::Row;

  /// 預設情況下，彈性項目都會嘗試放入同一行。你可以透過此屬性
  /// 改變此行為，讓項目視需要換行。
  enum class Wrap : uint8_t {
    NoWrap,        ///< 彈性項目都會嘗試放入同一行。
    Wrap,          ///< 彈性項目會換行到多行。
    WrapInversed,  ///< 彈性項目會換行到多行，但順序
                   ///< 相反。
  };
  Wrap wrap = Wrap::Wrap;

  /// 這定義了沿主軸的對齊方式。它有助於分配
  /// 當一行中所有彈性項目都是不可彈性的，或雖可彈性但已達最大
  /// 大小時，剩餘的額外可用空間。它也在項目
  /// 超出該行時對對齊方式有一定的控制。
  enum class JustifyContent : uint8_t {
    /// 項目對齊到 flexbox 方向的起始處。
    FlexStart,
    /// 項目對齊到 flexbox 方向的結束處。
    FlexEnd,
    /// 項目沿著該行置中對齊。
    Center,
    /// 項目被拉伸以填滿該行。
    Stretch,
    /// 項目在該行中平均分佈；第一個項目在起始
    // 行，最後一個項目在結束行
    SpaceBetween,
    /// 項目在該行中平均分佈，項目周圍留有相等的空間。
    /// 注意在視覺上這些空間並不相等，因為所有項目
    /// 在兩側都留有相等的空間。第一個項目相對於容器邊緣
    /// 只有一個單位的空間，但與下一個項目之間卻有兩個單位的
    /// 空間，因為下一個項目也套用了自己的間距。
    SpaceAround,
    /// 項目分佈的方式，使任意兩個項目之間（以及與邊緣之間）
    /// 的間距都相等。
    SpaceEvenly,
  };
  JustifyContent justify_content = JustifyContent::FlexStart;

  /// 這定義了彈性項目在當前行上沿交叉軸佈局的預設行為。
  /// 將其視為交叉軸（垂直於主軸）的 justify-content 版本。
  enum class AlignItems : uint8_t {
    FlexStart,  ///< 項目放置在交叉軸的起始處。
    FlexEnd,    ///< 項目放置在交叉軸的結束處。
    Center,     ///< 項目沿著交叉軸置中對齊。
    Stretch,    ///< 項目被拉伸以填滿交叉軸。
  };
  AlignItems align_items = AlignItems::FlexStart;

  // 這會在交叉軸有額外空間時對齊彈性容器的線，
  // 類似於 justify-content 在主軸內對齊單個項目。
  enum class AlignContent : uint8_t {
    FlexStart,     ///< 項目放置在交叉軸的起始處。
    FlexEnd,       ///< 項目放置在交叉軸的結束處。
    Center,        ///< 項目沿著交叉軸置中對齊。
    Stretch,       ///< 項目被拉伸以填滿交叉軸。
    SpaceBetween,  ///< 項目在交叉軸上平均分佈。
    SpaceAround,   ///< 項目平均分佈，每一行周圍留有
                   ///< 相等的空間。
    SpaceEvenly,  ///< 項目在交叉軸上平均分佈，項目周圍
                  ///< 留有相等的空間。
  };
  AlignContent align_content = AlignContent::FlexStart;

  int gap_x = 0;
  int gap_y = 0;

  // 建構子模式。用於鏈式使用，例如：
  // ```
  // FlexboxConfig()
  //    .Set(FlexboxConfig::Direction::Row)
  //    .Set(FlexboxConfig::Wrap::Wrap);
  // ```
  FlexboxConfig& Set(FlexboxConfig::Direction);
  FlexboxConfig& Set(FlexboxConfig::Wrap);
  FlexboxConfig& Set(FlexboxConfig::JustifyContent);
  FlexboxConfig& Set(FlexboxConfig::AlignItems);
  FlexboxConfig& Set(FlexboxConfig::AlignContent);
  FlexboxConfig& SetGap(int gap_x, int gap_y);
};

}  // namespace ftxui

#endif  // FTXUI_DOM_FLEXBOX_CONFIG_HPP
