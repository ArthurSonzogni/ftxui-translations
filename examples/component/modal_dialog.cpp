// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <ftxui/component/component_options.hpp>  // for ButtonOption
#include <ftxui/component/mouse.hpp>              // for ftxui
#include <functional>                             // for function
#include <memory>                                 // for allocator, shared_ptr

#include "ftxui/component/component.hpp"  // for Button, operator|=, Renderer, Vertical, Modal
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive, Component
#include "ftxui/dom/elements.hpp"  // for operator|, separator, text, size, Element, vbox, border, GREATER_THAN, WIDTH, center, HEIGHT

using namespace ftxui;

auto button_style = ButtonOption::Animated();

// 主元件的定義。細節不重要。
Component MainComponent(std::function<void()> show_modal,
                        std::function<void()> exit) {
  auto component = Container::Vertical({
      Button("Show modal", show_modal, button_style),
      Button("Quit", exit, button_style),
  });
  // 優化兩個按鈕的渲染方式：
  component |= Renderer([&](Element inner) {
    return vbox({
               text("Main component"),
               separator(),
               inner,
           })                                //
           | size(WIDTH, GREATER_THAN, 15)   //
           | size(HEIGHT, GREATER_THAN, 15)  //
           | border                          //
           | center;                         //
  });
  return component;
}

// 模態元件的定義。細節不重要。
Component ModalComponent(std::function<void()> do_nothing,
                         std::function<void()> hide_modal) {
  auto component = Container::Vertical({
      Button("Do nothing", do_nothing, button_style),
      Button("Quit modal", hide_modal, button_style),
  });
  // 優化兩個按鈕的渲染方式：
  component |= Renderer([&](Element inner) {
    return vbox({
               text("Modal component "),
               separator(),
               inner,
           })                               //
           | size(WIDTH, GREATER_THAN, 30)  //
           | border;                        //
  });
  return component;
}

int main(int argc, const char* argv[]) {
  auto screen = ScreenInteractive::TerminalOutput();

  // 應用程式的狀態：
  bool modal_shown = false;

  // 一些修改狀態的動作：
  auto show_modal = [&] { modal_shown = true; };
  auto hide_modal = [&] { modal_shown = false; };
  auto exit = screen.ExitLoopClosure();
  auto do_nothing = [&] {};

  // 實例化主元件和模態元件：
  auto main_component = MainComponent(show_modal, exit);
  auto modal_component = ModalComponent(do_nothing, hide_modal);

  // 使用 `Modal` 函數將主元件及其模態視窗一起使用。
  // `modal_shown` 布林值控制模態是否顯示。
  main_component |= Modal(modal_component, &modal_shown);

  screen.Loop(main_component);
  return 0;
}