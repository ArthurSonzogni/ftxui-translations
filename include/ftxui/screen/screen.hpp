// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#ifndef FTXUI_SCREEN_SCREEN_HPP
#define FTXUI_SCREEN_SCREEN_HPP

#include <cstdint>     // for uint8_t
#include <functional>  // for function
#include <string>      // for string, basic_string, allocator
#include <vector>      // for vector

#include "ftxui/screen/image.hpp"     // for Pixel, Image
#include "ftxui/screen/terminal.hpp"  // for Dimensions

namespace ftxui {

/// @brief Définit l'apparence des dimensions de l'écran.
/// @ingroup screen
namespace Dimension {
Dimensions Fixed(int);
Dimensions Full();
}  // namespace Dimension

/// @brief Une grille rectangulaire de pixels.
/// @ingroup screen
class Screen : public Image {
 public:
  // Constructors:
  Screen(int dimx, int dimy);
  static Screen Create(Dimensions dimension);
  static Screen Create(Dimensions width, Dimensions height);

  // Destructor:
  ~Screen() override = default;

  std::string ToString() const;

  // Affiche l'écran sur le terminal.
  void Print() const;

  // Remplit l'écran avec des espaces et réinitialise tout état de l'écran, comme les hyperliens, et
  // le curseur.
  void Clear();

  // Déplace le curseur du terminal de n lignes vers le haut, où n = dimy().
  std::string ResetPosition(bool clear = false) const;

  void ApplyShader();

  struct Cursor {
    int x = 0;
    int y = 0;

    enum Shape {
      Hidden = 0,
      BlockBlinking = 1,
      Block = 2,
      UnderlineBlinking = 3,
      Underline = 4,
      BarBlinking = 5,
      Bar = 6,
    };
    Shape shape = Hidden;
  };

  Cursor cursor() const { return cursor_; }
  void SetCursor(Cursor cursor) { cursor_ = cursor; }

  // Stocke un hyperlien dans l'écran. Retourne l'identifiant de l'hyperlien. L'identifiant est
  // utilisé pour identifier l'hyperlien lorsque l'utilisateur clique dessus.
  uint8_t RegisterHyperlink(const std::string& link);
  const std::string& Hyperlink(uint8_t id) const;

  using SelectionStyle = std::function<void(Pixel&)>;
  const SelectionStyle& GetSelectionStyle() const;
  void SetSelectionStyle(SelectionStyle decorator);

 protected:
  Cursor cursor_;
  std::vector<std::string> hyperlinks_ = {""};

  // Le style de sélection actuel. Ceci est remplacé par divers éléments DOM.
  SelectionStyle selection_style_ = [](Pixel& pixel) {
    pixel.inverted ^= true;
  };
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_SCREEN_HPP
