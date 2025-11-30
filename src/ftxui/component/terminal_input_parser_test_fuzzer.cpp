// Copyright 2021 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <cstddef>
#include <ftxui/component/event.hpp>
#include "ftxui/component/terminal_input_parser.hpp"

extern "C" int LLVMFuzzerTestOneInput(const char* data, size_t size) {
  using namespace ftxui;
  auto parser = TerminalInputParser([&](Event) {});
  for (size_t i = 0; i < size; ++i) {
    parser.Add(data[i]);
  }

  return 0;  // Los valores de retorno distintos de cero están reservados para uso futuro.
}
