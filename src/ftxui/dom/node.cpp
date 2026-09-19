// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <ftxui/screen/box.hpp>  // for Box
#include <string>
#include <utility>  // for move

#include <cstddef>
#include "ftxui/dom/node.hpp"
#include "ftxui/dom/selection.hpp"    // for Selection
#include "ftxui/screen/screen.hpp"    // for Screen
#include "ftxui/screen/terminal.hpp"  // for GetQuirks

namespace ftxui {

Node::Node() = default;
Node::Node(Elements children) : children_(std::move(children)) {}
Node::~Node() = default;

/// @brief 要素が必要とするスペースを計算します。
void Node::ComputeRequirement() {
  if (children_.empty()) {
    return;
  }
  for (auto& child : children_) {
    child->ComputeRequirement();
  }

  // デフォルトでは、要件は最初の子のものである。
  requirement_ = children_[0]->requirement();

  // フォーカス要求を伝播する。
  for (size_t i = 1; i < children_.size(); ++i) {
    if (requirement_.focused.Prefer(children_[i]->requirement().focused)) {
      requirement_.focused = children_[i]->requirement().focused;
    }
  }
}

/// @brief 描画のために要素に位置と次元を割り当てます。
void Node::SetBox(Box box) {
  box_ = box;
}

/// @brief 要素の選択を計算します。
void Node::Select(Selection& selection) {
  // このノードのbox_が選択範囲と交差しない場合、選択は行われません。
  if (Box::Intersection(selection.GetBox(), box_).IsEmpty()) {
    return;
  }

  // デフォルトでは選択を子に委ねる。
  for (auto& child : children_) {
    child->Select(selection);
  }
}

/// @brief 要素をftxui::Screenに表示します。
void Node::Render(Screen& screen) {
  for (auto& child : children_) {
    child->Render(screen);
  }
}

void Node::Check(Status* status) {
  for (auto& child : children_) {
    child->Check(status);
  }
  status->need_iteration |= (status->iteration == 0);
}

std::string Node::GetSelectedContent(Selection& selection) {
  std::string content;

  for (auto& child : children_) {
    content += child->GetSelectedContent(selection);
  }

  return content;
}

void Node::Reserved1() {}
void Node::Reserved2() {}
void Node::Reserved3() {}
void Node::Reserved4() {}
void Node::Reserved5() {}
void Node::Reserved6() {}
void Node::Reserved7() {}
void Node::Reserved8() {}

/// @brief 要素をftxui::Screenに表示します。
/// @ingroup dom
void Render(Screen& screen, const Element& element) {
  Selection selection;
  Render(screen, element.get(), selection);
}

/// @brief 要素をftxui::Screenに表示します。
/// @ingroup dom
void Render(Screen& screen, Node* node) {
  Selection selection;
  Render(screen, node, selection);
}

void Render(Screen& screen, Node* node, Selection& selection) {
  Box box;
  box.x_min = 0;
  box.y_min = 0;
  box.x_max = screen.dimx() - 1;
  box.y_max = screen.dimy() - 1;

  Node::Status status;
  node->Check(&status);
  const int max_iterations = 20;
  while (status.need_iteration && status.iteration < max_iterations) {
    // ステップ1: この要素がどの寸法になりたいかを見つける。
    node->ComputeRequirement();

    // ステップ2: 要素に寸法を割り当てる。
    node->SetBox(box);

    // 要素がレイアウトアルゴリズムのもう一回の反復を必要とするか確認する。
    status.need_iteration = false;
    status.iteration++;
    node->Check(&status);
  }

  // ステップ3: 選択
  if (!selection.IsEmpty()) {
    node->Select(selection);
  }

  bool use_cursor = node->requirement().focused.enabled;
  if (!Terminal::GetQuirks().CursorHiding() &&
      node->requirement().focused.cursor_shape ==
          Screen::Cursor::Shape::Hidden) {
    // カーソルを正しい位置に設定することで、CJK(中国語、日本語、韓国語など)
    // の文字を使う人々が自分の[input method editor]が正しい位置に表示される
    // のを見られるようにする。[issue]を参照。
    //
    // [input method editor]:
    // https://en.wikipedia.org/wiki/Input_method
    //
    // [issue]:
    // https://github.com/ArthurSonzogni/FTXUI/issues/2#issuecomment-505282355
    //
    // 残念ながら、Microsoftターミナルはカーソルを隠す処理を適切に扱わない。
    // 代わりにカーソルの下の文字が隠されてしまい、これは大きな問題である。
    // その結果、カーソルを正しい位置に設定することはできない。右下隅に
    // 表示される。
    // 参照:
    // https://github.com/microsoft/terminal/issues/1203
    // https://github.com/microsoft/terminal/issues/3093
    use_cursor = false;
  }

  if (use_cursor) {
    screen.SetCursor(Screen::Cursor{
        node->requirement().focused.node->box_.x_min,
        node->requirement().focused.node->box_.y_min,
        node->requirement().focused.cursor_shape,
    });
  } else {
    screen.SetCursor(Screen::Cursor{
        screen.dimx() - 1,
        screen.dimy() - 1,
        Screen::Cursor::Shape::Hidden,
    });
  }

  // ステップ4: 要素を描画する。
  screen.stencil = box;
  node->Render(screen);

  // ステップ5: シェーダーを適用する
  screen.ApplyShader();
}

std::string GetNodeSelectedContent(Screen& screen,
                                   Node* node,
                                   Selection& selection) {
  Box box;
  box.x_min = 0;
  box.y_min = 0;
  box.x_max = screen.dimx() - 1;
  box.y_max = screen.dimy() - 1;

  Node::Status status;
  node->Check(&status);
  const int max_iterations = 20;
  while (status.need_iteration && status.iteration < max_iterations) {
    // ステップ1: この要素がどの寸法になりたいかを見つける。
    node->ComputeRequirement();

    // ステップ2: 要素に寸法を割り当てる。
    node->SetBox(box);

    // 要素がレイアウトアルゴリズムのもう一回の反復を必要とするか確認する。
    status.need_iteration = false;
    status.iteration++;
    node->Check(&status);
  }

  // ステップ3: 選択
  node->Select(selection);

  // ステップ4: 選択されたコンテンツを取得する。
  return node->GetSelectedContent(selection);
}

}  // namespace ftxui
