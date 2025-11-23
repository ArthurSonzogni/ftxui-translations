// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <functional>  // for function
#include <sstream>     // for basic_istream, stringstream
#include <string>      // for string, allocator, getline
#include <utility>     // for move

#include "ftxui/dom/elements.hpp"  // for flexbox, Element, text, Elements, operator|, xflex, paragraph, paragraphAlignCenter, paragraphAlignJustify, paragraphAlignLeft, paragraphAlignRight
#include "ftxui/dom/flexbox_config.hpp"  // for FlexboxConfig, FlexboxConfig::JustifyContent, FlexboxConfig::JustifyContent::Center, FlexboxConfig::JustifyContent::FlexEnd, FlexboxConfig::JustifyContent::SpaceBetween

namespace ftxui {

namespace {
Elements Split(const std::string& the_text) {
  Elements output;
  std::stringstream ss(the_text);
  std::string word;
  while (std::getline(ss, word, ' ')) {
    output.push_back(text(word));
  }
  return output;
}

Element Split(const std::string& paragraph,
              const std::function<Element(std::string)>& f) {
  Elements output;
  std::stringstream ss(paragraph);
  std::string line;
  while (std::getline(ss, line, '\n')) {
    output.push_back(f(line));
  }
  return vbox(std::move(output));
}

}  // namespace

/// @brief Renvoie un élément affichant le paragraphe sur plusieurs lignes.
/// @ingroup dom
/// @see flexbox.
Element paragraph(const std::string& the_text) {
  return paragraphAlignLeft(the_text);
}

/// @brief Renvoie un élément affichant le paragraphe sur plusieurs lignes, aligné à
/// gauche.
/// @ingroup dom
/// @see flexbox.
Element paragraphAlignLeft(const std::string& the_text) {
  return Split(the_text, [](const std::string& line) {
    static const auto config = FlexboxConfig().SetGap(1, 0);
    return flexbox(Split(line), config);
  });
};

/// @brief Renvoie un élément affichant le paragraphe sur plusieurs lignes, aligné à
/// droite.
/// @ingroup dom
/// @see flexbox.
Element paragraphAlignRight(const std::string& the_text) {
  return Split(the_text, [](const std::string& line) {
    static const auto config = FlexboxConfig().SetGap(1, 0).Set(
        FlexboxConfig::JustifyContent::FlexEnd);
    return flexbox(Split(line), config);
  });
}

/// @brief Renvoie un élément affichant le paragraphe sur plusieurs lignes, aligné au
/// centre.
/// @ingroup dom
/// @see flexbox.
Element paragraphAlignCenter(const std::string& the_text) {
  return Split(the_text, [](const std::string& line) {
    static const auto config =
        FlexboxConfig().SetGap(1, 0).Set(FlexboxConfig::JustifyContent::Center);
    return flexbox(Split(line), config);
  });
}

/// @brief Renvoie un élément affichant le paragraphe sur plusieurs lignes, aligné
/// en utilisant un alignement justifié.
/// @ingroup dom
/// @see flexbox.
Element paragraphAlignJustify(const std::string& the_text) {
  return Split(the_text, [](const std::string& line) {
    static const auto config = FlexboxConfig().SetGap(1, 0).Set(
        FlexboxConfig::JustifyContent::SpaceBetween);
    Elements words = Split(line);
    words.push_back(text("") | xflex);
    return flexbox(std::move(words), config);
  });
}

}  // namespace ftxui
