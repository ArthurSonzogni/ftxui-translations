// Copyright 2020 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <ftxui/screen/box.hpp>  // for Box
#include <string>
#include <utility>  // for move

#include <cstddef>
#include "ftxui/dom/node.hpp"
#include "ftxui/dom/selection.hpp"  // for Selection
#include "ftxui/screen/screen.hpp"  // for Screen

namespace ftxui {

Node::Node() = default;
Node::Node(Elements children) : children_(std::move(children)) {}
Node::~Node() = default;

/// @brief Calcula cuánto espacio necesita un elemento.
void Node::ComputeRequirement() {
  if (children_.empty()) {
    return;
  }
  for (auto& child : children_) {
    child->ComputeRequirement();
  }

  // Por defecto, el requisito es el del primer hijo.
  requirement_ = children_[0]->requirement();

  // Propagar el requisito de enfoque.
  for (size_t i = 1; i < children_.size(); ++i) {
    if (!requirement_.focused.enabled &&
        children_[i]->requirement().focused.enabled) {
      requirement_.focused = children_[i]->requirement().focused;
    }
  }
}

/// @brief Asigna una posición y una dimensión a un elemento para dibujarlo.
void Node::SetBox(Box box) {
  box_ = box;
}

/// @brief Calcula la selección de un elemento.
void Node::Select(Selection& selection) {
  // Si el cuadro de este Nodo no se interseca con la selección, entonces no hay selección.
  if (Box::Intersection(selection.GetBox(), box_).IsEmpty()) {
    return;
  }

  // Por defecto, diferimos la selección a los hijos.
  for (auto& child : children_) {
    child->Select(selection);
  }
}

/// @brief Muestra un elemento en un ftxui::Screen.
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

/// @brief Muestra un elemento en un ftxui::Screen.
/// @ingroup dom
void Render(Screen& screen, const Element& element) {
  Selection selection;
  Render(screen, element.get(), selection);
}

/// @brief Muestra un elemento en un ftxui::Screen.
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
    // Paso 1: Encontrar qué dimensión quiere tener este elemento.
    node->ComputeRequirement();

    // Paso 2: Asignar una dimensión al elemento.
    node->SetBox(box);

    // Comprobar si el elemento necesita otra iteración del algoritmo de diseño.
    status.need_iteration = false;
    status.iteration++;
    node->Check(&status);
  }

  // Paso 3: Selección
  if (!selection.IsEmpty()) {
    node->Select(selection);
  }

  if (node->requirement().focused.enabled
#if defined(FTXUI_MICROSOFT_TERMINAL_FALLBACK)
      // Al colocar el cursor en la posición correcta, las personas que usan caracteres CJK (chino,
      // japonés, coreano, ...) pueden ver su [editor de métodos de entrada]
      // mostrado en la ubicación correcta. Ver [problema].
      //
      // [editor de métodos de entrada]:
      // https://en.wikipedia.org/wiki/Input_method
      //
      // [problema]:
      // https://github.com/ArthurSonzogni/FTXUI/issues/2#issuecomment-505282355
      //
      // Desafortunadamente, la terminal de Microsoft no maneja correctamente la ocultación del
      // cursor. En su lugar, el carácter debajo del cursor se oculta, lo cual es un
      // gran problema. Como resultado, no podemos habilitar la configuración del cursor en la
      // ubicación correcta. Se mostrará en la esquina inferior derecha.
      // Ver:
      // https://github.com/microsoft/terminal/issues/1203
      // https://github.com/microsoft/terminal/issues/3093
      &&
      node->requirement().focused.cursor_shape != Screen::Cursor::Shape::Hidden
#endif
  ) {
    screen.SetCursor(Screen::Cursor{
        node->requirement().focused.node->box_.x_max,
        node->requirement().focused.node->box_.y_max,
        node->requirement().focused.cursor_shape,
    });
  } else {
    screen.SetCursor(Screen::Cursor{
        screen.dimx() - 1,
        screen.dimy() - 1,
        Screen::Cursor::Shape::Hidden,
    });
  }

  // Paso 4: Dibujar el elemento.
  screen.stencil = box;
  node->Render(screen);

  // Paso 5: Aplicar sombreadores
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
    // Paso 1: Encontrar qué dimensión quiere tener este elemento.
    node->ComputeRequirement();

    // Paso 2: Asignar una dimensión al elemento.
    node->SetBox(box);

    // Comprobar si el elemento necesita otra iteración del algoritmo de diseño.
    status.need_iteration = false;
    status.iteration++;
    node->Check(&status);
  }

  // Paso 3: Selección
  node->Select(selection);

  // Paso 4: obtener el contenido seleccionado.
  return node->GetSelectedContent(selection);
}

}  // namespace ftxui