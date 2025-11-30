// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <algorithm>  // para find_if
#include <cassert>    // para assert
#include <cstddef>    // para size_t
#include <iterator>   // para begin, end
#include <memory>     // para unique_ptr, make_unique
#include <utility>    // para move
#include <vector>     // para vector, __alloc_traits<>::value_type

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

/// @brief Devuelve el ComponentBase padre, o nulo si no tiene.
/// @see Detach
/// @see Parent
ComponentBase* ComponentBase::Parent() const {
  return parent_;
}

/// @brief Accede al hijo en el índice `i`.
Component& ComponentBase::ChildAt(size_t i) {
  assert(i < ChildCount());  // NOLINT
  return children_[i];
}

/// @brief Devuelve el número de hijos.
size_t ComponentBase::ChildCount() const {
  return children_.size();
}

/// @brief Devuelve el índice del componente en su padre. -1 si no tiene padre.
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
  return -1;  // No alcanzado.
}

/// @brief Agrega un hijo.
/// @@param child El hijo a adjuntar.
void ComponentBase::Add(Component child) {
  child->Detach();
  child->parent_ = this;
  children_.push_back(std::move(child));
}

/// @brief Desvincula este hijo de su padre.
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
  parent->children_.erase(it);  // Podría eliminar |this|.
}

/// @brief Elimina todos los hijos.
void ComponentBase::DetachAllChildren() {
  while (!children_.empty()) {
    children_[0]->Detach();
  }
}

/// @brief Dibuja el componente.
/// Construye un ftxui::Element para ser dibujado en la ftxui::Screen representando este
/// ftxui::ComponentBase. Por favor, anule OnRender() para modificar el renderizado.
Element ComponentBase::Render() {
  // Algunos usuarios podrían llamar a `ComponentBase::Render()` desde
  // `T::OnRender()`. Para evitar la recursión infinita, usamos un indicador.
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

/// @brief Dibuja el componente.
/// Construye un ftxui::Element para ser dibujado en la ftxui::Screen representando este
/// ftxui::ComponentBase. Esta función está destinada a ser sobrescrita.
Element ComponentBase::OnRender() {
  if (children_.size() == 1) {
    return children_.front()->Render();
  }

  return text("Componente no implementado");
}

/// @brief Llamado en respuesta a un evento.
/// @param event El evento.
/// @return Verdadero cuando el evento ha sido manejado.
/// La implementación predeterminada llama a OnEvent en cada hijo hasta que uno devuelve
/// verdadero. Si ninguno devuelve verdadero, devuelve falso.
bool ComponentBase::OnEvent(Event event) {  // NOLINT
  for (Component& child : children_) {      // NOLINT
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
  for (const Component& child : children_) {
    child->OnAnimation(params);
  }
}

/// @brief Devuelve el hijo actualmente activo.
/// @return el hijo actualmente activo.
Component ComponentBase::ActiveChild() {
  for (auto& child : children_) {
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
  for (const Component& child : children_) {  // NOLINT
    if (child->Focusable()) {
      return true;
    }
  }
  return false;
}

/// @brief Devuelve si el elemento es el hijo actualmente activo de su padre.
bool ComponentBase::Active() const {
  return parent_ == nullptr || parent_->ActiveChild().get() == this;
}

/// @brief Devuelve si el elemento está enfocado por el usuario.
/// Verdadero cuando el ComponentBase está enfocado por el usuario. Un elemento está enfocado
/// cuando es, junto con todos sus ancestros, el ActiveChild() de sus padres, y es
/// Focusable().
bool ComponentBase::Focused() const {
  const auto* current = this;
  while (current && current->Active()) {
    current = current->parent_;
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
  while (ComponentBase* parent = child->parent_) {
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

}  // namespace ftxui
