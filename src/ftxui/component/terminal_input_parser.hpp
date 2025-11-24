// Copyright 2020 Arthur Sonzogni. 無断複写・転載を禁じます。
// このソースコードは、LICENSEファイルにあるMITライセンスに従って使用されます。
// LICENSEファイルを参照してください。
#ifndef FTXUI_COMPONENT_TERMINAL_INPUT_PARSER
#define FTXUI_COMPONENT_TERMINAL_INPUT_PARSER

#include <functional>
#include <string>  // 文字列用
#include <vector>  // ベクター用

#include "ftxui/component/mouse.hpp"  // マウス用

namespace ftxui {
struct Event;

// 時間をかけて|char|のシーケンスを解析します。|Event|を生成します。
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

#endif /* インクルードガードの終わり: FTXUI_COMPONENT_TERMINAL_INPUT_PARSER */
