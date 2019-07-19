/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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

#include <gf/Event.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/UI.h>
#include <gf/Window.h>

int main() {

  gf::Window window("UI", { 1024, 768 });
  gf::RenderWindow renderer(window);

  /// [context]
  gf::Font font("font.ttf");
  gf::UI ui(font, 13);
  /// [context]

  int groupWidth = 320;
  int groupHeight = 200;

  while (window.isOpen()) {
    /// [events]
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        default:
          break;
      }

      ui.processEvent(event); // update the imgui with input events
    }
    /// [events]

    /// [window]
    if (ui.begin("Title", gf::RectF(20, 20, 200, 200), gf::UIWindow::Title | gf::UIWindow::Border)) {

      // content of the window

    }

    ui.end();
    /// [window]

    /// [group]
    ui.layoutRowStatic(groupHeight, groupWidth, 2);

    if (ui.groupBegin("Group", gf::UIWindow::Title | gf::UIWindow::Border)) {

      // content of the group

      ui.groupEnd();
    }
    /// [group]

    /// [tree]
    static gf::UICollapse rootCollapsed = gf::UICollapse::Minimized;

    if (ui.treePush(gf::UITree::Tab, "Root", rootCollapsed)) {

      static gf::UICollapse childCollapsed = gf::UICollapse::Maximized;

      if (ui.treePush(gf::UITree::Node, "Child", childCollapsed)) {

        // content of the child node

        ui.treePop();
      }

      // next content of the root node

      ui.treePop();
    }
    /// [tree]


  }

  return EXIT_SUCCESS;
}
