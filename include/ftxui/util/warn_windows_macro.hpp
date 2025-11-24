// Copyright 2025 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#ifndef FTXUI_UTIL_WARN_WINDOWS_MACRO_H_
#define FTXUI_UTIL_WARN_WINDOWS_MACRO_H_

#ifdef min
#error \
    "定義了宏 'min'，這與標準 C++ 庫和 FTXUI 衝突。這通常是由於包含了 <windows.h> 引起的。要解決此問題，請在包含 <windows.h> 之前添加 '#define NOMINMAX'，或將 '/DNOMINMAX' 作為編譯器標誌傳遞。"
#endif

#ifdef max
#error \
    "定義了宏 'max'，這與標準 C++ 庫和 FTXUI 衝突。這通常是由於包含了 <windows.h> 引起的。要解決此問題，請在包含 <windows.h> 之前添加 '#define NOMINMAX'，或將 '/DNOMINMAX' 作為編譯器標誌傳遞。"
#endif

#endif  // FTXUI_UTIL_WARN_WINDOWS_MACRO_H_
