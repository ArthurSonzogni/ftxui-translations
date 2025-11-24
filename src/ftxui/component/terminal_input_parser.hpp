// Copyright 2020 Arthur Sonzogni. 保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，詳情請參閱
// LICENSE 檔案。
#ifndef FTXUI_COMPONENT_TERMINAL_INPUT_PARSER
#define FTXUI_COMPONENT_TERMINAL_INPUT_PARSER

#include <functional>
#include <string>  // for string
#include <vector>  // for vector

#include "ftxui/component/mouse.hpp"  // for Mouse

namespace ftxui {
struct Event;

// 解析一段時間內的 |char| 序列。產生 |Event|。
class TerminalInputParser {
 public:
  explicit TerminalInputParser(std::function<void(Event)> out);
  void Timeout(int time);
  void Add(char c);

 private:
  unsigned char Current();
  bool Eat();

  enum Type {
    UNCOMPLETED,
    DROP,
    CHARACTER,
    MOUSE,
    CURSOR_POSITION,
    CURSOR_SHAPE,
    SPECIAL,
  };

  struct CursorPosition {
    int x;
    int y;
  };

  struct Output {
    Type type;
    union {
      Mouse mouse;
      CursorPosition cursor{};
      int cursor_shape;
    };

    Output(Type t)  // NOLINT
        : type(t) {}
  };

  void Send(Output output);
  Output Parse();
  Output ParseUTF8();
  Output ParseESC();
  Output ParseDCS();
  Output ParseCSI();
  Output ParseOSC();
  Output ParseMouse(bool altered, bool pressed, std::vector<int> arguments);
  Output ParseCursorPosition(std::vector<int> arguments);

  std::function<void(Event)> out_;
  int position_ = -1;
  int timeout_ = 0;
  std::string pending_;
};

}  // namespace ftxui

#endif /* end of include guard: FTXUI_COMPONENT_TERMINAL_INPUT_PARSER */