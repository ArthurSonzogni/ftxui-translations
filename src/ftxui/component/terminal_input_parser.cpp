// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT qui se trouve dans
// le fichier LICENSE.
#include "ftxui/component/terminal_input_parser.hpp"

#include <cstdint>                    // for uint32_t
#include <ftxui/component/mouse.hpp>  // for Mouse, Mouse::Button, Mouse::Motion
#include <functional>                 // for std::function
#include <map>
#include <memory>   // for unique_ptr, allocator
#include <utility>  // for move
#include <vector>
#include "ftxui/component/event.hpp"  // for Event
#include "ftxui/component/task.hpp"   // for Task

namespace ftxui {

// NOLINTNEXTLINE
const std::map<std::string, std::string> g_uniformize = {
    // Le terminal de Microsoft utilise un caractère de nouvelle ligne différent pour la touche Entrée.
    // Cela se produit également sous Linux avec la commande `bind`:
    // Voir https://github.com/ArthurSonzogni/FTXUI/issues/337
    // Ici, nous uniformisons le caractère de nouvelle ligne en `\n`.
    {"\r", "\n"},

    // Voir : https://github.com/ArthurSonzogni/FTXUI/issues/508
    {std::string({8}), std::string({127})},

    // Voir : https://github.com/ArthurSonzogni/FTXUI/issues/626
    //
    // Selon le mode des touches de curseur (DECCKM), le terminal envoie différentes
    // séquences d'échappement :
    //
    //   Touche  Normal    Application
    //   -----   --------  -----------
    //   Haut    ESC [ A   ESC O A
    //   Bas     ESC [ B   ESC O B
    //   Droite  ESC [ C   ESC O C
    //   Gauche  ESC [ D   ESC O D
    //   Accueil ESC [ H   ESC O H
    //   Fin     ESC [ F   ESC O F
    //
    {"\x1BOA", "\x1B[A"},  // HAUT
    {"\x1BOB", "\x1B[B"},  // BAS
    {"\x1BOC", "\x1B[C"},  // DROITE
    {"\x1BOD", "\x1B[D"},  // GAUCHE
    {"\x1BOH", "\x1B[H"},  // ACCUEIL
    {"\x1BOF", "\x1B[F"},  // FIN

    // Variations autour des touches FN.
    // En interne, nous utilisons :
    // vt220, xterm-vt200, xterm-xf86-v44, xterm-new, mgt, screen
    // Voir : https://invisible-island.net/xterm/xterm-function-keys.html

    // Pour la console Linux (CTRL+ALT+FN), qui n'appartient à aucune
    // norme réelle.
    // Voir : https://github.com/ArthurSonzogni/FTXUI/issues/685
    {"\x1B[[A", "\x1BOP"},    // F1
    {"\x1B[[B", "\x1BOQ"},    // F2
    {"\x1B[[C", "\x1BOR"},    // F3
    {"\x1B[[D", "\x1BOS"},    // F4
    {"\x1B[[E", "\x1B[15~"},  // F5

    // xterm-r5, xterm-r6, rxvt
    {"\x1B[11~", "\x1BOP"},  // F1
    {"\x1B[12~", "\x1BOQ"},  // F2
    {"\x1B[13~", "\x1BOR"},  // F3
    {"\x1B[14~", "\x1BOS"},  // F4

    // vt100
    {"\x1BOt", "\x1B[15~"},  // F5
    {"\x1BOu", "\x1B[17~"},  // F6
    {"\x1BOv", "\x1B[18~"},  // F7
    {"\x1BOl", "\x1B[19~"},  // F8
    {"\x1BOw", "\x1B[20~"},  // F9
    {"\x1BOx", "\x1B[21~"},  // F10

    // scoansi
    {"\x1B[M", "\x1BOP"},    // F1
    {"\x1B[N", "\x1BOQ"},    // F2
    {"\x1B[O", "\x1BOR"},    // F3
    {"\x1B[P", "\x1BOS"},    // F4
    {"\x1B[Q", "\x1B[15~"},  // F5
    {"\x1B[R", "\x1B[17~"},  // F6
    {"\x1B[S", "\x1B[18~"},  // F7
    {"\x1B[T", "\x1B[19~"},  // F8
    {"\x1B[U", "\x1B[20~"},  // F9
    {"\x1B[V", "\x1B[21~"},  // F10
    {"\x1B[W", "\x1B[23~"},  // F11
    {"\x1B[X", "\x1B[24~"},  // F12
};

TerminalInputParser::TerminalInputParser(std::function<void(Event)> out)
    : out_(std::move(out)) {}

void TerminalInputParser::Timeout(int time) {
  timeout_ += time;
  const int timeout_threshold = 50;
  if (timeout_ < timeout_threshold) {
    return;
  }
  timeout_ = 0;
  if (!pending_.empty()) {
    Send(SPECIAL);
  }
}

void TerminalInputParser::Add(char c) {
  pending_ += c;
  timeout_ = 0;
  position_ = -1;
  Send(Parse());
}

unsigned char TerminalInputParser::Current() {
  return pending_[position_];
}

bool TerminalInputParser::Eat() {
  position_++;
  return position_ < static_cast<int>(pending_.size());
}

void TerminalInputParser::Send(TerminalInputParser::Output output) {
  switch (output.type) {
    case UNCOMPLETED:
      return;

    case DROP:
      pending_.clear();
      return;

    case CHARACTER:
      out_(Event::Character(std::move(pending_)));
      pending_.clear();
      return;

    case SPECIAL: {
      auto it = g_uniformize.find(pending_);
      if (it != g_uniformize.end()) {
        pending_ = it->second;
      }
      out_(Event::Special(std::move(pending_)));
      pending_.clear();
    }
      return;

    case MOUSE:
      out_(Event::Mouse(std::move(pending_), output.mouse));  // NOLINT
      pending_.clear();
      return;

    case CURSOR_POSITION:
      out_(Event::CursorPosition(std::move(pending_),  // NOLINT
                                 output.cursor.x,      // NOLINT
                                 output.cursor.y));    // NOLINT
      pending_.clear();
      return;

    case CURSOR_SHAPE:
      out_(Event::CursorShape(std::move(pending_), output.cursor_shape));
      pending_.clear();
      return;
  }
  // NOT_REACHED().
}

TerminalInputParser::Output TerminalInputParser::Parse() {
  if (!Eat()) {
    return UNCOMPLETED;
  }

  if (Current() == '\x1B') {
    return ParseESC();
  }

  if (Current() < 32) {  // C0 NOLINT
    return SPECIAL;
  }

  if (Current() == 127) {  // Delete // NOLINT
    return SPECIAL;
  }

  return ParseUTF8();
}

// Conversion point de code <-> UTF-8
//
// ┏━━━━━━━━┳━━━━━━━━┳━━━━━━━━┳━━━━━━━━┓
// ┃Octet 1 ┃Octet 2 ┃Octet 3 ┃Octet 4 ┃
// ┡━━━━━━━━╇━━━━━━━━╇━━━━━━━━╇━━━━━━━━┩
// │0xxxxxxx│        │        │        │
// ├────────┼────────┼────────┼────────┤
// │110xxxxx│10xxxxxx│        │        │
// ├────────┼────────┼────────┼────────┤
// │1110xxxx│10xxxxxx│10xxxxxx│        │
// ├────────┼────────┼────────┼────────┤
// │11110xxx│10xxxxxx│10xxxxxx│10xxxxxx│
// └────────┴────────┴────────┴────────┘
//
// Certaines séquences sont illégales s'il existe une représentation plus courte du
// même point de code.
TerminalInputParser::Output TerminalInputParser::ParseUTF8() {
  auto head = Current();
  unsigned char selector = 0b1000'0000;  // NOLINT

  // The non code-point part of the first byte.
  unsigned char mask = selector;

  // Find the first zero in the first byte.
  unsigned int first_zero = 8;            // NOLINT
  for (unsigned int i = 0; i < 8; ++i) {  // NOLINT
    mask |= selector;
    if (!(head & selector)) {
      first_zero = i;
      break;
    }
    selector >>= 1U;
  }

  // Accumulate the value of the first byte.
  auto value = uint32_t(head & ~mask);  // NOLINT

  // Invalid UTF8, with more than 5 bytes.
  const unsigned int max_utf8_bytes = 5;
  if (first_zero == 1 || first_zero >= max_utf8_bytes) {
    return DROP;
  }

  // Multi byte UTF-8.
  for (unsigned int i = 2; i <= first_zero; ++i) {
    if (!Eat()) {
      return UNCOMPLETED;
    }

    // Invalid continuation byte.
    head = Current();
    if ((head & 0b1100'0000) != 0b1000'0000) {  // NOLINT
      return DROP;
    }
    value <<= 6;                  // NOLINT
    value += head & 0b0011'1111;  // NOLINT
  }

  // Check for overlong UTF8 encoding.
  int extra_byte = 0;
  if (value <= 0b000'0000'0111'1111) {                 // NOLINT
    extra_byte = 0;                                    // NOLINT
  } else if (value <= 0b000'0111'1111'1111) {          // NOLINT
    extra_byte = 1;                                    // NOLINT
  } else if (value <= 0b1111'1111'1111'1111) {         // NOLINT
    extra_byte = 2;                                    // NOLINT
  } else if (value <= 0b1'0000'1111'1111'1111'1111) {  // NOLINT
    extra_byte = 3;                                    // NOLINT
  } else {                                             // NOLINT
    return DROP;
  }

  if (extra_byte != position_) {
    return DROP;
  }

  return CHARACTER;
}

TerminalInputParser::Output TerminalInputParser::ParseESC() {
  if (!Eat()) {
    return UNCOMPLETED;
  }
  switch (Current()) {
    case 'P':
      return ParseDCS();
    case '[':
      return ParseCSI();
    case ']':
      return ParseOSC();

    // En attente de 2 caractères.
    case ' ':
    case '#':
    case '%':
    case '(':
    case ')':
    case '*':
    case '+':
    case 'O':
    case 'N': {
      if (!Eat()) {
        return UNCOMPLETED;
      }
      return SPECIAL;
    }
    // En attente de 1 caractère :
    default:
      return SPECIAL;
  }
}

// ESC P ... ESC BACKSLASH
TerminalInputParser::Output TerminalInputParser::ParseDCS() {
  // Analyser jusqu'au terminateur de chaîne ST.
  while (true) {
    if (!Eat()) {
      return UNCOMPLETED;
    }

    if (Current() != '\x1B') {
      continue;
    }

    if (!Eat()) {
      return UNCOMPLETED;
    }

    if (Current() != '\\') {
      continue;
    }

    if (pending_.size() == 10 &&  //
        pending_[2] == '1' &&     //
        pending_[3] == '$' &&     //
        pending_[4] == 'r' &&     //
        true) {
      Output output(CURSOR_SHAPE);
      output.cursor_shape = pending_[5] - '0';
      return output;
    }

    return SPECIAL;
  }
}

TerminalInputParser::Output TerminalInputParser::ParseCSI() {
  bool altered = false;
  int argument = 0;
  std::vector<int> arguments;
  while (true) {
    if (!Eat()) {
      return UNCOMPLETED;
    }
    if (Current() == '<') {
      altered = true;
      continue;
    }
    if (Current() >= '0' && Current() <= '9') {
      argument *= 10;  // NOLINT
      argument += Current() - '0';
      continue;
    }
    if (Current() == ';') {
      arguments.push_back(argument);
      argument = 0;
      continue;
    }
    // Le CSI est terminé par un caractère dans la plage 0x40–0x7E
    // (ASCII @A–Z[\\\]^_`a–z{|}~),
    if (Current() >= '@' && Current() <= '~' &&
        // Note : Je ne me souviens plus pourquoi nous excluons '<'
        Current() != '<' &&
        // Pour gérer les touches F1-F4, nous excluons '['.
        Current() != '[') {
      arguments.push_back(argument);
      argument = 0;  // NOLINT

      switch (Current()) {
        case 'M':
          return ParseMouse(altered, true, std::move(arguments));
        case 'm':
          return ParseMouse(altered, false, std::move(arguments));
        case 'R':
          return ParseCursorPosition(std::move(arguments));
        default:
          return SPECIAL;
      }
    }
    // ESC invalide dans CSI.
    if (Current() == '\x1B') {
      return SPECIAL;
    }
  }
}

TerminalInputParser::Output TerminalInputParser::ParseOSC() {
  // Analyser jusqu'au terminateur de chaîne ST.
  while (true) {
    if (!Eat()) {
      return UNCOMPLETED;
    }
    if (Current() != '\x1B') {
      continue;
    }
    if (!Eat()) {
      return UNCOMPLETED;
    }
    if (Current() != '\\') {
      continue;
    }
    return SPECIAL;
  }
}

TerminalInputParser::Output TerminalInputParser::ParseMouse(  // NOLINT
    bool altered,
    bool pressed,
    std::vector<int> arguments) {
  if (arguments.size() != 3) {
    return SPECIAL;
  }

  (void)altered;

  Output output(MOUSE);
  output.mouse.motion = Mouse::Motion(pressed);  // NOLINT

  // Bits valeur Modificateur Commentaire
  // ---- ----- ------- ---------
  // 0 1  1 2   bouton   0 = Gauche, 1 = Milieu, 2 = Droite, 3 = Relâcher
  // 2    4     Maj
  // 3    8     Méta
  // 4    16    Contrôle
  // 5    32    Déplacer
  // 6    64    Molette

  // clang-format off
  const int button      = arguments[0] & (1 + 2); // NOLINT
  const bool is_shift   = arguments[0] & 4;       // NOLINT
  const bool is_meta    = arguments[0] & 8;       // NOLINT
  const bool is_control = arguments[0] & 16;      // NOLINT
  const bool is_move    = arguments[0] & 32;      // NOLINT
  const bool is_wheel   = arguments[0] & 64;      // NOLINT
  // clang-format on

  output.mouse.motion = is_move ? Mouse::Moved : Mouse::Motion(pressed);
  output.mouse.button = is_wheel ? Mouse::Button(Mouse::WheelUp + button)  //
                                 : Mouse::Button(button);
  output.mouse.shift = is_shift;
  output.mouse.meta = is_meta;
  output.mouse.control = is_control;
  output.mouse.x = arguments[1];  // NOLINT
  output.mouse.y = arguments[2];  // NOLINT

  // Événement de mouvement.
  return output;
}

// NOLINTNEXTLINE
TerminalInputParser::Output TerminalInputParser::ParseCursorPosition(
    std::vector<int> arguments) {
  if (arguments.size() != 2) {
    return SPECIAL;
  }
  Output output(CURSOR_POSITION);
  output.cursor.y = arguments[0];  // NOLINT
  output.cursor.x = arguments[1];  // NOLINT
  return output;
}

}  // namespace ftxui