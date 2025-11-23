// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
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
  int left_size = 20;
  int right_size = 20;
  int top_size = 10;
  int bottom_size = 10;

  // Rendu :
  auto RendererInfo = [](const std::string& name, int* size) {
    return Renderer([name, size] {
      return text(name + ": " + std::to_string(*size)) | center;
    });
  };
  auto middle = Renderer([] { return text("Middle") | center; });
  auto left = RendererInfo("Left", &left_size);
  auto right = RendererInfo("Right", &right_size);
  auto top = RendererInfo("Top", &top_size);
  auto bottom = RendererInfo("Bottom", &bottom_size);

  auto container = middle;
  container = ResizableSplitLeft(left, container, &left_size);
  container = ResizableSplitRight(right, container, &right_size);
  container = ResizableSplitTop(top, container, &top_size);
  container = ResizableSplitBottom(bottom, container, &bottom_size);

  auto renderer =
      Renderer(container, [&] { return container->Render() | border; });

  screen.Loop(renderer);
}