// Copyright 2024 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#include "ftxui/dom/selection.hpp"  // for Selection
#include <algorithm>                // for max, min
#include <string_view>
#include <tuple>  // for ignore

#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator

namespace ftxui {

namespace {
class Unselectable : public NodeDecorator {
 public:
  using NodeDecorator::NodeDecorator;

  void Select(Selection& ignored) override {
    std::ignore = ignored;
    // Sobreescribe el método select para no hacer nada.
  }
};
}  // namespace

/// @brief Crea una selección vacía.
Selection::Selection() = default;

/// @brief Crea una selección.
/// @param start_x La coordenada x del inicio de la selección.
/// @param start_y La coordenada y del inicio de la selección.
/// @param end_x La coordenada x del final de la selección.
/// @param end_y La coordenada y del final de la selección.
Selection::Selection(int start_x, int start_y, int end_x, int end_y)
    : start_x_(start_x),
      start_y_(start_y),
      end_x_(end_x),
      end_y_(end_y),
      box_{
          std::min(start_x, end_x),
          std::max(start_x, end_x),
          std::min(start_y, end_y),
          std::max(start_y, end_y),
      },
      empty_(false) {}

Selection::Selection(int start_x,
                     int start_y,
                     int end_x,
                     int end_y,
                     Selection* parent)
    : start_x_(start_x),
      start_y_(start_y),
      end_x_(end_x),
      end_y_(end_y),
      box_{
          std::min(start_x, end_x),
          std::max(start_x, end_x),
          std::min(start_y, end_y),
          std::max(start_y, end_y),
      },
      parent_(parent),
      empty_(false) {}

/// @brief Obtiene el cuadro de la selección.
/// @return El cuadro de la selección.
const Box& Selection::GetBox() const {
  return box_;
}

/// @brief Satura la selección para que esté dentro del cuadro.
/// Esto es llamado por `hbox` para propagar la selección a sus hijos.
/// @param box El cuadro en el que saturar la selección.
/// @return La selección saturada.
Selection Selection::SaturateHorizontal(Box box) {
  int start_x = start_x_;
  int start_y = start_y_;
  int end_x = end_x_;
  int end_y = end_y_;

  const bool start_outside = !box.Contain(start_x, start_y);
  const bool end_outside = !box.Contain(end_x, end_y);
  const bool properly_ordered =
      start_y < end_y || (start_y == end_y && start_x <= end_x);
  if (properly_ordered) {
    if (start_outside) {
      start_x = box.x_min;
      start_y = box.y_min;
    }
    if (end_outside) {
      end_x = box.x_max;
      end_y = box.y_max;
    }
  } else {
    if (start_outside) {
      start_x = box.x_max;
      start_y = box.y_max;
    }
    if (end_outside) {
      end_x = box.x_min;
      end_y = box.y_min;
    }
  }
  return {
      start_x, start_y, end_x, end_y, parent_,
  };
}

/// @brief Satura la selección para que esté dentro del cuadro.
/// Esto es llamado por `vbox` para propagar la selección a sus hijos.
/// @param box El cuadro en el que saturar la selección.
/// @return La selección saturada.
Selection Selection::SaturateVertical(Box box) {
  int start_x = start_x_;
  int start_y = start_y_;
  int end_x = end_x_;
  int end_y = end_y_;

  const bool start_outside = !box.Contain(start_x, start_y);
  const bool end_outside = !box.Contain(end_x, end_y);
  const bool properly_ordered =
      start_y < end_y || (start_y == end_y && start_x <= end_x);

  if (properly_ordered) {
    if (start_outside) {
      start_x = box.x_min;
      start_y = box.y_min;
    }
    if (end_outside) {
      end_x = box.x_max;
      end_y = box.y_max;
    }
  } else {
    if (start_outside) {
      start_x = box.x_max;
      start_y = box.y_max;
    }
    if (end_outside) {
      end_x = box.x_min;
      end_y = box.y_min;
    }
  }
  return {start_x, start_y, end_x, end_y, parent_};
}

void Selection::AddPart(std::string_view part, int y, int left, int right) {
  if (parent_ != this) {
    parent_->AddPart(part, y, left, right);
    return;
  }
  [&] {
    if (parts_.str().empty()) {
      parts_ << part;
      return;
    }

    if (y_ != y) {
      parts_ << '\n' << part;
      return;
    }

    if (x_ == left + 1) {
      parts_ << part;
      return;
    }

    // Hay un espacio horizontal de celdas en blanco entre la parte
    // previamente registrada y esta. Estos espacios surgen de diseños que
    // separan texto seleccionable con columnas vacías en lugar de caracteres
    // de espacio literales (por ejemplo, espacios flexbox de FlexboxConfig::SetGap,
    // rellenos, decoradores de espaciado). Esas celdas están dentro de la
    // región seleccionada y se leen como espacios en pantalla, así que el texto
    // copiado también debe contenerlas. Solo se rellenan los espacios hacia
    // adelante; las partes superpuestas o fuera de orden recurren a un
    // simple añadido.
    for (int x = x_ + 1; x < left; ++x) {
      parts_ << ' ';
    }
    parts_ << part;
  }();
  y_ = y;
  x_ = right;
}

}  // namespace ftxui
