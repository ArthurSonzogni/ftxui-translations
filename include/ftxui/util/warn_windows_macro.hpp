// Copyright 2025 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.

#ifndef FTXUI_UTIL_WARN_WINDOWS_MACRO_H_
#define FTXUI_UTIL_WARN_WINDOWS_MACRO_H_

#ifdef min
#error \
    "La macro 'min' est définie, ce qui entre en conflit avec la bibliothèque C++ standard et FTXUI. Cela est souvent causé par l'inclusion de <windows.h>. Pour résoudre ce problème, ajoutez '#define NOMINMAX' avant d'inclure <windows.h>, ou passez '/DNOMINMAX' comme option de compilation."
#endif

#ifdef max
#error \
    "La macro 'max' est définie, ce qui entre en conflit avec la bibliothèque C++ standard et FTXUI. Cela est souvent causé par l'inclusion de <windows.h>. Pour résoudre ce problème, ajoutez '#define NOMINMAX' avant d'inclure <windows.h>, ou passez '/DNOMINMAX' comme option de compilation."
#endif

#endif  // FTXUI_UTIL_WARN_WINDOWS_MACRO_H_
