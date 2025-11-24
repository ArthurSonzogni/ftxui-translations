// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_NODE_HPP
#define FTXUI_DOM_NODE_HPP

#include <memory>  // for shared_ptr
#include <vector>  // for vector

#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/dom/selection.hpp"    // for Selection
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/screen.hpp"

namespace ftxui {

class Node;
class Screen;

using Element = std::shared_ptr<Node>;
using Elements = std::vector<Element>;

/// @brief NodeはDOMツリー内のすべての要素の基底クラスです。
///
/// DOM（Document Object Model）内の単一のノードを表し、レイアウトとレンダリングのための
/// 基本的な構造を提供します。
/// レイアウト要件の計算、ボックスの寸法の設定、コンテンツの選択、画面へのレンダリング、
/// およびレイアウトステータスの確認のためのメソッドが含まれています。
/// 通常、子要素もNodeのインスタンスです。
///
/// ユーザーはカスタム要素を作成するためにこのクラスから派生することが期待されます。
///
/// 組み込み要素のリストは `elements.hpp` ファイルにあります。
///
/// @ingroup dom
class Node {
 public:
  Node();
  explicit Node(Elements children);
  Node(const Node&) = delete;
  Node(const Node&&) = delete;
  Node& operator=(const Node&) = delete;
  Node& operator=(const Node&&) = delete;

  virtual ~Node();

  // ステップ1: レイアウト要件を計算します。この要素がどの寸法になりたいかを親に伝えます。
  //          子から親へ伝播されます。
  virtual void ComputeRequirement();
  Requirement requirement() { return requirement_; }

  // ステップ2: この要素に最終的な寸法を割り当てます。
  //          親から子へ伝播されます。
  virtual void SetBox(Box box);

  // ステップ3: (オプション) 選択
  //          親から子へ伝播されます。
  virtual void Select(Selection& selection);

  // ステップ4: この要素を描画します。
  virtual void Render(Screen& screen);

  virtual std::string GetSelectedContent(Selection& selection);

  // 一部の要素では、単一のイテレーションでレイアウトが解決しない場合があります。
  // これにより、追加のイテレーションを要求できます。このシグナルは少なくとも一度は子に転送する必要があります。
  struct Status {
    int iteration = 0;
    bool need_iteration = false;
  };
  virtual void Check(Status* status);

  friend void Render(Screen& screen, Node* node, Selection& selection);

 protected:
  Elements children_;
  Requirement requirement_;
  Box box_;
};

void Render(Screen& screen, const Element& element);
void Render(Screen& screen, Node* node);
void Render(Screen& screen, Node* node, Selection& selection);
std::string GetNodeSelectedContent(Screen& screen,
                                   Node* node,
                                   Selection& selection);

}  // namespace ftxui

#endif  // FTXUI_DOM_NODE_HPP