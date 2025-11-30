// Copyright 2021 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include "ftxui/dom/box_helper.hpp"

#include <algorithm>  // for max
#include <cstdint>
#include <vector>  // for vector

namespace ftxui::box_helper {

namespace {

int SafeRatio(int value, int numerator, int denominator) {
  return static_cast<int64_t>(value) * static_cast<int64_t>(numerator) /
         std::max(static_cast<int64_t>(denominator), static_cast<int64_t>(1));
}

// Llamado cuando el tamaño permitido es mayor que el tamaño solicitado. Esto
// distribuye los espacios adicionales hacia los elementos flexibles, en
// proporciones relativas.
void ComputeGrow(std::vector<Element>* elements,
                 int extra_space,
                 int flex_grow_sum) {
  for (Element& element : *elements) {
    const int added_space =
        SafeRatio(extra_space, element.flex_grow, flex_grow_sum);
    extra_space -= added_space;
    flex_grow_sum -= element.flex_grow;
    element.size = element.min_size + added_space;
  }
}

// Llamado cuando el tamaño permitido es menor que el tamaño solicitado, y el
// elemento encogible puede absorber el extra_space (negativo). Esto distribuye
// el extra_space hacia esos elementos.
void ComputeShrinkEasy(std::vector<Element>* elements,
                       int extra_space,
                       int flex_shrink_sum) {
  for (Element& element : *elements) {
    const int added_space = SafeRatio(
        extra_space, element.min_size * element.flex_shrink, flex_shrink_sum);
    extra_space -= added_space;
    flex_shrink_sum -= element.flex_shrink * element.min_size;
    element.size = element.min_size + added_space;
  }
}

// Llamado cuando el tamaño permitido es menor que el tamaño solicitado, y el
// elemento encogible no puede absorber el extra_space (negativo). Esto asigna
// cero a los elementos encogibles y distribuye el extra_space (negativo) restante
// hacia los otros elementos no encogibles.
void ComputeShrinkHard(std::vector<Element>* elements,
                       int extra_space,
                       int size) {
  for (Element& element : *elements) {
    if (element.flex_shrink != 0) {
      element.size = 0;
      continue;
    }

    const int added_space = SafeRatio(extra_space, element.min_size, size);

    extra_space -= added_space;
    size -= element.min_size;

    element.size = element.min_size + added_space;
  }
}

}  // namespace

void Compute(std::vector<Element>* elements, int target_size) {
  int size = 0;
  int flex_grow_sum = 0;
  int flex_shrink_sum = 0;
  int flex_shrink_size = 0;

  for (auto& element : *elements) {
    flex_grow_sum += element.flex_grow;
    flex_shrink_sum += element.min_size * element.flex_shrink;
    if (element.flex_shrink != 0) {
      flex_shrink_size += element.min_size;
    }
    size += element.min_size;
  }

  const int extra_space = target_size - size;
  if (extra_space >= 0) {
    ComputeGrow(elements, extra_space, flex_grow_sum);
  } else if (flex_shrink_size + extra_space >= 0) {
    ComputeShrinkEasy(elements, extra_space, flex_shrink_sum);

  } else {
    ComputeShrinkHard(elements, extra_space + flex_shrink_size,
                      size - flex_shrink_size);
  }
}

}  // namespace ftxui::box_helper
