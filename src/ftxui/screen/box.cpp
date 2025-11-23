// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include "ftxui/screen/box.hpp"

#include <algorithm>

namespace ftxui {
/// @return la plus grande Box contenue dans |a| et |b|.
// static
Box Box::Intersection(Box a, Box b) {
  return Box{
      std::max(a.x_min, b.x_min),
      std::min(a.x_max, b.x_max),
      std::max(a.y_min, b.y_min),
      std::min(a.y_max, b.y_max),
  };
}

/// @return la plus petite Box contenant à la fois |a| et |b|.
// static
Box Box::Union(Box a, Box b) {
  return Box{
      std::min(a.x_min, b.x_min),
      std::max(a.x_max, b.x_max),
      std::min(a.y_min, b.y_min),
      std::max(a.y_max, b.y_max),
  };
}

/// Décale la boîte de (x,y).
/// @param x décalage horizontal.
/// @param y décalage vertical.
void Box::Shift(int x, int y) {
  x_min += x;
  x_max += x;
  y_min += y;
  y_max += y;
}

/// @return si (x,y) est contenu dans la boîte.
bool Box::Contain(int x, int y) const {
  return x_min <= x &&  //
         x_max >= x &&  //
         y_min <= y &&  //
         y_max >= y;
}

/// @return si la boîte est vide.
bool Box::IsEmpty() const {
  return x_min > x_max || y_min > y_max;
}

/// @return si |other| est identique à |this|
bool Box::operator==(const Box& other) const {
  return (x_min == other.x_min) && (x_max == other.x_max) &&
         (y_min == other.y_min) && (y_max == other.y_max);
}

/// @return si |other| et |this| sont différents.
bool Box::operator!=(const Box& other) const {
  return !operator==(other);
}

}  // namespace ftxui