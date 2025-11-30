// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef FTXUI_SCREEN_BOX_HPP
#define FTXUI_SCREEN_BOX_HPP

namespace ftxui {

/// @brief Box es una estructura que representa un área rectangular en un espacio 2D.
///
/// Se define por sus coordenadas mínimas y máximas a lo largo de los ejes x e y.
/// Tenga en cuenta que las coordenadas son inclusivas, lo que significa que la caja incluye
/// tanto los valores mínimos como los máximos.
///
/// @ingroup screen
struct Box {
  int x_min = 0;
  int x_max = 0;
  int y_min = 0;
  int y_max = 0;

  static auto Intersection(Box a, Box b) -> Box;
  static auto Union(Box a, Box b) -> Box;
  void Shift(int x, int y);
  bool Contain(int x, int y) const;
  bool IsEmpty() const;
  bool operator==(const Box& other) const;
  bool operator!=(const Box& other) const;
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_BOX_HPP