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


int main() {
  gf::Window window("41_ui", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::Font font;
  bool loaded = font.loadFromFile("16_DejaVuSans.ttf");

  if (!loaded) {
    return EXIT_FAILURE;
  }

  gf::DefaultUIRenderer uiRenderer(font);
  gf::UILayout layout;
  gf::UI ui(uiRenderer, layout);

  bool checked1 = false;
  bool checked2 = false;
  bool checked3 = true;
  bool checked4 = false;
  float scrollArea = 0;
  float value1 = 50.0f;
  float value2 = 30.0f;

  std::vector<std::string> choices = { "First Choice", "Next Choice", "Last Choice" };
  std::size_t choice = 0;

  bool toggle;

  renderer.clear(gf::Color::Gray());

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

    ui.clear();

    ui.beginScrollArea("Scroll area", { 10.0f, 10.0f, 200.0f, 400.0f }, &scrollArea);

    ui.separatorLine();
    ui.separator();

    ui.button("Button");
    ui.button("Disabled Button", false);
    ui.item("Item");
    ui.item("Disabled item", false);

    toggle = ui.check("Checkbox", checked1);

    if (toggle) {
      checked1 = !checked1;
    }

    toggle = ui.check("Disabled checkbox", checked2, false);

    if (toggle) {
      checked2 = !checked2;
    }

    toggle = ui.collapse("Collapse", checked3);

    if (checked3) {
      ui.indent();
      ui.label("Collapsible element");

      for (int i = 0; i < 20; ++i) {
        ui.label("More text...");
      }

      ui.unindent();
    }

    if (toggle) {
      checked3 = !checked3;
    }

    toggle = ui.collapse("Disabled collapse", checked4, false);

    if (toggle) {
      checked4 = !checked4;
    }

    ui.label("Label");
    ui.value("Value");

    ui.slider("Slider", &value1, 0.0f, 100.0f, 1.0f);
    ui.slider("Disabled slider", &value2, 0.0f, 100.0f, 1.0f, false);

    ui.indent();
    ui.label("Indented");
    ui.unindent();
    ui.label("Unindented");

    toggle = ui.cycle(choices, choice);

    if (toggle) {
      choice = (choice + 1) % choices.size();
    }

    ui.endScrollArea();

    renderer.clear();
    renderer.draw(ui);
    renderer.display();
  }

  return 0;
}
