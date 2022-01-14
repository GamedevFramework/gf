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

#include <gf/BufferedGeometry.h>
#include <gf/Event.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Window.h>

int main() {
  gf::Window window("17_buffer", { 640, 480 }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  /*
   * a shape in a buffer
   */

  gf::VertexBuffer shapeBuffer;
  gf::VertexBuffer shapeOutlineBuffer;
  gf::RectF shapeBounds;

  {
    gf::StarShape shape(50.0f, 100.0f, 5);
    shape.setColor(gf::Color::Chartreuse);
    shape.setOutlineThickness(5.0f);
    shape.setOutlineColor(gf::Color::darker(gf::Color::Chartreuse));

    shapeBuffer = shape.commitGeometry();
    shapeOutlineBuffer = shape.commitOutlineGeometry();
    shapeBounds = shape.getLocalBounds();

  }

  gf::BufferedGeometry shapeGeometry(shapeBuffer, shapeOutlineBuffer);
  shapeGeometry.setLocalBounds(shapeBounds);
  shapeGeometry.setPosition({ 150.0f, 200.0f });
  shapeGeometry.setAnchor(gf::Anchor::Center);

  /*
   * a sprite in a buffer
   */

  gf::Texture texture("assets/bomb.png");
  texture.setSmooth();

  gf::VertexBuffer spriteBuffer;
  gf::RectF spriteBounds;

  {
    gf::Sprite sprite(texture);

    spriteBuffer = sprite.commitGeometry();
    spriteBounds = sprite.getLocalBounds();
  }

  gf::BufferedGeometry spriteGeometry(spriteBuffer);
  spriteGeometry.setLocalBounds(spriteBounds);
  spriteGeometry.setTexture(texture);
  spriteGeometry.setPosition({ 500.0f, 200.0f });
  spriteGeometry.setScale({ 0.8f, 0.8f });
  spriteGeometry.setAnchor(gf::Anchor::Center);

  /*
   * a text in a buffer
   */

  gf::Font font("assets/DejaVuSans.ttf");

  gf::VertexBuffer textBuffer;
  gf::VertexBuffer textOutlineBuffer;
  gf::RectF textBounds;


  {
    gf::Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setColor(gf::Color::Orange);
    text.setOutlineThickness(1.5f);
    text.setOutlineColor(gf::Color::Azure);
    text.setString("Gamedev Framework");

    textBuffer = text.commitGeometry();
    textOutlineBuffer = text.commitOutlineGeometry();
    textBounds = text.getLocalBounds();
  }

  gf::BufferedGeometry textGeometry(textBuffer, textOutlineBuffer);
  textGeometry.setLocalBounds(textBounds);
  textGeometry.setTexture(*font.getTexture(30));
  textGeometry.setPosition({ 320.0f, 20.0f });
  textGeometry.setAnchor(gf::Anchor::Center);

  std::cout << "Gamedev Framework (gf) example #17: Buffer\n";
  std::cout << "This example prints various geometries using vertex buffers.\n";

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
    renderer.draw(textGeometry);
    renderer.draw(shapeGeometry);
    renderer.draw(spriteGeometry);
    renderer.display();
  }

  return 0;
}
