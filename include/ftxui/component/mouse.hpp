// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#ifndef FTXUI_COMPONENT_MOUSE_HPP
#define FTXUI_COMPONENT_MOUSE_HPP
namespace ftxui {

/// @brief Un événement de souris. Il contient les coordonnées de la souris, le bouton
/// pressé et le modificateur (shift, ctrl, méta).
/// @ingroup component
struct Mouse {
  enum Button {
    Left = 0,
    Middle = 1,
    Right = 2,
    None = 3,
    WheelUp = 4,
    WheelDown = 5,
    WheelLeft = 6,   /// Supporté par le terminal uniquement.
    WheelRight = 7,  /// Supporté par le terminal uniquement.
  };

  enum Motion {
    Released = 0,
    Pressed = 1,
    Moved = 2,
  };

  // Button
  Button button = Button::None;

  // Motion
  Motion motion = Motion::Pressed;

  // Modifiers:
  bool shift = false;
  bool meta = false;
  bool control = false;

  // Coordinates:
  int x = 0;
  int y = 0;
};

}  // namespace ftxui

#endif /* fin de la garde d'inclusion: FTXUI_COMPONENT_MOUSE_HPP */