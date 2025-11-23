// Droits d'auteur 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#include <algorithm>  // for max, min
#include <memory>     // for make_shared, __shared_ptr_access
#include <utility>    // for move

#include "ftxui/dom/elements.hpp"  // for Element, unpack, Elements, focus, frame, select, xframe, yframe
#include "ftxui/dom/node.hpp"         // for Node, Elements
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/screen.hpp"    // for Screen, Screen::Cursor
#include "ftxui/util/autoreset.hpp"   // for AutoReset

namespace ftxui {

namespace {
class Focus : public Node {
 public:
  explicit Focus(Elements children) : Node(std::move(children)) {}

  void ComputeRequirement() override {
    Node::ComputeRequirement();
    requirement_ = children_[0]->requirement();
    requirement_.focused.enabled = true;
    requirement_.focused.node = this;
    requirement_.focused.box.x_min = 0;
    requirement_.focused.box.y_min = 0;
    requirement_.focused.box.x_max = requirement_.min_x - 1;
    requirement_.focused.box.y_max = requirement_.min_y - 1;
  }

  void SetBox(Box box) override {
    Node::SetBox(box);
    children_[0]->SetBox(box);
  }
};

class Frame : public Node {
 public:
  Frame(Elements children, bool x_frame, bool y_frame)
      : Node(std::move(children)), x_frame_(x_frame), y_frame_(y_frame) {}

  void SetBox(Box box) override {
    Node::SetBox(box);
    auto& focused_box = requirement_.focused.box;
    Box children_box = box;

    if (x_frame_) {
      const int external_dimx = box.x_max - box.x_min;
      const int internal_dimx = std::max(requirement_.min_x, external_dimx);
      const int focused_dimx = focused_box.x_max - focused_box.x_min;
      int dx = focused_box.x_min - external_dimx / 2 + focused_dimx / 2;
      dx = std::max(0, std::min(internal_dimx - external_dimx - 1, dx));
      children_box.x_min = box.x_min - dx;
      children_box.x_max = box.x_min + internal_dimx - dx;
    }

    if (y_frame_) {
      const int external_dimy = box.y_max - box.y_min;
      const int internal_dimy = std::max(requirement_.min_y, external_dimy);
      const int focused_dimy = focused_box.y_max - focused_box.y_min;
      int dy = focused_box.y_min - external_dimy / 2 + focused_dimy / 2;
      dy = std::max(0, std::min(internal_dimy - external_dimy - 1, dy));
      children_box.y_min = box.y_min - dy;
      children_box.y_max = box.y_min + internal_dimy - dy;
    }

    children_[0]->SetBox(children_box);
  }

  void Render(Screen& screen) override {
    const AutoReset<Box> stencil(&screen.stencil,
                                 Box::Intersection(box_, screen.stencil));
    children_[0]->Render(screen);
  }

 private:
  bool x_frame_;
  bool y_frame_;
};

class FocusCursor : public Focus {
 public:
  FocusCursor(Elements children, Screen::Cursor::Shape shape)
      : Focus(std::move(children)), shape_(shape) {}

 private:
  void ComputeRequirement() override {
    Focus::ComputeRequirement();  // NOLINT
    requirement_.focused.cursor_shape = shape_;
  }
  Screen::Cursor::Shape shape_;
};

}  // namespace

/// @brief Définit l'élément `child` comme étant celui qui est focalisé parmi ses frères.
/// @param child L'élément à focaliser.
/// @ingroup dom
Element focus(Element child) {
  return std::make_shared<Focus>(unpack(std::move(child)));
}

/// Ceci est obsolète. Utilisez `focus` à la place.
/// @brief Définit l'élément `child` comme étant celui qui est focalisé parmi ses frères.
/// @param child L'élément à focaliser.
Element select(Element child) {
  return focus(std::move(child));
}

/// @brief Permet à un élément d'être affiché dans une zone 'virtuelle'. Sa taille peut
/// être plus grande que son conteneur. Dans ce cas, seule une plus petite partie est
/// affichée. La vue est défilable pour rendre l'élément focalisé visible.
/// @see frame
/// @see xframe
/// @see yframe
Element frame(Element child) {
  return std::make_shared<Frame>(unpack(std::move(child)), true, true);
}

/// @brief Identique à `frame`, mais seulement sur l'axe x.
/// @see frame
/// @see xframe
/// @see yframe
Element xframe(Element child) {
  return std::make_shared<Frame>(unpack(std::move(child)), true, false);
}

/// @brief Identique à `frame`, mais seulement sur l'axe y.
/// @see frame
/// @see xframe
/// @see yframe
Element yframe(Element child) {
  return std::make_shared<Frame>(unpack(std::move(child)), false, true);
}

/// @brief Identique à `focus`, mais définit la forme du curseur comme un bloc fixe.
/// @see focus
/// @see focusCursorBlock
/// @see focusCursorBlockBlinking
/// @see focusCursorBar
/// @see focusCursorBarBlinking
/// @see focusCursorUnderline
/// @see focusCursorUnderlineBlinking
/// @ingroup dom
Element focusCursorBlock(Element child) {
  return std::make_shared<FocusCursor>(unpack(std::move(child)),
                                       Screen::Cursor::Block);
}

/// @brief Identique à `focus`, mais définit la forme du curseur comme un bloc clignotant.
/// @see focus
/// @see focusCursorBlock
/// @see focusCursorBlockBlinking
/// @see focusCursorBar
/// @see focusCursorBarBlinking
/// @see focusCursorUnderline
/// @see focusCursorUnderlineBlinking
/// @ingroup dom
Element focusCursorBlockBlinking(Element child) {
  return std::make_shared<FocusCursor>(unpack(std::move(child)),
                                       Screen::Cursor::BlockBlinking);
}

/// @brief Identique à `focus`, mais définit la forme du curseur comme un bloc fixe.
/// @see focus
/// @see focusCursorBlock
/// @see focusCursorBlockBlinking
/// @see focusCursorBar
/// @see focusCursorBarBlinking
/// @see focusCursorUnderline
/// @see focusCursorUnderlineBlinking
/// @ingroup dom
Element focusCursorBar(Element child) {
  return std::make_shared<FocusCursor>(unpack(std::move(child)),
                                       Screen::Cursor::Bar);
}

/// @brief Identique à `focus`, mais définit la forme du curseur comme une barre clignotante.
/// @see focus
/// @see focusCursorBlock
/// @see focusCursorBlockBlinking
/// @see focusCursorBar
/// @see focusCursorBarBlinking
/// @see focusCursorUnderline
/// @see focusCursorUnderlineBlinking
/// @ingroup dom
Element focusCursorBarBlinking(Element child) {
  return std::make_shared<FocusCursor>(unpack(std::move(child)),
                                       Screen::Cursor::BarBlinking);
}

/// @brief Identique à `focus`, mais définit la forme du curseur comme un soulignement fixe.
/// @see focus
/// @see focusCursorBlock
/// @see focusCursorBlockBlinking
/// @see focusCursorBar
/// @see focusCursorBarBlinking
/// @see focusCursorUnderline
/// @see focusCursorUnderlineBlinking
/// @ingroup dom
Element focusCursorUnderline(Element child) {
  return std::make_shared<FocusCursor>(unpack(std::move(child)),
                                       Screen::Cursor::Underline);
}

/// @brief Identique à `focus`, mais définit la forme du curseur comme un soulignement clignotant.
/// @see focus
/// @see focusCursorBlock
/// @see focusCursorBlockBlinking
/// @see focusCursorBar
/// @see focusCursorBarBlinking
/// @see focusCursorUnderline
/// @see focusCursorUnderlineBlinking
/// @ingroup dom
Element focusCursorUnderlineBlinking(Element child) {
  return std::make_shared<FocusCursor>(unpack(std::move(child)),
                                       Screen::Cursor::UnderlineBlinking);
}

}  // namespace ftxui
