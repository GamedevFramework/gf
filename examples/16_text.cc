/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#include <cstdlib>
#include <iostream>

#include <gf/Event.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Text.h>
#include <gf/Window.h>

int main() {
  gf::Window window("16_text", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::Font font("assets/DejaVuSans.ttf");

  gf::Text text;
  text.setFont(font);
  text.setCharacterSize(30);
  text.setColor(gf::Color::Orange);
  text.setOutlineThickness(1.5f);
  text.setOutlineColor(gf::Color::Azure);
  text.setString("Gamedev Framework");
  text.setPosition({ 320.0f, 20.0f });
  text.setAnchor(gf::Anchor::Center);

  gf::Text paragraph;
  paragraph.setFont(font);
  paragraph.setCharacterSize(12);
  paragraph.setParagraphWidth(200.0f);
  paragraph.setAlignment(gf::Alignment::Justify);
  paragraph.setString("Line breaking, also known as word wrapping, is the process of breaking a section of text into lines such that it will fit in the available width of a page, window or other display area.\nIn text display, line wrap is the feature of continuing on a new line when a line is full, such that each line fits in the viewable window, allowing text to be read from top to bottom without any horizontal scrolling.\nWord wrap is the additional feature of most text editors, word processors, and web browsers, of breaking lines between words rather than within words, when possible. Word wrap makes it unnecessary to hard-code newline delimiters within paragraphs, and allows the display of text to adapt flexibly and dynamically to displays of varying sizes.");
  // Text from: https://en.wikipedia.org/wiki/Line_wrap_and_word_wrap
  paragraph.setPosition({ 10.0f, 50.0f });
  paragraph.setAnchor(gf::Anchor::TopLeft);

  gf::Text centered;
  centered.setFont(font);
  centered.setCharacterSize(16);
  centered.setAlignment(gf::Alignment::Center);
  centered.setString("A centered text with paragraph width = 0");
  centered.setPosition({ 400.0f, 100.0f });
  centered.setAnchor(gf::Anchor::TopCenter);

  std::cout << "Gamedev Framework (gf) example #16: Text\n";
  std::cout << "This example prints two text to show outlining and text justifying.\n";

  renderer.clear(gf::Color::White);

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          if (event.key.scancode == gf::Scancode::Escape) {
            window.close();
          }
          break;

        default:
          break;
      }
    }

    renderer.clear();
    renderer.draw(text);
    renderer.draw(paragraph);
    renderer.draw(centered);
    renderer.display();
  }

  return 0;
}
