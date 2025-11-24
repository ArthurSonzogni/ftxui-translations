// Copyright 2025 Arthur Sonzogni. All rights reserved. (日本語訳: Arthur Sonzogni 2025年著作権所有。無断複写・転載を禁じます。)
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスによって管理されています。
// LICENSE ファイルを参照してください。

#ifndef FTXUI_UTIL_WARN_WINDOWS_MACRO_H_
#define FTXUI_UTIL_WARN_WINDOWS_MACRO_H_

#ifdef min
#error \
    "マクロ 'min' が定義されています。これは標準 C++ ライブラリおよび FTXUI と競合します。これはしばしば <windows.h> のインクルードによって引き起こされます。これを解決するには、<windows.h> をインクルードする前に '#define NOMINMAX' を追加するか、コンパイラフラグとして '/DNOMINMAX' を渡してください。"
#endif

#ifdef max
#error \
    "マクロ 'max' が定義されています。これは標準 C++ ライブラリおよび FTXUI と競合します。これはしばしば <windows.h> のインクルードによって引き起こされます。これを解決するには、<windows.h> をインクルードする前に '#define NOMINMAX' を追加するか、コンパイラフラグとして '/DNOMINMAX' を渡してください。"
#endif

#endif  // FTXUI_UTIL_WARN_WINDOWS_MACRO_H_
