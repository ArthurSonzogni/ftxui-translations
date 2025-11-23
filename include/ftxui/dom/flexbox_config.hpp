// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef FTXUI_DOM_FLEXBOX_CONFIG_HPP
#define FTXUI_DOM_FLEXBOX_CONFIG_HPP

/*
  Ceci reproduit le modèle CSS flexbox.
  Voir le guide pour la documentation :
  https://css-tricks.com/snippets/css/a-guide-to-flexbox/
*/

namespace ftxui {

/// @brief FlexboxConfig est une structure de configuration qui définit les
/// propriétés de mise en page pour un conteneur flexbox.
//
/// Elle vous permet de spécifier la direction des éléments flexibles, s'ils
/// doivent s'enrouler, comment ils doivent être justifiés le long de l'axe
/// principal, et comment ils doivent être alignés le long de l'axe transversal.
/// Elle inclut également des propriétés pour les espacements entre les
/// éléments flexibles dans les deux axes principal et transversal.
/// Cette structure est utilisée pour configurer le comportement de la mise en
/// page des conteneurs flexbox dans une interface utilisateur de terminal.
///
/// @ingroup dom
struct FlexboxConfig {
  /// Ceci établit l'axe principal, définissant ainsi la direction dans laquelle
  /// les éléments flexibles sont placés dans le conteneur flex. Flexbox est (à
  /// part l'enveloppement) un concept de mise en page unidirectionnel. Pensez
  /// aux éléments flexibles comme étant principalement disposés en rangées
  /// horizontales ou en colonnes verticales.
  enum class Direction {
    Row,            ///< Les éléments flexibles sont disposés en ligne.
    RowInversed,    ///< Les éléments flexibles sont disposés en ligne, mais en
                    ///< ordre inverse.
    Column,         ///< Les éléments flexibles sont disposés en colonne.
    ColumnInversed  ///< Les éléments flexibles sont disposés en colonne, mais
                    ///< en ordre inverse.
  };
  Direction direction = Direction::Row;

  /// Par défaut, tous les éléments flexibles tenteront de tenir sur une seule
  /// ligne. Vous pouvez modifier cela et permettre aux éléments de s'enrouler
  /// si nécessaire avec cette propriété.
  enum class Wrap {
    NoWrap,        ///< Tous les éléments flexibles tenteront de tenir sur une
                   ///< seule ligne.
    Wrap,          ///< Les éléments flexibles s'enrouleront sur plusieurs
                   ///< lignes.
    WrapInversed,  ///< Les éléments flexibles s'enrouleront sur plusieurs
                   ///< lignes, mais en ordre inverse.
  };
  Wrap wrap = Wrap::Wrap;

  /// Ceci définit l'alignement le long de l'axe principal. Il aide à distribuer
  /// l'espace libre supplémentaire restant lorsque tous les éléments flexibles
  /// sur une ligne sont inflexibles, ou sont flexibles mais ont atteint leur
    /// taille maximale. Il exerce également un certain contrôle sur l'alignement
    /// des éléments lorsqu'ils débordent de la ligne.
  enum class JustifyContent {
    /// Les éléments sont alignés au début de la direction du flexbox.
    FlexStart,
    /// Les éléments sont alignés à la fin de la direction du flexbox.
    FlexEnd,
    /// Les éléments sont centrés le long de la ligne.
    Center,
    /// Les éléments sont étirés pour remplir la ligne.
    Stretch,
    /// Les éléments sont répartis uniformément sur la ligne ; le premier
    /// élément est sur la ligne de début, le dernier élément sur la ligne de fin.
    SpaceBetween,
    /// Les éléments sont répartis uniformément sur la ligne avec un espace égal
    /// autour d'eux. Notez que visuellement les espaces ne sont pas égaux, car
    /// tous les éléments ont un espace égal des deux côtés. Le premier élément
    /// aura une unité d'espace contre le bord du conteneur, mais deux unités
    /// d'espace entre le prochain élément car cet élément a son propre
    /// espacement qui s'applique.
    SpaceAround,
    /// Les éléments sont répartis de sorte que l'espacement entre deux éléments
    /// (et l'espace jusqu'aux bords) soit égal.
    SpaceEvenly,
  };
  JustifyContent justify_content = JustifyContent::FlexStart;

  /// Ceci définit le comportement par défaut de la disposition des éléments
  /// flexibles le long de l'axe transversal sur la ligne actuelle. Considérez-le
  /// comme la version de justify-content pour l'axe transversal
  /// (perpendiculaire à l'axe principal).
  enum class AlignItems {
    FlexStart,  ///< Les éléments sont placés au début de l'axe transversal.
    FlexEnd,    ///< Les éléments sont placés à la fin de l'axe transversal.
    Center,     ///< Les éléments sont centrés le long de l'axe transversal.
    Stretch,    ///< Les éléments sont étirés pour remplir l'axe transversal.
  };
  AlignItems align_items = AlignItems::FlexStart;

  // Ceci aligne les lignes d'un conteneur flex lorsqu'il y a un espace
  // supplémentaire dans l'axe transversal, de manière similaire à la façon dont
  // justify-content aligne les éléments individuels dans l'axe principal.
  enum class AlignContent {
    FlexStart,     ///< Les éléments sont placés au début de l'axe transversal.
    FlexEnd,       ///< Les éléments sont placés à la fin de l'axe transversal.
    Center,        ///< Les éléments sont centrés le long de l'axe transversal.
    Stretch,       ///< Les éléments sont étirés pour remplir l'axe transversal.
    SpaceBetween,  ///< Les éléments sont répartis uniformément dans l'axe
                   ///< transversal.
    SpaceAround,   ///< Les éléments sont répartis uniformément avec un espace
                   ///< égal autour de chaque ligne.
    SpaceEvenly,  ///< Les éléments sont répartis uniformément dans l'axe
                  ///< transversal avec un espace égal autour d'eux.
  };
  AlignContent align_content = AlignContent::FlexStart;

  int gap_x = 0;
  int gap_y = 0;

  // Modèle de constructeur. Pour une utilisation chaînée comme :
  // ```
  // FlexboxConfig()
  //    .Set(FlexboxConfig::Direction::Row)
  //    .Set(FlexboxConfig::Wrap::Wrap);
  // ```
  FlexboxConfig& Set(FlexboxConfig::Direction);
  FlexboxConfig& Set(FlexboxConfig::Wrap);
  FlexboxConfig& Set(FlexboxConfig::JustifyContent);
  FlexboxConfig& Set(FlexboxConfig::AlignItems);
  FlexboxConfig& Set(FlexboxConfig::AlignContent);
  FlexboxConfig& SetGap(int gap_x, int gap_y);
};

}  // namespace ftxui

#endif  // FTXUI_DOM_FLEXBOX_CONFIG_HPP