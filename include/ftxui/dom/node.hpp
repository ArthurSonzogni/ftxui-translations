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
#include "ftxui/util/export.hpp"

namespace ftxui {

class Node;
class Screen;

using Element = std::shared_ptr<Node>;
using Elements = std::vector<Element>;

/// @brief Node est la classe de base pour tous les éléments de l'arbre DOM.
///
/// Il représente un nœud unique dans le modèle d'objet de document (DOM) et fournit
/// la structure de base pour la mise en page et le rendu.
/// Il contient des méthodes pour calculer les exigences de mise en page, définir les dimensions
/// de la boîte, sélectionner le contenu, rendre à l'écran et vérifier l'état de la mise en page.
/// Il contient généralement des éléments enfants, qui sont également des instances de Node.
///
/// Les utilisateurs sont censés dériver de cette classe pour créer des éléments personnalisés.
///
/// Une liste des éléments intégrés peut être trouvée dans le fichier `elements.hpp`.
///
/// @ingroup dom
class FTXUI_EXPORT(DOM) Node {
 public:
  Node();
  explicit Node(Elements children);
  Node(const Node&) = delete;
  Node(const Node&&) = delete;
  Node& operator=(const Node&) = delete;
  Node& operator=(const Node&&) = delete;

  virtual ~Node();

  // Étape 1: Calculer les exigences de mise en page. Indique au parent les dimensions
  //          que cet élément souhaite avoir.
  //          Propagé des Enfants aux Parents.
  virtual void ComputeRequirement();
  Requirement requirement() { return requirement_; }

  // Étape 2: Attribuer à cet élément ses dimensions finales.
  //          Propagé des Parents aux Enfants.
  virtual void SetBox(Box box);

  // Étape 3: (optionnel) Sélection
  //          Propagé des Parents aux Enfants.
  virtual void Select(Selection& selection);

  // Étape 4: Dessiner cet élément.
  virtual void Render(Screen& screen);

  virtual std::string GetSelectedContent(Selection& selection);

  // Layout may not resolve within a single iteration for some elements. This
  // allows them to request additional iterations. This signal must be
  // forwarded to children at least once.
  struct Status {
    int iteration = 0;
    bool need_iteration = false;
  };
  virtual void Check(Status* status);

  // Réserve ABI :
  virtual void Reserved1();
  virtual void Reserved2();
  virtual void Reserved3();
  virtual void Reserved4();
  virtual void Reserved5();
  virtual void Reserved6();
  virtual void Reserved7();
  virtual void Reserved8();

  friend FTXUI_EXPORT(DOM) void Render(Screen& screen, Node* node, Selection& selection);

 protected:
  Elements children_;
  Requirement requirement_;
  Box box_;
};

FTXUI_EXPORT(DOM) void Render(Screen& screen, const Element& element);
FTXUI_EXPORT(DOM) void Render(Screen& screen, Node* node);
FTXUI_EXPORT(DOM)
void Render(Screen& screen, Node* node, Selection& selection);
FTXUI_EXPORT(DOM)
std::string GetNodeSelectedContent(Screen& screen,
                                   Node* node,
                                   Selection& selection);

}  // namespace ftxui

#endif  // FTXUI_DOM_NODE_HPP
