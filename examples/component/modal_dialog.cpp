// Copyright 2022 Arthur Sonzogni. All rights reserved.
// このソースコードの使用は、LICENSEファイルにあるMITライセンスに従います。
#include <ftxui/component/component_options.hpp>  // for ButtonOption
#include <ftxui/component/mouse.hpp>              // for ftxui
#include <functional>                             // for function
#include <memory>                                 // for allocator, shared_ptr

#include "ftxui/component/component.hpp"  // for Button, operator|=, Renderer, Vertical, Modal
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive, Component
#include "ftxui/dom/elements.hpp"  // for operator|, separator, text, size, Element, vbox, border, GREATER_THAN, WIDTH, center, HEIGHT

using namespace ftxui;

auto button_style = ButtonOption::Animated();

// メインコンポーネントの定義。詳細は重要ではありません。
Component MainComponent(std::function<void()> show_modal,
                        std::function<void()> exit) {
  auto component = Container::Vertical({
      Button("Show modal", show_modal, button_style),
      Button("Quit", exit, button_style),
  });
  // 2つのボタンの描画を調整します:
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

// モーダルコンポーネントの定義。詳細は重要ではありません。
Component ModalComponent(std::function<void()> do_nothing,
                         std::function<void()> hide_modal) {
  auto component = Container::Vertical({
      Button("Do nothing", do_nothing, button_style),
      Button("Quit modal", hide_modal, button_style),
  });
  // 2つのボタンの描画を調整します:
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

  // アプリケーションの状態:
  bool modal_shown = false;

  // 状態を変更するいくつかのアクション:
  auto show_modal = [&] { modal_shown = true; };
  auto hide_modal = [&] { modal_shown = false; };
  auto exit = screen.ExitLoopClosure();
  auto do_nothing = [&] {};

  // メインコンポーネントとモーダルコンポーネントをインスタンス化します:
  auto main_component = MainComponent(show_modal, exit);
  auto modal_component = ModalComponent(do_nothing, hide_modal);

  // `Modal` 関数を使用して、メインコンポーネントとそのモーダルウィンドウを一緒に使用します。
  // |modal_shown| ブール値は、モーダルが表示されているかどうかを制御します。
  main_component |= Modal(modal_component, &modal_shown);

  screen.Loop(main_component);
  return 0;
}
