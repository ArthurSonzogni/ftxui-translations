// Copyright 2020 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSE ファイルにある MIT ライセンスによって管理されます。
// the LICENSE file.
#include <ftxui/screen/box.hpp>  // for Box
#include <string>
#include <utility>  // for move

#include <cstddef>
#include "ftxui/dom/node.hpp"
#include "ftxui/dom/selection.hpp"  // for Selection
#include "ftxui/screen/screen.hpp"  // for Screen

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

  // By default, the requirement is the one of the first child.
  requirement_ = children_[0]->requirement();

  // Propagate the focused requirement.
  for (size_t i = 1; i < children_.size(); ++i) {
    if (!requirement_.focused.enabled &&
        children_[i]->requirement().focused.enabled) {
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

  // By default we defer the selection to the children.
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
    // ステップ1: この要素がどの次元になるかを検索します。
    node->ComputeRequirement();

    // ステップ2: 要素に次元を割り当てます。
    node->SetBox(box);

    // 要素がレイアウトアルゴリズムの別のイテレーションを必要とするかどうかを確認します。
    status.need_iteration = false;
    status.iteration++;
    node->Check(&status);
  }

  // ステップ3: 選択
  if (!selection.IsEmpty()) {
    node->Select(selection);
  }

  if (node->requirement().focused.enabled
#if defined(FTXUI_MICROSOFT_TERMINAL_FALLBACK)
      // カーソルを正しい位置に設定すると、CJK (中国語、日本語、韓国語など) 文字を使用するユーザーは、
      // [入力メソッドエディタ] が正しい場所に表示されるのを確認できます。 [issue] を参照してください。
      //
      // [input method editor]:
      // https://en.wikipedia.org/wiki/Input_method
      //
      // [issue]:
      // https://github.com/ArthurSonzogni/FTXUI/issues/2#issuecomment-505282355
      //
      // 残念ながら、Microsoft Terminal はカーソルを適切に非表示にすることを処理しません。
      // 代わりに、カーソルの下の文字が非表示になり、これは大きな問題です。
      // その結果、カーソルを正しい位置に設定することはできません。
      // 右下隅に表示されます。
      // 参照:
      // https://github.com/microsoft/terminal/issues/1203
      // https://github.com/microsoft/terminal/issues/3093
      &&
      node->requirement().focused.cursor_shape != Screen::Cursor::Shape::Hidden
#endif
  ) {
    screen.SetCursor(Screen::Cursor{
        node->requirement().focused.node->box_.x_max,
        node->requirement().focused.node->box_.y_max,
        node->requirement().focused.cursor_shape,
    });
  } else {
    screen.SetCursor(Screen::Cursor{
        screen.dimx() - 1,
        screen.dimy() - 1,
        Screen::Cursor::Shape::Hidden,
    });
  }

  // ステップ4: 要素を描画します。
  screen.stencil = box;
  node->Render(screen);

  // ステップ5: シェーダーを適用します。
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
    // ステップ1: この要素がどの次元になるかを検索します。
    node->ComputeRequirement();

    // ステップ2: 要素に次元を割り当てます。
    node->SetBox(box);

    // 要素がレイアウトアルゴリズムの別のイテレーションを必要とするかどうかを確認します。
    status.need_iteration = false;
    status.iteration++;
    node->Check(&status);
  }

  // ステップ3: 選択
  node->Select(selection);

  // ステップ4: 選択されたコンテンツを取得します。
  return node->GetSelectedContent(selection);
}

}  // namespace ftxui
