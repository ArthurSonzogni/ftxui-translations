// Copyright 2021 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#include <cstddef>
#include <ftxui/component/event.hpp>
#include "ftxui/component/terminal_input_parser.hpp"

extern "C" int LLVMFuzzerTestOneInput(const char* data, size_t size) {
  using namespace ftxui;
  auto parser = TerminalInputParser([&](Event) {});
  for (size_t i = 0; i < size; ++i) {
    parser.Add(data[i]);
  }

  return 0;  // Non-zero return values are reserved for future use.
}