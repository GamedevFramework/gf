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

/// [index]

#include <gf/Event.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Window.h>

int main() {
  // Create the main window and the renderer

  gf::Window window("Example", { 640, 480 });
  gf::RenderWindow renderer(window);

  // Load a sprite to display

  gf::Texture texture("sprite.png");

  gf::Sprite sprite(texture);
  sprite.setPosition({ 300, 200 });

  // Create a graphical text to display

  gf::Font font("DroidSans.ttf");

  gf::Text text("Hello gf!", font, 50);
  text.setPosition({ 100, 100 });

  renderer.clear(gf::Color::White);

  // Start the game loop

  while (window.isOpen()) {

    // Process events

    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        default:
          break;
      }
    }

    // Draw the entities

    renderer.clear();
    renderer.draw(sprite);
    renderer.draw(text);
    renderer.display();
  }

  return 0;
}

/// [index]
