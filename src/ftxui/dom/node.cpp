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

/// @brief Calcule l'espace nécessaire à un élément.
void Node::ComputeRequirement() {
  if (children_.empty()) {
    return;
  }
  for (auto& child : children_) {
    child->ComputeRequirement();
  }

  // Par défaut, l'exigence est celle du premier enfant.
  requirement_ = children_[0]->requirement();

  // Propager l'exigence de focus.
  for (size_t i = 1; i < children_.size(); ++i) {
    if (requirement_.focused.Prefer(children_[i]->requirement().focused)) {
      requirement_.focused = children_[i]->requirement().focused;
    }
  }
}

/// @brief Assigne une position et une dimension à un élément pour le dessin.
void Node::SetBox(Box box) {
  box_ = box;
}

/// @brief Calcule la sélection d'un élément.
void Node::Select(Selection& selection) {
  // Si la boîte de ce nœud n'intersecte pas la sélection, alors pas de sélection.
  if (Box::Intersection(selection.GetBox(), box_).IsEmpty()) {
    return;
  }

  // Par défaut, nous déléguons la sélection aux enfants.
  for (auto& child : children_) {
    child->Select(selection);
  }
}

/// @brief Affiche un élément sur un ftxui::Screen.
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

/// @brief Affiche un élément sur un ftxui::Screen.
/// @ingroup dom
void Render(Screen& screen, const Element& element) {
  Selection selection;
  Render(screen, element.get(), selection);
}

/// @brief Affiche un élément sur un ftxui::Screen.
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
    // Étape 1 : Détermine la dimension souhaitée par cet élément.
    node->ComputeRequirement();

    // Étape 2 : Assigne une dimension à l'élément.
    node->SetBox(box);

    // Vérifie si l'élément nécessite une autre itération de l'algorithme de mise en page.
    status.need_iteration = false;
    status.iteration++;
    node->Check(&status);
  }

  // Étape 3 : Sélection
  if (!selection.IsEmpty()) {
    node->Select(selection);
  }

  bool use_cursor = node->requirement().focused.enabled;
  if (!Terminal::GetQuirks().CursorHiding() &&
      node->requirement().focused.cursor_shape ==
          Screen::Cursor::Shape::Hidden) {
    // Placer le curseur à la bonne position permet aux personnes utilisant des caractères CJK (Chine,
    // Japon, Corée, ...) de voir leur [éditeur de méthode de saisie]
    // affiché au bon endroit. Voir [issue].
    //
    // [éditeur de méthode de saisie]:
    // https://en.wikipedia.org/wiki/Input_method
    //
    // [issue]:
    // https://github.com/ArthurSonzogni/FTXUI/issues/2#issuecomment-505282355
    //
    // Malheureusement, Microsoft terminal ne gère pas correctement le fait de cacher le
    // curseur. À la place, le caractère sous le curseur est caché, ce qui est un
    // gros problème. En conséquence, on ne peut pas activer le positionnement du curseur au bon
    // endroit. Il sera affiché dans le coin inférieur droit.
    // Voir :
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

  // Étape 4 : Dessine l'élément.
  screen.stencil = box;
  node->Render(screen);

  // Étape 5 : Applique les shaders.
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
    // Étape 1 : Détermine la dimension souhaitée par cet élément.
    node->ComputeRequirement();

    // Étape 2 : Assigne une dimension à l'élément.
    node->SetBox(box);

    // Vérifie si l'élément nécessite une autre itération de l'algorithme de mise en page.
    status.need_iteration = false;
    status.iteration++;
    node->Check(&status);
  }

  // Étape 3 : Sélection
  node->Select(selection);

  // Étape 4 : Récupère le contenu sélectionné.
  return node->GetSelectedContent(selection);
}

}  // namespace ftxui
