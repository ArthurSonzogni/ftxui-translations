// Copyright 2021 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
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
    {"┌", "┐", "└", "┘", "─", "│"},  // LIGERO
    {"┏", "┓", "┗", "┛", "╍", "╏"},  // DISCONTINUO
    {"┏", "┓", "┗", "┛", "━", "┃"},  // GRUESO
    {"╔", "╗", "╚", "╝", "═", "║"},  // DOBLE
    {"╭", "╮", "╰", "╯", "─", "│"},  // REDONDEADO
    {" ", " ", " ", " ", " ", " "},  // VACÍO
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

/// @brief Crea una tabla vacía.
Table::Table() {
  Initialize({});
}

/// @brief Crea una tabla a partir de un vector de vectores de cadenas.
/// @param input Los datos de entrada.
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

/// @brief Crea una tabla a partir de un vector de vectores de Element.
/// @param input Los elementos de entrada.
Table::Table(std::vector<std::vector<Element>> input) {
  Initialize(std::move(input));
}

// @brief Crea una tabla a partir de una lista de listas de cadenas.
// @param init Los datos de entrada.
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

/// @brief Selecciona una fila de la tabla.
/// @param index El índice de la fila a seleccionar.
/// @note Puedes usar índices negativos para seleccionar desde el final.
TableSelection Table::SelectRow(int index) {
  return SelectRectangle(0, -1, index, index);
}

/// @brief Selecciona un rango de filas de la tabla.
/// @param row_min La primera fila a seleccionar.
/// @param row_max La última fila a seleccionar.
/// @note Puedes usar índices negativos para seleccionar desde el final.
TableSelection Table::SelectRows(int row_min, int row_max) {
  return SelectRectangle(0, -1, row_min, row_max);
}

/// @brief Selecciona una columna de la tabla.
/// @param index El índice de la columna a seleccionar.
/// @note Puedes usar índices negativos para seleccionar desde el final.
TableSelection Table::SelectColumn(int index) {
  return SelectRectangle(index, index, 0, -1);
}

/// @brief Selecciona un rango de columnas de la tabla.
/// @param column_min La primera columna a seleccionar.
/// @param column_max La última columna a seleccionar.
/// @note Puedes usar índices negativos para seleccionar desde el final.
TableSelection Table::SelectColumns(int column_min, int column_max) {
  return SelectRectangle(column_min, column_max, 0, -1);
}

/// @brief Selecciona una celda de la tabla.
/// @param column La columna de la celda a seleccionar.
/// @param row La fila de la celda a seleccionar.
/// @note Puedes usar índices negativos para seleccionar desde el final.
TableSelection Table::SelectCell(int column, int row) {
  return SelectRectangle(column, column, row, row);
}

/// @brief Selecciona un rectángulo de la tabla.
/// @param column_min La primera columna a seleccionar.
/// @param column_max La última columna a seleccionar.
/// @param row_min La primera fila a seleccionar.
/// @param row_max La última fila a seleccionar.
/// @note Puedes usar índices negativos para seleccionar desde el final.
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

/// @brief Selecciona toda la tabla.
TableSelection Table::SelectAll() {
  TableSelection output;  // NOLINT
  output.table_ = this;
  output.x_min_ = 0;
  output.x_max_ = dim_x_ - 1;
  output.y_min_ = 0;
  output.y_max_ = dim_y_ - 1;
  return output;
}

/// @brief Renderiza la tabla.
/// @return La tabla renderizada. Este es un elemento que puedes dibujar.
Element Table::Render() {
  for (int y = 0; y < dim_y_; ++y) {
    for (int x = 0; x < dim_x_; ++x) {
      auto& it = elements_[y][x];

      // Línea
      if ((x + y) % 2 == 1) {
        it = std::move(it) | flex;
        continue;
      }

      // Celdas
      if ((x % 2) == 1 && (y % 2) == 1) {
        it = std::move(it) | flex_shrink;
        continue;
      }

      // Esquinas
      it = std::move(it) | size(WIDTH, EQUAL, 0) | size(HEIGHT, EQUAL, 0);
    }
  }
  dim_x_ = 0;
  dim_y_ = 0;
  return gridbox(std::move(elements_));
}

/// @brief Aplica el `decorator` a la selección.
/// Esto decora tanto las celdas, las líneas y las esquinas.
/// @param decorator El decorador a aplicar.
// NOLINTNEXTLINE
void TableSelection::Decorate(Decorator decorator) {
  for (int y = y_min_; y <= y_max_; ++y) {
    for (int x = x_min_; x <= x_max_; ++x) {
      Element& e = table_->elements_[y][x];
      e = std::move(e) | decorator;
    }
  }
}

/// @brief Aplica el `decorator` a la selección.
/// @param decorator El decorador a aplicar.
/// Esto decora solo las celdas.
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

/// @brief Aplica el `decorator` a la selección.
/// Esto decora solo las líneas módulo `modulo` con un desplazamiento de `shift`.
/// @param decorator El decorador a aplicar.
/// @param modulo El módulo de las líneas a decorar.
/// @param shift El desplazamiento de las líneas a decorar.
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

/// @brief Aplica el `decorator` a la selección.
/// Esto decora solo las líneas módulo `modulo` con un desplazamiento de `shift`.
/// @param decorator El decorador a aplicar.
/// @param modulo El módulo de las líneas a decorar.
/// @param shift El desplazamiento de las líneas a decorar.
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

/// @brief Aplica el `decorator` a la selección.
/// Esto decora solo las esquinas módulo `modulo` con un desplazamiento de `shift` por celdas.
/// @param decorator El decorador a aplicar.
/// @param modulo El módulo de las celdas a decorar.
/// @param shift El desplazamiento de las celdas a decorar.
// NOLINTNEXTLINE
void TableSelection::DecorateCellsAlternateColumn(Decorator decorator,
                                                  int modulo,
                                                  int shift) {
  for (int y = y_min_; y <= y_max_; ++y) {
    for (int x = x_min_; x <= x_max_; ++x) {
      if (y % 2 == 1 && x % 2 == 1 && ((x / 2) % modulo == shift)) {
        Element& e = table_->elements_[y][x];
        e = std::move(e) | decorator;
      }
    }
  }
}

/// @brief Aplica el `decorator` a la selección.
/// Esto decora solo las esquinas módulo `modulo` con un desplazamiento de `shift` por celdas.
/// @param decorator El decorador a aplicar.
/// @param modulo El módulo de las celdas a decorar.
/// @param shift El desplazamiento de las celdas a decorar.
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

/// @brief Aplica un `border` alrededor de la selección.
/// @param border El estilo de borde a aplicar.
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

/// @brief Dibuja algunas líneas separadoras en la selección.
/// @param border El estilo de borde a aplicar.
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

/// @brief Dibuja algunas líneas separadoras verticales en la selección.
/// @param border El estilo de borde a aplicar.
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

/// @brief Dibuja algunas líneas separadoras horizontales en la selección.
/// @param border El estilo de borde a aplicar.
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

/// @brief Dibuja algunas líneas separadoras en el lado izquierdo de la selección.
/// @param border El estilo de borde a aplicar.
void TableSelection::BorderLeft(BorderStyle border) {
  for (int y = y_min_; y <= y_max_; y++) {
    table_->elements_[y][x_min_] =
        separatorCharacter(charset[border][5]) | automerge;  // NOLINT
  }
}

/// @brief Dibuja algunas líneas separadoras en el lado derecho de la selección.
/// @param border El estilo de borde a aplicar.
void TableSelection::BorderRight(BorderStyle border) {
  for (int y = y_min_; y <= y_max_; y++) {
    table_->elements_[y][x_max_] =
        separatorCharacter(charset[border][5]) | automerge;  // NOLINT
  }
}

/// @brief Dibuja algunas líneas separadoras en la parte superior de la selección.
/// @param border El estilo de borde a aplicar.
void TableSelection::BorderTop(BorderStyle border) {
  for (int x = x_min_; x <= x_max_; x++) {
    table_->elements_[y_min_][x] =
        separatorCharacter(charset[border][4]) | automerge;  // NOLINT
  }
}

/// @brief Dibuja algunas líneas separadoras en la parte inferior de la selección.
/// @param border El estilo de borde a aplicar.
void TableSelection::BorderBottom(BorderStyle border) {
  for (int x = x_min_; x <= x_max_; x++) {
    table_->elements_[y_max_][x] =
        separatorCharacter(charset[border][4]) | automerge;  // NOLINT
  }
}

}  // namespace ftxui
