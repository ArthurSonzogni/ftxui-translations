// Copyright 2026 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#ifndef FTXUI_UTIL_EXPORT_H_
#define FTXUI_UTIL_EXPORT_H_

// 在合併建置中，我們不希望有任何 export/import 標注。
#if defined(FTXUI_AMALGAMATED)
#define FTXUI_EXPORT(component)
#define INSIDE_FTXUI_COMPONENT_IMPL(component) 0
#else

// 用來標注由名為 |component| 的元件所匯出的符號。請注意，
// 這只有在對應元件目標的原始碼是以 |IS_FTXUI_$component_IMPL|
// 定義為 1 進行編譯時，才會產生正確的效果。例如：
//
//   class FTXUI_EXPORT(FOO) Bar {};
//
// 若編譯時 IS_FTXUI_FOO_IMPL=1，則 Bar 會使用下方定義的
// FTXUI_EXPORT_ANNOTATION 巨集加以標注。否則會使用
// FTXUI_IMPORT_ANNOTATION 巨集加以標注。
#define FTXUI_EXPORT(component)                         \
  FTXUI_MACRO_CONDITIONAL_(IS_FTXUI_##component##_IMPL, \
                           FTXUI_EXPORT_ANNOTATION, FTXUI_IMPORT_ANNOTATION)

// 指出目前的編譯單元是否作為名為 |component| 的元件之
// 實作的一部分被編譯。若 |IS_FTXUI_$component_IMPL| 被定義
// 為 |1|，則展開為 |1|；否則展開為 |0|。
//
// 特別要注意的是，即使完全未定義 |IS_FTXUI_$component_IMPL|，
// 測試 INSIDE_FTXUI_COMPONENT_IMPL(component) 仍然是安全的，
// 它會如預期展開為 |0|。
#define INSIDE_FTXUI_COMPONENT_IMPL(component) \
  FTXUI_MACRO_CONDITIONAL_(IS_FTXUI_##component##_IMPL, 1, 0)

#endif

// 用於標注符號匯出或匯入的編譯器特定巨集。在非元件建置中
// 為空操作。這些巨集不應被直接大量使用。
// 應改用上方定義的 FTXUI_EXPORT 巨集。
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

// 此點之後是上述巨集實作所使用的幾個內部工具巨集。
// 不供外部使用。

#define FTXUI_MACRO_EXPAND(x) x

// 用於在兩個字符串中依條件展開其中一個的輔助巨集。若 |condition|
// 展開為 |1|，則此巨集展開為 |consequent|；否則展開
// 為 |alternate|。
#define FTXUI_MACRO_CONDITIONAL_(condition, consequent, alternate) \
  FTXUI_MACRO_EXPAND(FTXUI_MACRO_SELECT_THIRD_ARGUMENT_(           \
      FTXUI_MACRO_CONDITIONAL_COMMA_(condition), consequent, alternate))

// 若其第一個引數展開為 |1|，則展開為逗號 (,)。與
// |FTXUI_MACRO_SELECT_THIRD_ARGUMENT_()| 搭配使用，
// 因為額外逗號的有無可用來依條件移動後續
// 引數的位置，進而影響選取哪個引數。
#define FTXUI_MACRO_CONDITIONAL_COMMA_(...) \
  FTXUI_MACRO_EXPAND(FTXUI_MACRO_CONDITIONAL_COMMA_IMPL_(__VA_ARGS__, dummy))
#define FTXUI_MACRO_CONDITIONAL_COMMA_IMPL_(x, ...) \
  FTXUI_MACRO_CONDITIONAL_COMMA_##x##_
#define FTXUI_MACRO_CONDITIONAL_COMMA_1_ ,

// 僅單純選取其第三個引數的輔助巨集。與
// 上方的 |FTXUI_MACRO_CONDITIONAL_COMMA_()| 搭配使用，
// 以實作條件式巨集展開。
#define FTXUI_MACRO_SELECT_THIRD_ARGUMENT_(...) \
  FTXUI_MACRO_EXPAND(                           \
      FTXUI_MACRO_SELECT_THIRD_ARGUMENT_IMPL_(__VA_ARGS__, dummy))
#define FTXUI_MACRO_SELECT_THIRD_ARGUMENT_IMPL_(a, b, c, ...) c

#endif  // FTXUI_UTIL_EXPORT_H_
