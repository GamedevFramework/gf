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
#include <cstdio>

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

enum class Option {
  A,
  B,
  C,
};

enum class ColorMode {
  RGB,
  HSV,
};

/*
 * This is the overview given in nuklear itself with some minor changes.
 */
static void overview(gf::UI& ui) {
  // window flags
  static bool showMenu = true;

  static bool title = true;
  static bool border = true;
  static bool scalable = true;
  static bool movable = true;
  static bool noScrollbar = false;
  static bool minimizable = true;

  static gf::UIWindowFlags windowFlags = gf::None;


  // popups

  static bool showAppAbout = false;

  // window flags
  windowFlags = gf::None;
  if (title) { windowFlags |= gf::UIWindow::Title; }
  if (border) { windowFlags |= gf::UIWindow::Border; }
  if (scalable) { windowFlags |= gf::UIWindow::Scalable; }
  if (movable) { windowFlags |= gf::UIWindow::Movable; }
  if (noScrollbar) { windowFlags |= gf::UIWindow::NoScrollbar; }
  if (minimizable) { windowFlags |= gf::UIWindow::Minimizable; }

  if (ui.begin("Overview", gf::RectF(300, 50, 400, 600), windowFlags)) {
    if (showMenu) {
      ui.menubarBegin();

      ui.layoutRowBegin(gf::UILayoutFormat::Static, 25, 1);
      ui.layoutRowPush(45);
      if (ui.menuBeginLabel("Help", gf::UITextAlignment::Left, { 120.0f, 200.0f })) {
        ui.layoutRowDynamic(25, 1);

        if (ui.menuItemLabel("Hide", gf::UITextAlignment::Left)) {
          showMenu = false;
        }

        if (ui.menuItemLabel("About", gf::UITextAlignment::Left)) {
          showAppAbout = true;
        }

        ui.menuEnd();
      }


      ui.menubarEnd();
    }

    if (showAppAbout) {
      static gf::RectF bounds(20.0f, 100.0f, 300.0f, 190.0f);

      if (ui.popupBegin(gf::UIPopupType::Static, "About", gf::UIWindow::Closable, bounds)) {
        ui.layoutRowDynamic(20, 1);
        ui.label("Nuklear", gf::UITextAlignment::Left);
        ui.label("By Micha Mettke", gf::UITextAlignment::Left);
        ui.label("Nuklear is licensed under the public domain license.", gf::UITextAlignment::Left);
        ui.label("Adaptation for gf by Julien Bernard", gf::UITextAlignment::Left);
        ui.popupEnd();
      } else {
        showAppAbout = false;
      }
    }

    static gf::UICollapseStates windowCollapsed = gf::UICollapseStates::Minimized;

    if (ui.treePush(gf::UITreeType::Tab, "Window", windowCollapsed)) {
      ui.layoutRowDynamic(30, 2);
      ui.checkboxLabel("Titlebar", title);
      ui.checkboxLabel("Menu", showMenu);
      ui.checkboxLabel("Border", border);
      ui.checkboxLabel("Scalable", scalable);
      ui.checkboxLabel("Movable", movable);
      ui.checkboxLabel("No scrollbar", noScrollbar);
      ui.checkboxLabel("Minimizable", minimizable);
      ui.treePop();
    }

    static gf::UICollapseStates widgetsCollapsed = gf::UICollapseStates::Minimized;

    if (ui.treePush(gf::UITreeType::Tab, "Widgets", widgetsCollapsed)) {

      static gf::UICollapseStates textCollapsed = gf::UICollapseStates::Minimized;

      if (ui.treePush(gf::UITreeType::Node, "Text", textCollapsed)) {
        ui.layoutRowDynamic(20, 1);
        ui.label("Label aligned left", gf::UITextAlignment::Left);
        ui.label("Label aligned centered", gf::UITextAlignment::Centered);
        ui.label("Label aligned right", gf::UITextAlignment::Right);
        ui.labelColored("Blue text", gf::UITextAlignment::Left, gf::Color::Blue);
        ui.labelColored("Yellow text", gf::UITextAlignment::Left, gf::Color::Yellow);

        // TODO: multiline text

        ui.treePop();
      }

      static gf::UICollapseStates buttonCollapsed = gf::UICollapseStates::Minimized;

      if (ui.treePush(gf::UITreeType::Node, "Button", buttonCollapsed)) {
        ui.layoutRowStatic(30, 100, 3);

        if (ui.buttonLabel("Button")) {
          std::cout << "Button pressed!\n";
        }

        ui.buttonSetBehavior(gf::UIButtonBehavior::Repeater);

        if (ui.buttonLabel("Repeater")) {
          std::cout << "Repeater pressed!\n";
        }

        ui.buttonSetBehavior(gf::UIButtonBehavior::Default);
        ui.buttonColor(gf::Color::Blue);

        ui.layoutRowStatic(25, 25, 8);
        ui.buttonSymbol(gf::UISymbolType::CircleSolid);
        ui.buttonSymbol(gf::UISymbolType::CircleOutline);
        ui.buttonSymbol(gf::UISymbolType::RectSolid);
        ui.buttonSymbol(gf::UISymbolType::RectOutline);
        ui.buttonSymbol(gf::UISymbolType::TriangleUp);
        ui.buttonSymbol(gf::UISymbolType::TriangleDown);
        ui.buttonSymbol(gf::UISymbolType::TriangleLeft);
        ui.buttonSymbol(gf::UISymbolType::TriangleRight);

        ui.layoutRowStatic(30, 100, 2);
        ui.buttonSymbolLabel(gf::UISymbolType::TriangleLeft, "prev", gf::UITextAlignment::Right);
        ui.buttonSymbolLabel(gf::UISymbolType::TriangleRight, "next", gf::UITextAlignment::Left);

        ui.treePop();
      }

      static gf::UICollapseStates basicCollapsed = gf::UICollapseStates::Minimized;

      if (ui.treePush(gf::UITreeType::Node, "Basic", basicCollapsed)) {
        static bool checkbox = false;
        static Option option = Option::A;

        static int intSlider = 5;
        static float floatSlider = 2.5f;
        static std::size_t progValue = 40;
        static float propertyFloat = 2;
        static int propertyInt = 10;
        static int propertyNeg = 10;

        static float rangeFloatMin = 0;
        static float rangeFloatMax = 100;
        static float rangeFloatValue = 50;
        static int rangeIntMin = 0;
        static int rangeIntMax = 4096;
        static int rangeIntValue = 2048;
        static const float ratio[] = { 120, 150 };

        ui.layoutRowStatic(30, 100, 1);
        ui.checkboxLabel("Checkbox", checkbox);

        ui.layoutRowStatic(30, 80, 3);
        option = ui.optionLabel("optionA", option == Option::A) ? Option::A : option;
        option = ui.optionLabel("optionB", option == Option::B) ? Option::B : option;
        option = ui.optionLabel("optionC", option == Option::C) ? Option::C : option;

        ui.layoutRow(gf::UILayoutFormat::Static, 30, ratio);

        ui.label("Slider int", gf::UITextAlignment::Left);
        ui.sliderInt(0, intSlider, 10, 1);

        ui.label("Slider float", gf::UITextAlignment::Left);
        ui.sliderFloat(0, floatSlider, 5.0f, 0.5f);

        ui.label("Progressbar", gf::UITextAlignment::Left);
        ui.progress(progValue, 100, true);

        ui.layoutRow(gf::UILayoutFormat::Static, 25, ratio);

        ui.label("Property float:", gf::UITextAlignment::Left);
        ui.propertyFloat("Float:", 0, propertyFloat, 64.0f, 0.1f, 0.2f);

        ui.label("Property int:", gf::UITextAlignment::Left);
        ui.propertyInt("Int:", 0, propertyInt, 100, 1, 1);

        ui.label("Property neg:", gf::UITextAlignment::Left);
        ui.propertyInt("Neg:", -10, propertyNeg, 10, 1, 1);

        ui.layoutRowDynamic(25, 1);
        ui.label("Range:", gf::UITextAlignment::Left);

        ui.layoutRowDynamic(25, 3);

        ui.propertyFloat("#min", 0, rangeFloatMin, rangeFloatMax, 1.0f, 0.2f);
        ui.propertyFloat("#float", rangeFloatMin, rangeFloatValue, rangeFloatMax, 1.0f, 0.2f);
        ui.propertyFloat("#max", rangeFloatMin, rangeFloatMax, 100.0f, 1.0f, 0.2f);

        ui.propertyInt("#min", 0, rangeIntMin, rangeIntMax, 1, 10);
        ui.propertyInt("#int", rangeIntMin, rangeIntValue, rangeIntMax, 1, 10);
        ui.propertyInt("#max", rangeIntMin, rangeIntMax, 10000, 1, 10);

        ui.treePop();
      }

      static gf::UICollapseStates selectableCollapsed = gf::UICollapseStates::Minimized;

      if (ui.treePush(gf::UITreeType::Node, "Selectable", selectableCollapsed)) {

        static gf::UICollapseStates listCollapsed = gf::UICollapseStates::Minimized;

        if (ui.treePush(gf::UITreeType::Node, "List", listCollapsed)) {
          static bool selected[4] = { false, false, true, false };

          ui.layoutRowStatic(18, 100, 1);

          ui.selectableLabel("Selectable", gf::UITextAlignment::Left, selected[0]);
          ui.selectableLabel("Selectable", gf::UITextAlignment::Left, selected[1]);
          ui.label("Not Selectable", gf::UITextAlignment::Left);
          ui.selectableLabel("Selectable", gf::UITextAlignment::Left, selected[2]);
          ui.selectableLabel("Selectable", gf::UITextAlignment::Left, selected[3]);

          ui.treePop();
        }

        static gf::UICollapseStates gridCollapsed = gf::UICollapseStates::Minimized;

        if (ui.treePush(gf::UITreeType::Node, "Grid", gridCollapsed)) {
          static bool selected[16] = {
            true, false, false, false,
            false, true, false, false,
            false, false, true, false,
            false, false, false, true
          };

          ui.layoutRowStatic(50, 50, 4);

          for (int i = 0; i < 16; ++i) {
            if (ui.selectableLabel("+", gf::UITextAlignment::Centered, selected[i])) {
              int x = i % 4;
              int y = i / 4;

              if (x > 0) { selected[i - 1] = ! selected[i - 1]; }
              if (x < 3) { selected[i + 1] = ! selected[i + 1]; }
              if (y > 0) { selected[i - 4] = ! selected[i - 4]; }
              if (y < 3) { selected[i + 4] = ! selected[i + 4]; }
            }
          }

          ui.treePop();
        }

        ui.treePop();
      }

      static gf::UICollapseStates comboCollapsed = gf::UICollapseStates::Minimized;

      if (ui.treePush(gf::UITreeType::Node, "Combo", comboCollapsed)) {
        static int currentWeapon = 0;
        static std::vector<std::string> weapons = { "First", "Pistol", "Shotgun", "Plasma", "BFG" };

        static int comboColor1R = 130;
        static int comboColor1G = 50;
        static int comboColor1B = 50;
        static int comboColor1A = 255;
        static gf::Color4f comboColor1 = gf::Color::rgba(comboColor1R, comboColor1G, comboColor1B, comboColor1A);

        static int comboColor2R = 130;
        static int comboColor2G = 180;
        static int comboColor2B = 50;
        static int comboColor2A = 255;
        static gf::Color4f comboColor2 = gf::Color::rgba(comboColor2R, comboColor2G, comboColor2B, comboColor2A);

        static std::size_t progA = 20;
        static std::size_t progB = 40;
        static std::size_t progC = 10;
        static std::size_t progD = 90;

        static bool checkValues[4];
        static float position[3];

        // default combobox

        ui.layoutRowStatic(25, 200, 1);
        ui.combobox(weapons, currentWeapon, 25, { 200.0f, 200.0f });

        // slider color combobox

        if (ui.comboBeginColor(comboColor1, { 200.0f, 200.0f })) {
          const float ratio[] = { 0.15f, 0.85f };

          ui.layoutRow(gf::UILayoutFormat::Dynamic, 30, ratio);
          ui.label("R:", gf::UITextAlignment::Left);
          ui.sliderInt(0, comboColor1R, 255, 5);
          ui.label("G:", gf::UITextAlignment::Left);
          ui.sliderInt(0, comboColor1G, 255, 5);
          ui.label("B:", gf::UITextAlignment::Left);
          ui.sliderInt(0, comboColor1B, 255, 5);
          ui.label("A:", gf::UITextAlignment::Left);
          ui.sliderInt(0, comboColor1A, 255, 5);

          comboColor1 = gf::Color::rgba(comboColor1R, comboColor1G, comboColor1B, comboColor1A);

          ui.comboEnd();
        }

        // complex color combobox

        if (ui.comboBeginColor(comboColor2, { 200.0f, 400.0f })) {
          static ColorMode colorMode = ColorMode::RGB;

          ui.layoutRowDynamic(120, 1);
          ui.colorPicker(comboColor2);

          gf::Color4u localColor = gf::Color::convert(comboColor2);
          comboColor2R = localColor.r;
          comboColor2G = localColor.g;
          comboColor2B = localColor.b;
          comboColor2A = localColor.a;

          ui.layoutRowDynamic(25, 2);
          colorMode = ui.optionLabel("RGB", colorMode == ColorMode::RGB) ? ColorMode::RGB : colorMode;
          colorMode = ui.optionLabel("HSV", colorMode == ColorMode::HSV) ? ColorMode::HSV : colorMode;

          ui.layoutRowDynamic(25, 1);
          if (colorMode == ColorMode::RGB) {
            ui.propertyInt("#R", 0, comboColor2R, 255, 1, 1);
            ui.propertyInt("#G", 0, comboColor2G, 255, 1, 1);
            ui.propertyInt("#B", 0, comboColor2B, 255, 1, 1);
            ui.propertyInt("#A", 0, comboColor2A, 255, 1, 1);

            comboColor2 = gf::Color::rgba(comboColor2R, comboColor2G, comboColor2B, comboColor2A);
          } else {
            // TODO: RGB/HSV color conversion
          }

          ui.comboEnd();
        }

        // progressbar combobox

        std::size_t sum = progA + progB + progC + progD;

        if (ui.comboBeginLabel(std::to_string(sum), { 200.0f, 200.0f })) {
          ui.layoutRowDynamic(30, 1);
          ui.progress(progA, 100, true);
          ui.progress(progB, 100, true);
          ui.progress(progC, 100, true);
          ui.progress(progD, 100, true);
          ui.comboEnd();
        }

        // checkbox combobox

        sum = (checkValues[0] ? 1 : 0) + (checkValues[1] ? 1 : 0) + (checkValues[2] ? 1 : 0) + (checkValues[3] ? 1 : 0);

        if (ui.comboBeginLabel(std::to_string(sum), { 200.0f, 200.0f })) {
          ui.layoutRowDynamic(30, 1);
          ui.checkboxLabel(weapons[0], checkValues[0]);
          ui.checkboxLabel(weapons[1], checkValues[1]);
          ui.checkboxLabel(weapons[2], checkValues[2]);
          ui.checkboxLabel(weapons[3], checkValues[3]);
          ui.comboEnd();
        }

        // complex text combobox

        static constexpr std::size_t BufferSize = 256;
        char buffer[BufferSize];

        std::snprintf(buffer, BufferSize, "%.2f, %.2f, %.2f", position[0], position[1], position[2]);

        if (ui.comboBeginLabel(buffer, { 200.0f, 200.0f })) {
          ui.layoutRowDynamic(25, 1);
          ui.propertyFloat("#X:", -1024.0f, position[0], 1024.0f, 1.0f, 0.5f);
          ui.propertyFloat("#Y:", -1024.0f, position[1], 1024.0f, 1.0f, 0.5f);
          ui.propertyFloat("#Z:", -1024.0f, position[2], 1024.0f, 1.0f, 0.5f);
          ui.comboEnd();
        }

        // TODO: Chart

        // TODO: Date

        ui.treePop();
      }

      // TODO: Input

      ui.treePop();
    }

    // TODO: Chart

    static gf::UICollapseStates popupCollapsed = gf::UICollapseStates::Minimized;

    if (ui.treePush(gf::UITreeType::Tab, "Popup", popupCollapsed)) {
      static gf::Color4f color = gf::Color::Red;
      static bool select[4];
      static bool popupActive;

      // menu contextual

      ui.layoutRowStatic(30, 150, 1);
      gf::RectF bounds = ui.getWidgetBounds();
      ui.label("Right click me for menu", gf::UITextAlignment::Left);

      if (ui.contextualBegin(gf::None, { 100.0f, 300.0f }, bounds)) {
        static std::size_t prog = 40;
        static int slider = 10;

        ui.layoutRowDynamic(25, 1);
        ui.checkboxLabel("Menu", showMenu);
        ui.progress(prog, 100, true);
        ui.sliderInt(0, slider, 16, 1);

        if (ui.contextualItemLabel("About", gf::UITextAlignment::Centered)) {
          showAppAbout = true;
        }

        ui.selectableLabel(select[0] ? "Unselect" : "Select", gf::UITextAlignment::Left, select[0]);
        ui.selectableLabel(select[1] ? "Unselect" : "Select", gf::UITextAlignment::Left, select[1]);
        ui.selectableLabel(select[2] ? "Unselect" : "Select", gf::UITextAlignment::Left, select[2]);
        ui.selectableLabel(select[3] ? "Unselect" : "Select", gf::UITextAlignment::Left, select[3]);

        ui.contextualEnd();
      }

      // color contextual

      ui.layoutRowBegin(gf::UILayoutFormat::Static, 30, 2);
      ui.layoutRowPush(100);
      ui.label("Right click here:", gf::UITextAlignment::Left);
      ui.layoutRowPush(50);
      bounds = ui.getWidgetBounds();
      ui.buttonColor(color);
      ui.layoutRowEnd();

      if (ui.contextualBegin(gf::None, { 350, 60 }, bounds)) {
        ui.layoutRowDynamic(30, 4);
        ui.propertyFloat("#r", 0.0f, color.r, 1.0f, 0.05f, 0.01f);
        ui.propertyFloat("#g", 0.0f, color.g, 1.0f, 0.05f, 0.01f);
        ui.propertyFloat("#b", 0.0f, color.b, 1.0f, 0.05f, 0.01f);
        ui.propertyFloat("#a", 0.0f, color.a, 1.0f, 0.05f, 0.01f);
        ui.contextualEnd();
      }

      ui.layoutRowBegin(gf::UILayoutFormat::Static, 30, 2);
      ui.layoutRowPush(100);
      ui.label("Popup:", gf::UITextAlignment::Left);
      ui.layoutRowPush(50);

      if (ui.buttonLabel("Popup")) {
        popupActive = true;
      }

      ui.layoutRowEnd();

      if (popupActive) {
        static gf::RectF bounds(20.0f, 100.0f, 220.0f, 90.0f);

        if (ui.popupBegin(gf::UIPopupType::Static, "Error", gf::None, bounds)) {
          ui.layoutRowDynamic(25, 1);
          ui.label("A terrible error has occurred", gf::UITextAlignment::Left);

          ui.layoutRowDynamic(25, 2);

          if (ui.buttonLabel("OK")) {
            popupActive = false;
            ui.popupClose();
          }

          if (ui.buttonLabel("Cancel")) {
            popupActive = false;
            ui.popupClose();
          }

          ui.popupEnd();
        } else {
          popupActive = false;
        }
      }

      // tooltip

      ui.layoutRowStatic(30, 150, 1);
      bounds = ui.getWidgetBounds();
      ui.label("Hover me for tooltip", gf::UITextAlignment::Left);

      if (ui.isMouseHoveringRect(bounds)) {
        ui.tooltip("This is a tooltip");
      }

      ui.treePop();
    }

  }

  ui.end();
}

int main() {
  gf::Window window("41_ui", { 1024, 768 }, ~gf::WindowHints::Resizable);
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

      if (ui.optionLabel("easy", op == Difficulty::Easy)) {
        op = Difficulty::Easy;
      }

      if (ui.optionLabel("hard", op == Difficulty::Hard)) {
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

    overview(ui);

    renderer.clear();
    renderer.draw(ui);
    renderer.display();
  }

  return 0;
}
