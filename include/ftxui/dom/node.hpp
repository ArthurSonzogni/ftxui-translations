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

/// @brief Node es la clase base para todos los elementos en el árbol DOM.
///
/// Representa un único nodo en el modelo de objeto de documento (DOM) y proporciona
/// la estructura básica para el diseño y la representación.
/// Contiene métodos para calcular los requisitos de diseño, establecer las
/// dimensiones del cuadro, seleccionar contenido, renderizar en la pantalla y
/// verificar el estado del diseño.
/// Típicamente contiene elementos hijos, que también son instancias de Node.
///
/// Se espera que los usuarios deriven de esta clase para crear elementos personalizados.
///
/// Una lista de elementos incorporados se puede encontrar en el archivo `elements.hpp`.
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

  // Paso 1: Calcular los requisitos de diseño. Indicar al padre qué dimensiones
  //         desea tener este elemento.
  //         Propagado de Hijos a Padres.
  virtual void ComputeRequirement();
  Requirement requirement() { return requirement_; }

  // Paso 2: Asignar a este elemento sus dimensiones finales.
  //         Propagado de Padres a Hijos.
  virtual void SetBox(Box box);

  // Paso 3: (opcional) Selección
  //         Propagado de Padres a Hijos.
  virtual void Select(Selection& selection);

  // Paso 4: Dibujar este elemento.
  virtual void Render(Screen& screen);

  virtual std::string GetSelectedContent(Selection& selection);

  // El diseño puede no resolverse en una sola iteración para algunos elementos. Esto
  // les permite solicitar iteraciones adicionales. Esta señal debe
  // reenviarse a los hijos al menos una vez.
  struct Status {
    int iteration = 0;
    bool need_iteration = false;
  };
  virtual void Check(Status* status);

  // Reserva ABI:
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
