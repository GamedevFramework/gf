/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#include <cassert>
#include <algorithm>
#include <tuple>
#include <iostream>

#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/UI.h>
#include <gf/Window.h>


enum class Difficulty {
  Easy,
  Hard,
};

int main() {
  gf::Window window("41_ui", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::Font font;
  bool loaded = font.loadFromFile("16_DejaVuSans.ttf");

  if (!loaded) {
    return EXIT_FAILURE;
  }

  gf::UI ui(font);

  renderer.clear(gf::Color::Gray());

  Difficulty op = Difficulty::Easy;
  float value = 0.0f;
  bool selected = false;
  float valueFloat = 0.0f;
  int valueInt = 0;
  gf::Color4f color = gf::Color::Orange;
  int propertyInt = 0;
  int selectedItem = 0;

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        default:
          break;
      }

      ui.update(event);
    }

    if (ui.begin("Show", gf::RectF(50, 50, 220, 220), gf::UIWindow::Border | gf::UIWindow::Movable | gf::UIWindow::Scalable | gf::UIWindow::Closable | gf::UIWindow::Minimizable | gf::UIWindow::Title)) {

      /* fixed widget pixel width */

      ui.layoutRowStatic(30, 80, 1);

      if (ui.buttonLabel("button")) {
        std::cout << "Hello!\n";
      }

      /* fixed widget window ratio width */

      ui.layoutRowDynamic(30, 2);

      if (ui.radioLabel("easy", op == Difficulty::Easy)) {
        op = Difficulty::Easy;
      }

      if (ui.radioLabel("hard", op == Difficulty::Hard)) {
        op = Difficulty::Hard;
      }

      /* custom widget pixel width */

      ui.layoutRowBegin(gf::UILayoutFormat::Static, 30, 2);
      ui.layoutRowPush(50);
      ui.label("Volume:", gf::UITextAlignment::Left);
      ui.layoutRowPush(110);
      if (ui.sliderFloat(0.0f, value, 1.0f, 0.1f)) {
        std::cout << "Value: " << value << '\n';
      }
      ui.layoutRowEnd();


    }

    ui.end();


    if (ui.begin("Overview", gf::RectF(300, 50, 220, 420), gf::UIWindow::Border | gf::UIWindow::Movable | gf::UIWindow::Scalable | gf::UIWindow::Closable | gf::UIWindow::Minimizable | gf::UIWindow::Title)) {

      ui.layoutRowDynamic(30, 2);

      ui.label("Label", gf::UITextAlignment::Left);
      ui.labelColored("Label colored", gf::UITextAlignment::Right, color);

      ui.layoutRowDynamic(30, 2);

      ui.buttonLabel("Button");
      ui.buttonColor(color);

      ui.layoutRowBegin(gf::UILayoutFormat::Static, 30, 2);
      ui.layoutRowPush(30);
      ui.buttonSymbol(gf::UISymbolType::CircleOutline);
      ui.layoutRowPush(160);
      ui.buttonSymbolLabel(gf::UISymbolType::CircleSolid, "Button symbol", gf::UITextAlignment::Right);
      ui.layoutRowEnd();

      ui.layoutRowDynamic(30, 2);

      if (ui.checkboxLabel("easy", op == Difficulty::Easy)) {
        op = Difficulty::Easy;
      }

      if (ui.checkboxLabel("hard", op == Difficulty::Hard)) {
        op = Difficulty::Hard;
      }

      ui.layoutRowDynamic(30, 2);

      if (ui.radioLabel("easy", op == Difficulty::Easy)) {
        op = Difficulty::Easy;
      }

      if (ui.radioLabel("hard", op == Difficulty::Hard)) {
        op = Difficulty::Hard;
      }

      ui.layoutRowDynamic(30, 2);

      if (ui.selectableLabel("selected", gf::UITextAlignment::Centered, selected)) {
        selected = true;
      }

      if (ui.selectableLabel("!selected", gf::UITextAlignment::Centered, !selected)) {
        selected = false;
      }

      ui.layoutRowDynamic(30, 2);

      if (ui.sliderFloat(0.0f, valueFloat, 10.0f, 0.1f)) {
        std::cout << "Float value: " << valueFloat << '\n';
      }

      if (ui.sliderInt(0, valueInt, 100, 5)) {
        std::cout << "Int value: " << valueInt << '\n';
      }

      ui.layoutRowDynamic(30, 1);
      ui.propertyInt("int", 0, propertyInt, 10, 1, 0.1f);

      ui.layoutRowDynamic(25, 1);
      selectedItem = ui.combo({ "One", "Two", "Three" }, selectedItem, 20, { 180, 100 });

      ui.layoutRowBegin(gf::UILayoutFormat::Static, 60, 1);
      ui.layoutRowPush(100);
      ui.colorPicker(color);
      ui.layoutRowEnd();

    }

    ui.end();

    renderer.clear();
    renderer.draw(ui);
    renderer.display();
  }

  return 0;
}
