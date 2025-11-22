// Copyright 2025 Arthur Sonzogni. 保留所有权利。
// 本源代码的使用受 MIT 许可协议的约束，该协议可在
// LICENSE 文件中找到。

#ifndef FTXUI_UTIL_WARN_WINDOWS_MACRO_H_
#define FTXUI_UTIL_WARN_WINDOWS_MACRO_H_

#ifdef min
#error \
    "宏 'min' 已定义，这与标准 C++ 库和 FTXUI 冲突。这通常是由于包含了 <windows.h> 引起的。要解决此问题，请在包含 <windows.h> 之前添加 '#define NOMINMAX'，或者传递 '/DNOMINMAX' 作为编译器标志。"
#endif

#ifdef max
#error \
    "宏 'max' 已定义，这与标准 C++ 库和 FTXUI 冲突。这通常是由于包含了 <windows.h> 引起的。要解决此问题，请在包含 <windows.h> 之前添加 '#define NOMINMAX'，或者传递 '/DNOMINMAX' 作为编译器标志。"
#endif

#endif  // FTXUI_UTIL_WARN_WINDOWS_MACRO_H_
