// Copyright 2021 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <algorithm>                              // for max
#include <ftxui/component/component_options.hpp>  // for ResizableSplitOption
#include <ftxui/dom/direction.hpp>  // for Direction, Direction::Down, Direction::Left, Direction::Right, Direction::Up
#include <ftxui/util/ref.hpp>       // for Ref
#include <functional>               // for function
#include <utility>                  // for move

#include "ftxui/component/captured_mouse.hpp"  // for CapturedMouse
#include "ftxui/component/component.hpp"  // for Horizontal, Make, ResizableSplit, ResizableSplitBottom, ResizableSplitLeft, ResizableSplitRight, ResizableSplitTop
#include "ftxui/component/component_base.hpp"  // for Component, ComponentBase
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/component/mouse.hpp"  // for Mouse, Mouse::Left, Mouse::Pressed, Mouse::Released
#include "ftxui/dom/elements.hpp"  // for operator|, reflect, Element, size, EQUAL, xflex, yflex, hbox, vbox, HEIGHT, WIDTH, text
#include "ftxui/screen/box.hpp"    // for Box

namespace ftxui {
namespace {

class ResizableSplitBase : public ComponentBase, public ResizableSplitOption {
 public:
  explicit ResizableSplitBase(ResizableSplitOption options)
      : ResizableSplitOption(std::move(options)) {
    switch (direction()) {
      case Direction::Left:
        Add(Container::Horizontal({main, back}));
        break;
      case Direction::Right:
        Add(Container::Horizontal({back, main}));
        break;
      case Direction::Up:
        Add(Container::Vertical({main, back}));
        break;
      case Direction::Down:
        Add(Container::Vertical({back, main}));
        break;
    }
  }

  bool OnEvent(Event event) final {
    if (event.is_mouse()) {
      return OnMouseEvent(std::move(event));
    }
    return ComponentBase::OnEvent(std::move(event));
  }

  bool OnMouseEvent(Event event) {
    if (captured_mouse_ && event.mouse().motion == Mouse::Released) {
      captured_mouse_.reset();
      return true;
    }

    if (event.mouse().button == Mouse::Left &&
        event.mouse().motion == Mouse::Pressed &&
        separator_box_.Contain(event.mouse().x, event.mouse().y) &&
        !captured_mouse_) {
      captured_mouse_ = CaptureMouse(event);
      return true;
    }

    if (!captured_mouse_) {
      return ComponentBase::OnEvent(event);
    }

    switch (direction()) {
      case Direction::Left:
        main_size() = std::max(0, event.mouse().x - box_.x_min);
        break;
      case Direction::Right:
        main_size() = std::max(0, box_.x_max - event.mouse().x);
        break;
      case Direction::Up:
        main_size() = std::max(0, event.mouse().y - box_.y_min);
        break;
      case Direction::Down:
        main_size() = std::max(0, box_.y_max - event.mouse().y);
        break;
    }

    main_size() = std::clamp(main_size(), min(), max());
    return true;
  }

  Element OnRender() final {
    switch (direction()) {
      case Direction::Left:
        return RenderLeft();
      case Direction::Right:
        return RenderRight();
      case Direction::Up:
        return RenderTop();
      case Direction::Down:
        return RenderBottom();
    }
    // NOTREACHED()
    return text("unreacheable");
  }

  Element RenderLeft() {
    return hbox({
               main->Render() | size(WIDTH, EQUAL, main_size()),
               separator_func() | reflect(separator_box_),
               back->Render() | xflex,
           }) |
           reflect(box_);
  }

  Element RenderRight() {
    return hbox({
               back->Render() | xflex,
               separator_func() | reflect(separator_box_),
               main->Render() | size(WIDTH, EQUAL, main_size()),
           }) |
           reflect(box_);
  }

  Element RenderTop() {
    return vbox({
               main->Render() | size(HEIGHT, EQUAL, main_size()),
               separator_func() | reflect(separator_box_),
               back->Render() | yflex,
           }) |
           reflect(box_);
  }

  Element RenderBottom() {
    return vbox({
               back->Render() | yflex,
               separator_func() | reflect(separator_box_),
               main->Render() | size(HEIGHT, EQUAL, main_size()),
           }) |
           reflect(box_);
  }

 private:
  CapturedMouse captured_mouse_;
  Box separator_box_;
  Box box_;
};

}  // namespace

/// @brief Una división entre dos componentes.
/// @param options todos los parámetros.
///
/// ### Ejemplo
///
/// ```cpp
/// auto left = Renderer([] { return text("Left") | center;});
/// auto right = Renderer([] { return text("right") | center;});
/// int left_size = 10;
/// auto component = ResizableSplit({
///   .main = left,
///   .back = right,
///   .direction = Direction::Left,
///   .main_size = &left_size,
///   .separator_func = [] { return separatorDouble(); },
/// });
/// ```
///
/// ### Salida
///
/// ```bash
///           ║
///    left   ║   right
///           ║
/// ```
Component ResizableSplit(ResizableSplitOption options) {
  return Make<ResizableSplitBase>(std::move(options));
}

/// @brief Una división horizontal entre dos componentes, configurable usando el
/// ratón.
/// @param main El componente principal de tamaño |main_size|, a la izquierda.
/// @param back El componente secundario que toma el tamaño restante, a la derecha.
/// @param main_size El tamaño del componente |main|.
/// @ingroup component
///
/// ### Ejemplo
///
/// ```cpp
/// auto screen = ScreenInteractive::Fullscreen();
/// int left_size = 10;
/// auto left = Renderer([] { return text("Left") | center;});
/// auto right = Renderer([] { return text("right") | center;});
/// auto split = ResizableSplitLeft(left, right, &left_size);
/// screen.Loop(split);
/// ```
///
/// ### Salida
///
/// ```bash
///           │
///    left   │   right
///           │
/// ```
Component ResizableSplitLeft(Component main, Component back, int* main_size) {
  return ResizableSplit({
      std::move(main),
      std::move(back),
      Direction::Left,
      main_size,
  });
}

/// @brief Una división horizontal entre dos componentes, configurable usando el
/// ratón.
/// @param main El componente principal de tamaño |main_size|, a la derecha.
/// @param back El componente secundario que toma el tamaño restante, a la izquierda.
/// @param main_size El tamaño del componente |main|.
/// @ingroup component
///
/// ### Ejemplo
///
/// ```cpp
/// auto screen = ScreenInteractive::Fullscreen();
/// int right_size = 10;
/// auto left = Renderer([] { return text("Left") | center;});
/// auto right = Renderer([] { return text("right") | center;});
/// auto split = ResizableSplitRight(right, left, &right_size);
/// screen.Loop(split);
/// ```
///
/// ### Salida
///
/// ```bash
///           │
///    left   │   right
///           │
/// ```
Component ResizableSplitRight(Component main, Component back, int* main_size) {
  return ResizableSplit({
      std::move(main),
      std::move(back),
      Direction::Right,
      main_size,
  });
}

/// @brief Una división vertical entre dos componentes, configurable usando el
/// ratón.
/// @param main El componente principal de tamaño |main_size|, en la parte superior.
/// @param back El componente secundario que toma el tamaño restante, en la parte inferior.
/// @param main_size El tamaño del componente |main|.
/// @ingroup component
///
/// ### Ejemplo
///
/// ```cpp
/// auto screen = ScreenInteractive::Fullscreen();
/// int top_size = 1;
/// auto top = Renderer([] { return text("Top") | center;});
/// auto bottom = Renderer([] { return text("Bottom") | center;});
/// auto split = ResizableSplitTop(top, bottom, &top_size);
/// screen.Loop(split);
/// ```
///
/// ### Salida
///
/// ```bash
///    top
/// ────────────
///    bottom
/// ```
Component ResizableSplitTop(Component main, Component back, int* main_size) {
  return ResizableSplit({
      std::move(main),
      std::move(back),
      Direction::Up,
      main_size,
  });
}

/// @brief Una división vertical entre dos componentes, configurable usando el
/// ratón.
/// @param main El componente principal de tamaño |main_size|, en la parte inferior.
/// @param back El componente secundario que toma el tamaño restante, en la parte superior.
/// @param main_size El tamaño del componente |main|.
/// @ingroup component
///
/// ### Ejemplo
///
/// ```cpp
/// auto screen = ScreenInteractive::Fullscreen();
/// int bottom_size = 1;
/// auto top = Renderer([] { return text("Top") | center;});
/// auto bottom = Renderer([] { return text("Bottom") | center;});
/// auto split = ResizableSplit::Bottom(bottom, top, &bottom_size);
/// screen.Loop(split);
/// ```
///
/// ### Salida
///
/// ```bash
///    top
/// ────────────
///    bottom
/// ```
Component ResizableSplitBottom(Component main, Component back, int* main_size) {
  return ResizableSplit({
      std::move(main),
      std::move(back),
      Direction::Down,
      main_size,
  });
}

}  // namespace ftxui