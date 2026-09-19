// Copyright 2026 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#ifndef FTXUI_UTIL_EXPORT_H_
#define FTXUI_UTIL_EXPORT_H_

// En una compilación amalgamada, no queremos ninguna anotación de exportación/importación.
#if defined(FTXUI_AMALGAMATED)
#define FTXUI_EXPORT(component)
#define INSIDE_FTXUI_COMPONENT_IMPL(component) 0
#else

// Se usa para anotar símbolos que son exportados por el componente llamado
// |component|. Tenga en cuenta que esto solo funciona correctamente si las fuentes del
// objetivo del componente correspondiente se compilan con |IS_FTXUI_$component_IMPL|
// definido como 1. Por ejemplo:
//
//   class FTXUI_EXPORT(FOO) Bar {};
//
// Si IS_FTXUI_FOO_IMPL=1 en tiempo de compilación, entonces Bar se anotará usando la
// macro FTXUI_EXPORT_ANNOTATION definida más abajo. De lo contrario se
// anotará usando la macro FTXUI_IMPORT_ANNOTATION.
#define FTXUI_EXPORT(component)                         \
  FTXUI_MACRO_CONDITIONAL_(IS_FTXUI_##component##_IMPL, \
                           FTXUI_EXPORT_ANNOTATION, FTXUI_IMPORT_ANNOTATION)

// Indica si la unidad de compilación actual se está compilando como parte de
// la implementación del componente llamado |component|. Se expande a |1| si
// |IS_FTXUI_$component_IMPL| está definido como |1|; se expande a |0| en caso contrario.
//
// Nótese en particular que si |IS_FTXUI_$component_IMPL| no está definido en absoluto,
// sigue siendo correcto probar INSIDE_FTXUI_COMPONENT_IMPL(component), que
// se expande a |0| como se espera.
#define INSIDE_FTXUI_COMPONENT_IMPL(component) \
  FTXUI_MACRO_CONDITIONAL_(IS_FTXUI_##component##_IMPL, 1, 0)

#endif

// Macros específicas del compilador para anotar exportación o importación de un símbolo. No hace nada
// en compilaciones que no son de componentes. No deberían usarse mucho directamente.
// En su lugar, use la macro FTXUI_EXPORT definida anteriormente.
#if defined(COMPONENT_BUILD)
#if defined(WIN32)
#define FTXUI_EXPORT_ANNOTATION __declspec(dllexport)
#define FTXUI_IMPORT_ANNOTATION __declspec(dllimport)
#else  // defined(WIN32)
#define FTXUI_EXPORT_ANNOTATION __attribute__((visibility("default")))
#define FTXUI_IMPORT_ANNOTATION __attribute__((visibility("default")))
#endif  // defined(WIN32)
#else   // defined(COMPONENT_BUILD)
#define FTXUI_EXPORT_ANNOTATION
#define FTXUI_IMPORT_ANNOTATION
#endif  // defined(COMPONENT_BUILD)

// Debajo de este punto hay varias macros de utilidad internas usadas para la
// implementación de las macros anteriores. No están destinadas a uso externo.

#define FTXUI_MACRO_EXPAND(x) x

// Ayudante para expansión condicional a una de dos cadenas de tokens. Si |condition|
// se expande a |1| entonces esta macro se expande a |consequent|; de lo contrario se expande
// a |alternate|.
#define FTXUI_MACRO_CONDITIONAL_(condition, consequent, alternate) \
  FTXUI_MACRO_EXPAND(FTXUI_MACRO_SELECT_THIRD_ARGUMENT_(           \
      FTXUI_MACRO_CONDITIONAL_COMMA_(condition), consequent, alternate))

// Se expande a una coma (,) si su primer argumento se expande a |1|. Se usa en
// conjunto con |FTXUI_MACRO_SELECT_THIRD_ARGUMENT_()|, ya que la presencia
// o ausencia de una coma extra se puede usar para desplazar condicionalmente las
// posiciones de los argumentos siguientes y así influir en qué argumento se selecciona.
#define FTXUI_MACRO_CONDITIONAL_COMMA_(...) \
  FTXUI_MACRO_EXPAND(FTXUI_MACRO_CONDITIONAL_COMMA_IMPL_(__VA_ARGS__, dummy))
#define FTXUI_MACRO_CONDITIONAL_COMMA_IMPL_(x, ...) \
  FTXUI_MACRO_CONDITIONAL_COMMA_##x##_
#define FTXUI_MACRO_CONDITIONAL_COMMA_1_ ,

// Ayudante que simplemente selecciona su tercer argumento. Se usa en conjunto con
// |FTXUI_MACRO_CONDITIONAL_COMMA_()| arriba para implementar la expansión
// condicional de macros.
#define FTXUI_MACRO_SELECT_THIRD_ARGUMENT_(...) \
  FTXUI_MACRO_EXPAND(                           \
      FTXUI_MACRO_SELECT_THIRD_ARGUMENT_IMPL_(__VA_ARGS__, dummy))
#define FTXUI_MACRO_SELECT_THIRD_ARGUMENT_IMPL_(a, b, c, ...) c

#endif  // FTXUI_UTIL_EXPORT_H_
