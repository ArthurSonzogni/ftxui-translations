// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.

#include <stddef.h>   // para size_t
#include <algorithm>  // para max
#include <memory>     // para allocator, shared_ptr
#include <string>  // para char_traits, operator+, string, basic_string, to_string
#include <utility>  // para move
#include <vector>   // para vector

#include "ftxui/component/captured_mouse.hpp"  // para ftxui
#include "ftxui/component/component.hpp"       // para CatchEvent, Renderer
#include "ftxui/component/event.hpp"           // para Event
#include "ftxui/component/mouse.hpp"  // para Mouse, Mouse::Left, Mouse::Middle, Mouse::None, Mouse::Pressed, Mouse::Released, Mouse::Right, Mouse::WheelDown, Mouse::WheelUp
#include "ftxui/component/screen_interactive.hpp"  // para ScreenInteractive
#include "ftxui/dom/elements.hpp"  // para text, vbox, window, Element, Elements

using namespace ftxui;

std::string Code(Event event) {
  std::string codes;
  for (auto& it : event.input()) {
    codes += " " + std::to_string((unsigned int)it);
  }
  return codes;
}

int main() {
  auto screen = ScreenInteractive::TerminalOutput();

  std::vector<Event> keys;

  auto left_column = Renderer([&] {
    Elements children = {
        text("Codes"),
        separator(),
    };
    for (size_t i = std::max(0, (int)keys.size() - 20); i < keys.size(); ++i) {
      children.push_back(text(Code(keys[i])));
    }
    return vbox(children);
  });

  auto right_column = Renderer([&] {
    Elements children = {
        text("Event"),
        separator(),
    };
    for (size_t i = std::max(0, (int)keys.size() - 20); i < keys.size(); ++i) {
      children.push_back(text(keys[i].DebugString()));
    }
    return vbox(children);
  });

  int split_size = 40;
  auto component = ResizableSplitLeft(left_column, right_column, &split_size);
  component |= border;

  component |= CatchEvent([&](Event event) {
    keys.push_back(event);
    return false;
  });

  screen.Loop(component);
}