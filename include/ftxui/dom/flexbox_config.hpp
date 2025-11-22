// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_FLEXBOX_CONFIG_HPP
#define FTXUI_DOM_FLEXBOX_CONFIG_HPP

/*

  这复刻了 CSS 弹性盒子模型。

  请参阅文档指南：

  https://css-tricks.com/snippets/css/a-guide-to-flexbox/

*/

namespace ftxui {

/// @brief FlexboxConfig 是一个配置结构体，定义了弹性盒子容器的布局属性。
//
/// 它允许您指定弹性项目的方向、是否应该换行、它们在主轴上如何对齐，以及它们在交叉轴上如何对齐。
/// 它还包括弹性项目在主轴和交叉轴上间距的属性。
/// 该结构体用于配置终端用户界面中弹性盒子容器的布局行为。
///
/// @ingroup dom
struct FlexboxConfig {
  /// 这建立了主轴，从而定义了弹性项目在弹性容器中的放置方向。弹性盒子（除了换行）是单方向的
  /// 布局概念。可以将弹性项目主要视为水平行或垂直列的布局。
  enum class Direction {
    Row,            ///< 弹性项目按行排列。
    RowInversed,    ///< 弹性项目按行反向排列。
    Column,         ///< 弹性项目按列排列。
    ColumnInversed  ///< 弹性项目按列反向排列。
                    ///< 序。
  };
  Direction direction = Direction::Row;

  /// 默认情况下，弹性项目将全部尝试适应一行。您可以通过此属性更改它，并允许项目根据需要换行。
  enum class Wrap {
    NoWrap,        ///< 弹性项目将全部尝试适应一行。
    Wrap,          ///< 弹性项目将换行到多行。
    WrapInversed,  ///< 弹性项目将换行到多行，但顺序相反。
                   ///< order.
  };
  Wrap wrap = Wrap::Wrap;

  /// 这定义了沿主轴的对齐方式。当一行上的所有弹性项目都是不可伸缩的，
  /// 或者它们是可伸缩的但已达到其最大大小时，它有助于分配剩余的额外可用空间。
  /// 它还对项目溢出线时的对齐方式施加一些控制。
  enum class JustifyContent {
    /// 项目与弹性盒子方向的起始对齐。
    FlexStart,
    /// 项目与弹性盒子方向的末尾对齐。
    FlexEnd,
    /// 项目沿线居中。
    Center,
    /// 项目拉伸以填充该行。
    Stretch,
    /// 项目在线上均匀分布；第一个项目在起始线，最后一个项目在结束线
    // line, last item on the end line
    SpaceBetween,
    /// 项目在线上均匀分布，周围有相等的空间。
    /// 请注意，视觉上空间不相等，因为所有项目两侧都有相等的空间。
    /// 第一个项目与容器边缘之间有一个单位的空间，但与下一个项目之间有两个单位的空间，
    /// 因为下一个项目有其自己的间距。
    SpaceAround,
    /// 项目分布使得任意两个项目之间（以及到边缘的空间）的间距相等。
    SpaceEvenly,
  };
  JustifyContent justify_content = JustifyContent::FlexStart;

  /// 这定义了弹性项目在当前行上沿交叉轴的默认布局行为。
  /// 可以将其视为交叉轴（垂直于主轴）的 justify-content 版本。
  enum class AlignItems {
    FlexStart,  ///< 项目放置在交叉轴的起始位置。
    FlexEnd,    ///< 项目放置在交叉轴的结束位置。
    Center,     ///< 项目沿交叉轴居中。
    Stretch,    ///< 项目拉伸以填充交叉轴。
  };
  AlignItems align_items = AlignItems::FlexStart;

  // 这定义了弹性容器在交叉轴中有额外空间时，如何对齐其行，
  // 类似于 justify-content 如何对齐主轴内的单个项目。
  enum class AlignContent {
    FlexStart,     ///< 项目放置在交叉轴的起始位置。
    FlexEnd,       ///< 项目放置在交叉轴的结束位置。
    Center,        ///< 项目沿交叉轴居中。
    Stretch,       ///< 项目拉伸以填充交叉轴。
    SpaceBetween,  ///< 项目在交叉轴上均匀分布。
    SpaceAround,   ///< 项目均匀分布，每行周围有相等的空间。
                   ///< line.
    SpaceEvenly,  ///< 项目在交叉轴上均匀分布，周围有相等的空间。
                  ///< space around them.
  };
  AlignContent align_content = AlignContent::FlexStart;

  int gap_x = 0;
  int gap_y = 0;

  // 构造函数模式。用于链式调用，例如：
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
