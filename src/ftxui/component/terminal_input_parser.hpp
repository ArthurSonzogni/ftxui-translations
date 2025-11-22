// 版权所有 2020 Arthur Sonzogni。保留所有权利。
// 本源代码的使用受 MIT 许可证的约束，该许可证可在
// LICENSE 文件中找到。
#ifndef FTXUI_COMPONENT_TERMINAL_INPUT_PARSER
#define FTXUI_COMPONENT_TERMINAL_INPUT_PARSER

#include <functional>
#include <string>  // for string
#include <vector>  // for vector

#include "ftxui/component/mouse.hpp"  // for Mouse

namespace ftxui {
struct Event;

// 解析一段时间内的字符序列。生成事件。
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
