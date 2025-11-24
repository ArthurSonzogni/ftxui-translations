// Copyright 2021 Arthur Sonzogni. 版權所有。
// 本源代碼的使用受 MIT 許可證的約束，該許可證可在
// LICENSE 文件中找到。
#include <cstddef>
#include <ftxui/component/event.hpp>
#include "ftxui/component/terminal_input_parser.hpp"

extern "C" int LLVMFuzzerTestOneInput(const char* data, size_t size) {
  using namespace ftxui;
  auto parser = TerminalInputParser([&](Event) {});
  for (size_t i = 0; i < size; ++i) {
    parser.Add(data[i]);
  }

  return 0;  // 非零返回值保留供將來使用。
}
