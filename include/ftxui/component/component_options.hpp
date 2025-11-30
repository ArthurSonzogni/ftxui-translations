// Copyright 2021 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
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

/// @brief Argumentos para la transformación de |ButtonOption|, |CheckboxOption|,
/// |RadioboxOption|, |MenuEntryOption|, |MenuOption|.
struct EntryState {
  std::string label;  ///< La etiqueta a mostrar.
  bool state;         ///< El estado del botón/checkbox/radiobox
  bool active;        ///< Si la entrada es la activa.
  bool focused;       ///< Si la entrada está enfocada por el usuario.
  int index;          ///< Índice de la entrada cuando sea aplicable o -1.
};

/// @brief Opción para el efecto de subrayado.
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

/// @brief Opción sobre un color potencialmente animado.
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

/// @brief Opción para el componente MenuEntry.
/// @ingroup component
struct MenuEntryOption {
  ConstStringRef label = "MenuEntry";
  std::function<Element(const EntryState& state)> transform;
  AnimatedColorsOption animated_colors;
};

/// @brief Opción para el componente Menu.
/// @ingroup component
struct MenuOption {
  // Constructores estándar:
  static MenuOption Horizontal();
  static MenuOption HorizontalAnimated();
  static MenuOption Vertical();
  static MenuOption VerticalAnimated();
  static MenuOption Toggle();

  ConstStringListRef entries;  ///> La lista de entradas.
  Ref<int> selected = 0;       ///> El índice de la entrada seleccionada.

  // Estilo:
  UnderlineOption underline;
  MenuEntryOption entries_option;
  Direction direction = Direction::Down;
  std::function<Element()> elements_prefix;
  std::function<Element()> elements_infix;
  std::function<Element()> elements_postfix;

  // Observadores:
  std::function<void()> on_change;  ///> Llamado cuando la entrada seleccionada cambia.
  std::function<void()> on_enter;   ///> Llamado cuando el usuario presiona enter.
  Ref<int> focused_entry = 0;
};

/// @brief Opción para el componente AnimatedButton.
/// @ingroup component
struct ButtonOption {
  // Standard constructors:
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

/// @brief Opción para el componente Checkbox.
/// @ingroup component
struct CheckboxOption {
  // Constructores estándar:
  static CheckboxOption Simple();

  ConstStringRef label = "Checkbox";

  Ref<bool> checked = false;

  // Style:
  std::function<Element(const EntryState&)> transform;

  // Observer:
  /// Llamado cuando el usuario cambia el estado.
  std::function<void()> on_change = [] {};
};

/// @brief Usado para definir el estilo del componente Input.
struct InputState {
  Element element;
  bool hovered;         ///< Si el input está siendo "hovered" por el ratón.
  bool focused;         ///< Si el input está enfocado por el usuario.
  bool is_placeholder;  ///< Si el input está vacío y mostrando el
                        ///< marcador de posición.
};

/// @brief Opción para el componente Input.
/// @ingroup component
struct InputOption {
  // Un conjunto de estilos predefinidos:

  /// @brief Crea el estilo de entrada predeterminado:
  static InputOption Default();
  /// @brief Un estilo blanco sobre negro con márgenes altos:
  static InputOption Spacious();

  /// El contenido del input.
  StringRef content = "";

  /// El contenido del input cuando está vacío.
  StringRef placeholder = "";

  // Estilo:
  std::function<Element(InputState)> transform;
  Ref<bool> password = false;  ///< Oscurece el contenido del input usando '*'.
  Ref<bool> multiline = true;  ///< Si el input puede ser multilínea.
  Ref<bool> insert = true;     ///< Modo de inserción o sobrescritura de caracteres.

  /// Llamado cuando el contenido cambia.
  std::function<void()> on_change = [] {};
  /// Llamado cuando el usuario presiona enter.
  std::function<void()> on_enter = [] {};

  // La posición del caracter del cursor:
  Ref<int> cursor_position = 0;
};

/// @brief Opción para el componente Radiobox.
/// @ingroup component
struct RadioboxOption {
  // Constructores estándar:
  static RadioboxOption Simple();

  // Contenido:
  ConstStringListRef entries;
  Ref<int> selected = 0;

  // Estilo:
  std::function<Element(const EntryState&)> transform;

  // Observadores:
  /// Llamado cuando la entrada seleccionada cambia.
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

  // Constraints on main_size:
  Ref<int> min = 0;
  Ref<int> max = std::numeric_limits<int>::max();
};

// @brief Opción para el componente `Slider`.
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
  std::function<void()> on_change;  ///> Llamado cuando `value` es actualizado.
};

/// @brief Estado pasado a la función de renderizado del componente `Window`.
/// @ingroup component
struct WindowRenderState {
  Element inner;             ///< El elemento envuelto dentro de esta ventana.
  const std::string& title;  ///< El título de la ventana.
  bool active = false;       ///< Si la ventana es la activa.
  bool drag = false;         ///< Si la ventana está siendo arrastrada.
  bool resize = false;       ///< Si la ventana está siendo redimensionada.
  bool hover_left = false;   ///< Si el lado izquierdo redimensionable está siendo "hovered".
  bool hover_right = false;  ///< Si el lado derecho redimensionable está siendo "hovered".
  bool hover_top = false;    ///< Si el lado superior redimensionable está siendo "hovered".
  bool hover_down = false;   ///< Si el lado inferior redimensionable está siendo "hovered".
};

// @brief Opción para el componente `Window`.
// @ingroup component
struct WindowOptions {
  Component inner;            ///< El componente envuelto por esta ventana.
  ConstStringRef title = "";  ///< El título mostrado por esta ventana.

  Ref<int> left = 0;     ///< La posición del lado izquierdo de la ventana.
  Ref<int> top = 0;      ///< La posición del lado superior de la ventana.
  Ref<int> width = 20;   ///< El ancho de la ventana.
  Ref<int> height = 10;  ///< La altura de la ventana.

  Ref<bool> resize_left = true;   ///< ¿Se puede redimensionar el lado izquierdo?
  Ref<bool> resize_right = true;  ///< ¿Se puede redimensionar el lado derecho?
  Ref<bool> resize_top = true;    ///< ¿Se puede redimensionar el lado superior?
  Ref<bool> resize_down = true;   ///< ¿Se puede redimensionar el lado inferior?

  /// Una función opcional para personalizar la apariencia de la ventana:
  std::function<Element(const WindowRenderState&)> render;
};

/// @brief Opción para el componente Dropdown.
/// @ingroup component
/// Un menú desplegable es un checkbox que abre/cierra un radiobox.
struct DropdownOption {
  /// Si el desplegable está abierto o cerrado:
  Ref<bool> open = false;
  // Las opciones para el checkbox:
  CheckboxOption checkbox;
  // Las opciones para el radiobox:
  RadioboxOption radiobox;
  // La función de transformación:
  std::function<Element(bool open, Element checkbox, Element radiobox)>
      transform;
};

}  // namespace ftxui

#endif /* fin de la guarda de inclusión: FTXUI_COMPONENT_COMPONENT_OPTIONS_HPP */