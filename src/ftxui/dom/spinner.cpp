// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Uso de este código fuente se rige por la licencia MIT que se puede encontrar en
// el archivo LICENSE.
#include <cstddef>  // for size_t
#include <string>   // for basic_string, string
#include <utility>  // for move
#include <vector>   // for vector, __alloc_traits<>::value_type

#include "ftxui/dom/elements.hpp"  // for Element, gauge, text, vbox, spinner

namespace ftxui {

namespace {
// NOLINTNEXTLINE
const std::vector<std::vector<std::vector<std::string>>> elements = {
    {
        {"Replaced by the gauge"},
    },
    {
        {".  "},
        {".. "},
        {"..."},
    },
    {
        {"|"},
        {"/"},
        {"-"},
        {"\"},
    },
    {
        {"+"},
        {"x"},
    },
    {
        {"|  "},
        {"|| "},
        {"|||"},
    },
    {
        {"←"},
        {"↖"},
        {"↑"},
        {"↗"},
        {"→"},
        {"↘"},
        {"↓"},
        {"↙"},
    },
    {
        {" "},
        {"▂"},
        {"▃"},
        {"▄"},
        {"▅"},
        {"▆"},
        {"▇"},
        {"█"},
        {"▇"},
        {"▆"},
        {"▅"},
        {"▄"},
        {"▃"},
        {" "},
    },
    {
        {"▉"},
        {"▊"},
        {"▋"},
        {"▌"},
        {"▍"},
        {"▎"},
        {"▏"},
        {"▎"},
        {"▍"},
        {"▌"},
        {"▋"},
        {"▊"},
    },
    {
        {"▖"},
        {"▘"},
        {"▝"},
        {"▗"},
    },
    {
        {"◢"},
        {"◣"},
        {"◤"},
        {"◥"},
    },
    {
        {"◰"},
        {"◳"},
        {"◲"},
        {"◱"},
    },
    {
        {"◴"},
        {"◷"},
        {"◶"},
        {"◵"},
    },
    {
        {"◐"},
        {"◓"},
        {"◑"},
        {"◒"},
    },
    {
        {"◡"},
        {"⊙"},
        {"◠"},
    },
    {
        {"⠁"},
        {"⠂"},
        {"⠄"},
        {"⡀"},
        {"⢀"},
        {"⠠"},
        {"⠐"},
        {"⠈"},
    },
    {
        {"⠋"},
        {"⠙"},
        {"⠹"},
        {"⠸"},
        {"⠼"},
        {"⠴"},
        {"⠦"},
        {"⠧"},
        {"⠇"},
        {"⠏"},
    },
    {
        {"(*----------)"}, {"(-*---------)"}, {"(--*--------)"},
        {"(---*-------)"}, {"(----*------)"}, {"(-----*-----)"},
        {"(------*----)"}, {"(-------*---)"}, {"(--------*--)"},
        {"(---------*-)"}, {"(----------*)"}, {"(---------*-)"},
        {"(--------*--)"}, {"(-------*---)"}, {"(------*----)"},
        {"(-----*-----)"}, {"(----*------)"}, {"(---*-------)"},
        {"(--*--------)"}, {"(-*---------)"},
    },
    {
        {"[      ]"},
        {"[=     ]"},
        {"[==    ]"},
        {"[===   ]"},
        {"[====  ]"},
        {"[===== ]"},
        {"[======]"},
        {"[===== ]"},
        {"[====  ]"},
        {"[===   ]"},
        {"[==    ]"},
        {"[=     ]"},
    },
    {
        {"[      ]"},
        {"[=     ]"},
        {"[==    ]"},
        {"[===   ]"},
        {"[====  ]"},
        {"[===== ]"},
        {"[======]"},
        {"[ =====]"},
        {"[  ====]"},
        {"[   ===]"},
        {"[    ==]"},
        {"[     =]"},
    },
    {
        {"[==    ]"},
        {"[==    ]"},
        {"[==    ]"},
        {"[==    ]"},
        {"[==    ]"},
        {" [==   ]"},
        {"[  ==  ]"},
        {"[   == ]"},
        {"[    ==]"},
        {"[    ==]"},
        {"[    ==]"},
        {"[    ==]"},
        {"[    ==]"},
        {"[   ==] "},
        {"[  ==  ]"},
        {"[ ==   ]"},
    },
    {
        {
            " ─╮",
            "  │",
            "   ",
        },
        {
            "  ╮",
            "  │",
            "  ╯",
        },
        {
            "   ",
            "  │",
            " ─╯",
        },
        {
            "   ",
            "   ",
            "╰─╯",
        },
        {
            "   ",
            "│  ",
            "╰─ ",
        },
        {
            "╭  ",
            "│  ",
            "╰  ",
        },
        {
            "╭─ ",
            "│  ",
            "   ",
        },
        {
            "╭─╮",
            "   ",
            "   ",
        },
    },
    {
        {
            "   /\O ",
            "    /\/",
            "   /\  ",
            "  /  \ ",
            "LOL  LOL",
        },
        {
            "    _O  ",
            "   //|_ ",
            "    |   ",
            "   /|   ",
            "   LLOL ",
        },
        {
            "     O  ",
            "    /_  ",
            "    |\  ",
            "   / |  ",
            " LOLLOL ",
        },
    },
    {
        {"       ", "_______", "       "},
        {"       ", "______/", "       "},
        {"      _", "_____/ ", "       "},
        {"     _ ", "____/ \", "       "},
        {"    _  ", "___/ \ ", "      \"},
        {"   _   ", "__/ \  ", "     \_"},
        {"  _    ", "_/ \   ", "    \_/"},
        {" _     ", "/ \   _", "   \_/ "},
        {"_      ", " \   __", "  \_/  "},
        {"       ", " \   ___", " \_    "},
        {"       ", "    ___", "\_/    "},
        {"       ", "  _____", "_/     "},
        {"       ", " ______", "/      "},
        {"       ", "_______", "       "},
    },
};

}  // namespace

/// @brief Útil para representar el efecto del tiempo y/o eventos. Esto muestra
/// un "video" de arte ASCII.
/// @param charset_index El tipo de "video".
/// @param image_index El "cuadro" del video. Necesitas aumentarlo para
/// cada "paso".
/// @ingroup dom
Element spinner(int charset_index, size_t image_index) {
  if (charset_index <= 0) {
    const int progress_size = 40;
    image_index %= progress_size;
    if (image_index > progress_size / 2) {
      image_index = progress_size - image_index;
    }
    return gauge(float(image_index) * 0.05F);  // NOLINT
  }
  charset_index %= int(elements.size());
  image_index %= int(elements[charset_index].size());
  std::vector<Element> lines;
  for (const auto& it : elements[charset_index][image_index]) {
    lines.push_back(text(it));
  }
  return vbox(std::move(lines));
}

}  // namespace ftxui