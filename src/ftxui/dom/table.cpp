// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/dom/table.hpp"

#include <algorithm>         // for max
#include <initializer_list>  // for initializer_list
#include <memory>   // for allocator, shared_ptr, allocator_traits<>::value_type
#include <utility>  // for move, swap
#include <vector>   // for vector

#include "ftxui/dom/elements.hpp"  // for Element, operator|, text, separatorCharacter, Elements, BorderStyle, Decorator, emptyElement, size, gridbox, EQUAL, flex, flex_shrink, HEIGHT, WIDTH

namespace ftxui {
namespace {

bool IsCell(int x, int y) {
  return x % 2 == 1 && y % 2 == 1;
}

// NOLINTNEXTLINE
static std::string charset[6][6] = {
    {"┌", "┐", "└", "┘", "─", "│"},  // LIGHT
    {"┏", "┓", "┗", "┛", "╍", "╏"},  // DASHED
    {"┏", "┓", "┗", "┛", "━", "┃"},  // HEAVY
    {"╔", "╗", "╚", "╝", "═", "║"},  // DOUBLE
    {"╭", "╮", "╰", "╯", "─", "│"},  // ROUNDED
    {" ", " ", " ", " ", " ", " "},  // EMPTY
};

int Wrap(int input, int modulo) {
  input %= modulo;
  input += modulo;
  input %= modulo;
  return input;
}

void Order(int& a, int& b) {
  if (a >= b) {
    std::swap(a, b);
  }
}

}  // namespace

/// @brief Crée une table vide.
Table::Table() {
  Initialize({});
}

/// @brief Crée une table à partir d'un vecteur de vecteurs de chaînes de caractères.
/// @param input Les données d'entrée.
Table::Table(std::vector<std::vector<std::string>> input) {
  std::vector<std::vector<Element>> output;
  output.reserve(input.size());
  for (auto& row : input) {
    output.emplace_back();
    auto& output_row = output.back();
    output_row.reserve(row.size());
    for (auto& cell : row) {
      output_row.push_back(text(std::move(cell)));
    }
  }
  Initialize(std::move(output));
}

/// @brief Crée une table à partir d'un vecteur de vecteurs d'éléments.
/// @param input Les éléments d'entrée.
Table::Table(std::vector<std::vector<Element>> input) {
  Initialize(std::move(input));
}

// @brief Crée une table à partir d'une liste de listes de chaînes de caractères.
// @param init Les données d'entrée.
Table::Table(std::initializer_list<std::vector<std::string>> init) {
  std::vector<std::vector<Element>> input;
  for (const auto& row : init) {
    std::vector<Element> output_row;
    output_row.reserve(row.size());
    for (const auto& cell : row) {
      output_row.push_back(text(cell));
    }
    input.push_back(std::move(output_row));
  }
  Initialize(std::move(input));
}

// private
void Table::Initialize(std::vector<std::vector<Element>> input) {
  input_dim_y_ = static_cast<int>(input.size());
  input_dim_x_ = 0;
  for (auto& row : input) {
    input_dim_x_ = std::max(input_dim_x_, int(row.size()));
  }

  dim_y_ = 2 * input_dim_y_ + 1;
  dim_x_ = 2 * input_dim_x_ + 1;

  // Reserve space.
  elements_.resize(dim_y_);
  for (int y = 0; y < dim_y_; ++y) {
    elements_[y].resize(dim_x_);
  }

  // Transfert elements_ from |input| toward |elements_|.
  {
    int y = 1;
    for (auto& row : input) {
      int x = 1;
      for (auto& cell : row) {
        elements_[y][x] = std::move(cell);
        x += 2;
      }
      y += 2;
    }
  }

  // Add empty element for the border.
  for (int y = 0; y < dim_y_; ++y) {
    for (int x = 0; x < dim_x_; ++x) {
      auto& element = elements_[y][x];

      if (IsCell(x, y)) {
        if (!element) {
          element = emptyElement();
        }
        continue;
      }

      element = emptyElement();
    }
  }
}

/// @brief Sélectionne une ligne de la table.
/// @param index L'indice de la ligne à sélectionner.
/// @note Vous pouvez utiliser des indices négatifs pour sélectionner à partir de la fin.
TableSelection Table::SelectRow(int index) {
  return SelectRectangle(0, -1, index, index);
}

/// @brief Sélectionne une plage de lignes de la table.
/// @param row_min La première ligne à sélectionner.
/// @param row_max La dernière ligne à sélectionner.
/// @note Vous pouvez utiliser des indices négatifs pour sélectionner à partir de la fin.
TableSelection Table::SelectRows(int row_min, int row_max) {
  return SelectRectangle(0, -1, row_min, row_max);
}

/// @brief Sélectionne une colonne de la table.
/// @param index L'indice de la colonne à sélectionner.
/// @note Vous pouvez utiliser des indices négatifs pour sélectionner à partir de la fin.
TableSelection Table::SelectColumn(int index) {
  return SelectRectangle(index, index, 0, -1);
}

/// @brief Sélectionne une plage de colonnes de la table.
/// @param column_min La première colonne à sélectionner.
/// @param column_max La dernière colonne à sélectionner.
/// @note Vous pouvez utiliser des indices négatifs pour sélectionner à partir de la fin.
TableSelection Table::SelectColumns(int column_min, int column_max) {
  return SelectRectangle(column_min, column_max, 0, -1);
}

/// @brief Sélectionne une cellule de la table.
/// @param column La colonne de la cellule à sélectionner.
/// @param row La ligne de la cellule à sélectionner.
/// @note Vous pouvez utiliser des indices négatifs pour sélectionner à partir de la fin.
TableSelection Table::SelectCell(int column, int row) {
  return SelectRectangle(column, column, row, row);
}

/// @brief Sélectionne un rectangle de la table.
/// @param column_min La première colonne à sélectionner.
/// @param column_max La dernière colonne à sélectionner.
/// @param row_min La première ligne à sélectionner.
/// @param row_max La dernière ligne à sélectionner.
/// @note Vous pouvez utiliser des indices négatifs pour sélectionner à partir de la fin.
TableSelection Table::SelectRectangle(int column_min,
                                      int column_max,
                                      int row_min,
                                      int row_max) {
  column_min = Wrap(column_min, input_dim_x_);
  column_max = Wrap(column_max, input_dim_x_);
  Order(column_min, column_max);
  row_min = Wrap(row_min, input_dim_y_);
  row_max = Wrap(row_max, input_dim_y_);
  Order(row_min, row_max);

  TableSelection output;  // NOLINT
  output.table_ = this;
  output.x_min_ = 2 * column_min;
  output.x_max_ = 2 * column_max + 2;
  output.y_min_ = 2 * row_min;
  output.y_max_ = 2 * row_max + 2;
  return output;
}

/// @brief Sélectionne toute la table.
TableSelection Table::SelectAll() {
  TableSelection output;  // NOLINT
  output.table_ = this;
  output.x_min_ = 0;
  output.x_max_ = dim_x_ - 1;
  output.y_min_ = 0;
  output.y_max_ = dim_y_ - 1;
  return output;
}

/// @brief Rend la table.
/// @return La table rendue. C'est un élément que vous pouvez dessiner.
Element Table::Render() {
  for (int y = 0; y < dim_y_; ++y) {
    for (int x = 0; x < dim_x_; ++x) {
      auto& it = elements_[y][x];

      // Line
      if ((x + y) % 2 == 1) {
        it = std::move(it) | flex;
        continue;
      }

      // Cells
      if ((x % 2) == 1 && (y % 2) == 1) {
        it = std::move(it) | flex_shrink;
        continue;
      }

      // Corners
      it = std::move(it) | size(WIDTH, EQUAL, 0) | size(HEIGHT, EQUAL, 0);
    }
  }
  dim_x_ = 0;
  dim_y_ = 0;
  return gridbox(std::move(elements_));
}

/// @brief Applique le décorateur à la sélection.
/// Ceci décore à la fois les cellules, les lignes et les coins.
/// @param decorator Le décorateur à appliquer.
// NOLINTNEXTLINE
void TableSelection::Decorate(Decorator decorator) {
  for (int y = y_min_; y <= y_max_; ++y) {
    for (int x = x_min_; x <= x_max_; ++x) {
      Element& e = table_->elements_[y][x];
      e = std::move(e) | decorator;
    }
  }
}

/// @brief Applique le décorateur à la sélection.
/// @param decorator Le décorateur à appliquer.
/// Ceci décore uniquement les cellules.
// NOLINTNEXTLINE
void TableSelection::DecorateCells(Decorator decorator) {
  for (int y = y_min_; y <= y_max_; ++y) {
    for (int x = x_min_; x <= x_max_; ++x) {
      if (y % 2 == 1 && x % 2 == 1) {
        Element& e = table_->elements_[y][x];
        e = std::move(e) | decorator;
      }
    }
  }
}

/// @brief Applique le décorateur à la sélection.
/// Ceci décore uniquement les lignes modulo `modulo` avec un décalage de `shift`.
/// @param decorator Le décorateur à appliquer.
/// @param modulo Le modulo des lignes à décorer.
/// @param shift Le décalage des lignes à décorer.
// NOLINTNEXTLINE
void TableSelection::DecorateAlternateColumn(Decorator decorator,
                                             int modulo,
                                             int shift) {
  for (int y = y_min_; y <= y_max_; ++y) {
    for (int x = x_min_; x <= x_max_; ++x) {
      if (y % 2 == 1 && (x / 2) % modulo == shift) {
        Element& e = table_->elements_[y][x];
        e = std::move(e) | decorator;
      }
    }
  }
}

/// @brief Applique le décorateur à la sélection.
/// Ceci décore uniquement les lignes modulo `modulo` avec un décalage de `shift`.
/// @param decorator Le décorateur à appliquer.
/// @param modulo Le modulo des lignes à décorer.
/// @param shift Le décalage des lignes à décorer.
// NOLINTNEXTLINE
void TableSelection::DecorateAlternateRow(Decorator decorator,
                                          int modulo,
                                          int shift) {
  for (int y = y_min_ + 1; y <= y_max_ - 1; ++y) {
    for (int x = x_min_; x <= x_max_; ++x) {
      if (y % 2 == 1 && (y / 2) % modulo == shift) {
        Element& e = table_->elements_[y][x];
        e = std::move(e) | decorator;
      }
    }
  }
}

/// @brief Applique le décorateur à la sélection.
/// Ceci décore uniquement les coins modulo `modulo` avec un décalage de `shift`.
/// @param decorator Le décorateur à appliquer.
/// @param modulo Le modulo des coins à décorer.
/// @param shift Le décalage des coins à décorer. {
  for (int y = y_min_; y <= y_max_; ++y) {
    for (int x = x_min_; x <= x_max_; ++x) {
      if (y % 2 == 1 && x % 2 == 1 && ((x / 2) % modulo == shift)) {
        Element& e = table_->elements_[y][x];
        e = std::move(e) | decorator;
      }
    }
  }
}

/// @brief Applique le décorateur à la sélection.
/// Ceci décore uniquement les coins modulo `modulo` avec un décalage de `shift`.
/// @param decorator Le décorateur à appliquer.
/// @param modulo Le modulo des coins à décorer.
/// @param shift Le décalage des coins à décorer.
// NOLINTNEXTLINE
void TableSelection::DecorateCellsAlternateRow(Decorator decorator,
                                               int modulo,
                                               int shift) {
  for (int y = y_min_; y <= y_max_; ++y) {
    for (int x = x_min_; x <= x_max_; ++x) {
      if (y % 2 == 1 && x % 2 == 1 && ((y / 2) % modulo == shift)) {
        Element& e = table_->elements_[y][x];
        e = std::move(e) | decorator;
      }
    }
  }
}

/// @brief Applique une `bordure` autour de la sélection.
/// @param border Le style de bordure à appliquer.
void TableSelection::Border(BorderStyle border) {
  BorderLeft(border);
  BorderRight(border);
  BorderTop(border);
  BorderBottom(border);

  // NOLINTNEXTLINE
  table_->elements_[y_min_][x_min_] = text(charset[border][0]) | automerge;
  // NOLINTNEXTLINE
  table_->elements_[y_min_][x_max_] = text(charset[border][1]) | automerge;
  // NOLINTNEXTLINE
  table_->elements_[y_max_][x_min_] = text(charset[border][2]) | automerge;
  // NOLINTNEXTLINE
  table_->elements_[y_max_][x_max_] = text(charset[border][3]) | automerge;
}

/// @brief Dessine des lignes de séparation dans la sélection.
/// @param border Le style de bordure à appliquer.
void TableSelection::Separator(BorderStyle border) {
  for (int y = y_min_ + 1; y <= y_max_ - 1; ++y) {
    for (int x = x_min_ + 1; x <= x_max_ - 1; ++x) {
      if (y % 2 == 0 || x % 2 == 0) {
        Element& e = table_->elements_[y][x];
        e = (y % 2 == 1)
                ? separatorCharacter(charset[border][5]) | automerge   // NOLINT
                : separatorCharacter(charset[border][4]) | automerge;  // NOLINT
      }
    }
  }
}

/// @brief Dessine des lignes de séparation verticales dans la sélection.
/// @param border Le style de bordure à appliquer.
void TableSelection::SeparatorVertical(BorderStyle border) {
  for (int y = y_min_ + 1; y <= y_max_ - 1; ++y) {
    for (int x = x_min_ + 1; x <= x_max_ - 1; ++x) {
      if (x % 2 == 0) {
        table_->elements_[y][x] =
            separatorCharacter(charset[border][5]) | automerge;  // NOLINT
      }
    }
  }
}

/// @brief Dessine des lignes de séparation horizontales dans la sélection.
/// @param border Le style de bordure à appliquer.
void TableSelection::SeparatorHorizontal(BorderStyle border) {
  for (int y = y_min_ + 1; y <= y_max_ - 1; ++y) {
    for (int x = x_min_ + 1; x <= x_max_ - 1; ++x) {
      if (y % 2 == 0) {
        table_->elements_[y][x] =
            separatorCharacter(charset[border][4]) | automerge;  // NOLINT
      }
    }
  }
}

/// @brief Dessine des lignes de séparation sur le côté gauche de la sélection.
/// @param border Le style de bordure à appliquer.
void TableSelection::BorderLeft(BorderStyle border) {
  for (int y = y_min_; y <= y_max_; y++) {
    table_->elements_[y][x_min_] =
        separatorCharacter(charset[border][5]) | automerge;  // NOLINT
  }
}

/// @brief Dessine des lignes de séparation sur le côté droit de la sélection.
/// @param border Le style de bordure à appliquer.
void TableSelection::BorderRight(BorderStyle border) {
  for (int y = y_min_; y <= y_max_; y++) {
    table_->elements_[y][x_max_] =
        separatorCharacter(charset[border][5]) | automerge;  // NOLINT
  }
}

/// @brief Dessine des lignes de séparation sur le côté supérieur de la sélection.
/// @param border Le style de bordure à appliquer.
void TableSelection::BorderTop(BorderStyle border) {
  for (int x = x_min_; x <= x_max_; x++) {
    table_->elements_[y_min_][x] =
        separatorCharacter(charset[border][4]) | automerge;  // NOLINT
  }
}

/// @brief Dessine des lignes de séparation sur le côté inférieur de la sélection.
/// @param border Le style de bordure à appliquer.
void TableSelection::BorderBottom(BorderStyle border) {
  for (int x = x_min_; x <= x_max_; x++) {
    table_->elements_[y_max_][x] =
        separatorCharacter(charset[border][4]) | automerge;  // NOLINT
  }
}

}  // namespace ftxui
