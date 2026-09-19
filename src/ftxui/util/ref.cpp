// Copyright 2026 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/util/ref.hpp"

#include <utility>

namespace ftxui {

namespace {

// StringRef、ConstStringRef、ConstStringListRef はエクスポートされています
// (FTXUI_EXPORT(SCREEN)) が、それらのメンバー関数はすべて -- Ref<string>/ConstRef<string>
// から継承されたものも含め -- ヘッダー内でインライン定義されており、
// libftxui-screen 自体の内部から odr-use されることはありません。Windows では、
// MSVC は最初にそれを odr-use するインラインメンバー関数のみを DLL に
// 出力(したがって dllexport)します。そうでない場合、dllimport 宣言に対して
// リンクする側は未解決の外部シンボルを得ることになります。この関数は、
// そのインスタンス化を強制するためだけに存在します。
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
