// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include "ftxui/screen/box.hpp"

#include <algorithm>

namespace ftxui {
/// @return el Box más grande contenido tanto en |a| como en |b|.
// static
Box Box::Intersection(Box a, Box b) {
  return Box{
      std::max(a.x_min, b.x_min),
      std::min(a.x_max, b.x_max),
      std::max(a.y_min, b.y_min),
      std::min(a.y_max, b.y_max),
  };
}

/// @return el Box más pequeño que contiene tanto a |a| como a |b|.
// static
Box Box::Union(Box a, Box b) {
  return Box{
      std::min(a.x_min, b.x_min),
      std::max(a.x_max, b.x_max),
      std::min(a.y_min, b.y_min),
      std::max(a.y_max, b.y_max),
  };
}

/// Desplaza la caja por (x,y).
/// @param x desplazamiento horizontal.
/// @param y desplazamiento vertical.
void Box::Shift(int x, int y) {
  x_min += x;
  x_max += x;
  y_min += y;
  y_max += y;
}

/// @return si (x,y) está contenido dentro de la caja.
bool Box::Contain(int x, int y) const {
  return x_min <= x &&  //
         x_max >= x &&  //
         y_min <= y &&  //
         y_max >= y;
}

/// @return si la caja está vacía.
bool Box::IsEmpty() const {
  return x_min > x_max || y_min > y_max;
}

/// @return si |other| es lo mismo que |this|
bool Box::operator==(const Box& other) const {
  return (x_min == other.x_min) && (x_max == other.x_max) &&
         (y_min == other.y_min) && (y_max == other.y_max);
}

/// @return si |other| y |this| son diferentes.
bool Box::operator!=(const Box& other) const {
  return !operator==(other);
}

}  // namespace ftxui
