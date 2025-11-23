// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui se trouve
// dans le fichier LICENSE.
#ifndef FTXUI_SCREEN_BOX_HPP
#define FTXUI_SCREEN_BOX_HPP

namespace ftxui {

/// @brief Box est une structure qui représente une zone rectangulaire dans un espace 2D.
///
/// Elle est définie par ses coordonnées minimales et maximales le long des axes x et y.
/// Notez que les coordonnées sont inclusives, ce qui signifie que la boîte comprend à la fois
/// les valeurs minimales et maximales.
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