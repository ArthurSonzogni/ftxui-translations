// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_CELL_HPP
#define FTXUI_SCREEN_CELL_HPP

#include <cstdint>                 // for uint8_t
#include <string>                  // for string, basic_string, allocator
#include "ftxui/screen/color.hpp"  // for Color, Color::Default
#include "ftxui/util/export.hpp"

namespace ftxui {

/// @brief Un caractère Unicode et son style associé.
///
/// @note Cette structure s'appelait précédemment Pixel.
///
/// @ingroup screen
struct FTXUI_EXPORT(SCREEN) Cell {
  Cell()
      : blink(false),
        bold(false),
        dim(false),
        italic(false),
        inverted(false),
        underlined(false),
        underlined_double(false),
        strikethrough(false),
        automerge(false) {}

  // Un champ de bits représentant le style :
  bool blink : 1;
  bool bold : 1;
  bool dim : 1;
  bool italic : 1;
  bool inverted : 1;
  bool underlined : 1;
  bool underlined_double : 1;
  bool strikethrough : 1;
  bool automerge : 1;

  // Le lien hypertexte associé à la cellule.
  // 0 est la valeur par défaut, signifiant l'absence de lien hypertexte.
  // C'est un index permettant d'accéder aux méta-données de Screen.
  uint8_t hyperlink = 0;

  // Les graphèmes stockés dans la cellule. Pour supporter les caractères
  // combinants, comme : a?, ceci peut potentiellement contenir plusieurs
  // codepoints.
  std::string character = "";

  // Couleurs :
  Color background_color = Color::Default;
  Color foreground_color = Color::Default;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_CELL_HPP
