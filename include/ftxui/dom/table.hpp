// Copyright 2021 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef FTXUI_DOM_TABLE
#define FTXUI_DOM_TABLE

#include <string>  // for string
#include <vector>  // for vector

#include "ftxui/dom/elements.hpp"  // for Element, BorderStyle, LIGHT, Decorator

namespace ftxui {

class Table;
class TableSelection;

/// @brief Table es una utilidad para dibujar tablas.
///
/// **ejemplo**
/// ```cpp
/// auto table = Table({
///  {"X", "Y"},
///  {"-1", "1"},
///  {"+0", "0"},
///  {"+1", "1"},
/// });
///
/// table.SelectAll().Border(LIGHT);
/// table.SelectRow(1).Border(DOUBLE);
/// table.SelectRow(1).SeparatorInternal(LIGHT);
///
/// std::move(table).Render();
/// ```
///
/// @ingroup dom
class Table {
 public:
  Table();
  explicit Table(std::vector<std::vector<std::string>>);
  explicit Table(std::vector<std::vector<Element>>);
  Table(std::initializer_list<std::vector<std::string>> init);
  TableSelection SelectAll();
  TableSelection SelectCell(int column, int row);
  TableSelection SelectRow(int row_index);
  TableSelection SelectRows(int row_min, int row_max);
  TableSelection SelectColumn(int column_index);
  TableSelection SelectColumns(int column_min, int column_max);
  TableSelection SelectRectangle(int column_min,
                                 int column_max,
                                 int row_min,
                                 int row_max);
  Element Render();

 private:
  void Initialize(std::vector<std::vector<Element>>);
  friend TableSelection;
  std::vector<std::vector<Element>> elements_;
  int input_dim_x_ = 0;
  int input_dim_y_ = 0;
  int dim_x_ = 0;
  int dim_y_ = 0;
};

class TableSelection {
 public:
  void Decorate(Decorator);
  void DecorateAlternateRow(Decorator, int modulo = 2, int shift = 0);
  void DecorateAlternateColumn(Decorator, int modulo = 2, int shift = 0);

  void DecorateCells(Decorator);
  void DecorateCellsAlternateColumn(Decorator, int modulo = 2, int shift = 0);
  void DecorateCellsAlternateRow(Decorator, int modulo = 2, int shift = 0);

  void Border(BorderStyle border = LIGHT);
  void BorderLeft(BorderStyle border = LIGHT);
  void BorderRight(BorderStyle border = LIGHT);
  void BorderTop(BorderStyle border = LIGHT);
  void BorderBottom(BorderStyle border = LIGHT);

  void Separator(BorderStyle border = LIGHT);
  void SeparatorVertical(BorderStyle border = LIGHT);
  void SeparatorHorizontal(BorderStyle border = LIGHT);

 private:
  friend Table;
  Table* table_;
  int x_min_;
  int x_max_;
  int y_min_;
  int y_max_;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_DOM_TABLE */