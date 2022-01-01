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
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Drawable.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

class DummyDrawable : public gf::Drawable {
public:
  virtual void draw(gf::RenderTarget& target, const gf::RenderStates& states) override {
    (void) target;
    (void) states;
  }
};


void dummyViewContainerUsage() {
  static constexpr gf::Vector2i ScreenSize(1024, 576);
  static constexpr gf::Vector2f ViewSize(100.0f, 100.0f); // dummy values
  static constexpr gf::Vector2f ViewCenter(0.0f, 0.0f); // dummy values

  gf::Window window("Game", ScreenSize);
  gf::RenderWindow renderer(window);

  DummyDrawable sprite, ui;

  /// [view_container]
  gf::ViewContainer views;

  gf::ExtendView extendView;
  extendView.setSize(ViewSize);
  views.addView(extendView);

  gf::ScreenView screenView;
  views.addView(screenView);

  // initialize the views with the initial screen size
  views.setInitialFramebufferSize(ScreenSize);

  // ...

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      // ...

      views.processEvent(event);
    }

    // ...

    renderer.clear();

    renderer.setView(extendView);
    renderer.draw(sprite);

    renderer.setView(screenView);
    renderer.draw(ui);

    renderer.display();
  }
  /// [view_container]

}
