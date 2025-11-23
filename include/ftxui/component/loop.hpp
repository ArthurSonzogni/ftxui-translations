// Copyright 2022 Arthur Sonzogni. All rights reserved.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée
// dans le fichier LICENSE.
#ifndef FTXUI_COMPONENT_LOOP_HPP
#define FTXUI_COMPONENT_LOOP_HPP

#include <memory>  // for shared_ptr

#include "ftxui/component/component_base.hpp"  // for ComponentBase

namespace ftxui {
class ComponentBase;

using Component = std::shared_ptr<ComponentBase>;
class ScreenInteractive;

/// @brief Loop est une classe qui gère la boucle d'événements pour un composant.
///
/// Elle est responsable de l'exécution du composant, de la gestion des événements et
/// de la mise à jour de l'écran.
///
/// La classe Loop est conçue pour être utilisée avec un objet ScreenInteractive,
/// qui représente l'écran du terminal.
///
/// **Exemple**
/// ```cpp
/// #include <ftxui/component/component.hpp>
/// #include <ftxui/component/screen_interactive.hpp>
/// #include <ftxui/component/loop.hpp>
///
/// int main() {
///  auto screen = ftxui::ScreenInteractive::TerminalOutput();
///  auto component = ftxui::Button("Click me", [] { ... });
///
///  ftxui::Loop loop(screen.get(), component);

///  // Soit
///  loop.Run();  // Bloquant jusqu'à ce que le composant quitte.
///
///  // Ou
///  loop.RunOnce();  // Non-bloquant, retourne immédiatement.
///
///  // Ou
///  loop.RunOnceBlocking();  // Bloquant jusqu'à ce qu'un événement soit géré.
///
///  // Ou dans une boucle:
///  while (!loop.HasQuitted()) {
///    loop.RunOnce();
///
///    // Faites autre chose comme exécuter une autre fonction de boucle de bibliothèque.
///  }
/// }
/// ```
///
/// @ingroup component
class Loop {
 public:
  Loop(ScreenInteractive* screen, Component component);
  ~Loop();

  bool HasQuitted();
  void RunOnce();
  void RunOnceBlocking();
  void Run();

  // Cette classe n'est pas copiable/déplaçable.
  Loop(const Loop&) = default;
  Loop(Loop&&) = delete;
  Loop& operator=(Loop&&) = delete;
  Loop(const ScreenInteractive&) = delete;
  Loop& operator=(const Loop&) = delete;

 private:
  ScreenInteractive* screen_;
  Component component_;
};

}  // namespace ftxui

#endif  // FTXUI_COMPONENT_LOOP_HPP