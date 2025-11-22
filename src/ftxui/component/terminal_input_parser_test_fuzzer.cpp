// Copyright 2021 Arthur Sonzogni. 保留所有权利。
// 本源代码的使用受可在 LICENSE 文件中找到的 MIT 许可的约束。
#include <cstddef>
#include <ftxui/component/event.hpp>
#include "ftxui/component/terminal_input_parser.hpp"

extern "C" int LLVMFuzzerTestOneInput(const char* data, size_t size) {
  using namespace ftxui;
  auto parser = TerminalInputParser([&](Event) {});
  for (size_t i = 0; i < size; ++i) {
    parser.Add(data[i]);
  }

  return 0;  // 非零返回值保留供将来使用。
}
