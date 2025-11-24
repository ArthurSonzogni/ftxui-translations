// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_FLEXBOX_CONFIG_HPP
#define FTXUI_DOM_FLEXBOX_CONFIG_HPP

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
struct FlexboxConfig {
  /// 這確立了主軸，從而定義了彈性項目在彈性容器中放置的方向。彈性盒（除了換行）是
  /// 單方向佈局概念。將彈性項目主要視為在水平行或垂直列中佈局。
  enum class Direction {
    Row,            ///< 彈性項目沿著一行排列。
    RowInversed,    ///< 彈性項目沿著一行排列，但順序相反。
    Column,         ///< 彈性項目沿著一列排列。
    ColumnInversed  ///< 彈性項目沿著一列排列，但順序相反。
  };
  Direction direction = Direction::Row;

  /// 預設情況下，所有彈性項目都將嘗試放在一行中。您可以使用此屬性更改該行為，
  /// 並允許項目根據需要換行。
  enum class Wrap {
    NoWrap,        ///< 所有彈性項目都將嘗試放在一行中。
    Wrap,          ///< 彈性項目將換行到多行。
    WrapInversed,  ///< 彈性項目將換行到多行，但順序相反。
  };
  Wrap wrap = Wrap::Wrap;

  /// 這定義了沿主軸的對齊方式。它有助於分配當一行中的所有彈性項目都不靈活，
  /// 或者靈活但已達到其最大尺寸時剩餘的額外可用空間。它還對項目溢出行時的
  /// 對齊方式施加一些控制。
  enum class JustifyContent {
    /// 項目對齊到彈性盒方向的起點。
    FlexStart,
    /// 項目對齊到彈性盒方向的終點。
    FlexEnd,
    /// 項目沿線居中。
    Center,
    /// 項目拉伸以填滿該行。
    Stretch,
    /// 項目在行中平均分佈；第一個項目在起始線，最後一個項目在結束線。
    SpaceBetween,
    /// 項目在行中平均分佈，周圍有相等的空間。
    /// 請注意，視覺上空間不相等，因為所有項目兩側都有相等的空間。
    /// 第一個項目與容器邊緣之間有一個單位的空間，但與下一個項目之間有兩個單位的空間，
    /// 因為下一個項目有自己的間距。
    SpaceAround,
    /// 項目分佈使得任意兩個項目之間（以及到邊緣）的間距相等。
    SpaceEvenly,
  };
  JustifyContent justify_content = JustifyContent::FlexStart;

  /// 這定義了彈性項目在當前行上沿交叉軸佈局的預設行為。
  /// 將其視為交叉軸（垂直於主軸）的 justify-content 版本。
  enum class AlignItems {
    FlexStart,  ///< 項目放置在交叉軸的起點。
    FlexEnd,    ///< 項目放置在交叉軸的終點。
    Center,     ///< 項目沿交叉軸居中。
    Stretch,    ///< 項目拉伸以填滿交叉軸。
  };
  AlignItems align_items = AlignItems::FlexStart;

  // 這會在交叉軸有額外空間時對齊彈性容器的線，
  // 類似於 justify-content 在主軸內對齊單個項目。
  enum class AlignContent {
    FlexStart,     ///< 項目放置在交叉軸的起點。
    FlexEnd,       ///< 項目放置在交叉軸的終點。
    Center,        ///< 項目沿交叉軸居中。
    Stretch,       ///< 項目拉伸以填滿交叉軸。
    SpaceBetween,  ///< 項目在交叉軸中平均分佈。
    SpaceAround,   ///< 項目平均分佈，每條線周圍有相等的空間。
    SpaceEvenly,  ///< 項目在交叉軸中平均分佈，周圍有相等的空間。
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