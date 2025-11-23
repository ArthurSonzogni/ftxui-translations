// Copyright 2022 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui se trouve
// dans le fichier LICENSE.
#include "ftxui/component/component_options.hpp"

#include <ftxui/screen/color.hpp>  // for Color, Color::White, Color::Black, Color::GrayDark, Color::Blue, Color::GrayLight, Color::Red
#include <memory>                  // for shared_ptr
#include <utility>                 // for move
#include "ftxui/component/animation.hpp"  // for Function, Duration
#include "ftxui/dom/direction.hpp"
#include "ftxui/dom/elements.hpp"  // for operator|=, Element, text, bgcolor, inverted, bold, dim, operator|, color, borderEmpty, hbox, automerge, border, borderLight

namespace ftxui {

/// @brief Une option de couleur qui peut être animée.
/// @params _inactive La couleur lorsque le composant est inactif.
/// @params _active La couleur lorsque le composant est actif.
/// @params _duration La durée de l'animation.
/// @params _function La fonction d'interpolation de l'animation.
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

/// @brief Définit comment le soulignement doit être animé.
/// @param d La durée de l'animation.
/// @param f La fonction d'interpolation de l'animation.
void UnderlineOption::SetAnimation(animation::Duration d,
                                   animation::easing::Function f) {
  SetAnimationDuration(d);
  SetAnimationFunction(std::move(f));
}

/// @brief Définit comment le soulignement doit être animé.
/// @param d La durée de l'animation.
void UnderlineOption::SetAnimationDuration(animation::Duration d) {
  leader_duration = d;
  follower_duration = d;
}

/// @brief Définit comment le soulignement doit être animé.
/// @param f La fonction d'interpolation de l'animation.
void UnderlineOption::SetAnimationFunction(animation::easing::Function f) {
  leader_function = f;
  follower_function = std::move(f);
}

/// @brief Définit comment le soulignement doit être animé.
/// Ceci est utile pour désynchroniser l'animation du leader et du
/// follower.
/// @param f_leader La durée de l'animation pour le leader.
/// @param f_follower La durée de l'animation pour le follower.
void UnderlineOption::SetAnimationFunction(
    animation::easing::Function f_leader,
    animation::easing::Function f_follower) {
  leader_function = std::move(f_leader);
  follower_function = std::move(f_follower);
}

/// @brief Options standard pour un menu horizontal.
/// Cela peut être utile pour implémenter une barre d'onglets.
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

/// @brief Options standard pour un menu horizontal animé.
/// Cela peut être utile pour implémenter une barre d'onglets.
// static
MenuOption MenuOption::HorizontalAnimated() {
  auto option = Horizontal();
  option.underline.enabled = true;
  return option;
}

/// @brief Options standard pour un menu vertical.
/// Cela peut être utile pour implémenter une liste d'éléments sélectionnables.
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

/// @brief Options standard pour un menu vertical animé.
/// Cela peut être utile pour implémenter une liste d'éléments sélectionnables.
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

/// @brief Options standard pour un menu horizontal avec un séparateur.
/// Cela peut être utile pour implémenter une barre d'onglets.
// static
MenuOption MenuOption::Toggle() {
  auto option = MenuOption::Horizontal();
  option.elements_infix = [] { return text("│") | automerge; };
  return option;
}

/// @brief Crée une ButtonOption, mise en évidence à l'aide des caractères [].
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

/// @brief Crée une ButtonOption, inversée lorsqu'elle est sélectionnée.
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

/// @brief Crée une ButtonOption. Le bouton est affiché avec une bordure, inversée
/// lorsqu'il est sélectionné. C'est la valeur par défaut actuelle.
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

/// @brief Crée une ButtonOption, utilisant des couleurs animées.
// static
ButtonOption ButtonOption::Animated() {
  return Animated(Color::Black, Color::GrayLight,  //
                  Color::GrayDark, Color::White);
}

/// @brief Crée une ButtonOption, utilisant des couleurs animées.
// static
ButtonOption ButtonOption::Animated(Color color) {
  return ButtonOption::Animated(
      Color::Interpolate(0.85F, color, Color::Black),   // NOLINT
      Color::Interpolate(0.10F, color, Color::White),   // NOLINT
      Color::Interpolate(0.10F, color, Color::Black),   // NOLINT
      Color::Interpolate(0.85F, color, Color::White));  // NOLINT
}

/// @brief Crée une ButtonOption, utilisant des couleurs animées.
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

/// @brief Crée une ButtonOption, utilisant des couleurs animées.
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

/// @brief Option pour une case à cocher standard.
// static
CheckboxOption CheckboxOption::Simple() {
  auto option = CheckboxOption();
  option.transform = [](const EntryState& s) {
#if defined(FTXUI_MICROSOFT_TERMINAL_FALLBACK)
    // Le terminal Microsoft n'utilise pas de polices capables de rendre correctement le glyphe
    // de la case à cocher par défaut.
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

/// @brief Option pour un bouton radio standard
// static
RadioboxOption RadioboxOption::Simple() {
  auto option = RadioboxOption();
  option.transform = [](const EntryState& s) {
#if defined(FTXUI_MICROSOFT_TERMINAL_FALLBACK)
    // Le terminal Microsoft n'utilise pas de polices capables de rendre correctement le glyphe
    // de la case d'option par défaut.
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

/// @brief Options standard pour le composant d'entrée.
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

/// @brief Options standard pour un composant d'entrée plus esthétique.
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
