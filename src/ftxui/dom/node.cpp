// 版权所有 2020 Arthur Sonzogni. 保留所有权利。
// 本源代码的使用受 MIT 许可协议的约束，该协议可在
// LICENSE 文件中找到。
#include <ftxui/screen/box.hpp>  // for Box
#include <string>
#include <utility>  // for move

#include <cstddef>
#include "ftxui/dom/node.hpp"
#include "ftxui/dom/selection.hpp"  // for Selection
#include "ftxui/screen/screen.hpp"  // for Screen

namespace ftxui {

Node::Node() = default;
Node::Node(Elements children) : children_(std::move(children)) {}
Node::~Node() = default;

/// @brief 计算元素所需的空间。
void Node::ComputeRequirement() {
  if (children_.empty()) {
    return;
  }
  for (auto& child : children_) {
    child->ComputeRequirement();
  }

  // 默认情况下，需求是第一个子元素的需求。
  requirement_ = children_[0]->requirement();

  // 传播焦点需求。
  for (size_t i = 1; i < children_.size(); ++i) {
    if (!requirement_.focused.enabled &&
        children_[i]->requirement().focused.enabled) {
      requirement_.focused = children_[i]->requirement().focused;
    }
  }
}

/// @brief 为绘图元素分配位置和尺寸。
void Node::SetBox(Box box) {
  box_ = box;
}

/// @brief 计算元素的选区。
void Node::Select(Selection& selection) {
  // 如果此节点 box_ 不与选区相交，则不进行选区。
  if (Box::Intersection(selection.GetBox(), box_).IsEmpty()) {
    return;
  }

  // 默认情况下，我们将选择推迟到子元素。
  for (auto& child : children_) {
    child->Select(selection);
  }
}

/// @brief 在 ftxui::Screen 上显示元素。
void Node::Render(Screen& screen) {
  for (auto& child : children_) {
    child->Render(screen);
  }
}

void Node::Check(Status* status) {
  for (auto& child : children_) {
    child->Check(status);
  }
  status->need_iteration |= (status->iteration == 0);
}

std::string Node::GetSelectedContent(Selection& selection) {
  std::string content;

  for (auto& child : children_) {
    content += child->GetSelectedContent(selection);
  }

  return content;
}

/// @brief 在 ftxui::Screen 上显示元素。
/// @ingroup dom
void Render(Screen& screen, const Element& element) {
  Selection selection;
  Render(screen, element.get(), selection);
}

/// @brief 在 ftxui::Screen 上显示元素。
/// @ingroup dom
void Render(Screen& screen, Node* node) {
  Selection selection;
  Render(screen, node, selection);
}

void Render(Screen& screen, Node* node, Selection& selection) {
  Box box;
  box.x_min = 0;
  box.y_min = 0;
  box.x_max = screen.dimx() - 1;
  box.y_max = screen.dimy() - 1;

  Node::Status status;
  node->Check(&status);
  const int max_iterations = 20;
  while (status.need_iteration && status.iteration < max_iterations) {
    // 步骤 1：查找此元素所需的尺寸。
    node->ComputeRequirement();

    // 步骤 2：为元素分配尺寸。
    node->SetBox(box);

    // 检查元素是否需要布局算法的另一次迭代。
    status.need_iteration = false;
    status.iteration++;
    node->Check(&status);
  }

  // 步骤 3：选区
  if (!selection.IsEmpty()) {
    node->Select(selection);
  }

  if (node->requirement().focused.enabled
#if defined(FTXUI_MICROSOFT_TERMINAL_FALLBACK)
      // 将光标设置到正确的位置，可以让使用 CJK（中文、日文、韩文等）
      // 字符的用户在正确的位置看到他们的[输入法编辑器]显示。参见[问题]。
      //
      // [输入法编辑器]:
      // https://en.wikipedia.org/wiki/Input_method
      //
      // [问题]:
      // https://github.com/ArthurSonzogni/FTXUI/issues/2#issuecomment-505282355
      //
      // 不幸的是，Microsoft 终端无法正确处理隐藏光标。
      // 相反，光标下的字符被隐藏，这是一个大问题。
      // 因此，我们无法启用将光标设置到正确位置的功能。
      // 它将显示在右下角。
      // 参见：
      // https://github.com/microsoft/terminal/issues/1203
      // https://github.com/microsoft/terminal/issues/3093
      &&
      node->requirement().focused.cursor_shape != Screen::Cursor::Shape::Hidden
#endif
  ) {
    screen.SetCursor(Screen::Cursor{
        node->requirement().focused.node->box_.x_max,
        node->requirement().focused.node->box_.y_max,
        node->requirement().focused.cursor_shape,
    });
  } else {
    screen.SetCursor(Screen::Cursor{
        screen.dimx() - 1,
        screen.dimy() - 1,
        Screen::Cursor::Shape::Hidden,
    });
  }

  // 步骤 4：绘制元素。
  screen.stencil = box;
  node->Render(screen);

  // 步骤 5：应用着色器
  screen.ApplyShader();
}

std::string GetNodeSelectedContent(Screen& screen,
                                   Node* node,
                                   Selection& selection) {
  Box box;
  box.x_min = 0;
  box.y_min = 0;
  box.x_max = screen.dimx() - 1;
  box.y_max = screen.dimy() - 1;

  Node::Status status;
  node->Check(&status);
  const int max_iterations = 20;
  while (status.need_iteration && status.iteration < max_iterations) {
    // 步骤 1：查找此元素所需的尺寸。
    node->ComputeRequirement();

    // 步骤 2：为元素分配尺寸。
    node->SetBox(box);

    // 检查元素是否需要布局算法的另一次迭代。
    status.need_iteration = false;
    status.iteration++;
    node->Check(&status);
  }

  // 步骤 3：选区
  node->Select(selection);

  // 步骤 4：获取选定的内容。
  return node->GetSelectedContent(selection);
}

}  // namespace ftxui
