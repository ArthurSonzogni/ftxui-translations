// Copyright 2026 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/util/ref.hpp"

#include <utility>

namespace ftxui {

namespace {

// StringRef, ConstStringRef et ConstStringListRef sont exportées
// (FTXUI_EXPORT(SCREEN)), mais toutes leurs fonctions membres -- y compris
// celles héritées de Ref<string>/ConstRef<string> -- sont définies en ligne
// dans l'en-tête et ne sont jamais odr-used depuis libftxui-screen elle-même.
// Sous Windows, MSVC n'émet (et donc n'exporte via dllexport) une fonction
// membre en ligne dans la DLL que lorsqu'elle est odr-used pour la première
// fois ; sinon, les consommateurs qui se lient à la déclaration dllimport
// obtiennent un symbole externe non résolu. Cette fonction existe uniquement
// pour forcer cette instanciation.
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
