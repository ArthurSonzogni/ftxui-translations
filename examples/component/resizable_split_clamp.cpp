// Copyright 2025 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui se trouve dans
// le fichier LICENSE.
#include <memory>  // pour shared_ptr, allocator, __shared_ptr_access

#include "ftxui/component/component.hpp"  // pour Renderer, ResizableSplitBottom, ResizableSplitLeft, ResizableSplitRight, ResizableSplitTop
#include "ftxui/component/component_base.hpp"      // pour ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // pour ScreenInteractive
#include "ftxui/dom/elements.hpp"  // pour Element, operator|, text, center, border

using namespace ftxui;

int main() {
  auto screen = ScreenInteractive::Fullscreen();

  // État :
  int size = 40;
  int size_min = 10;
  int size_max = 80;

  // Rendu :
  auto split = ResizableSplit({
      .main = Renderer([] { return text("Left") | center; }),
      .back = Renderer([] { return text("Right") | center; }),
      .direction = Direction::Left,
      .main_size = &size,
      .min = &size_min,
      .max = &size_max,
  });

  auto renderer = Renderer(split, [&] {
    return window(text("Faites glisser le séparateur avec la souris"),
                  vbox({
                      text("Min:  " + std::to_string(size_min)),
                      text("Max:  " + std::to_string(size_max)),
                      text("Size: " + std::to_string(size)),
                      separator(),
                      split->Render() | flex,
                  }));
  });

  screen.Loop(renderer);
}