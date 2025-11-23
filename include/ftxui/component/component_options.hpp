// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Utilisation de ce code source régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#ifndef FTXUI_COMPONENT_COMPONENT_OPTIONS_HPP
#define FTXUI_COMPONENT_COMPONENT_OPTIONS_HPP

#include <chrono>                         // for milliseconds
#include <ftxui/component/animation.hpp>  // for Duration, QuadraticInOut, Function
#include <ftxui/dom/direction.hpp>  // for Direction, Direction::Left, Direction::Right, Direction::Down
#include <ftxui/dom/elements.hpp>  // for Element, separator
#include <ftxui/util/ref.hpp>      // for Ref, ConstRef, StringRef
#include <ftxui/util/warn_windows_macro.hpp>
#include <functional>  // for function
#include <limits>      // for numeric_limits
#include <string>      // for string

#include "ftxui/component/component_base.hpp"  // for Component
#include "ftxui/screen/color.hpp"  // for Color, Color::GrayDark, Color::White

namespace ftxui {

/// @brief arguments pour la transformation depuis |ButtonOption|, |CheckboxOption|,
/// |RadioboxOption|, |MenuEntryOption|, |MenuOption|.
struct EntryState {
  std::string label;  ///< Le libellé à afficher.
  bool state;         ///< L'état du bouton/checkbox/radiobox
  bool active;        ///< Indique si l'entrée est active.
  bool focused;       ///< Indique si l'entrée est celle sur laquelle l'utilisateur a le focus.
  int index;          ///< Index de l'entrée lorsqu'applicable ou -1.
};

/// @brief Option pour l'effet de soulignement.
/// @ingroup component
struct UnderlineOption {
  bool enabled = false;

  Color color_active = Color::White;
  Color color_inactive = Color::GrayDark;

  animation::easing::Function leader_function =
      animation::easing::QuadraticInOut;
  animation::easing::Function follower_function =
      animation::easing::QuadraticInOut;

  animation::Duration leader_duration = std::chrono::milliseconds(250);
  animation::Duration leader_delay = std::chrono::milliseconds(0);
  animation::Duration follower_duration = std::chrono::milliseconds(250);
  animation::Duration follower_delay = std::chrono::milliseconds(0);

  void SetAnimation(animation::Duration d, animation::easing::Function f);
  void SetAnimationDuration(animation::Duration d);
  void SetAnimationFunction(animation::easing::Function f);
  void SetAnimationFunction(animation::easing::Function f_leader,
                            animation::easing::Function f_follower);
};

/// @brief Option concernant une couleur potentiellement animée.
/// @ingroup component
struct AnimatedColorOption {
  void Set(
      Color inactive,
      Color active,
      animation::Duration duration = std::chrono::milliseconds(250),
      animation::easing::Function function = animation::easing::QuadraticInOut);

  bool enabled = false;
  Color inactive;
  Color active;
  animation::Duration duration = std::chrono::milliseconds(250);
  animation::easing::Function function = animation::easing::QuadraticInOut;
};

struct AnimatedColorsOption {
  AnimatedColorOption background;
  AnimatedColorOption foreground;
};

/// @brief Option pour le composant MenuEntry.
/// @ingroup component
struct MenuEntryOption {
  ConstStringRef label = "MenuEntry";
  std::function<Element(const EntryState& state)> transform;
  AnimatedColorsOption animated_colors;
};

/// @brief Option pour le composant Menu.
/// @ingroup component
struct MenuOption {
  // Constructeurs standards:
  static MenuOption Horizontal();
  static MenuOption HorizontalAnimated();
  static MenuOption Vertical();
  static MenuOption VerticalAnimated();
  static MenuOption Toggle();

  ConstStringListRef entries;  ///> La liste des entrées.
  Ref<int> selected = 0;       ///> L'index de l'entrée sélectionnée.

  // Style:
  UnderlineOption underline;
  MenuEntryOption entries_option;
  Direction direction = Direction::Down;
  std::function<Element()> elements_prefix;
  std::function<Element()> elements_infix;
  std::function<Element()> elements_postfix;

  // Observateurs:
  std::function<void()> on_change;  ///> Appelé lorsque l'entrée sélectionnée change.
  std::function<void()> on_enter;   ///> Appelé lorsque l'utilisateur appuie sur Entrée.
  Ref<int> focused_entry = 0;
};

/// @brief Option pour le composant AnimatedButton.
/// @ingroup component
struct ButtonOption {
  // Constructeurs standards:
  static ButtonOption Ascii();
  static ButtonOption Simple();
  static ButtonOption Border();
  static ButtonOption Animated();
  static ButtonOption Animated(Color color);
  static ButtonOption Animated(Color background, Color foreground);
  static ButtonOption Animated(Color background,
                               Color foreground,
                               Color background_active,
                               Color foreground_active);

  ConstStringRef label = "Button";
  std::function<void()> on_click = [] {};

  // Style:
  std::function<Element(const EntryState&)> transform;
  AnimatedColorsOption animated_colors;
};

/// @brief Option pour le composant Checkbox.
/// @ingroup component
struct CheckboxOption {
  // Constructeurs standards:
  static CheckboxOption Simple();

  ConstStringRef label = "Checkbox";

  Ref<bool> checked = false;

  // Style:
  std::function<Element(const EntryState&)> transform;

  // Observateur:
  /// Appelé lorsque l'utilisateur change l'état.
  std::function<void()> on_change = [] {};
};

/// @brief Utilisé pour définir le style du composant Input.
struct InputState {
  Element element;
  bool hovered;         ///< Indique si l'entrée est survolée par la souris.
  bool focused;         ///< Indique si l'entrée a le focus de l'utilisateur.
  bool is_placeholder;  ///< Indique si l'entrée est vide et affiche le
                        ///< texte de substitution (placeholder).
};

/// @brief Option pour le composant Input.
/// @ingroup component
struct InputOption {
  // Un ensemble de styles prédéfinis:

  /// @brief Crée le style d'entrée par défaut:
  static InputOption Default();
  /// @brief Un style blanc sur noir avec des marges élevées:
  static InputOption Spacious();

  /// Le contenu de l'entrée.
  StringRef content = "";

  /// Le contenu de l'entrée lorsqu'elle est vide.
  StringRef placeholder = "";

  // Style:
  std::function<Element(InputState)> transform;
  Ref<bool> password = false;  ///< Obscurcit le contenu de l'entrée en utilisant '*'.
  Ref<bool> multiline = true;  ///< Indique si l'entrée peut être multiligne.
  Ref<bool> insert = true;     ///< Mode d'insertion ou de remplacement des caractères.

  /// Appelé lorsque le contenu change.
  std::function<void()> on_change = [] {};
  /// Appelé lorsque l'utilisateur appuie sur Entrée.
  std::function<void()> on_enter = [] {};

  // La position du curseur:
  Ref<int> cursor_position = 0;
};

/// @brief Option pour le composant Radiobox.
/// @ingroup component
struct RadioboxOption {
  // Constructeurs standards:
  static RadioboxOption Simple();

  // Contenu:
  ConstStringListRef entries;
  Ref<int> selected = 0;

  // Style:
  std::function<Element(const EntryState&)> transform;

  // Observateurs:
  /// Appelé lorsque l'entrée sélectionnée change.
  std::function<void()> on_change = [] {};
  Ref<int> focused_entry = 0;
};

struct ResizableSplitOption {
  Component main;
  Component back;
  Ref<Direction> direction = Direction::Left;
  Ref<int> main_size =
      (direction() == Direction::Left || direction() == Direction::Right) ? 20
                                                                          : 10;
  std::function<Element()> separator_func = [] { return ::ftxui::separator(); };

  // Contraintes sur main_size:
  Ref<int> min = 0;
  Ref<int> max = std::numeric_limits<int>::max();
};

// @brief Option pour le composant `Slider`.
// @ingroup component
template <typename T>
struct SliderOption {
  Ref<T> value;
  ConstRef<T> min = T(0);
  ConstRef<T> max = T(100);
  ConstRef<T> increment = (max() - min()) / 20;
  Direction direction = Direction::Right;
  Color color_active = Color::White;
  Color color_inactive = Color::GrayDark;
  std::function<void()> on_change;  ///> Appelé lorsque `value` est mis à jour.
};

/// @brief État passé à la fonction de rendu du composant `Window`.
/// @ingroup component
struct WindowRenderState {
  Element inner;             ///< L'élément enveloppé à l'intérieur de cette fenêtre.
  const std::string& title;  ///< Le titre de la fenêtre.
  bool active = false;       ///< Indique si la fenêtre est active.
  bool drag = false;         ///< Indique si la fenêtre est en cours de déplacement.
  bool resize = false;       ///< Indique si la fenêtre est en cours de redimensionnement.
  bool hover_left = false;   ///< Indique si le côté gauche redimensionnable est survolé.
  bool hover_right = false;  ///< Indique si le côté droit redimensionnable est survolé.
  bool hover_top = false;    ///< Indique si le côté supérieur redimensionnable est survolé.
  bool hover_down = false;   ///< Indique si le côté inférieur redimensionnable est survolé.
};

// @brief Option pour le composant `Window`.
// @ingroup component
struct WindowOptions {
  Component inner;            ///< Le composant enveloppé par cette fenêtre.
  ConstStringRef title = "";  ///< Le titre affiché par cette fenêtre.

  Ref<int> left = 0;     ///< La position du côté gauche de la fenêtre.
  Ref<int> top = 0;      ///< La position du côté supérieur de la fenêtre.
  Ref<int> width = 20;   ///< La largeur de la fenêtre.
  Ref<int> height = 10;  ///< La hauteur de la fenêtre.

  Ref<bool> resize_left = true;   ///< Le côté gauche peut-il être redimensionné ?
  Ref<bool> resize_right = true;  ///< Le côté droit peut-il être redimensionné ?
  Ref<bool> resize_top = true;    ///< Le côté supérieur peut-il être redimensionné ?
  Ref<bool> resize_down = true;   ///< Le côté inférieur peut-il être redimensionné ?

  /// Une fonction optionnelle pour personnaliser l'apparence de la fenêtre :
  std::function<Element(const WindowRenderState&)> render;
};

/// @brief Option pour le composant Dropdown.
/// @ingroup component
/// Un menu déroulant est une case à cocher qui ouvre/ferme une radiobox.
struct DropdownOption {
  /// Indique si le menu déroulant est ouvert ou fermé :
  Ref<bool> open = false;
  // Les options pour la case à cocher :
  CheckboxOption checkbox;
  // Les options pour la radiobox :
  RadioboxOption radiobox;
  // La fonction de transformation :
  std::function<Element(bool open, Element checkbox, Element radiobox)>
      transform;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_COMPONENT_OPTIONS_HPP */