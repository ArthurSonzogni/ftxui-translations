// 版權所有 2020 Arthur Sonzogni。保留所有權利。
// 本原始碼受 MIT 授權條款約束，詳情請參閱
// LICENSE 檔案。
                 // 用於 getchar
                   // 用於 cos
  // 用於 Fit, canvas, operator|, border, Element
  // 用於 Pixel, Screen
                   // 用於 vector, allocator

#include "ftxui/dom/canvas.hpp"  // 用於 Canvas
#include "ftxui/dom/node.hpp"    // 用於 Render
#include "ftxui/screen/color.hpp"  // 用於 Color, Color::Red, Color::Blue, Color::Green, ftxui

int main() {
  using namespace ftxui;

  auto c = Canvas(100, 100);

  c.DrawText(0, 0, "This is a canvas", [](Pixel& p) {
    p.foreground_color = Color::Red;
    p.underlined = true;
  });

  // 三角形：
  c.DrawPointLine(10, 10, 80, 10, Color::Red);
  c.DrawPointLine(80, 10, 80, 40, Color::Blue);
  c.DrawPointLine(80, 40, 10, 10, Color::Green);

  // 圓形，未填充和已填充：
  c.DrawPointCircle(30, 50, 20);
  c.DrawPointCircleFilled(40, 40, 10);

  // 繪製函數：
  std::vector<int> ys(100);
  for (int x = 0; x < 100; x++) {
    ys[x] = int(80 + 20 * cos(x * 0.2));
  }
  for (int x = 0; x < 99; x++) {
    c.DrawPointLine(x, ys[x], x + 1, ys[x + 1], Color::Red);
  }

  auto document = canvas(&c) | border;

  auto screen = Screen::Create(Dimension::Fit(document));
  Render(screen, document);
  screen.Print();
  getchar();

  return 0;
}
