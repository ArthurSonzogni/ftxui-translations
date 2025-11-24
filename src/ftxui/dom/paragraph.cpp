// 版權所有 2020 Arthur Sonzogni。保留所有權利。
// 本原始碼的使用受 MIT 授權條款約束，該條款可在 LICENSE 檔案中找到。
#include <functional>  // for function
#include <sstream>     // for basic_istream, stringstream
#include <string>      // for string, allocator, getline
#include <utility>     // for move

#include "ftxui/dom/elements.hpp"  // for flexbox, Element, text, Elements, operator|, xflex, paragraph, paragraphAlignCenter, paragraphAlignJustify, paragraphAlignLeft, paragraphAlignRight
#include "ftxui/dom/flexbox_config.hpp"  // for FlexboxConfig, FlexboxConfig::JustifyContent, FlexboxConfig::JustifyContent::Center, FlexboxConfig::JustifyContent::FlexEnd, FlexboxConfig::JustifyContent::SpaceBetween

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

/// @brief 回傳一個在多行上繪製段落的元素。
/// @ingroup dom
/// @see flexbox.
Element paragraph(const std::string& the_text) {
  return paragraphAlignLeft(the_text);
}

/// @brief 回傳一個在多行上繪製段落並靠左對齊的元素。
/// @ingroup dom
/// @see flexbox.
Element paragraphAlignLeft(const std::string& the_text) {
  return Split(the_text, [](const std::string& line) {
    static const auto config = FlexboxConfig().SetGap(1, 0);
    return flexbox(Split(line), config);
  });
};

/// @brief 回傳一個在多行上繪製段落並靠右對齊的元素。
/// @ingroup dom
/// @see flexbox.
Element paragraphAlignRight(const std::string& the_text) {
  return Split(the_text, [](const std::string& line) {
    static const auto config = FlexboxConfig().SetGap(1, 0).Set(
        FlexboxConfig::JustifyContent::FlexEnd);
    return flexbox(Split(line), config);
  });
}

/// @brief 回傳一個在多行上繪製段落並置中對齊的元素。
/// @ingroup dom
/// @see flexbox.
Element paragraphAlignCenter(const std::string& the_text) {
  return Split(the_text, [](const std::string& line) {
    static const auto config =
        FlexboxConfig().SetGap(1, 0).Set(FlexboxConfig::JustifyContent::Center);
    return flexbox(Split(line), config);
  });
}

/// @brief 回傳一個在多行上繪製段落並使用左右對齊的元素。
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
