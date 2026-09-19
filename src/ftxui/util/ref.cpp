// Copyright 2026 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/util/ref.hpp"

#include <utility>

namespace ftxui {

namespace {

// StringRef, ConstStringRef, y ConstStringListRef se exportan
// (FTXUI_EXPORT(SCREEN)), pero todas sus funciones miembro -- incluyendo
// las heredadas de Ref<string>/ConstRef<string> -- se definen en línea en
// el encabezado y nunca se usan (odr-used) dentro de libftxui-screen. En
// Windows, MSVC solo emite (y por lo tanto exporta con dllexport) una función
// miembro en línea hacia la DLL que la usa (odr-use) por primera vez; de lo
// contrario, los consumidores que enlazan contra la declaración dllimport
// obtienen un símbolo externo no resuelto. Esta función existe únicamente
// para forzar esa instanciación.
[[maybe_unused]] void ForceSymbolInstantiation() {
  StringRef s = "a";
  StringRef s2(s);
  StringRef s3(std::move(s2));
  s = s3;
  s = std::move(s3);
  (void)s();
  (void)*s;
  (void)s.operator->();

  ConstStringRef c = "a";
  ConstStringRef c2(c);
  ConstStringRef c3(std::move(c2));
  c = c3;
  c = std::move(c3);
  (void)c();
  (void)*c;

  ConstStringListRef l;
  ConstStringListRef l2(l);
  ConstStringListRef l3(std::move(l2));
  l = l3;
  l = std::move(l3);
  (void)l.size();
  (void)l[0];
}

}  // namespace

}  // namespace ftxui
