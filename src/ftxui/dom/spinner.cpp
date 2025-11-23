// Copyright 2020 Arthur Sonzogni. All rights reserved.
// L'utilisation de ce code source est régie par la licence MIT qui peut être trouvée dans
// le fichier LICENSE.
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
        {"Remplacé par la jauge"},
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
        {"(*----------)"},
        {"(-*---------"}
        {"(--*--------)"},
        {"("---*-------"}
        {"("----*------)"},
        {"("-----*-----"}
        {"("------*----)"},
        {"("-------*---"}
        {"("--------*--)"},
        {"("---------*-"}
        {"("----------*)"},
        {"("---------*-"}
        {"("--------*--)"},
        {"("-------*---"}
        {"("------*----)"},
        {"("-----*-----"}
        {"("----*------)"},
        {"("---*-------"}
        {"(--*--------)"},
        {"(-*---------"}
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
        {"       ", "\   ___", " \_/   "},
        {"       ", "    ___", "\_/    "},
        {"       ", "  _____", "_/     "},
        {"       ", " ______", "/      "},
        {"       ", "_______", "       "},
    },
};

}  // namespace

/// @brief Utile pour représenter l'effet du temps et/ou des événements. Cela affiche
/// une "vidéo" en art ASCII.
/// @param charset_index Le type de "vidéo".
/// @param image_index L'"image" de la vidéo. Vous devez l'augmenter pour
/// chaque "étape".
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