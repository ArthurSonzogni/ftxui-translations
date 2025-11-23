// Copyright 2024 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée
// dans le fichier LICENSE.

#include "ftxui/dom/selection.hpp"  // for Selection
#include <algorithm>                // for max, min
#include <string>                   // for string
#include <tuple>                    // for ignore

#include "ftxui/dom/node_decorator.hpp"  // for NodeDecorator

namespace ftxui {

namespace {
class Unselectable : public NodeDecorator {
 public:
  using NodeDecorator::NodeDecorator;

  void Select(Selection& ignored) override {
    std::ignore = ignored;
    // Écrase la méthode de sélection pour ne rien faire.
  }
};
}  // namespace

/// @brief Crée une sélection vide.
Selection::Selection() = default;

/// @brief Crée une sélection.
/// @param start_x La coordonnée x du début de la sélection.
/// @param start_y La coordonnée y du début de la sélection.
/// @param end_x La coordonnée x de la fin de la sélection.
/// @param end_y La coordonnée y de la fin de la sélection.
Selection::Selection(int start_x, int start_y, int end_x, int end_y)
    : start_x_(start_x),
      start_y_(start_y),
      end_x_(end_x),
      end_y_(end_y),
      box_{
          std::min(start_x, end_x),
          std::max(start_x, end_x),
          std::min(start_y, end_y),
          std::max(start_y, end_y),
      },
      empty_(false) {}

Selection::Selection(int start_x,
                     int start_y,
                     int end_x,
                     int end_y,
                     Selection* parent)
    : start_x_(start_x),
      start_y_(start_y),
      end_x_(end_x),
      end_y_(end_y),
      box_{
          std::min(start_x, end_x),
          std::max(start_x, end_x),
          std::min(start_y, end_y),
          std::max(start_y, end_y),
      },
      parent_(parent),
      empty_(false) {}

/// @brief Récupère la boîte de sélection.
/// @return La boîte de sélection.
const Box& Selection::GetBox() const {
  return box_;
}

/// @brief Sature la sélection pour qu'elle soit à l'intérieur de la boîte.
/// Ceci est appelé par `hbox` pour propager la sélection à ses enfants.
/// @param box La boîte dans laquelle saturer la sélection.
/// @return La sélection saturée.
Selection Selection::SaturateHorizontal(Box box) {
  int start_x = start_x_;
  int start_y = start_y_;
  int end_x = end_x_;
  int end_y = end_y_;

  const bool start_outside = !box.Contain(start_x, start_y);
  const bool end_outside = !box.Contain(end_x, end_y);
  const bool properly_ordered =
      start_y < end_y || (start_y == end_y && start_x <= end_x);
  if (properly_ordered) {
    if (start_outside) {
      start_x = box.x_min;
      start_y = box.y_min;
    }
    if (end_outside) {
      end_x = box.x_max;
      end_y = box.y_max;
    }
  } else {
    if (start_outside) {
      start_x = box.x_max;
      start_y = box.y_max;
    }
    if (end_outside) {
      end_x = box.x_min;
      end_y = box.y_min;
    }
  }
  return {
      start_x, start_y, end_x, end_y, parent_,
  };
}

/// @brief Sature la sélection pour qu'elle soit à l'intérieur de la boîte.
/// Ceci est appelé par `vbox` pour propager la sélection à ses enfants.
/// @param box La boîte dans laquelle saturer la sélection.
/// @return La sélection saturée.
Selection Selection::SaturateVertical(Box box) {
  int start_x = start_x_;
  int start_y = start_y_;
  int end_x = end_x_;
  int end_y = end_y_;

  const bool start_outside = !box.Contain(start_x, start_y);
  const bool end_outside = !box.Contain(end_x, end_y);
  const bool properly_ordered =
      start_y < end_y || (start_y == end_y && start_x <= end_x);

  if (properly_ordered) {
    if (start_outside) {
      start_x = box.x_min;
      start_y = box.y_min;
    }
    if (end_outside) {
      end_x = box.x_max;
      end_y = box.y_max;
    }
  } else {
    if (start_outside) {
      start_x = box.x_max;
      start_y = box.y_max;
    }
    if (end_outside) {
      end_x = box.x_min;
      end_y = box.y_min;
    }
  }
  return {start_x, start_y, end_x, end_y, parent_};
}

void Selection::AddPart(const std::string& part, int y, int left, int right) {
  if (parent_ != this) {
    parent_->AddPart(part, y, left, right);
    return;
  }
  [&] {
    if (parts_.str().empty()) {
      parts_ << part;
      return;
    }

    if (y_ != y) {
      parts_ << '\n' << part;
      return;
    }

    if (x_ == left + 1) {
      parts_ << part;
      return;
    }

    parts_ << part;
  }();
  y_ = y;
  x_ = right;
}

}  // namespace ftxui
