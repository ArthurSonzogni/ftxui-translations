// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef FTXUI_SCREEN_SCREEN_HPP
#define FTXUI_SCREEN_SCREEN_HPP

#include <cstdint>     // for uint8_t
#include <functional>  // for function
#include <string>      // for string, basic_string, allocator
#include <vector>      // for vector

#include "ftxui/screen/image.hpp"     // for Pixel, Image
#include "ftxui/screen/terminal.hpp"  // for Dimensions

namespace ftxui {

/// @brief Define cómo deben ser las dimensiones de la pantalla.
/// @ingroup screen
namespace Dimension {
Dimensions Fixed(int);
Dimensions Full();
}  // namespace Dimension

/// @brief Una cuadrícula rectangular de píxeles.
/// @ingroup screen
class Screen : public Image {
 public:
  // Constructores:
  Screen(int dimx, int dimy);
  static Screen Create(Dimensions dimension);
  static Screen Create(Dimensions width, Dimensions height);

  // Destructor:
  ~Screen() override = default;

  std::string ToString() const;

  // Imprime la pantalla en la terminal.
  void Print() const;

  // Rellena la pantalla con espacios y reinicia cualquier estado de la pantalla, como hipervínculos y
  // cursor
  void Clear();

  // Mueve el cursor de la terminal n líneas hacia arriba con n = dimy().
  std::string ResetPosition(bool clear = false) const;

  void ApplyShader();

  struct Cursor {
    int x = 0;
    int y = 0;

    enum Shape {
      Hidden = 0,
      BlockBlinking = 1, // Bloque parpadeante
      Block = 2, // Bloque
      UnderlineBlinking = 3, // Subrayado parpadeante
      Underline = 4, // Subrayado
      BarBlinking = 5, // Barra parpadeante
      Bar = 6, // Barra
    };
    Shape shape = Hidden; // Oculto
  };

  Cursor cursor() const { return cursor_; }
  void SetCursor(Cursor cursor) { cursor_ = cursor; }

  // Almacena un hipervínculo en la pantalla. Devuelve el ID del hipervínculo. El ID se
  // utiliza para identificar el hipervínculo cuando el usuario hace clic en él.
  uint8_t RegisterHyperlink(const std::string& link);
  const std::string& Hyperlink(uint8_t id) const;

  using SelectionStyle = std::function<void(Pixel&)>;
  const SelectionStyle& GetSelectionStyle() const;
  void SetSelectionStyle(SelectionStyle decorator);

 protected:
  Cursor cursor_;
  std::vector<std::string> hyperlinks_ = {""};

  // El estilo de selección actual. Esto es anulado por varios elementos del DOM.
  SelectionStyle selection_style_ = [](Pixel& pixel) {
    pixel.inverted ^= true;
  };
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_SCREEN_HPP
