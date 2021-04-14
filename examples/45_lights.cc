/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2021 Julien Bernard
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

#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/LightSystem.h>
#include <gf/LightTextures.h>
#include <gf/RenderWindow.h>
#include <gf/Views.h>
#include <gf/ViewContainer.h>
#include <gf/Window.h>

int main() {
  static constexpr gf::Vector2i ScreenSize(640, 480);

  gf::Window window("45_lights", ScreenSize);
  gf::RenderWindow renderer(window);

  gf::ViewContainer views;

  gf::ExtendView view;
  view.setCenter({ 0.0f, 0.0f });
  view.setSize({ 1000.0f, 1000.0f });
  views.addView(view);

  views.setInitialFramebufferSize(ScreenSize);

  gf::LightSystem lights(ScreenSize);

  std::cout << "Gamedev Framework (gf) example #45: Lights\n";

  gf::RectF red = gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 128.0f, 128.0f });
  gf::RectF green = gf::RectF::fromPositionSize({ 128.0f, 0.0f }, { 128.0f, 128.0f });
  gf::CircF blue({ -200, -200 }, 32);
  gf::Vector2f pos0 = { 0.0f, -200.0f };
  gf::Vector2f pos1 = { 250.0f, 250.0f };

  static constexpr int Size = 1024;

  gf::Texture texture = gf::LightTextures::createRealisticLight(Size, 1.0f, Size / 40);

  gf::LightPointEmission light0(texture);
  light0.setPosition(pos0);
  light0.setAnchor(gf::Anchor::Center);
  light0.setSourceRadius(Size * 0.05f);
  light0.setShadowOverExtendMultiplier(1.5f);
  light0.setLocaleCastCenter({ 0.0f, 0.0f }); // gf::vec(Size * 0.5f, Size * 0.5f));
  lights.addLightPoint(light0);

  gf::LightPointEmission light1(texture);
  light1.setPosition(pos1);
  light1.setAnchor(gf::Anchor::Center);
  light1.setSourceRadius(Size * 0.01f);
  light1.setShadowOverExtendMultiplier(20.0f);
  light1.setLocaleCastCenter({ 0.0f, 0.0f }); // gf::vec(Size * 0.5f, Size * 0.5f));
  lights.addLightPoint(light1);

  gf::CircleShape circle(light0.getSourceRadius());
  circle.setColor(gf::Color::Transparent);
  circle.setOutlineColor(gf::Color::Red);
  circle.setOutlineThickness(2.0f);
  circle.setPosition(pos0);
  circle.setAnchor(gf::Anchor::Center);


  gf::LightShape occluder0(red);
  lights.addLightShape(occluder0);

  gf::RectangleShape shape0(red);
  shape0.setColor(gf::Color::Red);

  gf::LightShape occluder1(blue);
  lights.addLightShape(occluder1);

  gf::CircleShape shape1(blue);
  shape1.setColor(gf::Color::Blue);

  gf::LightShape occluder2(green);
  lights.addLightShape(occluder2);

  gf::RectangleShape shape2(green);
  shape2.setColor(gf::Color::Green);


  renderer.clear(gf::Color::Chartreuse);

  bool change = false;

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          if (event.key.keycode == gf::Keycode::F1) {
            lights.dump();
          }
          break;

        case gf::EventType::MouseButtonPressed:
          change = true;
          pos0 = renderer.mapPixelToCoords(event.mouseButton.coords);
          light0.setPosition(pos0);
          circle.setPosition(pos0);
          break;

        case gf::EventType::MouseButtonReleased:
          change = false;
          break;

        case gf::EventType::MouseMoved:
          if (change) {
            pos0 = renderer.mapPixelToCoords(event.mouseCursor.coords);
            light0.setPosition(pos0);
            circle.setPosition(pos0);
          }
          break;

        default:
          break;
      }

      views.processEvent(event);
    }

    renderer.setView(view);
    renderer.clear();
    renderer.draw(shape0);
    renderer.draw(shape1);
    renderer.draw(shape2);
    renderer.draw(lights);
    renderer.draw(circle);
    renderer.display();
  }

  return 0;
}
