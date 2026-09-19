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

/// @brief Devuelve el ComponentBase padre, o nulo si no tiene.
/// @see Detach
/// @see Parent
ComponentBase* ComponentBase::Parent() const {
  return impl_->parent;
}

/// @brief Accede al hijo en el índice `i`.
Component& ComponentBase::ChildAt(size_t i) {
  assert(i < ChildCount());  // NOLINT
  return impl_->children[i];
}

/// @brief Devuelve el número de hijos.
size_t ComponentBase::ChildCount() const {
  return impl_->children.size();
}

/// @brief Devuelve el índice del componente en su padre. -1 si no tiene padre.
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
  return -1;  // No alcanzado.
}

/// @brief Agrega un hijo.
/// @param child El hijo a adjuntar.
void ComponentBase::Add(Component child) {
  child->Detach();
  child->impl_->parent = this;
  impl_->children.push_back(std::move(child));
}

/// @brief Desvincula este hijo de su padre.
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
  parent->impl_->children.erase(it);  // Podría eliminar |this|.
}

/// @brief Elimina todos los hijos.
void ComponentBase::DetachAllChildren() {
  while (!impl_->children.empty()) {
    impl_->children[0]->Detach();
  }
}

/// @brief Dibuja el componente.
/// Construye un ftxui::Element para ser dibujado en la ftxui::Screen representando este
/// ftxui::ComponentBase. Por favor, anule OnRender() para modificar el renderizado.
Element ComponentBase::Render() {
  // Algunos usuarios podrían llamar a `ComponentBase::Render()` desde
  // `T::OnRender()`. Para evitar la recursión infinita, usamos un indicador.
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

/// @brief Dibuja el componente.
/// Construye un ftxui::Element para ser dibujado en la ftxui::Screen representando este
/// ftxui::ComponentBase. Esta función está destinada a ser sobrescrita.
Element ComponentBase::OnRender() {
  if (impl_->children.size() == 1) {
    return impl_->children.front()->Render();
  }

  return text("Not implemented component");
}

/// @brief Llamado en respuesta a un evento.
/// @param event El evento.
/// @return Verdadero cuando el evento ha sido manejado.
/// La implementación predeterminada llama a OnEvent en cada hijo hasta que uno devuelve
/// verdadero. Si ninguno devuelve verdadero, devuelve falso.
bool ComponentBase::OnEvent(Event event) {    // NOLINT
  for (Component& child : impl_->children) {  // NOLINT
    if (child->OnEvent(event)) {
      return true;
    }
  }
  return false;
}

/// @brief Llamado en respuesta a un evento de animación.
/// @param params los parámetros de la animación
/// La implementación predeterminada envía el evento a cada hijo.
void ComponentBase::OnAnimation(animation::Params& params) {
  for (const Component& child : impl_->children) {
    child->OnAnimation(params);
  }
}

/// @brief Devuelve el hijo actualmente activo.
/// @return el hijo actualmente activo.
Component ComponentBase::ActiveChild() {
  for (auto& child : impl_->children) {
    if (child->Focusable()) {
      return child;
    }
  }
  return nullptr;
}

/// @brief Devuelve verdadero cuando el componente contiene elementos enfocables.
/// Los componentes no enfocables se omitirán al navegar con el
/// teclado.
bool ComponentBase::Focusable() const {
  for (const Component& child : impl_->children) {  // NOLINT
    if (child->Focusable()) {
      return true;
    }
  }
  return false;
}

/// @brief Devuelve si el elemento es el hijo actualmente activo de su padre.
bool ComponentBase::Active() const {
  return impl_->parent == nullptr || impl_->parent->ActiveChild().get() == this;
}

/// @brief Devuelve si el elemento está enfocado por el usuario.
/// Verdadero cuando el ComponentBase está enfocado por el usuario. Un elemento está enfocado
/// cuando es, junto con todos sus ancestros, el ActiveChild() de sus padres, y es
/// Focusable().
bool ComponentBase::Focused() const {
  const auto* current = this;
  while (current && current->Active()) {
    current = current->impl_->parent;
  }
  return !current && Focusable();
}

/// @brief Hace que |child| sea el activo.
/// @param child el hijo que se activará.
void ComponentBase::SetActiveChild([[maybe_unused]] ComponentBase* child) {}

/// @brief Hace que |child| sea el activo.
/// @param child el hijo que se activará.
void ComponentBase::SetActiveChild(Component child) {  // NOLINT
  SetActiveChild(child.get());
}

/// @brief Configura todos los ancestros para dar foco a este componente.
void ComponentBase::TakeFocus() {
  ComponentBase* child = this;
  while (ComponentBase* parent = child->impl_->parent) {
    parent->SetActiveChild(child);
    child = parent;
  }
}

/// @brief Toma el CapturedMouse si está disponible. Solo hay un componente de
/// ellos. Representa un componente que toma prioridad sobre otros.
/// @param event El evento
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
