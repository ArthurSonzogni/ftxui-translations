// 版权所有 2020 Arthur Sonzogni. 保留所有权利。
// 本源代码的使用受 MIT 许可的约束，该许可可在以下文件中找到：
// LICENSE 文件。
#include <functional>  // for function
#include <sstream>     // for basic_istream, stringstream
#include <string>      // for string, allocator, getline
#include <utility>     // for move

#include "ftxui/dom/elements.hpp"  // for flexbox, Element, text, Elements, operator|, xflex, paragraph, paragraphAlignCenter, paragraphAlignJustify, paragraphAlignLeft, paragraphAlignRight
#include "ftxui/dom/flexbox_config.hpp"  // for FlexboxConfig, FlexboxConfig::JustifyContent, FlexboxConfig::JustifyConfig::Center, FlexboxConfig::JustifyConfig::FlexEnd, FlexboxConfig::JustifyConfig::SpaceBetween

namespace ftxui {

namespace {
Elements Split(const std::string& the_text) {
  Elements output;
  std::stringstream ss(the_text);
  std::string word;
  while (std::getline(ss, word, ' ')) {
    output.push_back(text(word));
  }
  return output;
}

Element Split(const std::string& paragraph,
              const std::function<Element(std::string)>& f) {
  Elements output;
  std::stringstream ss(paragraph);
  std::string line;
  while (std::getline(ss, line, '\n')) {
    output.push_back(f(line));
  }
  return vbox(std::move(output));
}

}  // namespace

/// @brief 返回一个在多行上绘制段落的元素。
/// @ingroup dom
/// @see flexbox.
Element paragraph(const std::string& the_text) {
  return paragraphAlignLeft(the_text);
}

/// @brief 返回一个在多行上绘制段落并左对齐的元素。
/// @ingroup dom
/// @see flexbox.
Element paragraphAlignLeft(const std::string& the_text) {
  return Split(the_text, [](const std::string& line) {
    static const auto config = FlexboxConfig().SetGap(1, 0);
    return flexbox(Split(line), config);
  });
};

/// @brief 返回一个在多行上绘制段落并右对齐的元素。
/// @ingroup dom
/// @see flexbox.
Element paragraphAlignRight(const std::string& the_text) {
  return Split(the_text, [](const std::string& line) {
    static const auto config = FlexboxConfig().SetGap(1, 0).Set(
        FlexboxConfig::JustifyContent::FlexEnd);
    return flexbox(Split(line), config);
  });
}

/// @brief 返回一个在多行上绘制段落并居中对齐的元素。
/// @ingroup dom
/// @see flexbox.
Element paragraphAlignCenter(const std::string& the_text) {
  return Split(the_text, [](const std::string& line) {
    static const auto config =
        FlexboxConfig().SetGap(1, 0).Set(FlexboxConfig::JustifyContent::Center);
    return flexbox(Split(line), config);
  });
}

/// @brief 返回一个在多行上绘制段落并使用两端对齐方式的元素。
/// 居中对齐。
/// @ingroup dom
/// @see flexbox.
Element paragraphAlignJustify(const std::string& the_text) {
  return Split(the_text, [](const std::string& line) {
    static const auto config = FlexboxConfig().SetGap(1, 0).Set(
        FlexboxConfig::JustifyContent::SpaceBetween);
    Elements words = Split(line);
    words.push_back(text("") | xflex);
    return flexbox(std::move(words), config);
  });
}

}  // namespace ftxui