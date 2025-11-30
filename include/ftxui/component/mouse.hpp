// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef FTXUI_COMPONENT_MOUSE_HPP
#define FTXUI_COMPONENT_MOUSE_HPP
namespace ftxui {

/// @brief Un evento del ratón. Contiene las coordenadas del ratón, el botón
/// presionado y el modificador (shift, ctrl, meta).
/// @ingroup component
struct Mouse {
  enum Button {
    Left = 0,
    Middle = 1,
    Right = 2,
    None = 3,
    WheelUp = 4,
    WheelDown = 5,
    WheelLeft = 6,   /// Solo terminales compatibles.
    WheelRight = 7,  /// Solo terminales compatibles.
  };

  enum Motion {
    Released = 0,
    Pressed = 1,
    Moved = 2,
  };

  // Botón
  Button button = Button::None;

  // Movimiento
  Motion motion = Motion::Pressed;

  // Modificadores:
  bool shift = false;
  bool meta = false;
  bool control = false;

  // Coordenadas:
  int x = 0;
  int y = 0;
};

}  // namespace ftxui

#endif /* fin de la guarda de inclusión: FTXUI_COMPONENT_MOUSE_HPP */
