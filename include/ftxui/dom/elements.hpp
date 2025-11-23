// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#ifndef FTXUI_DOM_ELEMENTS_HPP
#define FTXUI_DOM_ELEMENTS_HPP

#include <functional>
#include <memory>

#include "ftxui/dom/canvas.hpp"
#include "ftxui/dom/direction.hpp"
#include "ftxui/dom/flexbox_config.hpp"
#include "ftxui/dom/linear_gradient.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/box.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/screen/terminal.hpp"
#include "ftxui/util/ref.hpp"

namespace ftxui {
class Node;
using Element = std::shared_ptr<Node>;
using Elements = std::vector<Element>;
using Decorator = std::function<Element(Element)>;
using GraphFunction = std::function<std::vector<int>(int, int)>;

/// @brief BorderStyle est une énumération qui représente les différents styles
/// de bordures pouvant être appliqués aux éléments de l'interface utilisateur du terminal.
///
/// BorderStyle est une énumération qui représente les différents styles de
/// bordures pouvant être appliqués aux éléments de l'interface utilisateur du terminal.
/// Il est utilisé pour définir l'apparence visuelle des bordures autour des éléments,
/// tels que les fenêtres, les cadres ou les séparateurs.
/// @ingroup dom
enum BorderStyle {
  LIGHT,
  DASHED,
  HEAVY,
  DOUBLE,
  ROUNDED,
  EMPTY,
};

// Enchaîner des éléments dans un décorateur.
// Par exemple, les lignes suivantes sont équivalentes :
// -> text("ftxui") | bold | underlined
// -> underlined(bold(text("FTXUI")))Element operator|(Element, Decorator);
Element& operator|=(Element&, Decorator);
Elements operator|(Elements, Decorator);
Decorator operator|(Decorator, Decorator);

// --- Composant ---
Element text(std::string text);
Element vtext(std::string text);
Element separator();
Element separatorLight();
Element separatorDashed();
Element separatorHeavy();
Element separatorDouble();
Element separatorEmpty();
Element separatorStyled(BorderStyle);
Element separator(Pixel);
Element separatorCharacter(std::string);
Element separatorHSelector(float left,
                           float right,
                           Color unselected_color,
                           Color selected_color);
Element separatorVSelector(float up,
                           float down,
                           Color unselected_color,
                           Color selected_color);
Element gauge(float progress);
Element gaugeLeft(float progress);
Element gaugeRight(float progress);
Element gaugeUp(float progress);
Element gaugeDown(float progress);
Element gaugeDirection(float progress, Direction direction);
Element border(Element);
Element borderLight(Element);
Element borderDashed(Element);
Element borderHeavy(Element);
Element borderDouble(Element);
Element borderRounded(Element);
Element borderEmpty(Element);
Decorator borderStyled(BorderStyle);
Decorator borderStyled(BorderStyle, Color);
Decorator borderStyled(Color);
Decorator borderWith(const Pixel&);
Element window(Element title, Element content, BorderStyle border = ROUNDED);
Element spinner(int charset_index, size_t image_index);
Element paragraph(const std::string& text);
Element paragraphAlignLeft(const std::string& text);
Element paragraphAlignRight(const std::string& text);
Element paragraphAlignCenter(const std::string& text);
Element paragraphAlignJustify(const std::string& text);
Element graph(GraphFunction);
Element emptyElement();
Element canvas(ConstRef<Canvas>);
Element canvas(int width, int height, std::function<void(Canvas&)>);
Element canvas(std::function<void(Canvas&)>);

// -- Décorateur ---
Element bold(Element);
Element dim(Element);
Element italic(Element);
Element inverted(Element);
Element underlined(Element);
Element underlinedDouble(Element);
Element blink(Element);
Element strikethrough(Element);
Decorator color(Color);
Decorator bgcolor(Color);
Decorator color(const LinearGradient&);
Decorator bgcolor(const LinearGradient&);
Element color(Color, Element);
Element bgcolor(Color, Element);
Element color(const LinearGradient&, Element);
Element bgcolor(const LinearGradient&, Element);
Decorator focusPosition(int x, int y);
Decorator focusPositionRelative(float x, float y);
Element automerge(Element child);
Decorator hyperlink(std::string link);
Element hyperlink(std::string link, Element child);
Element selectionStyleReset(Element);
Decorator selectionColor(Color foreground);
Decorator selectionBackgroundColor(Color foreground);
Decorator selectionForegroundColor(Color foreground);
Decorator selectionStyle(std::function<void(Pixel&)> style);

// --- Disposition ---
// Horizontal, Vertical ou empilé d'éléments.
Element hbox(Elements);
Element vbox(Elements);
Element dbox(Elements);
Element flexbox(Elements, FlexboxConfig config = FlexboxConfig());
Element gridbox(std::vector<Elements> lines);

Element hflow(Elements);  // Aide : flexbox par défaut avec direction de ligne.
Element vflow(Elements);  // Aide : flexbox par défaut avec direction de colonne.

// -- Flexibilité ---
// Définit comment partager l'espace restant lorsque tout n'est pas utilisé à l'intérieur d'un
// conteneur.
Element flex(Element);         // Développer/Réduire si possible/nécessaire.
Element flex_grow(Element);    // Développer l'élément si possible.
Element flex_shrink(Element);  // Réduire l'élément si nécessaire.

Element xflex(Element);         // Développer/Réduire si possible/nécessaire sur l'axe X.
Element xflex_grow(Element);    // Développer l'élément si possible sur l'axe X.
Element xflex_shrink(Element);  // Réduire l'élément si nécessaire sur l'axe X.

Element yflex(Element);         // Développer/Réduire si possible/nécessaire sur l'axe Y.
Element yflex_grow(Element);    // Développer l'élément si possible sur l'axe Y.
Element yflex_shrink(Element);  // Réduire l'élément si nécessaire sur l'axe Y.

Element notflex(Element);  // Réinitialiser l'attribut flex.
Element filler();          // Un élément vide extensible.

// -- Surcharger la taille ---
enum WidthOrHeight { WIDTH, HEIGHT };
enum Constraint { LESS_THAN, EQUAL, GREATER_THAN };
Decorator size(WidthOrHeight, Constraint, int value);

// --- Cadre ---
// Un cadre est une zone défilante. La zone interne est potentiellement plus grande que
// la zone externe. La zone interne est défilée afin de rendre visible l'élément focalisé.
Element frame(Element);
Element xframe(Element);
Element yframe(Element);
Element focus(Element);
Element select(Element e);  // Obsolète - Alias pour focus.

// --- Curseur ---
// Ceux-ci sont similaires à `focus`, mais changent également la forme du curseur.
Element focusCursorBlock(Element);
Element focusCursorBlockBlinking(Element);
Element focusCursorBar(Element);
Element focusCursorBarBlinking(Element);
Element focusCursorUnderline(Element);
Element focusCursorUnderlineBlinking(Element);

// --- Divers ---
Element vscroll_indicator(Element);
Element hscroll_indicator(Element);
Decorator reflect(Box& box);
// Avant de dessiner l'|element|, effacez le pixel en dessous. Ceci est utile en
// combinaison avec dbox.
Element clear_under(Element element);

// --- Utilitaire --------------------------------------------------------------------
Element hcenter(Element);
Element vcenter(Element);
Element center(Element);
Element align_right(Element);
Element nothing(Element element);

namespace Dimension {
Dimensions Fit(Element&, bool extend_beyond_screen = false);
}  // namespace Dimension

}  // namespace ftxui

// Rendre le conteneur capable de prendre n'importe quel nombre d'enfants en entrée.
#include "ftxui/dom/take_any_args.hpp"

// Inclure les anciennes définitions utilisant wstring.
#include "ftxui/dom/deprecated.hpp"
#endif  // FTXUI_DOM_ELEMENTS_HPP
