// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_SCREEN_SCREEN_HPP
#define FTXUI_SCREEN_SCREEN_HPP

#include <cstdint>     // for uint8_t
#include <functional>  // for function
#include <string>      // for string, basic_string, allocator
#include <vector>      // for vector

#include "ftxui/screen/surface.hpp"   // for Surface
#include "ftxui/screen/terminal.hpp"  // for Dimensions
#include "ftxui/util/export.hpp"      // for FTXUI_EXPORT

namespace ftxui {

/// @brief Définit l'apparence des dimensions de l'écran.
/// @ingroup screen
namespace Dimension {
FTXUI_EXPORT(SCREEN) Dimensions Fixed(int);
FTXUI_EXPORT(SCREEN) Dimensions Full();
}  // namespace Dimension

/// @brief Une grille rectangulaire de Cell.
/// @ingroup screen
class FTXUI_EXPORT(SCREEN) Screen : public Surface {
 public:
  // Constructeurs :
  Screen(int dimx, int dimy);
  static Screen Create(Dimensions dimension);
  static Screen Create(Dimensions width, Dimensions height);

  // Destructeur :
  ~Screen() override = default;

  // Copie :
  Screen(const Screen&) = default;
  Screen& operator=(const Screen&) = default;

  std::string ToString() const;
  void ToString(std::string& ss) const;

  // Affiche l'écran sur le terminal.
  void Print() const;

  // Remplit l'écran avec des espaces et réinitialise tout état de l'écran, comme les hyperliens, et
  // le curseur.
  void Clear();

  // Déplace le curseur du terminal de n lignes vers le haut, où n = dimy().
  std::string ResetPosition(bool clear = false) const;
  void ResetPosition(std::string& ss, bool clear = false) const;

  void ApplyShader();

  struct Cursor {
    int x = 0;
    int y = 0;

    enum Shape : uint8_t {
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

  // Réserve ABI :
  void Reserved1() override;
  void Reserved2() override;
  void Reserved3() override;
  void Reserved4() override;
  void Reserved5() override;
  void Reserved6() override;
  void Reserved7() override;
  void Reserved8() override;

  // Stocke un hyperlien dans l'écran. Retourne l'identifiant de l'hyperlien. L'identifiant est
  // utilisé pour identifier l'hyperlien lorsque l'utilisateur clique dessus.
  uint8_t RegisterHyperlink(std::string_view link);
  const std::string& Hyperlink(uint8_t id) const;

  using SelectionStyle = std::function<void(Cell&)>;
  const SelectionStyle& GetSelectionStyle() const;
  void SetSelectionStyle(SelectionStyle decorator);

 protected:
  Cursor cursor_;
  std::vector<std::string> hyperlinks_ = {""};

  // Le style de sélection actuel. Ceci est remplacé par divers éléments DOM.
  SelectionStyle selection_style_ = [](Cell& cell) { cell.inverted ^= true; };
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_SCREEN_HPP
