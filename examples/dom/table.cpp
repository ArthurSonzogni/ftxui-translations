// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée
// dans le fichier LICENSE.
#include <ftxui/dom/elements.hpp>  // for color, Fit, LIGHT, align_right, bold, DOUBLE
#include <ftxui/dom/table.hpp>      // for Table, TableSelection
#include <ftxui/screen/screen.hpp>  // for Screen
#include <iostream>                 // for endl, cout, ostream
#include <string>                   // for basic_string, allocator, string
#include <vector>                   // for vector

#include "ftxui/dom/node.hpp"  // for Render
#include "ftxui/screen/color.hpp"  // for Color, Color::Blue, Color::Cyan, Color::White, ftxui

int main() {
  using namespace ftxui;

  auto table = Table({
      {"Version", "Marketing name", "Release date", "API level", "Runtime"},
      {"2.3", "Gingerbread", "February 9 2011", "10", "Dalvik 1.4.0"},
      {"4.0", "Ice Cream Sandwich", "October 19 2011", "15", "Dalvik"},
      {"4.1", "Jelly Bean", "July 9 2012", "16", "Dalvik"},
      {"4.2", "Jelly Bean", "November 13 2012", "17", "Dalvik"},
      {"4.3", "Jelly Bean", "July 24 2013", "18", "Dalvik"},
      {"4.4", "KitKat", "October 31 2013", "19", "Dalvik and ART"},
      {"5.0", "Lollipop", "November 3 2014", "21", "ART"},
      {"5.1", "Lollipop", "March 9 2015", "22", "ART"},
      {"6.0", "Marshmallow", "October 5 2015", "23", "ART"},
      {"7.0", "Nougat", "August 22 2016", "24", "ART"},
      {"7.1", "Nougat", "October 4 2016", "25", "ART"},
      {"8.0", "Oreo", "August 21 2017", "26", "ART"},
      {"8.1", "Oreo", "December 5 2017", "27", "ART"},
      {"9", "Pie", "August 6 2018", "28", "ART"},
      {"10", "10", "September 3 2019", "29", "ART"},
      {"11", "11", "September 8 2020", "30", "ART"},
  });

  table.SelectAll().Border(LIGHT);

  // Ajouter une bordure autour de la première colonne.
  table.SelectColumn(0).Border(LIGHT);

  // Rendre la première ligne en gras avec une double bordure.
  table.SelectRow(0).Decorate(bold);
  table.SelectRow(0).SeparatorVertical(LIGHT);
  table.SelectRow(0).Border(DOUBLE);

  // Aligner à droite la colonne "Release date".
  table.SelectColumn(2).DecorateCells(align_right);

  // Sélectionner la ligne de la deuxième à la dernière.
  auto content = table.SelectRows(1, -1);
  // Alterner entre 3 couleurs.
  content.DecorateCellsAlternateRow(color(Color::Blue), 3, 0);
  content.DecorateCellsAlternateRow(color(Color::Cyan), 3, 1);
  content.DecorateCellsAlternateRow(color(Color::White), 3, 2);

  auto document = table.Render();
  auto screen =
      Screen::Create(Dimension::Fit(document, /*extend_beyond_screen=*/true));
  Render(screen, document);
  screen.Print();
  std::cout << std::endl;

  return 0;
}
