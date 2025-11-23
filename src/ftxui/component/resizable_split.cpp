// Copyright 2021 Arthur Sonzogni. All rights reserved.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
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

/// @brief Une séparation entre deux composants.
/// @param options tous les paramètres.
///
/// ### Exemple
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
/// ### Sortie
///
/// ```bash
///           ║
///    left   ║   right
///           ║
/// ```
Component ResizableSplit(ResizableSplitOption options) {
  return Make<ResizableSplitBase>(std::move(options));
}

/// @brief Une séparation horizontale entre deux composants, configurable à l'aide de la
/// souris.
/// @param main Le composant principal de taille |main_size|, à gauche.
/// @param back Le composant arrière prenant la taille restante, à droite.
/// @param main_size La taille du composant |main|.
/// @ingroup component
///
/// ### Exemple
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
/// ### Sortie
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

/// @brief Une séparation horizontale entre deux composants, configurable à l'aide de la
/// souris.
/// @param main Le composant principal de taille |main_size|, à droite.
/// @param back Le composant arrière prenant la taille restante, à gauche.
/// @param main_size La taille du composant |main|.
/// @ingroup component
///
/// ### Exemple
///
/// ```cpp
/// auto screen = ScreenInteractive::Fullscreen();
/// int right_size = 10;
/// auto left = Renderer([] { return text("Left") | center;});
/// auto right = Renderer([] { return text("right") | center;});
/// auto split = ResizableSplitRight(right, left, &right_size)
/// screen.Loop(split);
/// ```
///
/// ### Sortie
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

/// @brief Une séparation verticale entre deux composants, configurable à l'aide de la
/// souris.
/// @param main Le composant principal de taille |main_size|, en haut.
/// @param back Le composant arrière prenant la taille restante, en bas.
/// @param main_size La taille du composant |main|.
/// @ingroup component
///
/// ### Exemple
///
/// ```cpp
/// auto screen = ScreenInteractive::Fullscreen();
/// int top_size = 1;
/// auto top = Renderer([] { return text("Top") | center;});
/// auto bottom = Renderer([] { return text("Bottom") | center;});
/// auto split = ResizableSplitTop(top, bottom, &top_size)
/// screen.Loop(split);
/// ```
///
/// ### Sortie
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

/// @brief Une séparation verticale entre deux composants, configurable à l'aide de la
/// souris.
/// @param main Le composant principal de taille |main_size|, en bas.
/// @param back Le composant arrière prenant la taille restante, en haut.
/// @param main_size La taille du composant |main|.
/// @ingroup component
///
/// ### Exemple
///
/// ```cpp
/// auto screen = ScreenInteractive::Fullscreen();
/// int bottom_size = 1;
/// auto top = Renderer([] { return text("Top") | center;});
/// auto bottom = Renderer([] { return text("Bottom") | center;});
/// auto split = ResizableSplit::Bottom(bottom, top, &bottom_size)
/// screen.Loop(split);
/// ```
///
/// ### Sortie
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