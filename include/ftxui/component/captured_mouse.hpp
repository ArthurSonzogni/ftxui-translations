// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
#ifndef FTXUI_CAPTURED_MOUSE_HPP
#define FTXUI_CAPTURED_MOUSE_HPP

#include <memory>

namespace ftxui {

class CapturedMouseInterface {
 public:
  CapturedMouseInterface() = default;
  CapturedMouseInterface(const CapturedMouseInterface&) = default;
  CapturedMouseInterface(CapturedMouseInterface&&) = delete;
  CapturedMouseInterface& operator=(const CapturedMouseInterface&) = default;
  CapturedMouseInterface& operator=(CapturedMouseInterface&&) = delete;
  virtual ~CapturedMouseInterface() = default;
};
using CapturedMouse = std::unique_ptr<CapturedMouseInterface>;
}  // namespace ftxui

#endif /* end of include guard: FTXUI_CAPTURED_MOUSE_HPP */
