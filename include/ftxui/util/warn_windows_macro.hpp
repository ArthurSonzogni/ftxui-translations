// Copyright 2025 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#ifndef FTXUI_UTIL_WARN_WINDOWS_MACRO_H_
#define FTXUI_UTIL_WARN_WINDOWS_MACRO_H_

#ifdef min
#error \
    "La macro 'min' está definida, lo que entra en conflicto con la librería estándar de C++ y FTXUI. Esto a menudo es causado por la inclusión de <windows.h>. Para solucionarlo, añada '#define NOMINMAX' antes de incluir <windows.h>, o pase '/DNOMINMAX' como un flag del compilador."
#endif

#ifdef max
#error \
    "La macro 'max' está definida, lo que entra en conflicto con la librería estándar de C++ y FTXUI. Esto a menudo es causado por la inclusión de <windows.h>. Para solucionarlo, añada '#define NOMINMAX' antes de incluir <windows.h>, o pase '/DNOMINMAX' como un flag del compilador."
#endif

#endif  // FTXUI_UTIL_WARN_WINDOWS_MACRO_H_