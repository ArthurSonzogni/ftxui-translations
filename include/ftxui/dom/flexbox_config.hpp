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
    Row,            ///< フレックス項目は行方向にレイアウトされます。
    RowInversed,    ///< フレックス項目は行方向にレイアウトされますが、逆順です。
    Column,         ///< フレックス項目は列方向にレイアウトされます。
    ColumnInversed  ///< フレックスアイテムは列に配置されますが、逆順になります。
                    ///< order.
  };
  Direction direction = Direction::Row;

  /// デフォルトでは、フレックス項目はすべて1行に収まろうとします。この
  /// プロパティにより、必要に応じて項目を折り返すことを許可するように
  /// 変更できます。
  enum class Wrap : uint8_t {
    NoWrap,        ///< フレックス項目はすべて1行に収まろうとします。
    Wrap,          ///< フレックス項目は複数行に折り返されます。
    WrapInversed,  ///< フレックスアイテムは複数行に折り返されますが、逆順になります。
                   ///< order.
  };
  Wrap wrap = Wrap::Wrap;

  /// これは主軸に沿った配置を定義します。行内のすべてのフレックス項目が
  /// 非柔軟である場合、または柔軟であるが最大サイズに達している場合に
  /// 残る余分な空きスペースを分配するのに役立ちます。また、項目が行を
  /// オーバーフローしたときの配置もある程度制御します。
  enum class JustifyContent : uint8_t {
    /// アイテムはflexboxの方向の開始位置に揃えられます。
    FlexStart,
    /// アイテムはflexboxの方向の終了位置に揃えられます。
    FlexEnd,
    /// 項目は行に沿って中央揃えされます。
    Center,
    /// 項目は行を埋めるように伸長されます。
    Stretch,
    /// 項目は行内に均等に分配されます。最初の項目は開始行に、最後の項目は
    // 終了行にあります。
    SpaceBetween,
    /// 項目は行内に均等に分配され、周囲に等しいスペースがあります。
    /// すべての項目が両側に等しいスペースを持つため、視覚的にはスペースが
    /// 等しくないことに注意してください。最初の項目はコンテナの端に対して
    /// 1単位のスペースを持ちますが、次の項目にはその項目自体の間隔が
    /// 適用されるため、2単位のスペースがあります。
    SpaceAround,
    /// 項目は、任意の2つの項目間の間隔(および端までのスペース)が
    /// 等しくなるように分配されます。
    SpaceEvenly,
  };
  JustifyContent justify_content = JustifyContent::FlexStart;

  /// これは、現在の行の交差軸に沿ってフレックスアイテムがどのように配置されるかのデフォルトの動作を定義します。
  /// 主軸に垂直な交差軸に対するjustify-contentバージョンと考えてください。
  enum class AlignItems : uint8_t {
    FlexStart,  ///< 項目は交差軸の開始位置に配置されます。
    FlexEnd,    ///< 項目は交差軸の終了位置に配置されます。
    Center,     ///< 項目は交差軸に沿って中央揃えされます。
    Stretch,    ///< 項目は交差軸を埋めるように伸長されます。
  };
  AlignItems align_items = AlignItems::FlexStart;

  // これは、主軸内で個々のアイテムを整列させるjustify-contentと似ており、
  // 交差軸に余分なスペースがある場合にフレックスコンテナの行を整列させます。
  enum class AlignContent : uint8_t {
    FlexStart,     ///< 項目は交差軸の開始位置に配置されます。
    FlexEnd,       ///< 項目は交差軸の終了位置に配置されます。
    Center,        ///< 項目は交差軸に沿って中央揃えされます。
    Stretch,       ///< 項目は交差軸を埋めるように伸長されます。
    SpaceBetween,  ///< 項目は交差軸に均等に分配されます。
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
