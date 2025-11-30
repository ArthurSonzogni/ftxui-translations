// Copyright 2021 Arthur Sonzogni. Todos los derechos reservados.
// El uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#ifndef FTXUI_DOM_FLEXBOX_CONFIG_HPP
#define FTXUI_DOM_FLEXBOX_CONFIG_HPP

/*
  Esto replica el modelo flexbox de CSS.
  Ver guía para la documentación:
  https://css-tricks.com/snippets/css/a-guide-to-flexbox/
*/

namespace ftxui {

/// @brief FlexboxConfig es una estructura de configuración que define las
/// propiedades de diseño para un contenedor flexbox.
//
/// Permite especificar la dirección de los elementos flex, si deben
/// ajustarse (wrap), cómo deben justificarse a lo largo del eje principal y cómo
/// deben alinearse a lo largo del eje transversal.
/// También incluye propiedades para los espacios entre los elementos flex en
/// ambos ejes, principal y transversal.
/// Esta estructura se utiliza para configurar el comportamiento de diseño de los
/// contenedores flexbox en una interfaz de usuario de terminal.
///
/// @ingroup dom
struct FlexboxConfig {
  /// Esto establece el eje principal, definiendo así la dirección en la que los
  /// elementos flex se colocan en el contenedor flex. Flexbox es (aparte del
  /// ajuste) un concepto de diseño de una sola dirección. Piense en los
  /// elementos flex como si se dispusieran principalmente en filas
  /// horizontales o columnas verticales.
  enum class Direction {
    Row,            ///< Los elementos flex se distribuyen en una fila.
    RowInversed,    ///< Los elementos flex se distribuyen en una fila, pero en orden inverso.
    Column,         ///< Los elementos flex se distribuyen en una columna.
    ColumnInversed  ///< Los elementos flex se distribuyen en una columna, pero en orden
                    ///< inverso.
  };
  Direction direction = Direction::Row;

  /// Por defecto, todos los elementos flex intentarán ajustarse en una sola línea. Puede
  /// cambiar eso y permitir que los elementos se ajusten según sea necesario con esta
  /// propiedad.
  enum class Wrap {
    NoWrap,        ///< Todos los elementos flex intentarán ajustarse en una sola línea.
    Wrap,          ///< Los elementos flex se ajustarán en varias líneas.
    WrapInversed,  ///< Los elementos flex se ajustarán en varias líneas, pero en orden
                   ///< inverso.
  };
  Wrap wrap = Wrap::Wrap;

  /// Esto define la alineación a lo largo del eje principal. Ayuda a distribuir el espacio
  /// libre sobrante cuando todos los elementos flex en una línea son inflexibles, o
  /// son flexibles pero han alcanzado su tamaño máximo. También ejerce cierto
  /// control sobre la alineación de los elementos cuando desbordan la línea.
  enum class JustifyContent {
    /// Los elementos se alinean al inicio de la dirección del flexbox.
    FlexStart,
    /// Los elementos se alinean al final de la dirección del flexbox.
    FlexEnd,
    /// Los elementos se centran a lo largo de la línea.
    Center,
    /// Los elementos se estiran para llenar la línea.
    Stretch,
    /// Los elementos se distribuyen uniformemente en la línea; el primer elemento está al
    // inicio de la línea, el último elemento al final de la línea.
    SpaceBetween,
    /// Los elementos se distribuyen uniformemente en la línea con igual espacio a su
    /// alrededor. Tenga en cuenta que visualmente los espacios no son iguales, ya que
    /// todos los elementos tienen el mismo espacio en ambos lados. El primer elemento
    /// tendrá una unidad de espacio contra el borde del contenedor, pero dos unidades
    /// de espacio entre el siguiente elemento porque ese siguiente elemento tiene su
    /// propio espaciado que se aplica.
    SpaceAround,
    /// Los elementos se distribuyen de manera que el espacio entre dos elementos (y el
    /// espacio a los bordes) sea igual.
    SpaceEvenly,
  };
  JustifyContent justify_content = JustifyContent::FlexStart;

  /// Esto define el comportamiento predeterminado de cómo se disponen los elementos flex
  /// a lo largo del eje transversal en la línea actual. Piense en ello como la versión
  /// de justify-content para el eje transversal (perpendicular al eje principal).
  enum class AlignItems {
    FlexStart,  ///< Los elementos se colocan al inicio del eje transversal.
    FlexEnd,    ///< Los elementos se colocan al final del eje transversal.
    Center,     ///< Los elementos se centran a lo largo del eje transversal.
    Stretch,    ///< Los elementos se estiran para llenar el eje transversal.
  };
  AlignItems align_items = AlignItems::FlexStart;

  // Esto alinea las líneas de un contenedor flex cuando hay espacio extra en
  // el eje transversal, de manera similar a cómo justify-content alinea los elementos
  // individuales dentro del eje principal.
  enum class AlignContent {
    FlexStart,     ///< Los elementos se colocan al inicio del eje transversal.
    FlexEnd,       ///< Los elementos se colocan al final del eje transversal.
    Center,        ///< Los elementos se centran a lo largo del eje transversal.
    Stretch,       ///< Los elementos se estiran para llenar el eje transversal.
    SpaceBetween,  ///< Los elementos se distribuyen uniformemente en el eje transversal.
    SpaceAround,   ///< Los elementos se distribuyen uniformemente con igual espacio alrededor de cada
                   ///< línea.
    SpaceEvenly,  ///< Los elementos se distribuyen uniformemente en el eje transversal con igual
                  ///< espacio a su alrededor.
  };
  AlignContent align_content = AlignContent::FlexStart;

  int gap_x = 0;
  int gap_y = 0;

  // Patrón de constructor. Para uso encadenado como:
  // ```
  // FlexboxConfig()
  //    .Set(FlexboxConfig::Direction::Row)
  //    .Set(FlexboxConfig::Wrap::Wrap);
  // ```
  FlexboxConfig& Set(FlexboxConfig::Direction);
  FlexboxConfig& Set(FlexboxConfig::Wrap);
  FlexboxConfig& Set(FlexboxConfig::JustifyContent);
  FlexboxConfig& Set(FlexboxConfig::AlignItems);
  FlexboxConfig& Set(FlexboxConfig::AlignContent);
  FlexboxConfig& SetGap(int gap_x, int gap_y);
};

}  // namespace ftxui

#endif  // FTXUI_DOM_FLEXBOX_CONFIG_HPP
