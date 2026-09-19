// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
//
// Content of this file was created thanks to:
// -
// https://www.unicode.org/Public/UCD/latest/ucd/auxiliary/WordBreakProperty.txt
// - Markus Kuhn -- 2007-05-26 (Unicode 5.0)
//   http://www.cl.cam.ac.uk/~mgk25/ucs/wcwidth.c
// Thanks you!

#include "ftxui/screen/string.hpp"

#include <array>        // for array
#include <cstddef>      // for size_t
#include <cstdint>      // for uint32_t, uint8_t, uint16_t, int32_t
#include <string>       // for string, basic_string, wstring
#include <string_view>  // for string_view
#include <tuple>        // for _Swallow_assign, ignore
#include <vector>

#include "ftxui/screen/deprecated.hpp"       // for wchar_width, wstring_width
#include "ftxui/screen/string_internal.hpp"  // for WordBreakProperty, EatCodePoint, CodepointToWordBreakProperty, GlyphCount, GlyphIterate, GlyphNext, GlyphPrevious, IsCombining, IsControl, IsFullWidth, Utf8ToWordBreakProperty

namespace {

struct Interval {
  uint32_t first;
  uint32_t last;
};

using WBP = ftxui::WordBreakProperty;
struct WordBreakPropertyInterval {
  uint32_t first;
  uint32_t last;
  WBP property;
};

// g_full_width_characters y g_word_break_intervals, generados a partir de la
// Base de Datos de Caracteres Unicode por tools/gen_unicode_tables.py.
#include "ftxui/screen/string_unicode_tables.ipp"

// Construir tabla de solo los intervalos de caracteres WBP::Extend
constexpr auto g_extend_characters{[]() constexpr {
  // Calcular número de intervalos de caracteres extend
  constexpr size_t size = []() constexpr {
    size_t count = 0;
    for (auto interval : g_word_break_intervals) {
      if (interval.property == WBP::Extend) {
        count++;
      }
    }
    return count;
  }();

  // Crear array de intervalos de caracteres extend
  std::array<Interval, size> result{};
  size_t index = 0;
  for (auto interval : g_word_break_intervals) {
    if (interval.property == WBP::Extend) {
      result[index++] = {interval.first, interval.last};  // NOLINT
    }
  }
  return result;
}()};

// Encontrar un punto de código dentro de una lista ordenada de Interval.
template <size_t N>
bool Bisearch(uint32_t ucs, const std::array<Interval, N>& table) {
  if (ucs < table.front().first || ucs > table.back().last) {  // NOLINT
    return false;
  }

  int min = 0;
  int max = N - 1;
  while (max >= min) {
    const int mid = (min + max) / 2;
    if (ucs > table[mid].last) {  // NOLINT
      min = mid + 1;
    } else if (ucs < table[mid].first) {  // NOLINT
      max = mid - 1;
    } else {
      return true;
    }
  }

  return false;
}

// Encontrar un valor dentro de una lista ordenada de Interval + propiedad.
template <class C, size_t N>
bool Bisearch(uint32_t ucs, const std::array<C, N>& table, C* out) {
  if (ucs < table.front().first || ucs > table.back().last) {  // NOLINT
    return false;
  }

  int min = 0;
  int max = N - 1;
  while (max >= min) {
    const int mid = (min + max) / 2;
    if (ucs > table[mid].last) {  // NOLINT
      min = mid + 1;
    } else if (ucs < table[mid].first) {  // NOLINT
      max = mid - 1;
    } else {
      *out = table[mid];  // NOLINT
      return true;
    }
  }

  return false;
}

int codepoint_width(uint32_t ucs) {
  if (ftxui::IsControl(ucs)) {
    return -1;
  }

  if (ftxui::IsCombining(ucs)) {
    return 0;
  }

  if (ftxui::IsFullWidth(ucs)) {
    return 2;
  }

  return 1;
}

}  // namespace

namespace ftxui {

/// From UTF8 encoded string |input|, eat in between 1 and 4 byte representing
/// one codepoint. Put the codepoint into |ucs|. Start at |start| and update
/// |end| to represent the beginning of the next byte to eat for consecutive
/// executions.
// A partir de la cadena codificada en UTF8 |input|, lee entre 1 y 4 bytes que representan
// un punto de código. Coloca el punto de código en |ucs|. Comienza en |start| y actualiza
// |end| para representar el comienzo del siguiente byte a leer para ejecuciones
// consecutivas.
bool EatCodePoint(std::string_view input,
                  size_t start,
                  size_t* end,
                  uint32_t* ucs) {
  if (start >= input.size()) {
    *end = start + 1;
    return false;
  }
  const uint8_t C0 = input[start];

  // Cadena de 1 byte.
  if ((C0 & 0b1000'0000) == 0b0000'0000) {  // NOLINT
    *ucs = C0 & 0b0111'1111;                // NOLINT
    *end = start + 1;
    return true;
  }

  // Cadena de 2 bytes.
  if ((C0 & 0b1110'0000) == 0b1100'0000 &&  // NOLINT
      start + 1 < input.size()) {
    const uint8_t C1 = input[start + 1];
    *ucs = 0;
    *ucs += C0 & 0b0001'1111;  // NOLINT
    *ucs <<= 6;                // NOLINT
    *ucs += C1 & 0b0011'1111;  // NOLINT
    *end = start + 2;
    return true;
  }

  // Cadena de 3 bytes.
  if ((C0 & 0b1111'0000) == 0b1110'0000 &&  // NOLINT
      start + 2 < input.size()) {
    const uint8_t C1 = input[start + 1];
    const uint8_t C2 = input[start + 2];
    *ucs = 0;
    *ucs += C0 & 0b0000'1111;  // NOLINT
    *ucs <<= 6;                // NOLINT
    *ucs += C1 & 0b0011'1111;  // NOLINT
    *ucs <<= 6;                // NOLINT
    *ucs += C2 & 0b0011'1111;  // NOLINT
    *end = start + 3;
    return true;
  }

  // Cadena de 4 bytes.
  if ((C0 & 0b1111'1000) == 0b1111'0000 &&  // NOLINT
      start + 3 < input.size()) {
    const uint8_t C1 = input[start + 1];
    const uint8_t C2 = input[start + 2];
    const uint8_t C3 = input[start + 3];
    *ucs = 0;
    *ucs += C0 & 0b0000'0111;  // NOLINT
    *ucs <<= 6;                // NOLINT
    *ucs += C1 & 0b0011'1111;  // NOLINT
    *ucs <<= 6;                // NOLINT
    *ucs += C2 & 0b0011'1111;  // NOLINT
    *ucs <<= 6;                // NOLINT
    *ucs += C3 & 0b0011'1111;  // NOLINT
    *end = start + 4;
    return true;
  }

  *end = start + 1;
  return false;
}

/// From UTF16 encoded string |input|, eat in between 1 and 4 byte representing
/// one codepoint. Put the codepoint into |ucs|. Start at |start| and update
/// |end| to represent the beginning of the next byte to eat for consecutive
/// executions.
// A partir de la cadena codificada en UTF16 |input|, lee entre 1 y 4 bytes que representan
// un punto de código. Coloca el punto de código en |ucs|. Comienza en |start| y actualiza
// |end| para representar el comienzo del siguiente byte a leer para ejecuciones
// consecutivas.
bool EatCodePoint(std::wstring_view input,
                  size_t start,
                  size_t* end,
                  uint32_t* ucs) {
  if (start >= input.size()) {
    *end = start + 1;
    return false;
  }

  // En linux wstring usa la codificación UTF32:
  if constexpr (sizeof(wchar_t) == 4) {
    *ucs = input[start];  // NOLINT
    *end = start + 1;
    return true;
  }

  // En windows, wstring usa la codificación UTF16:
  int32_t C0 = input[start];  // NOLINT

  // tamaño de palabra 1:
  if (C0 < 0xd800 || C0 >= 0xdc00) {  // NOLINT
    *ucs = C0;
    *end = start + 1;
    return true;
  }

  // tamaño de palabra 2:
  if (start + 1 >= input.size()) {
    *end = start + 2;
    return false;
  }

  int32_t C1 = input[start + 1];                         // NOLINT
  *ucs = ((C0 & 0x3ff) << 10) + (C1 & 0x3ff) + 0x10000;  // NOLINT
  *end = start + 2;
  return true;
}

bool IsCombining(uint32_t ucs) {
  return Bisearch(ucs, g_extend_characters);
}

bool IsFullWidth(uint32_t ucs) {
  if (ucs < 0x0300) {  // Ruta rápida: // NOLINT
    return false;
  }

  return Bisearch(ucs, g_full_width_characters);
}

bool IsControl(uint32_t ucs) {
  if (ucs == 0) {
    return true;
  }
  if (ucs < 32) {  // NOLINT
    const uint32_t LINE_FEED = 10;
    return ucs != LINE_FEED;
  }
  if (ucs >= 0x7f && ucs < 0xa0) {  // NOLINT
    return true;
  }
  return false;
}

WordBreakProperty CodepointToWordBreakProperty(uint32_t codepoint) {
  WordBreakPropertyInterval interval = {0, 0, WBP::ALetter};
  std::ignore = Bisearch(codepoint, g_word_break_intervals, &interval);
  return interval.property;
}

int wchar_width(wchar_t ucs) {
  return codepoint_width(uint32_t(ucs));
}

int wstring_width(const std::wstring& text) {
  int width = 0;

  for (const wchar_t& it : text) {
    const int w = wchar_width(it);
    if (w < 0) {
      return -1;
    }
    width += w;
  }
  return width;
}

// Devuelve cuántas celdas ocupa la cadena codificada en UTF8 |input| al imprimirse.
// Los caracteres de control no ocupan espacio, los caracteres combinantes
// modifican el carácter anterior y no ocupan espacio, los caracteres de
// ancho completo ocupan dos celdas y todos los demás caracteres ocupan una
// celda.
int string_width(std::string_view input) {
  // Optimización de 1 byte: Esta función se llama a menudo con un único carácter
  // ASCII, así que podemos optimizar este caso saltando la decodificación UTF8.
  if (input.size() == 1) {
    const char c = input[0];
    if (c >= 32 && c < 127) {  // NOLINT
      return 1;
    }
  }

  // Optimización ASCII: Si la cadena es ASCII puro, podemos saltar la decodificación
  // UTF8 y simplemente contar el número de caracteres, ignorando los
  // caracteres de control.
  bool is_pure_ascii = true;
  for (const char c : input) {
    if (c < 31 || c >= 127) {  // NOLINT
      is_pure_ascii = false;
      break;
    }
  }
  if (is_pure_ascii) {
    return static_cast<int>(input.size());
  }

  int width = 0;
  size_t start = 0;
  while (start < input.size()) {
    uint32_t codepoint = 0;
    if (!EatCodePoint(input, start, &start, &codepoint)) {
      continue;
    }

    if (IsControl(codepoint)) {
      continue;
    }

    if (IsCombining(codepoint)) {
      continue;
    }

    if (IsFullWidth(codepoint)) {
      width += 2;
      continue;
    }

    width += 1;
  }
  return width;
}

std::vector<std::string> Utf8ToGlyphs(std::string_view input) {
  std::vector<std::string> out;
  out.reserve(input.size());
  size_t start = 0;
  size_t end = 0;
  while (start < input.size()) {
    uint32_t codepoint = 0;
    if (!EatCodePoint(input, start, &end, &codepoint)) {
      start = end;
      continue;
    }

    const auto append = input.substr(start, end - start);
    start = end;

    // Combining characters are put with the previous glyph they are modifying.
    // Los caracteres combinatorios se colocan con el glifo anterior que están modificando.
    if (IsControl(codepoint)) {
      continue;
    }

    // Los caracteres combinatorios se colocan con el glifo anterior que están modificando.
    if (IsCombining(codepoint)) {
      if (!out.empty()) {
        out.back() += append;
      }
      continue;
    }

    // Fullwidth characters take two cells. The second is made of the empty
    // string to reserve the space the first is taking.
    // Los caracteres de ancho completo ocupan dos celdas. La segunda se compone de una
    // cadena vacía para reservar el espacio que ocupa la primera.
    if (IsFullWidth(codepoint)) {
      out.emplace_back(append);
      out.emplace_back("");
      continue;
    }

    // Caracteres normales:
    out.emplace_back(append);
  }
  return out;
}

size_t GlyphPrevious(std::string_view input, size_t start) {
  while (true) {
    if (start == 0) {
      return 0;
    }
    start--;

    // Skip the UTF8 continuation bytes.
    // Omite los bytes de continuación UTF8.
    if ((input[start] & 0b1100'0000) == 0b1000'0000) {
      continue;
    }

    uint32_t codepoint = 0;
    size_t end = 0;
    const bool eaten = EatCodePoint(input, start, &end, &codepoint);

    // Ignora los caracteres inválidos, de control y combinatorios.
    if (!eaten || IsControl(codepoint) || IsCombining(codepoint)) {
      continue;
    }

    return start;
  }
}

size_t GlyphNext(std::string_view input, size_t start) {
  bool glyph_found = false;
  while (start < input.size()) {
    size_t end = 0;
    uint32_t codepoint = 0;
    const bool eaten = EatCodePoint(input, start, &end, &codepoint);

    // Ignora los caracteres inválidos, de control y combinatorios.
    if (!eaten || IsControl(codepoint) || IsCombining(codepoint)) {
      start = end;
      continue;
    }

    // We eat the beginning of the next glyph. If we are eating the one
    // requested, return its start position immediately.
    // Leemos el comienzo del siguiente glifo. Si estamos leyendo el glifo
    // solicitado, devuelve su posición de inicio inmediatamente.
    if (glyph_found) {
      return static_cast<int>(start);
    }

    // Otherwise, skip this glyph and iterate:
    // De lo contrario, omite este glifo e itera:
    glyph_found = true;
    start = end;
  }
  return static_cast<int>(input.size());
}

size_t GlyphIterate(std::string_view input, int glyph_offset, size_t start) {
  if (glyph_offset >= 0) {
    for (int i = 0; i < glyph_offset; ++i) {
      start = GlyphNext(input, start);
    }
    return start;
  } else {
    for (int i = 0; i < -glyph_offset; ++i) {
      start = GlyphPrevious(input, start);
    }
    return start;
  }
}

std::vector<int> CellToGlyphIndex(std::string_view input) {
  int x = -1;
  std::vector<int> out;
  out.reserve(input.size());
  size_t start = 0;
  size_t end = 0;
  while (start < input.size()) {
    uint32_t codepoint = 0;
    const bool eaten = EatCodePoint(input, start, &end, &codepoint);
    start = end;

    // Ignorar caracteres inválidos / de control.
    if (!eaten || IsControl(codepoint)) {
      continue;
    }

    // Los caracteres combinatorios se colocan con el glifo anterior que están modificando.
    if (IsCombining(codepoint)) {
      if (x == -1) {
        ++x;
        out.push_back(x);
      }
      continue;
    }

    // Fullwidth characters take two cells. The second is made of the empty
    // string to reserve the space the first is taking.
    // Los caracteres de ancho completo ocupan dos celdas. La segunda se compone de una
    // cadena vacía para reservar el espacio que ocupa la primera.
    if (IsFullWidth(codepoint)) {
      ++x;
      out.push_back(x);
      out.push_back(x);
      continue;
    }

    // Caracteres normales:
    ++x;
    out.push_back(x);
  }
  return out;
}

int GlyphCount(std::string_view input) {
  int size = 0;
  size_t start = 0;
  size_t end = 0;
  while (start < input.size()) {
    uint32_t codepoint = 0;
    const bool eaten = EatCodePoint(input, start, &end, &codepoint);
    start = end;

    // Ignorar caracteres inválidos:
    if (!eaten || IsControl(codepoint)) {
      continue;
    }

    // Ignore combining characters, except when they don\'t have a preceding to
    // combine with.
    // Ignora los caracteres combinatorios, excepto cuando no tienen un precedente con
    // el que combinar.
    if (IsCombining(codepoint)) {
      if (size == 0) {
        size++;
      }
      continue;
    }

    size++;
  }
  return size;
}

std::vector<WordBreakProperty> Utf8ToWordBreakProperty(std::string_view input) {
  std::vector<WordBreakProperty> out;
  out.reserve(input.size());
  size_t start = 0;
  size_t end = 0;
  while (start < input.size()) {
    uint32_t codepoint = 0;
    if (!EatCodePoint(input, start, &end, &codepoint)) {
      start = end;
      continue;
    }
    start = end;

    // Combining characters are put with the previous glyph they are modifying.
    // Los caracteres combinatorios se colocan con el glifo anterior que están modificando.
    if (IsControl(codepoint)) {
      continue;
    }

    // Ignorar caracteres combinantes.
    if (IsCombining(codepoint)) {
      continue;
    }

    WordBreakPropertyInterval interval = {0, 0, WBP::ALetter};
    std::ignore = Bisearch(codepoint, g_word_break_intervals, &interval);
    out.push_back(interval.property);
  }
  return out;
}

/// Convert a std::wstring into a UTF8 std::string.
/// Convierte un std::wstring en un std::string UTF8.
std::string to_string(std::wstring_view s) {
  std::string out;

  size_t i = 0;
  uint32_t codepoint = 0;
  while (EatCodePoint(s, i, &i, &codepoint)) {
    // Code point <-> UTF-8 conversion
    // Conversión de punto de código <-> UTF-8
    //
    // ┏━━━━━━━━┳━━━━━━━━┳━━━━━━━━┳━━━━━━━━┓
    // ┃Byte 1  ┃Byte 2  ┃Byte 3  ┃Byte 4  ┃
    // ┡━━━━━━━━╇━━━━━━━━╇━━━━━━━━╇━━━━━━━━┩
    // │0xxxxxxx│        │        │        │
    // ├────────┼────────┼────────┼────────┤
    // │110xxxxx│10xxxxxx│        │        │
    // ├────────┼────────┼────────┼────────┤
    // │1110xxxx│10xxxxxx│10xxxxxx│        │
    // ├────────┼────────┼────────┼────────┤
    // │11110xxx│10xxxxxx│10xxxxxx│10xxxxxx│
    // └────────┴────────┴────────┴────────┘

    // UTF8 de 1 byte
    if (codepoint <= 0b000'0000'0111'1111) {  // NOLINT
      const uint8_t p1 = codepoint;
      out.push_back(p1);  // NOLINT
      continue;
    }

    // UTF8 de 2 bytes
    if (codepoint <= 0b000'0111'1111'1111) {  // NOLINT
      uint8_t p2 = codepoint & 0b111111;      // NOLINT
      codepoint >>= 6;                        // NOLINT
      uint8_t p1 = codepoint;                 // NOLINT
      out.push_back(0b11000000 + p1);         // NOLINT
      out.push_back(0b10000000 + p2);         // NOLINT
      continue;
    }

    // UTF8 de 3 bytes
    if (codepoint <= 0b1111'1111'1111'1111) {  // NOLINT
      uint8_t p3 = codepoint & 0b111111;       // NOLINT
      codepoint >>= 6;                         // NOLINT
      uint8_t p2 = codepoint & 0b111111;       // NOLINT
      codepoint >>= 6;                         // NOLINT
      uint8_t p1 = codepoint;                  // NOLINT
      out.push_back(0b11100000 + p1);          // NOLINT
      out.push_back(0b10000000 + p2);          // NOLINT
      out.push_back(0b10000000 + p3);          // NOLINT
      continue;
    }

    // UTF8 de 4 bytes
    if (codepoint <= 0b1'0000'1111'1111'1111'1111) {  // NOLINT
      uint8_t p4 = codepoint & 0b111111;              // NOLINT
      codepoint >>= 6;                                // NOLINT
      uint8_t p3 = codepoint & 0b111111;              // NOLINT
      codepoint >>= 6;                                // NOLINT
      uint8_t p2 = codepoint & 0b111111;              // NOLINT
      codepoint >>= 6;                                // NOLINT
      uint8_t p1 = codepoint;                         // NOLINT
      out.push_back(0b11110000 + p1);                 // NOLINT
      out.push_back(0b10000000 + p2);                 // NOLINT
      out.push_back(0b10000000 + p3);                 // NOLINT
      out.push_back(0b10000000 + p4);                 // NOLINT
      continue;
    }

    // ¿Algo más?
  }
  return out;
}

/// Convert a UTF8 std::string into a std::wstring.
/// Convierte un std::string UTF8 en un std::wstring.
std::wstring to_wstring(std::string_view s) {
  std::wstring out;

  size_t i = 0;
  uint32_t codepoint = 0;
  while (EatCodePoint(s, i, &i, &codepoint)) {
    // On linux wstring are UTF32 encoded:
    // En Linux, wstring se codifican en UTF32:
    if constexpr (sizeof(wchar_t) == 4) {
      out.push_back(codepoint);  // NOLINT
      continue;
    }

    // On Windows, wstring are UTF16 encoded:
    // En Windows, wstring se codifican en UTF16:

    // Codepoint encoded using 1 word:
    // Punto de código codificado usando 1 palabra:
    // NOLINTNEXTLINE
    if (codepoint < 0xD800 || (codepoint > 0xDFFF && codepoint < 0x10000)) {
      uint16_t p0 = codepoint;  // NOLINT
      out.push_back(p0);        // NOLINT
      continue;
    }

    // Codepoint encoded using 2 words:
    // Punto de código codificado usando 2 palabras:
    codepoint -= 0x010000;                               // NOLINT
    uint16_t p0 = (((codepoint << 12) >> 22) + 0xD800);  // NOLINT
    uint16_t p1 = (((codepoint << 22) >> 22) + 0xDC00);  // NOLINT
    out.push_back(p0);                                   // NOLINT
    out.push_back(p1);                                   // NOLINT
  }
  return out;
}

}  // namespace ftxui
