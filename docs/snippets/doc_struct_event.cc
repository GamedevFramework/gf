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
#include <gf/Event.h>
#include <gf/Window.h>

void doSomethingWithTheNewSize([[maybe_unused]] gf::Vector2i size) {
}

void dummyEventUsage(gf::Window& window) {
  /// [event]
  gf::Event event;

  while (window.pollEvent(event)) {
    // Request for closing the window
    if (event.type == gf::EventType::Closed) {
      window.close();
    }

    // The escape key was pressed
    if (event.type == gf::EventType::KeyPressed && event.key.keycode == gf::Keycode::Escape) {
      window.close();
    }

    // The window was resized
    if (event.type == gf::EventType::Resized) {
      doSomethingWithTheNewSize(event.resize.size);
    }

    // etc ...
  }
  /// [event]
}
