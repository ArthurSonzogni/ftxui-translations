// Copyright 2026 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/util/ref.hpp"

#include <utility>

namespace ftxui {

namespace {

// StringRef、ConstStringRef 及 ConstStringListRef 都有匯出
// (FTXUI_EXPORT(SCREEN))，但它們所有的成員函式──包括
// 從 Ref<string>/ConstRef<string> 繼承而來的函式──都是內聯定義在
// 標頭檔中，且從未在 libftxui-screen 內部被 odr-use。在
// Windows 上，MSVC 只會將第一次被 odr-use 的內聯成員
// 函式發出（因此匯出）到 DLL 中；否則針對 dllimport 宣告連結的
// 使用者會遇到未解析的外部符號。此
// 函式的存在純粹是為了強制進行該實例化。
[[maybe_unused]] void ForceSymbolInstantiation() {
  StringRef s = "a";
  StringRef s2(s);
  StringRef s3(std::move(s2));
  s = s3;
  s = std::move(s3);
  (void)s();
  (void)*s;
  (void)s.operator->();

  ConstStringRef c = "a";
  ConstStringRef c2(c);
  ConstStringRef c3(std::move(c2));
  c = c3;
  c = std::move(c3);
  (void)c();
  (void)*c;

  ConstStringListRef l;
  ConstStringListRef l2(l);
  ConstStringListRef l3(std::move(l2));
  l = l3;
  l = std::move(l3);
  (void)l.size();
  (void)l[0];
}

}  // namespace

}  // namespace ftxui
