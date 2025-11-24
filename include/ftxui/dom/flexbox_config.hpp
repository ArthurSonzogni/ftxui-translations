// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_FLEXBOX_CONFIG_HPP
#define FTXUI_DOM_FLEXBOX_CONFIG_HPP

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
struct FlexboxConfig {
  /// これは主軸を確立し、フレックスアイテムがフレックスコンテナ内に配置される方向を定義します。
  /// Flexboxは（折り返しを除けば）単一方向のレイアウト概念です。
  /// フレックスアイテムが主に水平な行または垂直な列に配置されると考えてください。
  enum class Direction {
    Row,            ///< フレックスアイテムは行に配置されます。
    RowInversed,    ///< フレックスアイテムは行に配置されますが、逆順になります。
    Column,         ///< フレックスアイテムは列に配置されます。
    ColumnInversed  ///< フレックスアイテムは列に配置されますが、逆順になります。
                    ///< order.
  };
  Direction direction = Direction::Row;

  /// デフォルトでは、フレックスアイテムはすべて1行に収まろうとします。このプロパティを使用すると、
  /// 必要に応じてアイテムを折り返すことができます。
  enum class Wrap {
    NoWrap,        ///< フレックスアイテムはすべて1行に収まろうとします。
    Wrap,          ///< フレックスアイテムは複数行に折り返されます。
    WrapInversed,  ///< フレックスアイテムは複数行に折り返されますが、逆順になります。
                   ///< order.
  };
  Wrap wrap = Wrap::Wrap;

  /// これは主軸に沿った配置を定義します。
  /// これは、行内のすべてのフレックスアイテムが柔軟でない場合、または柔軟であっても最大サイズに達した場合に
  /// 余分な空きスペースをどのように分配するかを決定します。
  /// また、アイテムが一行に収まらない場合の配置にも影響を与えます。
  enum class JustifyContent {
    /// アイテムはflexboxの方向の開始位置に揃えられます。
    FlexStart,
    /// アイテムはflexboxの方向の終了位置に揃えられます。
    FlexEnd,
    /// アイテムは行の中央に配置されます。
    Center,
    /// アイテムは行を埋めるように引き伸ばされます。
    Stretch,
    /// アイテムは行に沿って均等に分配されます。最初のアイテムは開始行に、最後のアイテムは終了行に配置されます。
    SpaceBetween,
    /// アイテムは、それらの周囲に等しいスペースを持って行に沿って均等に分配されます。
    /// 視覚的にはスペースが均等ではないことに注意してください。すべてのアイテムが両側に等しいスペースを持つためです。
    /// 最初のアイテムはコンテナの端に対して1単位のスペースを持ちますが、
    /// 次のアイテムとの間には2単位のスペースがあります。これは、その次のアイテムが自身のスペースを持っているためです。
    SpaceAround,
    /// アイテムは、任意の2つのアイテム間のスペース（および端までのスペース）が均等になるように分配されます。
    SpaceEvenly,
  };
  JustifyContent justify_content = JustifyContent::FlexStart;

  /// これは、現在の行の交差軸に沿ってフレックスアイテムがどのように配置されるかのデフォルトの動作を定義します。
  /// 主軸に垂直な交差軸に対するjustify-contentバージョンと考えてください。
  enum class AlignItems {
    FlexStart,  ///< アイテムは交差軸の開始位置に配置されます。
    FlexEnd,    ///< アイテムは交差軸の終了位置に配置されます。
    Center,     ///< アイテムは交差軸の中央に配置されます。
    Stretch,    ///< アイテムは交差軸を埋めるように引き伸ばされます。
  };
  AlignItems align_items = AlignItems::FlexStart;

  // これは、主軸内で個々のアイテムを整列させるjustify-contentと似ており、
  // 交差軸に余分なスペースがある場合にフレックスコンテナの行を整列させます。
  enum class AlignContent {
    FlexStart,     ///< アイテムは交差軸の開始位置に配置されます。
    FlexEnd,       ///< アイテムは交差軸の終了位置に配置されます。
    Center,        ///< アイテムは交差軸の中央に配置されます。
    Stretch,       ///< アイテムは交差軸を埋めるように引き伸ばされます。
    SpaceBetween,  ///< アイテムは交差軸に沿って均等に分配されます。
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
