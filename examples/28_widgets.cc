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
#include <cstdio>

#include <deque>
#include <iostream>
#include <vector>

#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Texture.h>
#include <gf/TextureAtlas.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Window.h>

int main() {
  gf::Window window("28_widgets", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::Font font("assets/DejaVuSans.ttf");

  gf::TextureAtlas atlas("assets/ui.xml");
  gf::Texture texture("assets/ui.png");
  atlas.setTexture(texture);

  std::cout << "Gamedev Framework (gf) example #28: widgets\n";
  std::cout << "This example shows some game widgets.\n";
  std::cout << "How to use:\n";
  std::cout << "\tUp/Down: Navigate through widgets\n";
  std::cout << "\tReturn: Activate the selected widget\n";

  gf::WidgetContainer widgets;

  gf::TextWidget start("Start", font, 25);
  start.setPosition({ 50.0f, 50.0f });
  start.setTextOutlineThickness(1.0f);
  start.setAnchor(gf::Anchor::TopLeft);
  start.setCallback([]() { std::cout << "Start!\n"; });
  widgets.addWidget(start);

  gf::TextWidget options("Options", font, 25);
  options.setPosition({ 50.0f, 80.0f });
  options.setTextOutlineThickness(1.0f);
  options.setAnchor(gf::Anchor::TopLeft);
  options.setCallback([]() { std::cout << "Options!\n"; });
  widgets.addWidget(options);

  gf::TextWidget disabled("Disabled", font, 25);
  disabled.setPosition({ 50.0f, 110.0f });
  disabled.setTextOutlineThickness(1.0f);
  disabled.setAnchor(gf::Anchor::TopLeft);
  disabled.setCallback([]() { std::cout << "Disabled!\n"; });
  disabled.setDisabled();
  widgets.addWidget(disabled);

  gf::TextButtonWidget quit("Quit", font, 25);
  quit.setPosition({ 50.0f, 140.0f });
  quit.setTextOutlineThickness(1.0f);
  quit.setBackgroundOutlineThickness(1.0f);
  quit.setAnchor(gf::Anchor::TopLeft);
  quit.setRadius(2.0f);
  quit.setPadding(4.0f);
  quit.setCallback([]() { std::cout << "Quit!\n"; });
  widgets.addWidget(quit);

  gf::TextButtonWidget transformed("Transformed", font, 25);
  transformed.setPosition({ 50.0f, 180.0f });
  transformed.setRotation(gf::Pi / 4);
  transformed.setScale({ 1.0f , 0.8f });
  transformed.setTextOutlineThickness(1.0f);
  transformed.setAnchor(gf::Anchor::TopLeft);
  transformed.setBackgroundOutlineThickness(1.0f);
  transformed.setRadius(2.0f);
  transformed.setPadding(4.0f);
  transformed.setCallback([]() { std::cout << "Rotated and scaled!\n"; });
  widgets.addWidget(transformed);

  gf::TextButtonWidget disabledAgain("Disabled again", font, 25);
  disabledAgain.setPosition({ 50.0f, 320.0f });
  disabledAgain.setTextOutlineThickness(1.0f);
  disabledAgain.setBackgroundOutlineThickness(1.0f);
  disabledAgain.setRadius(2.0f);
  disabledAgain.setPadding(4.0f);
  disabledAgain.setAnchor(gf::Anchor::TopLeft);
  disabledAgain.setDisabled();
  disabledAgain.setCallback([]() { std::cout << "Disabled again!\n"; });
  widgets.addWidget(disabledAgain);

  gf::SpriteWidget sprite(texture, atlas.getTextureRect("grey_button04.png"), atlas.getTextureRect("grey_button02.png"), atlas.getTextureRect("grey_button05.png"));
  sprite.setPosition({ 300.0f, 50.0f });
  sprite.setAnchor(gf::Anchor::TopLeft);
  sprite.setCallback([]() { std::cout << "Sprite!\n";  });
  widgets.addWidget(sprite);

  gf::ChoiceSpriteWidget choice(texture, atlas.getTextureRect("grey_box.png"), atlas.getTextureRect("grey_boxCheckmark.png"));
  choice.setPosition({ 300.0f, 100.0f });
  sprite.setAnchor(gf::Anchor::TopLeft);
  choice.setCallback([&choice]() { std::cout << (choice.isChosen() ? "Chosen\n" : "Empty\n"); });
  widgets.addWidget(choice);

  gf::Clock clock;
  renderer.clear(gf::Color::White);

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          switch (event.key.keycode) {
            case gf::Keycode::Escape:
              window.close();
              break;

            case gf::Keycode::Up:
              widgets.selectPreviousWidget();
              break;

            case gf::Keycode::Down:
              widgets.selectNextWidget();
              break;

            case gf::Keycode::Return:
              widgets.triggerAction();
              break;

            default:
              break;
          }
          break;

        case gf::EventType::MouseMoved:
          widgets.pointTo(renderer.mapPixelToCoords(event.mouseCursor.coords));
          break;

        case gf::EventType::MouseButtonPressed:
          widgets.pointTo(renderer.mapPixelToCoords(event.mouseButton.coords));
          widgets.triggerAction();
          break;

        default:
          break;
      }
    }

    renderer.clear();
    widgets.render(renderer);
    renderer.display();
  }

  return 0;
}
