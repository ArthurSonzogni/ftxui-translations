// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>  // for find_if
#include <cassert>    // for assert
#include <cstddef>    // for size_t
#include <iterator>   // for begin, end
#include <memory>     // for unique_ptr, make_unique
#include <utility>    // for move
#include <vector>     // for vector, __alloc_traits<>::value_type

#include "ftxui/component/app.hpp"             // for Component, App
#include "ftxui/component/captured_mouse.hpp"  // for CapturedMouse, CapturedMouseInterface
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"  // for ComponentBase, Components
#include "ftxui/component/event.hpp"           // for Event
#include "ftxui/dom/elements.hpp"              // for text, Element
#include "ftxui/dom/node.hpp"                  // for Node, Elements
#include "ftxui/screen/box.hpp"                // for Box

namespace ftxui::animation {
class Params;
}  // namespace ftxui::animation

namespace ftxui {

namespace {
class CaptureMouseImpl : public CapturedMouseInterface {};
}  // namespace

struct ComponentBase::Impl {
  Components children;
  ComponentBase* parent = nullptr;
  bool in_render = false;
};

ComponentBase::ComponentBase() : impl_(std::make_unique<Impl>()) {}

ComponentBase::ComponentBase(Components children)
    : impl_(std::make_unique<Impl>()) {
  impl_->children = std::move(children);
}

ComponentBase::~ComponentBase() {
  DetachAllChildren();
}

Components& ComponentBase::children() {
  return impl_->children;
}

const Components& ComponentBase::children() const {
  return impl_->children;
}

/// @brief Retourne le ComponentBase parent, ou nul s'il n'y en a pas.
/// @see Detach
/// @see Parent
ComponentBase* ComponentBase::Parent() const {
  return impl_->parent;
}

/// @brief Accède à l'enfant à l'index `i`.
Component& ComponentBase::ChildAt(size_t i) {
  assert(i < ChildCount());  // NOLINT
  return impl_->children[i];
}

/// @brief Retourne le nombre d'enfants.
size_t ComponentBase::ChildCount() const {
  return impl_->children.size();
}

/// @brief Retourne l'index du composant dans son parent. -1 si pas de parent.
int ComponentBase::Index() const {
  if (impl_->parent == nullptr) {
    return -1;
  }
  int index = 0;
  for (const Component& child : impl_->parent->impl_->children) {
    if (child.get() == this) {
      return index;
    }
    index++;
  }
  return -1;  // Non atteint.
}

/// @brief Ajoute un enfant.
/// @param child L'enfant à attacher.
void ComponentBase::Add(Component child) {
  child->Detach();
  child->impl_->parent = this;
  impl_->children.push_back(std::move(child));
}

/// @brief Détache cet enfant de son parent.
/// @see Detach
/// @see Parent
void ComponentBase::Detach() {
  if (impl_->parent == nullptr) {
    return;
  }
  auto it = std::find_if(std::begin(impl_->parent->impl_->children),  // NOLINT
                         std::end(impl_->parent->impl_->children),    //
                         [this](const Component& that) {              //
                           return this == that.get();
                         });
  ComponentBase* parent = impl_->parent;
  impl_->parent = nullptr;
  parent->impl_->children.erase(it);  // Peut supprimer |this|.
}

/// @brief Supprime tous les enfants.
void ComponentBase::DetachAllChildren() {
  while (!impl_->children.empty()) {
    impl_->children[0]->Detach();
  }
}

/// @brief Dessine le composant.
/// Construit un ftxui::Element à dessiner sur l'écran ftxui::Screen représentant ce
/// ftxui::ComponentBase. Veuillez surcharger OnRender() pour modifier le rendu.
Element ComponentBase::Render() {
  // Certains utilisateurs pourraient appeler `ComponentBase::Render()` depuis
  // `T::OnRender()`. Pour éviter une récursion infinie, nous utilisons un drapeau.
  if (impl_->in_render) {
    return ComponentBase::OnRender();
  }

  impl_->in_render = true;
  Element element = OnRender();
  impl_->in_render = false;

  class Wrapper : public Node {
   public:
    bool active_ = false;
    bool focused_ = false;

    Wrapper(Element child, bool active, bool focused)
        : Node({std::move(child)}), active_(active), focused_(focused) {}

    void SetBox(Box box) override {
      Node::SetBox(box);
      children_[0]->SetBox(box);
    }

    void ComputeRequirement() override {
      Node::ComputeRequirement();
      requirement_.focused.component_active = active_;
      requirement_.focused.component_focused = focused_;
    }
  };

  return std::make_shared<Wrapper>(std::move(element), Active(), Focused());
}

/// @brief Dessine le composant.
/// Construit un ftxui::Element à dessiner sur l'écran ftxui::Screen représentant ce
/// ftxui::ComponentBase. Cette fonction est destinée à être surchargée.
Element ComponentBase::OnRender() {
  if (impl_->children.size() == 1) {
    return impl_->children.front()->Render();
  }

  return text("Not implemented component");
}

/// @brief Appelé en réponse à un événement.
/// @param event L'événement.
/// @return Vrai si l'événement a été géré.
/// L'implémentation par défaut appelle OnEvent sur chaque enfant jusqu'à ce que l'un d'eux retourne
/// vrai. Si aucun ne retourne vrai, retourne faux.
bool ComponentBase::OnEvent(Event event) {    // NOLINT
  for (Component& child : impl_->children) {  // NOLINT
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
  for (const Component& child : impl_->children) {
    child->OnAnimation(params);
  }
}

/// @brief Retourne l'enfant actuellement actif.
/// @return l'enfant actuellement actif.
Component ComponentBase::ActiveChild() {
  for (auto& child : impl_->children) {
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
  for (const Component& child : impl_->children) {  // NOLINT
    if (child->Focusable()) {
      return true;
    }
  }
  return false;
}

/// @brief Indique si l'élément est l'enfant actuellement actif de son parent.
bool ComponentBase::Active() const {
  return impl_->parent == nullptr || impl_->parent->ActiveChild().get() == this;
}

/// @brief Indique si les éléments sont focusés par l'utilisateur.
/// Vrai lorsque le ComponentBase est focusé par l'utilisateur. Un élément est focusé
/// lorsqu'il est, avec tous ses ancêtres, l'ActiveChild() de leurs parents, et qu'il est
/// Focusable().
bool ComponentBase::Focused() const {
  const auto* current = this;
  while (current && current->Active()) {
    current = current->impl_->parent;
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
  while (ComponentBase* parent = child->impl_->parent) {
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

void ComponentBase::Reserved1() {}
void ComponentBase::Reserved2() {}
void ComponentBase::Reserved3() {}
void ComponentBase::Reserved4() {}
void ComponentBase::Reserved5() {}
void ComponentBase::Reserved6() {}
void ComponentBase::Reserved7() {}
void ComponentBase::Reserved8() {}

}  // namespace ftxui
