// Copyright 2020 Arthur Sonzogni. Tous droits réservés.
// L'utilisation de ce code source est régie par la licence MIT que l'on peut trouver dans
// le fichier LICENSE.
#include <chrono>                   // for operator""s, chrono_literals
#include <ftxui/screen/screen.hpp>  // for Screen
#include <iostream>                 // for cout, ostream
#include <string>                   // for allocator, operator<<, string
#include <thread>                   // for sleep_for

#include "ftxui/dom/elements.hpp"  // for paragraph, text, operator|, Element, border, Fit, color, hflow, spinner, vbox, bold, dim, underlined
#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/box.hpp"    // for ftxui
#include "ftxui/screen/color.hpp"  // for Color, Color::Red

int main() {
  using namespace ftxui;
  using namespace std::chrono_literals;

  auto img1 = []() { return text("img") | border; };
  auto img2 = []() { return vbox({text("big"), text("image")}) | border; };

  std::string reset_position;
  for (int i = 0;; ++i) {
    auto document =  //
        hflow(
            paragraph("Bonjour le monde ! Voici une image :"), img1(),
            paragraph(" Voici un texte "), text("souligné ") | underlined,
            paragraph(" Voici un texte "), text("en gras ") | bold,
            paragraph("Bonjour le monde ! Voici une image :"), img2(),
            paragraph(
                "Le Lorem Ipsum est simplement du faux texte employé dans la "
                "composition et la mise en page avant impression. Le Lorem "
                "Ipsum est le faux texte standard de l'imprimerie depuis les "
                "années 1500, quand un imprimeur anonyme assembla ensemble "
                "des morceaux de texte pour réaliser un livre spécimen de "
                "polices de texte. Il n'a pas fait que survivre cinq siècles, "
                "mais s'est aussi adapté à la bureautique informatique, sans "
                "que son contenu n'en soit modifié. Il a été popularisé dans "
                "les années 1960 grâce à la vente de feuilles Letraset "
                "contenant des passages du Lorem Ipsum, et, plus récemment, "
                "par son inclusion dans des applications de mise en page de "
                "texte, comme Aldus PageMaker."),
            paragraph(" Voici un texte "), text("estompé ") | dim,
            paragraph("Bonjour le monde ! Voici une image :"), img1(),
            paragraph(" Voici un texte "), text("rouge ") | color(Color::Red),
            paragraph(" Un spinner "), spinner(6, i / 10)) |
        border;

    auto screen = Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    std::cout << reset_position;
    screen.Print();
    reset_position = screen.ResetPosition();

    std::this_thread::sleep_for(0.01s);
  }

  return 0;
}
