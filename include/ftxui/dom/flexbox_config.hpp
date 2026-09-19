// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_FLEXBOX_CONFIG_HPP
#define FTXUI_DOM_FLEXBOX_CONFIG_HPP

#include <cstdint>

#include "ftxui/util/export.hpp"  // for FTXUI_EXPORT

/*
  これはCSSのFlexboxモデルを再現したものです。
  詳細なドキュメントについては、以下のガイドを参照してください:
  https://css-tricks.com/snippets/css/a-guide-to-flexbox/
*/

namespace ftxui {

/// @brief FlexboxConfigは、flexboxコンテナのレイアウトプロパティを定義する構成構造体です。
//
/// これにより、フレックスアイテムの方向、折り返し、主軸に沿った整列方法、
/// および交差軸に沿った整列方法を指定できます。
/// また、主軸と交差軸の両方におけるフレックスアイテム間のギャップのプロパティも含まれます。
/// この構造体は、ターミナルユーザーインターフェースにおけるflexboxコンテナのレイアウト動作を
/// 設定するために使用されます。
///
/// @ingroup dom
struct FTXUI_EXPORT(DOM) FlexboxConfig {
  /// これは主軸を確立し、フレックスアイテムがフレックスコンテナ内に配置される方向を定義します。
  /// Flexboxは（折り返しを除けば）単一方向のレイアウト概念です。
  /// フレックスアイテムが主に水平な行または垂直な列に配置されると考えてください。
  enum class Direction : uint8_t {
    Row,            ///< Flex items are laid out in a row.
    RowInversed,    ///< Flex items are laid out in a row, but in reverse order.
    Column,         ///< Flex items are laid out in a column.
    ColumnInversed  ///< フレックスアイテムは列に配置されますが、逆順になります。
                    ///< order.
  };
  Direction direction = Direction::Row;

  /// By default, flex items will all try to fit onto one line. You can change
  /// that and allow the items to wrap as needed with this property.
  enum class Wrap : uint8_t {
    NoWrap,        ///< Flex items will all try to fit onto one line.
    Wrap,          ///< Flex items will wrap onto multiple lines.
    WrapInversed,  ///< フレックスアイテムは複数行に折り返されますが、逆順になります。
                   ///< order.
  };
  Wrap wrap = Wrap::Wrap;

  /// This defines the alignment along the main axis. It helps distribute extra
  /// free space leftover when either all the flex items on a line are
  /// inflexible, or are flexible but have reached their maximum size. It also
  /// exerts some control over the alignment of items when they overflow the
  /// line.
  enum class JustifyContent : uint8_t {
    /// アイテムはflexboxの方向の開始位置に揃えられます。
    FlexStart,
    /// アイテムはflexboxの方向の終了位置に揃えられます。
    FlexEnd,
    /// Items are centered along the line.
    Center,
    /// Items are stretched to fill the line.
    Stretch,
    /// Items are evenly distributed in the line; first item is on the start
    // line, last item on the end line
    SpaceBetween,
    /// Items are evenly distributed in the line with equal space around them.
    /// Note that visually the spaces aren’t equal, since all the items have
    /// equal space on both sides. The first item will have one unit of space
    /// against the container edge, but two units of space between the next item
    /// because that next item has its own spacing that applies.
    SpaceAround,
    /// Items are distributed so that the spacing between any two items (and the
    /// space to the edges) is equal.
    SpaceEvenly,
  };
  JustifyContent justify_content = JustifyContent::FlexStart;

  /// これは、現在の行の交差軸に沿ってフレックスアイテムがどのように配置されるかのデフォルトの動作を定義します。
  /// 主軸に垂直な交差軸に対するjustify-contentバージョンと考えてください。
  enum class AlignItems : uint8_t {
    FlexStart,  ///< items are placed at the start of the cross axis.
    FlexEnd,    ///< items are placed at the end of the cross axis.
    Center,     ///< items are centered along the cross axis.
    Stretch,    ///< items are stretched to fill the cross axis.
  };
  AlignItems align_items = AlignItems::FlexStart;

  // これは、主軸内で個々のアイテムを整列させるjustify-contentと似ており、
  // 交差軸に余分なスペースがある場合にフレックスコンテナの行を整列させます。
  enum class AlignContent : uint8_t {
    FlexStart,     ///< items are placed at the start of the cross axis.
    FlexEnd,       ///< items are placed at the end of the cross axis.
    Center,        ///< items are centered along the cross axis.
    Stretch,       ///< items are stretched to fill the cross axis.
    SpaceBetween,  ///< items are evenly distributed in the cross axis.
    SpaceAround,   ///< アイテムは各行の周囲に等しいスペースを持って均等に分配されます。
                   ///< line.
    SpaceEvenly,  ///< アイテムは交差軸に沿って等しいスペースを持って均等に分配されます。
                  ///< space around them.
  };
  AlignContent align_content = AlignContent::FlexStart;

  int gap_x = 0;
  int gap_y = 0;

  // コンストラクタパターン。以下のようにチェーンして使用します:
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
