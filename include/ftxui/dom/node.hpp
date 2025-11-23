// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui se trouve
// dans le fichier LICENSE.
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
class Node {
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

  // La mise en page peut ne pas se résoudre en une seule itération pour certains éléments. Ceci
  // leur permet de demander des itérations supplémentaires. Ce signal doit être
  // transmis aux enfants au moins une fois.
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
