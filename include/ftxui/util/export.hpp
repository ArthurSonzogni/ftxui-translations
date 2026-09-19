// Copyright 2026 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#ifndef FTXUI_UTIL_EXPORT_H_
#define FTXUI_UTIL_EXPORT_H_

// 統合ビルドでは、export/importアノテーションは必要ありません。
#if defined(FTXUI_AMALGAMATED)
#define FTXUI_EXPORT(component)
#define INSIDE_FTXUI_COMPONENT_IMPL(component) 0
#else

// |component|という名前のコンポーネントによってエクスポートされる
// シンボルに注釈を付けるために使用されます。これが正しく機能するのは、
// 対応するコンポーネントターゲットのソースが|IS_FTXUI_$component_IMPL|を
// 1として定義してコンパイルされている場合のみであることに注意してください。
// 例えば:
//
//   class FTXUI_EXPORT(FOO) Bar {};
//
// コンパイル時にIS_FTXUI_FOO_IMPL=1であれば、Barは以下で定義される
// FTXUI_EXPORT_ANNOTATIONマクロを使用して注釈付けされます。そうでない場合は
// FTXUI_IMPORT_ANNOTATIONマクロを使用して注釈付けされます。
#define FTXUI_EXPORT(component)                         \
  FTXUI_MACRO_CONDITIONAL_(IS_FTXUI_##component##_IMPL, \
                           FTXUI_EXPORT_ANNOTATION, FTXUI_IMPORT_ANNOTATION)

// 現在のコンパイル単位が、|component|という名前のコンポーネントの実装の
// 一部としてコンパイルされているかどうかを示します。|IS_FTXUI_$component_IMPL|
// が|1|として定義されている場合は|1|に展開され、そうでない場合は|0|に
// 展開されます。
//
// 特に、|IS_FTXUI_$component_IMPL|がまったく定義されていない場合でも、
// INSIDE_FTXUI_COMPONENT_IMPL(component)をテストしても問題なく、
// 期待通り|0|に展開されることに注意してください。
#define INSIDE_FTXUI_COMPONENT_IMPL(component) \
  FTXUI_MACRO_CONDITIONAL_(IS_FTXUI_##component##_IMPL, 1, 0)

#endif

// シンボルのエクスポートまたはインポートに注釈を付けるためのコンパイラ
// 固有のマクロ。非コンポーネントビルドではノーオペレーションです。
// これらは直接使用されることはほとんどないはずです。代わりに上記で
// 定義されているFTXUI_EXPORTマクロを使用してください。
#if defined(COMPONENT_BUILD)
#if defined(WIN32)
#define FTXUI_EXPORT_ANNOTATION __declspec(dllexport)
#define FTXUI_IMPORT_ANNOTATION __declspec(dllimport)
#else  // defined(WIN32)
#define FTXUI_EXPORT_ANNOTATION __attribute__((visibility("default")))
#define FTXUI_IMPORT_ANNOTATION __attribute__((visibility("default")))
#endif  // defined(WIN32)
#else   // defined(COMPONENT_BUILD)
#define FTXUI_EXPORT_ANNOTATION
#define FTXUI_IMPORT_ANNOTATION
#endif  // defined(COMPONENT_BUILD)

// この時点以降は、上記のマクロの実装に使用されるいくつかの内部
// ユーティリティマクロです。外部使用を意図していません。

#define FTXUI_MACRO_EXPAND(x) x

// 2つのトークン文字列のいずれかへの条件付き展開のためのヘルパー。
// |condition|が|1|に展開される場合、このマクロは|consequent|に展開され、
// そうでない場合は|alternate|に展開されます。
#define FTXUI_MACRO_CONDITIONAL_(condition, consequent, alternate) \
  FTXUI_MACRO_EXPAND(FTXUI_MACRO_SELECT_THIRD_ARGUMENT_(           \
      FTXUI_MACRO_CONDITIONAL_COMMA_(condition), consequent, alternate))

// 最初の引数が|1|に展開される場合にのみコンマ(,)に展開されます。
// |FTXUI_MACRO_SELECT_THIRD_ARGUMENT_()|と組み合わせて使用され、
// 余分なコンマの有無を使用して後続の引数位置を条件付きでシフトさせ、
// どの引数が選択されるかに影響を与えることができます。
#define FTXUI_MACRO_CONDITIONAL_COMMA_(...) \
  FTXUI_MACRO_EXPAND(FTXUI_MACRO_CONDITIONAL_COMMA_IMPL_(__VA_ARGS__, dummy))
#define FTXUI_MACRO_CONDITIONAL_COMMA_IMPL_(x, ...) \
  FTXUI_MACRO_CONDITIONAL_COMMA_##x##_
#define FTXUI_MACRO_CONDITIONAL_COMMA_1_ ,

// 単純に3番目の引数を選択するヘルパー。上記の
// |FTXUI_MACRO_CONDITIONAL_COMMA_()|と組み合わせて条件付きマクロ展開を
// 実装するために使用されます。
#define FTXUI_MACRO_SELECT_THIRD_ARGUMENT_(...) \
  FTXUI_MACRO_EXPAND(                           \
      FTXUI_MACRO_SELECT_THIRD_ARGUMENT_IMPL_(__VA_ARGS__, dummy))
#define FTXUI_MACRO_SELECT_THIRD_ARGUMENT_IMPL_(a, b, c, ...) c

#endif  // FTXUI_UTIL_EXPORT_H_
