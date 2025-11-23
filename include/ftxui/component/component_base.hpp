// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#ifndef FTXUI_COMPONENT_BASE_HPP
#define FTXUI_COMPONENT_BASE_HPP

#include <memory>  // for unique_ptr
#include <vector>  // for vector

#include "ftxui/component/captured_mouse.hpp"  // for CaptureMouse
#include "ftxui/dom/elements.hpp"              // for Element

namespace ftxui {

class Delegate;
class Focus;
struct Event;

namespace animation {
class Params;
}  // namespace animation

class ComponentBase;
using Component = std::shared_ptr<ComponentBase>;
using Components = std::vector<Component>;

/// @brief Il implémente son propre rendu en tant que ftxui::Element. Il implémente
/// la navigation au clavier en répondant à ftxui::Event.
/// @ingroup component
class ComponentBase {
 public:
  explicit ComponentBase(Components children)
      : children_(std::move(children)) {}
  virtual ~ComponentBase();
  ComponentBase() = default;

  // A component is not copyable/movable.
  ComponentBase(const ComponentBase&) = delete;
  ComponentBase(ComponentBase&&) = delete;
  ComponentBase& operator=(const ComponentBase&) = delete;
  ComponentBase& operator=(ComponentBase&&) = delete;

  // Hiérarchie des composants:
  ComponentBase* Parent() const;
  Component& ChildAt(size_t i);
  size_t ChildCount() const;
  int Index() const;
  void Add(Component children);
  void Detach();
  void DetachAllChildren();

  // Rend le composant.
  Element Render();

  // Outrepasse cette fonction pour modifier le fonctionnement de `Render`.
  virtual Element OnRender();

  // Gère un événement.
  // Par défaut, réduit sur les enfants avec un OU paresseux.
  //
  // Retourne si l'événement a été géré ou non.
  virtual bool OnEvent(Event);

  // Gère une étape d'animation.
  virtual void OnAnimation(animation::Params& params);

  // Gestion du focus ----------------------------------------------------------
  //
  // Si ce composant contient des enfants, cela indique lequel est actif,
  // nullptr si aucun n'est actif.
  //
  // On dit qu'un élément a le focus si la chaîne de ActiveChild() à partir du
  // composant racine contient cet objet.
  virtual Component ActiveChild();

  // Retourne vrai lorsque le composant contient des éléments focusables.
  // Les composants non focusables seront ignorés lors de la navigation à l'aide du
  // clavier.
  virtual bool Focusable() const;

  // Indique si c'est l'enfant actif de son parent.
  bool Active() const;
  // Indique si tous les ancêtres sont actifs.
  bool Focused() const;

  // Fait de l'enfant le composant "actif".
  virtual void SetActiveChild(ComponentBase* child);
  void SetActiveChild(Component child);

  // Configure tous les ancêtres pour donner le focus à ce composant.
  void TakeFocus();

 protected:
  CapturedMouse CaptureMouse(const Event& event);

  Components children_;

 private:
  ComponentBase* parent_ = nullptr;
  bool in_render = false;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_BASE_HPP */
