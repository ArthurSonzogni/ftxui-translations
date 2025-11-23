// Copyright 2024 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#ifndef FTXUI_SCREEN_PIXEL_HPP
#define FTXUI_SCREEN_PIXEL_HPP

#include <cstdint>                 // for uint8_t
#include <string>                  // for string, basic_string, allocator
#include "ftxui/screen/color.hpp"  // for Color, Color::Default

namespace ftxui {

/// @brief Un caractère Unicode et son style associé.
/// @ingroup screen
struct Pixel {
  Pixel()
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

  // Le hyperlien associé au pixel.
  // 0 est la valeur par défaut, signifiant aucun hyperlien.
  // C'est un index pour accéder aux méta-données de l'écran.
  uint8_t hyperlink = 0;

  // Les graphèmes stockés dans le pixel. Pour supporter la combinaison de caractères,
  // comme : a?, cela peut potentiellement contenir plusieurs points de code.
  std::string character = "";

  // Couleurs :
  Color background_color = Color::Default;
  Color foreground_color = Color::Default;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_PIXEL_HPP