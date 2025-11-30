// Copyright 2022 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar
// en el archivo LICENSE.
#include "ftxui/component/component_options.hpp"

#include <ftxui/screen/color.hpp>  // for Color, Color::White, Color::Black, Color::GrayDark, Color::Blue, Color::GrayLight, Color::Red
#include <memory>                  // for shared_ptr
#include <utility>                 // for move
#include "ftxui/component/animation.hpp"  // for Function, Duration
#include "ftxui/dom/direction.hpp"
#include "ftxui/dom/elements.hpp"  // for operator|=, Element, text, bgcolor, inverted, bold, dim, operator|, color, borderEmpty, hbox, automerge, border, borderLight

namespace ftxui {

/// @brief Una opción de color que puede ser animada.
/// @param _inactive El color cuando el componente está inactivo.
/// @param _active El color cuando el componente está activo.
/// @param _duration La duración de la animación.
/// @param _function La función de aceleración de la animación.
void AnimatedColorOption::Set(Color _inactive,
                              Color _active,
                              animation::Duration _duration,
                              animation::easing::Function _function) {
  enabled = true;
  inactive = _inactive;
  active = _active;
  duration = _duration;
  function = std::move(_function);
}

/// @brief Establece cómo debe animarse el subrayado.
/// @param d La duración de la animación.
/// @param f La función de aceleración de la animación.
void UnderlineOption::SetAnimation(animation::Duration d,
                                   animation::easing::Function f) {
  SetAnimationDuration(d);
  SetAnimationFunction(std::move(f));
}

/// @brief Establece cómo debe animarse el subrayado.
/// @param d La duración de la animación.
void UnderlineOption::SetAnimationDuration(animation::Duration d) {
  leader_duration = d;
  follower_duration = d;
}

/// @brief Establece cómo debe animarse el subrayado.
/// @param f La función de aceleración de la animación.
void UnderlineOption::SetAnimationFunction(animation::easing::Function f) {
  leader_function = f;
  follower_function = std::move(f);
}

/// @brief Establece cómo debe animarse el subrayado.
/// Esto es útil para desincronizar la animación del líder y el seguidor.
/// @param f_leader La duración de la animación para el líder.
/// @param f_follower La duración de la animación para el seguidor.
void UnderlineOption::SetAnimationFunction(
    animation::easing::Function f_leader,
    animation::easing::Function f_follower) {
  leader_function = std::move(f_leader);
  follower_function = std::move(f_follower);
}

/// @brief Opciones estándar para un menú horizontal.
/// Esto puede ser útil para implementar una barra de pestañas.
// static
MenuOption MenuOption::Horizontal() {
  MenuOption option;
  option.direction = Direction::Right;
  option.entries_option.transform = [](const EntryState& state) {
    Element e = text(state.label);
    if (state.focused) {
      e |= inverted;
    }
    if (state.active) {
      e |= bold;
    }
    if (!state.focused && !state.active) {
      e |= dim;
    }
    return e;
  };
  option.elements_infix = [] { return text(" "); };

  return option;
}

/// @brief Opciones estándar para un menú horizontal animado.
/// Esto puede ser útil para implementar una barra de pestañas.
// static
MenuOption MenuOption::HorizontalAnimated() {
  auto option = Horizontal();
  option.underline.enabled = true;
  return option;
}

/// @brief Opciones estándar para un menú vertical.
/// Esto puede ser útil para implementar una lista de elementos seleccionables.
// static
MenuOption MenuOption::Vertical() {
  MenuOption option;
  option.entries_option.transform = [](const EntryState& state) {
    Element e = text((state.active ? "> " : "  ") + state.label);  // NOLINT
    if (state.focused) {
      e |= inverted;
    }
    if (state.active) {
      e |= bold;
    }
    if (!state.focused && !state.active) {
      e |= dim;
    }
    return e;
  };
  return option;
}

/// @brief Opciones estándar para un menú vertical animado.
/// Esto puede ser útil para implementar una lista de elementos seleccionables.
// static
MenuOption MenuOption::VerticalAnimated() {
  auto option = MenuOption::Vertical();
  option.entries_option.transform = [](const EntryState& state) {
    Element e = text(state.label);
    if (state.focused) {
      e |= inverted;
    }
    if (state.active) {
      e |= bold;
    }
    if (!state.focused && !state.active) {
      e |= dim;
    }
    return e;
  };
  option.underline.enabled = true;
  return option;
}

/// @brief Opciones estándar para un menú horizontal con un separador.
/// Esto puede ser útil para implementar una barra de pestañas.
// static
MenuOption MenuOption::Toggle() {
  auto option = MenuOption::Horizontal();
  option.elements_infix = [] { return text("│") | automerge; };
  return option;
}

/// @brief Crea una ButtonOption, resaltada usando los caracteres [].
// static
ButtonOption ButtonOption::Ascii() {
  ButtonOption option;
  option.transform = [](const EntryState& s) {
    const std::string t = s.focused ? "[" + s.label + "]"  //
                                    : " " + s.label + " ";
    return text(t);
  };
  return option;
}

/// @brief Crea una ButtonOption, invertida cuando está enfocada.
// static
ButtonOption ButtonOption::Simple() {
  ButtonOption option;
  option.transform = [](const EntryState& s) {
    auto element = text(s.label) | borderLight;
    if (s.focused) {
      element |= inverted;
    }
    return element;
  };
  return option;
}

/// @brief Crea una ButtonOption. El botón se muestra usando un borde, invertido
/// cuando está enfocado. Este es el valor predeterminado actual.
ButtonOption ButtonOption::Border() {
  ButtonOption option;
  option.transform = [](const EntryState& s) {
    auto element = text(s.label) | border;
    if (s.active) {
      element |= bold;
    }
    if (s.focused) {
      element |= inverted;
    }
    return element;
  };
  return option;
}

/// @brief Crea una ButtonOption, usando colores animados.
  return Animated(Color::Black, Color::GrayLight,  //
                  Color::GrayDark, Color::White);
}

/// @brief Crea una ButtonOption, usando colores animados.
// static
ButtonOption ButtonOption::Animated(Color color) {
  return ButtonOption::Animated(
      Color::Interpolate(0.85F, color, Color::Black),   // NOLINT
      Color::Interpolate(0.10F, color, Color::White),   // NOLINT
      Color::Interpolate(0.10F, color, Color::Black),   // NOLINT
      Color::Interpolate(0.85F, color, Color::White));  // NOLINT
}

/// @brief Crea una ButtonOption, usando colores animados.
// static
ButtonOption ButtonOption::Animated(Color background, Color foreground) {
  // NOLINTBEGIN
  return ButtonOption::Animated(
      /*bakground=*/background,
      /*foreground=*/foreground,
      /*background_active=*/foreground,
      /*foreground_active=*/background);
  // NOLINTEND
}

/// @brief Crea una ButtonOption, usando colores animados.
// static
ButtonOption ButtonOption::Animated(Color background,
                                    Color foreground,
                                    Color background_active,
                                    Color foreground_active) {
  ButtonOption option;
  option.transform = [](const EntryState& s) {
    auto element = text(s.label) | borderEmpty;
    if (s.focused) {
      element |= bold;
    }
    return element;
  };
  option.animated_colors.foreground.Set(foreground, foreground_active);
  option.animated_colors.background.Set(background, background_active);
  return option;
}

/// @brief Opción para Checkbox estándar.
// static
CheckboxOption CheckboxOption::Simple() {
  auto option = CheckboxOption();
  option.transform = [](const EntryState& s) {
#if defined(FTXUI_MICROSOFT_TERMINAL_FALLBACK)
    // La terminal de Microsoft no utiliza fuentes capaces de renderizar correctamente el glifo
    // predeterminado del radiobox.
    auto prefix = text(s.state ? "[X] " : "[ ] ");  // NOLINT
#else
    auto prefix = text(s.state ? "▣ " : "☐ ");  // NOLINT
#endif
    auto t = text(s.label);
    if (s.active) {
      t |= bold;
    }
    if (s.focused) {
      t |= inverted;
    }
    return hbox({prefix, t});
  };
  return option;
}

/// @brief Opción para Radiobox estándar
// static
RadioboxOption RadioboxOption::Simple() {
  auto option = RadioboxOption();
  option.transform = [](const EntryState& s) {
#if defined(FTXUI_MICROSOFT_TERMINAL_FALLBACK)
    // La terminal de Microsoft no utiliza fuentes capaces de renderizar correctamente el glifo
    // predeterminado del radiobox.
    auto prefix = text(s.state ? "(*) " : "( ) ");  // NOLINT
#else
    auto prefix = text(s.state ? "◉ " : "○ ");  // NOLINT
#endif
    auto t = text(s.label);
    if (s.active) {
      t |= bold;
    }
    if (s.focused) {
      t |= inverted;
    }
    return hbox({prefix, t});
  };
  return option;
}

/// @brief Opciones estándar para el componente de entrada.
// static
InputOption InputOption::Default() {
  InputOption option;
  option.transform = [](InputState state) {
    state.element |= color(Color::White);

    if (state.is_placeholder) {
      state.element |= dim;
    }

    if (state.focused) {
      state.element |= inverted;
    } else if (state.hovered) {
      state.element |= bgcolor(Color::GrayDark);
    }

    return state.element;
  };
  return option;
}

/// @brief Opciones estándar para un componente de entrada más bonito.
// static
InputOption InputOption::Spacious() {
  InputOption option;
  option.transform = [](InputState state) {
    state.element |= borderEmpty;
    state.element |= color(Color::White);

    if (state.is_placeholder) {
      state.element |= dim;
    }

    if (state.focused) {
      state.element |= bgcolor(Color::Black);
    }

    if (state.hovered) {
      state.element |= bgcolor(Color::GrayDark);
    }

    return state.element;
  };
  return option;
}

}  // namespace ftxui
