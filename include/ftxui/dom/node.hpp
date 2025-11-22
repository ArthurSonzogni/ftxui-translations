// 版权所有 2020 Arthur Sonzogni. 保留所有权利。
// 本源代码的使用受 MIT 许可证的约束，该许可证可在 LICENSE 文件中找到。
#ifndef FTXUI_DOM_NODE_HPP
#define FTXUI_DOM_NODE_HPP

#include <memory>  // for shared_ptr
#include <vector>  // for vector

#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/dom/selection.hpp"    // for Selection
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/screen.hpp"

namespace ftxui {

class Node;
class Screen;

using Element = std::shared_ptr<Node>;
using Elements = std::vector<Element>;

/// @brief Node 是 DOM 树中所有元素的基类。
///
/// 它代表文档对象模型 (DOM) 中的单个节点，并提供布局和渲染的基本结构。
/// 它包含用于计算布局要求、设置盒子尺寸、选择内容、渲染到屏幕以及检查布局状态的方法。
/// 它通常包含子元素，这些子元素也是 Node 的实例。
///
/// 用户应从该类派生以创建自定义元素。
///
/// 内置元素的列表可在 `elements.hpp` 文件中找到。
///
/// @ingroup dom
class Node {
 public:
  Node();
  explicit Node(Elements children);
  Node(const Node&) = delete;
  Node(const Node&&) = delete;
  Node& operator=(const Node&) = delete;
  Node& operator=(const Node&&) = delete;

  virtual ~Node();

  // 步骤 1: 计算布局要求。告诉父级此元素希望的尺寸。
  //         从子级传播到父级。
  virtual void ComputeRequirement();
  Requirement requirement() { return requirement_; }

  // 步骤 2: 为此元素分配最终尺寸。
  //         从父级传播到子级。
  virtual void SetBox(Box box);

  // 步骤 3: (可选) 选择
  //         从父级传播到子级。
  virtual void Select(Selection& selection);

  // 步骤 4: 绘制此元素。
  virtual void Render(Screen& screen);

  virtual std::string GetSelectedContent(Selection& selection);

  // 对于某些元素，布局可能无法在单次迭代中解决。这允许它们请求额外的迭代。
  // 此信号必须至少转发给子级一次。
  struct Status {
    int iteration = 0;
    bool need_iteration = false;
  };
  virtual void Check(Status* status);

  friend void Render(Screen& screen, Node* node, Selection& selection);

 protected:
  Elements children_;
  Requirement requirement_;
  Box box_;
};

void Render(Screen& screen, const Element& element);
void Render(Screen& screen, Node* node);
void Render(Screen& screen, Node* node, Selection& selection);
std::string GetNodeSelectedContent(Screen& screen,
                                   Node* node,
                                   Selection& selection);

}  // namespace ftxui

#endif  // FTXUI_DOM_NODE_HPP