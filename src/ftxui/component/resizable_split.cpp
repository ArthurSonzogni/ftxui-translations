// Copyright 2021 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、MITライセンスに従います。
// LICENSEファイルに記載されています。
#include <algorithm>                              // max用
#include <ftxui/component/component_options.hpp>  // ResizableSplitOption用
#include <ftxui/dom/direction.hpp>  // Direction、Direction::Down、Direction::Left、Direction::Right、Direction::Up用
#include <ftxui/util/ref.hpp>       // Ref用
#include <functional>               // function用
#include <utility>                  // move用

#include "ftxui/component/captured_mouse.hpp"  // CapturedMouse用
#include "ftxui/component/component.hpp"  // Horizontal、Make、ResizableSplit、ResizableSplitBottom、ResizableSplitLeft、ResizableSplitRight、ResizableSplitTop用
#include "ftxui/component/component_base.hpp"  // Component、ComponentBase用
#include "ftxui/component/event.hpp"           // Event用
#include "ftxui/component/mouse.hpp"  // Mouse、Mouse::Left、Mouse::Pressed、Mouse::Released用
#include "ftxui/dom/elements.hpp"  // operator|、reflect、Element、size、EQUAL、xflex、yflex、hbox、vbox、HEIGHT、WIDTH、text用
#include "ftxui/screen/box.hpp"    // Box用

namespace ftxui {
namespace {

class ResizableSplitBase : public ComponentBase, public ResizableSplitOption {
 public:
  explicit ResizableSplitBase(ResizableSplitOption options)
      : ResizableSplitOption(std::move(options)) {
    switch (direction()) {
      case Direction::Left:
        Add(Container::Horizontal({main, back}));
        break;
      case Direction::Right:
        Add(Container::Horizontal({back, main}));
        break;
      case Direction::Up:
        Add(Container::Vertical({main, back}));
        break;
      case Direction::Down:
        Add(Container::Vertical({back, main}));
        break;
    }
  }

  bool OnEvent(Event event) final {
    if (event.is_mouse()) {
      return OnMouseEvent(std::move(event));
    }
    return ComponentBase::OnEvent(std::move(event));
  }

  bool OnMouseEvent(Event event) {
    if (captured_mouse_ && event.mouse().motion == Mouse::Released) {
      captured_mouse_.reset();
      return true;
    }

    if (event.mouse().button == Mouse::Left &&
        event.mouse().motion == Mouse::Pressed &&
        separator_box_.Contain(event.mouse().x, event.mouse().y) &&
        !captured_mouse_) {
      captured_mouse_ = CaptureMouse(event);
      return true;
    }

    if (!captured_mouse_) {
      return ComponentBase::OnEvent(event);
    }

    switch (direction()) {
      case Direction::Left:
        main_size() = std::max(0, event.mouse().x - box_.x_min);
        break;
      case Direction::Right:
        main_size() = std::max(0, box_.x_max - event.mouse().x);
        break;
      case Direction::Up:
        main_size() = std::max(0, event.mouse().y - box_.y_min);
        break;
      case Direction::Down:
        main_size() = std::max(0, box_.y_max - event.mouse().y);
        break;
    }

    main_size() = std::clamp(main_size(), min(), max());
    return true;
  }

  Element OnRender() final {
    switch (direction()) {
      case Direction::Left:
        return RenderLeft();
      case Direction::Right:
        return RenderRight();
      case Direction::Up:
        return RenderTop();
      case Direction::Down:
        return RenderBottom();
    }
    // 到達不能
    return text("unreacheable");
  }

  Element RenderLeft() {
    return hbox({
               main->Render() | size(WIDTH, EQUAL, main_size()),
               separator_func() | reflect(separator_box_),
               back->Render() | xflex,
           }) |
           reflect(box_);
  }

  Element RenderRight() {
    return hbox({
               back->Render() | xflex,
               separator_func() | reflect(separator_box_),
               main->Render() | size(WIDTH, EQUAL, main_size()),
           }) |
           reflect(box_);
  }

  Element RenderTop() {
    return vbox({
               main->Render() | size(HEIGHT, EQUAL, main_size()),
               separator_func() | reflect(separator_box_),
               back->Render() | yflex,
           }) |
           reflect(box_);
  }

  Element RenderBottom() {
    return vbox({
               back->Render() | yflex,
               separator_func() | reflect(separator_box_),
               main->Render() | size(HEIGHT, EQUAL, main_size()),
           }) |
           reflect(box_);
  }

 private:
  CapturedMouse captured_mouse_;
  Box separator_box_;
  Box box_;
};

}  // namespace

/// @brief 2つのコンポーネント間の分割。
/// @param options すべてのパラメータ。
///
/// ### 例
///
/// ```cpp
/// auto left = Renderer([] { return text("Left") | center;});
/// auto right = Renderer([] { return text("right") | center;});
/// int left_size = 10;
/// auto component = ResizableSplit({
///   .main = left,
///   .back = right,
///   .direction = Direction::Left,
///   .main_size = &left_size,
///   .separator_func = [] { return separatorDouble(); },
/// });
/// ```
///
/// ### 出力
///
/// ```bash
///           ║
///    left   ║   right
///           ║
/// ```
Component ResizableSplit(ResizableSplitOption options) {
  return Make<ResizableSplitBase>(std::move(options));
}

/// @brief 2つのコンポーネント間の水平分割。マウスで設定可能。
/// @param main 左側の|main_size|サイズのメインコンポーネント。
/// @param back 右側に残りのサイズを占めるバックコンポーネント。
/// @param main_size |main|コンポーネントのサイズ。
/// @ingroup component
///
/// ### 例
///
/// ```cpp
/// auto screen = ScreenInteractive::Fullscreen();
/// int left_size = 10;
/// auto left = Renderer([] { return text("Left") | center;});
/// auto right = Renderer([] { return text("right") | center;});
/// auto split = ResizableSplitLeft(left, right, &left_size);
/// screen.Loop(split);
/// ```
///
/// ### 出力
///
/// ```bash
///           │
///    left   │   right
///           │
/// ```
Component ResizableSplitLeft(Component main, Component back, int* main_size) {
  return ResizableSplit({
      std::move(main),
      std::move(back),
      Direction::Left,
      main_size,
  });
}

/// @brief 2つのコンポーネント間の水平分割。マウスで設定可能。
/// @param main 右側の|main_size|サイズのメインコンポーネント。
/// @param back 左側に残りのサイズを占めるバックコンポーネント。
/// @param main_size |main|コンポーネントのサイズ。
/// @ingroup component
///
/// ### 例
///
/// ```cpp
/// auto screen = ScreenInteractive::Fullscreen();
/// int right_size = 10;
/// auto left = Renderer([] { return text("Left") | center;});
/// auto right = Renderer([] { return text("right") | center;});
/// auto split = ResizableSplitRight(right, left, &right_size)
/// screen.Loop(split);
/// ```
///
/// ### 出力
///
/// ```bash
///           │
///    left   │   right
///           │
/// ```
Component ResizableSplitRight(Component main, Component back, int* main_size) {
  return ResizableSplit({
      std::move(main),
      std::move(back),
      Direction::Right,
      main_size,
  });
}

/// @brief 2つのコンポーネント間の垂直分割。マウスで設定可能。
/// @param main 上部の|main_size|サイズのメインコンポーネント。
/// @param back 下部に残りのサイズを占めるバックコンポーネント。
/// @param main_size |main|コンポーネントのサイズ。
/// @ingroup component
///
/// ### 例
///
/// ```cpp
/// auto screen = ScreenInteractive::Fullscreen();
/// int top_size = 1;
/// auto top = Renderer([] { return text("Top") | center;});
/// auto bottom = Renderer([] { return text("Bottom") | center;});
/// auto split = ResizableSplitTop(top, bottom, &top_size)
/// screen.Loop(split);
/// ```
///
/// ### 出力
///
/// ```bash
///    top
/// ────────────
///    bottom
/// ```
Component ResizableSplitTop(Component main, Component back, int* main_size) {
  return ResizableSplit({
      std::move(main),
      std::move(back),
      Direction::Up,
      main_size,
  });
}

/// @brief 2つのコンポーネント間の垂直分割。マウスで設定可能。
/// @param main 下部の|main_size|サイズのメインコンポーネント。
/// @param back 上部に残りのサイズを占めるバックコンポーネント。
/// @param main_size |main|コンポーネントのサイズ。
/// @ingroup component
///
/// ### 例
///
/// ```cpp
/// auto screen = ScreenInteractive::Fullscreen();
/// int bottom_size = 1;
/// auto top = Renderer([] { return text("Top") | center;});
/// auto bottom = Renderer([] { return text("Bottom") | center;});
/// auto split = ResizableSplit::Bottom(bottom, top, &bottom_size)
/// screen.Loop(split);
/// ```
///
/// ### 出力
///
/// ```bash
///    top
/// ────────────
///    bottom
/// ```
Component ResizableSplitBottom(Component main, Component back, int* main_size) {
  return ResizableSplit({
      std::move(main),
      std::move(back),
      Direction::Down,
      main_size,
  });
}

}  // namespace ftxui
