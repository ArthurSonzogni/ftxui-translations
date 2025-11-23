// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <algorithm>  // for find_if
#include <cassert>    // for assert
#include <cstddef>    // for size_t
#include <iterator>   // for begin, end
#include <memory>     // for unique_ptr, make_unique
#include <utility>    // for move
#include <vector>     // for vector, __alloc_traits<>::value_type

#include "ftxui/component/captured_mouse.hpp"  // for CapturedMouse, CapturedMouseInterface
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"  // for ComponentBase, Components
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"                  // for text, Element
#include "ftxui/dom/node.hpp"                      // for Node, Elements
#include "ftxui/screen/box.hpp"                    // for Box

namespace ftxui::animation {
class Params;
}  // namespace ftxui::animation

namespace ftxui {

namespace {
class CaptureMouseImpl : public CapturedMouseInterface {};
}  // namespace

ComponentBase::~ComponentBase() {
  DetachAllChildren();
}

/// @brief Retourne le ComponentBase parent, ou nul s'il n'y en a pas.
/// @see Detach
/// @see Parent
ComponentBase* ComponentBase::Parent() const {
  return parent_;
}

/// @brief Accède à l'enfant à l'index `i`.
Component& ComponentBase::ChildAt(size_t i) {
  assert(i < ChildCount());  // NOLINT
  return children_[i];
}

/// @brief Retourne le nombre d'enfants.
size_t ComponentBase::ChildCount() const {
  return children_.size();
}

/// @brief Retourne l'index du composant dans son parent. -1 si pas de parent.
int ComponentBase::Index() const {
  if (parent_ == nullptr) {
    return -1;
  }
  int index = 0;
  for (const Component& child : parent_->children_) {
    if (child.get() == this) {
      return index;
    }
    index++;
  }
  return -1;  // Not reached.
}

/// @brief Ajoute un enfant.
/// @@param child L'enfant à attacher.
void ComponentBase::Add(Component child) {
  child->Detach();
  child->parent_ = this;
  children_.push_back(std::move(child));
}

/// @brief Détache cet enfant de son parent.
/// @see Detach
/// @see Parent
void ComponentBase::Detach() {
  if (parent_ == nullptr) {
    return;
  }
  auto it = std::find_if(std::begin(parent_->children_),  //
                         std::end(parent_->children_),    //
                         [this](const Component& that) {  //
                           return this == that.get();
                         });
  ComponentBase* parent = parent_;
  parent_ = nullptr;
  parent->children_.erase(it);  // Might delete |this|.
}

/// @brief Supprime tous les enfants.
void ComponentBase::DetachAllChildren() {
  while (!children_.empty()) {
    children_[0]->Detach();
  }
}

/// @brief Dessine le composant.
/// Construit un ftxui::Element à dessiner sur l'écran ftxui::Screen représentant ce
/// ftxui::ComponentBase. Veuillez surcharger OnRender() pour modifier le rendu.
Element ComponentBase::Render() {
  // Some users might call `ComponentBase::Render()` from
  // `T::OnRender()`. To avoid infinite recursion, we use a flag.
  if (in_render) {
    return ComponentBase::OnRender();
  }

  in_render = true;
  Element element = OnRender();
  in_render = false;

  class Wrapper : public Node {
   public:
    bool active_ = false;

    Wrapper(Element child, bool active)
        : Node({std::move(child)}), active_(active) {}

    void SetBox(Box box) override {
      Node::SetBox(box);
      children_[0]->SetBox(box);
    }

    void ComputeRequirement() override {
      Node::ComputeRequirement();
      requirement_.focused.component_active = active_;
    }
  };

  return std::make_shared<Wrapper>(std::move(element), Active());
}

/// @brief Dessine le composant.
/// Construit un ftxui::Element à dessiner sur l'écran ftxui::Screen représentant ce
/// ftxui::ComponentBase. Cette fonction est destinée à être surchargée.
Element ComponentBase::OnRender() {
  if (children_.size() == 1) {
    return children_.front()->Render();
  }

  return text("Not implemented component");
}

/// @brief Appelé en réponse à un événement.
/// @param event L'événement.
/// @return Vrai si l'événement a été géré.
/// L'implémentation par défaut appelle OnEvent sur chaque enfant jusqu'à ce que l'un d'eux retourne
/// vrai. Si aucun ne retourne vrai, retourne faux.
bool ComponentBase::OnEvent(Event event) {  // NOLINT
  for (Component& child : children_) {      // NOLINT
    if (child->OnEvent(event)) {
      return true;
    }
  }
  return false;
}

/// @brief Appelé en réponse à un événement d'animation.
/// @param params les paramètres de l'animation
/// L'implémentation par défaut distribue l'événement à chaque enfant.
void ComponentBase::OnAnimation(animation::Params& params) {
  for (const Component& child : children_) {
    child->OnAnimation(params);
  }
}

/// @brief Retourne l'enfant actuellement actif.
/// @return l'enfant actuellement actif.
Component ComponentBase::ActiveChild() {
  for (auto& child : children_) {
    if (child->Focusable()) {
      return child;
    }
  }
  return nullptr;
}

/// @brief Retourne vrai si le composant contient des éléments focusables.
/// Les composants non focusables seront ignorés lors de la navigation au
/// clavier.
bool ComponentBase::Focusable() const {
  for (const Component& child : children_) {  // NOLINT
    if (child->Focusable()) {
      return true;
    }
  }
  return false;
}

/// @brief Indique si l'élément est l'enfant actuellement actif de son parent.
bool ComponentBase::Active() const {
  return parent_ == nullptr || parent_->ActiveChild().get() == this;
}

/// @brief Indique si les éléments sont focusés par l'utilisateur.
/// Vrai lorsque le ComponentBase est focusé par l'utilisateur. Un élément est focusé
/// lorsqu'il est, avec tous ses ancêtres, l'ActiveChild() de leurs parents, et qu'il est
/// Focusable().
bool ComponentBase::Focused() const {
  const auto* current = this;
  while (current && current->Active()) {
    current = current->parent_;
  }
  return !current && Focusable();
}

/// @brief Fait du |child| l'élément "actif".
/// @param child l'enfant à activer.
void ComponentBase::SetActiveChild([[maybe_unused]] ComponentBase* child) {}

/// @brief Fait du |child| l'élément "actif".
/// @param child l'enfant à activer.
void ComponentBase::SetActiveChild(Component child) {  // NOLINT
  SetActiveChild(child.get());
}

/// @brief Configure tous les ancêtres pour donner le focus à ce composant.
void ComponentBase::TakeFocus() {
  ComponentBase* child = this;
  while (ComponentBase* parent = child->parent_) {
    parent->SetActiveChild(child);
    child = parent;
  }
}

/// @brief Prend le CapturedMouse si disponible. Il n'y en a qu'un seul composant de
/// eux. Il représente un composant prenant la priorité sur les autres.
/// @param event L'événement
CapturedMouse ComponentBase::CaptureMouse(const Event& event) {  // NOLINT
  if (event.screen_) {
    return event.screen_->CaptureMouse();
  }
  return std::make_unique<CaptureMouseImpl>();
}

}  // namespace ftxui