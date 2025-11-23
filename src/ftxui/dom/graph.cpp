// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée
// dans le fichier LICENSE.
#include <functional>  // for function
#include <memory>      // for allocator, make_shared
#include <string>      // for string
#include <utility>     // for move
#include <vector>      // for vector

#include "ftxui/dom/elements.hpp"     // for GraphFunction, Element, graph
#include "ftxui/dom/node.hpp"         // for Node
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/screen.hpp"    // for Screen

namespace ftxui {

namespace {
// NOLINTNEXTLINE
static std::string charset[] =
#if defined(FTXUI_MICROSOFT_TERMINAL_FALLBACK)
    // Les terminaux Microsoft utilisent souvent des polices ne gérant pas les 8 caractères Unicode
    // pour représenter l'intégralité du graphique. Utiliser un jeu de caractères réduit.
    {" ", " ", "█", " ", "█", "█", "█", "█", "█"};
#else
    {" ", "▗", "▐", "▖", "▄", "▟", "▌", "▙", "█"};
#endif

class Graph : public Node {
 public:
  explicit Graph(GraphFunction graph_function)
      : graph_function_(std::move(graph_function)) {}

  void ComputeRequirement() override {
    requirement_.flex_grow_x = 1;
    requirement_.flex_grow_y = 1;
    requirement_.flex_shrink_x = 1;
    requirement_.flex_shrink_y = 1;
    requirement_.min_x = 3;
    requirement_.min_y = 3;
  }

  void Render(Screen& screen) override {
    const int width = (box_.x_max - box_.x_min + 1) * 2;
    const int height = (box_.y_max - box_.y_min + 1) * 2;
    if (width <= 0 || height <= 0) {
      return;
    }
    auto data = graph_function_(width, height);
    int i = 0;
    for (int x = box_.x_min; x <= box_.x_max; ++x) {
      const int height_1 = 2 * box_.y_max - data[i++];
      const int height_2 = 2 * box_.y_max - data[i++];
      for (int y = box_.y_min; y <= box_.y_max; ++y) {
        const int yy = 2 * y;
        int i_1 = yy < height_1 ? 0 : yy == height_1 ? 3 : 6;  // NOLINT
        int i_2 = yy < height_2 ? 0 : yy == height_2 ? 1 : 2;  // NOLINT
        screen.at(x, y) = charset[i_1 + i_2];                  // NOLINT
      }

    }

  }


 private:

  GraphFunction graph_function_;

};


}  // namespace


/// @brief Dessine un graphique en utilisant une GraphFunction.

/// @param graph_function la fonction à appeler pour obtenir les données.

Element graph(GraphFunction graph_function) {

  return std::make_shared<Graph>(std::move(graph_function));

}


}  // namespace ftxui