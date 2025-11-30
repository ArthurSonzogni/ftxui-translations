// Derechos de autor 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <memory>   // for make_shared
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for Decorator, Element, focusPosition, focusPositionRelative
#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator
#include "ftxui/dom/requirement.hpp"  // for Requirement, Requirement::NORMAL, Requirement::Selection
#include "ftxui/screen/box.hpp"  // for Box

namespace ftxui {

/// @brief Utilizado dentro de un `frame`, esto fuerza que la vista se desplace hacia
/// una posición determinada. La posición se expresa en proporción del tamaño solicitado.
///
/// Por ejemplo:
/// - (0, 0) significa que la vista se desplaza hacia la parte superior izquierda.
/// - (1, 0) significa que la vista se desplaza hacia la parte superior derecha.
/// - (0, 1) significa que la vista se desplaza hacia la parte inferior izquierda.
/// @ingroup dom
///
/// ### Ejemplo
///
/// ```cpp
/// Element document = huge_document()
///   | focusPositionRelative(0.f, 1.f)
///   | frame;
/// ```
Decorator focusPositionRelative(float x, float y) {
  class Impl : public NodeDecorator {
   public:
    Impl(Element child, float x, float y)
        : NodeDecorator(std::move(child)), x_(x), y_(y) {}

    void ComputeRequirement() override {
      NodeDecorator::ComputeRequirement();
      requirement_.focused.enabled = true;
      requirement_.focused.node = this;
      requirement_.focused.box.x_min = int(float(requirement_.min_x) * x_);
      requirement_.focused.box.y_min = int(float(requirement_.min_y) * y_);
      requirement_.focused.box.x_max = int(float(requirement_.min_x) * x_);
      requirement_.focused.box.y_max = int(float(requirement_.min_y) * y_);
    }

   private:
    const float x_;
    const float y_;
  };

  return [x, y](Element child) {
    return std::make_shared<Impl>(std::move(child), x, y);
  };
}

/// @brief Utilizado dentro de un `frame`, esto fuerza que la vista se desplace hacia
/// una posición determinada. La posición se expresa en el número de celdas.
///
/// @ingroup dom
///
/// ### Ejemplo
///
/// ```cpp
/// Element document = huge_document()
///   | focusPosition(10, 10)
///   | frame;
/// ```
Decorator focusPosition(int x, int y) {
  class Impl : public NodeDecorator {
   public:
    Impl(Element child, int x, int y)
        : NodeDecorator(std::move(child)), x_(x), y_(y) {}

    void ComputeRequirement() override {
      NodeDecorator::ComputeRequirement();
      requirement_.focused.enabled = false;

      Box& box = requirement_.focused.box;
      box.x_min = x_;
      box.y_min = y_;
      box.x_max = x_;
      box.y_max = y_;
    }

   private:
    const int x_;
    const int y_;
  };

  return [x, y](Element child) {
    return std::make_shared<Impl>(std::move(child), x, y);
  };
}

}  // namespace ftxui