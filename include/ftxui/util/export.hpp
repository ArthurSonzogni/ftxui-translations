// Copyright 2026 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#ifndef FTXUI_UTIL_EXPORT_H_
#define FTXUI_UTIL_EXPORT_H_

// Dans un build amalgamé, on ne veut aucune annotation d'export/import.
#if defined(FTXUI_AMALGAMATED)
#define FTXUI_EXPORT(component)
#define INSIDE_FTXUI_COMPONENT_IMPL(component) 0
#else

// Utilisé pour annoter les symboles exportés par le composant nommé
// |component|. Notez que ceci ne fonctionne correctement que si les sources
// de la cible du composant correspondant sont compilées avec
// |IS_FTXUI_$component_IMPL| défini à 1. Par exemple :
//
//   class FTXUI_EXPORT(FOO) Bar {};
//
// Si IS_FTXUI_FOO_IMPL=1 à la compilation, alors Bar sera annoté en utilisant
// la macro FTXUI_EXPORT_ANNOTATION définie ci-dessous. Sinon, il sera annoté
// en utilisant la macro FTXUI_IMPORT_ANNOTATION.
#define FTXUI_EXPORT(component)                         \
  FTXUI_MACRO_CONDITIONAL_(IS_FTXUI_##component##_IMPL, \
                           FTXUI_EXPORT_ANNOTATION, FTXUI_IMPORT_ANNOTATION)

// Indique si l'unité de compilation actuelle est compilée dans le cadre de
// l'implémentation du composant nommé |component|. S'étend en |1| si
// |IS_FTXUI_$component_IMPL| est défini à |1| ; s'étend en |0| sinon.
//
// Notez en particulier que si |IS_FTXUI_$component_IMPL| n'est pas du tout
// défini, il reste correct de tester INSIDE_FTXUI_COMPONENT_IMPL(component),
// qui s'étend alors en |0| comme attendu.
#define INSIDE_FTXUI_COMPONENT_IMPL(component) \
  FTXUI_MACRO_CONDITIONAL_(IS_FTXUI_##component##_IMPL, 1, 0)

#endif

// Macros spécifiques au compilateur pour annoter l'export ou l'import d'un
// symbole. Sans effet en dehors des builds par composant. Elles ne devraient
// pas être utilisées directement. Utilisez plutôt la macro FTXUI_EXPORT
// définie ci-dessus.
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

// Au-delà de ce point se trouvent plusieurs macros utilitaires internes
// utilisées pour l'implémentation des macros ci-dessus. Non destinées à un
// usage externe.

#define FTXUI_MACRO_EXPAND(x) x

// Auxiliaire pour l'expansion conditionnelle vers l'une de deux chaînes de
// tokens. Si |condition| s'étend en |1| alors cette macro s'étend en
// |consequent| ; sinon elle s'étend en |alternate|.
#define FTXUI_MACRO_CONDITIONAL_(condition, consequent, alternate) \
  FTXUI_MACRO_EXPAND(FTXUI_MACRO_SELECT_THIRD_ARGUMENT_(           \
      FTXUI_MACRO_CONDITIONAL_COMMA_(condition), consequent, alternate))

// S'étend en une virgule (,) si et seulement si son premier argument s'étend
// en |1|. Utilisé conjointement avec |FTXUI_MACRO_SELECT_THIRD_ARGUMENT_()|,
// car la présence ou l'absence d'une virgule supplémentaire peut être
// utilisée pour décaler conditionnellement les positions des arguments
// suivants et ainsi influencer l'argument sélectionné.
#define FTXUI_MACRO_CONDITIONAL_COMMA_(...) \
  FTXUI_MACRO_EXPAND(FTXUI_MACRO_CONDITIONAL_COMMA_IMPL_(__VA_ARGS__, dummy))
#define FTXUI_MACRO_CONDITIONAL_COMMA_IMPL_(x, ...) \
  FTXUI_MACRO_CONDITIONAL_COMMA_##x##_
#define FTXUI_MACRO_CONDITIONAL_COMMA_1_ ,

// Auxiliaire qui sélectionne simplement son troisième argument. Utilisé
// conjointement avec |FTXUI_MACRO_CONDITIONAL_COMMA_()| ci-dessus pour
// implémenter l'expansion conditionnelle de macro.
#define FTXUI_MACRO_SELECT_THIRD_ARGUMENT_(...) \
  FTXUI_MACRO_EXPAND(                           \
      FTXUI_MACRO_SELECT_THIRD_ARGUMENT_IMPL_(__VA_ARGS__, dummy))
#define FTXUI_MACRO_SELECT_THIRD_ARGUMENT_IMPL_(a, b, c, ...) c

#endif  // FTXUI_UTIL_EXPORT_H_
