// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_COMPONENT_LOOP_HPP
#define FTXUI_COMPONENT_LOOP_HPP

#include <memory>  // for shared_ptr

#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/util/export.hpp"               // for FTXUI_EXPORT

namespace ftxui {
class ComponentBase;

using Component = std::shared_ptr<ComponentBase>;
class App;

/// @brief Loop est une classe qui gère la boucle d'événements d'un composant.
///
/// Elle est responsable de l'exécution du composant, de la gestion des événements, et
/// de la mise à jour de l'écran.
///
/// La classe Loop est conçue pour être utilisée avec un objet App,
/// qui représente l'écran du terminal.
///
/// **Exemple**
/// ```cpp
/// #include <ftxui/component/component.hpp>
/// #include <ftxui/component/app.hpp>
/// #include <ftxui/component/loop.hpp>
///
/// int main() {
///  auto screen = ftxui::App::TerminalOutput();
///  auto component = ftxui::Button("Click me", [] { ... });
///
///  ftxui::Loop loop(screen.get(), component);
///
///  // Soit
///  loop.Run();  // Bloquant jusqu'à ce que le composant se termine.
///
///  // Soit
///  loop.RunOnce();  // Non bloquant, retourne immédiatement.
///
///  // Soit
///  loop.RunOnceBlocking();  // Bloquant jusqu'au traitement d'un événement.
///
///  // Soit dans une boucle :
///  while (!loop.HasQuitted()) {
///    loop.RunOnce();
///
///    // Faire autre chose comme exécuter une autre fonction de boucle de bibliothèque.
///  }
/// }
/// ```
///
/// @ingroup component
class FTXUI_EXPORT(COMPONENT) Loop {
 public:
  Loop(App* screen, Component component);
  ~Loop();

  bool HasQuitted();
  void RunOnce();
  void RunOnceBlocking();
  void Run();

  // Cette classe n'est pas copiable/déplaçable.
  Loop& operator=(Loop&&) = delete;
  Loop& operator=(const Loop&) = delete;
  Loop(Loop&&) = delete;
  Loop(const Loop&) = delete;

 private:
  App* screen_;
  Component component_;
};

}  // namespace ftxui

#endif  // FTXUI_COMPONENT_LOOP_HPP
