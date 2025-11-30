// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
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

/// @brief Implementa el renderizado de sí mismo como ftxui::Element. Implementa
/// la navegación por teclado respondiendo a ftxui::Event.
/// @ingroup component
class ComponentBase {
 public:
  explicit ComponentBase(Components children)
      : children_(std::move(children)) {}
  virtual ~ComponentBase();
  ComponentBase() = default;

  // Un componente no es copiable/movible.
  ComponentBase(const ComponentBase&) = delete;
  ComponentBase(ComponentBase&&) = delete;
  ComponentBase& operator=(const ComponentBase&) = delete;
  ComponentBase& operator=(ComponentBase&&) = delete;

  // Jerarquía de componentes:
  ComponentBase* Parent() const;
  Component& ChildAt(size_t i);
  size_t ChildCount() const;
  int Index() const;
  void Add(Component children);
  void Detach();
  void DetachAllChildren();

  // Renderiza el componente.
  Element Render();

  // Sobreescribe esta función para modificar cómo funciona `Render`.
  virtual Element OnRender();

  // Maneja un evento.
  // Por defecto, se reduce a los hijos con un OR perezoso.
  //
  // Devuelve si el evento fue manejado o no.
  virtual bool OnEvent(Event);

  // Maneja un paso de animación.
  virtual void OnAnimation(animation::Params& params);

  // Gestión del foco ----------------------------------------------------------
  //
  // Si este componente contiene hijos, esto indica cuál está activo,
  // nullptr si ninguno está activo.
  //
  // Decimos que un elemento tiene el foco si la cadena de ActiveChild() desde el
  // componente raíz contiene este objeto.
  virtual Component ActiveChild();

  // Devuelve true cuando el componente contiene elementos enfocables.
  // El componente no enfocable se omitirá al navegar usando el
  // teclado.
  virtual bool Focusable() const;

  // Si este es el hijo activo de su padre.
  bool Active() const;
  // Si todos los ancestros están activos.
  bool Focused() const;

  // Hace que el |hijo| sea el "activo".
  virtual void SetActiveChild(ComponentBase* child);
  void SetActiveChild(Component child);

  // Configura todos los ancestros para dar foco a este componente.
  void TakeFocus();

 protected:
  CapturedMouse CaptureMouse(const Event& event);

  Components children_;

 private:
  ComponentBase* parent_ = nullptr;
  bool in_render = false;
};

}  // namespace ftxui

#endif /* fin de la guarda de inclusión: FTXUI_COMPONENT_BASE_HPP */
