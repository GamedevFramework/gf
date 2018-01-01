/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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
  static bool scaleLeft = false;

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
  if (scaleLeft) { windowFlags |= gf::UIWindow::ScaleLeft; }

  if (ui.begin("Overview", gf::RectF(300, 50, 400, 600), windowFlags)) {
    if (showMenu) {
      ui.menubarBegin();

      ui.layoutRowBegin(gf::UILayout::Static, 25, 3);

      ui.layoutRowPush(45);
      if (ui.menuBeginLabel("File", gf::UIAlignment::Left, { 120.0f, 200.0f })) {
        ui.layoutRowDynamic(30, 1);
        ui.menuItemLabel("Open");
        ui.menuItemLabel("Close");
        ui.menuEnd();
      }

      ui.layoutRowPush(45);
      if (ui.menuBeginLabel("Edit", gf::UIAlignment::Left, { 120.0f, 200.0f })) {
        ui.layoutRowDynamic(30, 1);
        ui.menuItemLabel("Copy");
        ui.menuItemLabel("Cut");
        ui.menuItemLabel("Paste");
        ui.menuEnd();
      }

      ui.layoutRowPush(45);
      if (ui.menuBeginLabel("Help", gf::UIAlignment::Left, { 120.0f, 200.0f })) {
        ui.layoutRowDynamic(25, 1);

        if (ui.menuItemLabel("Hide")) {
          showMenu = false;
        }

        if (ui.menuItemLabel("About")) {
          showAppAbout = true;
        }

        ui.menuEnd();
      }

      ui.layoutRowEnd();
      ui.menubarEnd();
    }

    if (showAppAbout) {
      static gf::RectF bounds(20.0f, 100.0f, 300.0f, 190.0f);

      if (ui.popupBegin(gf::UIPopup::Static, "About", gf::UIWindow::Closable, bounds)) {
        ui.layoutRowDynamic(20, 1);
        ui.label("Nuklear");
        ui.label("By Micha Mettke");
        ui.label("Nuklear is licensed under the public domain license.");
        ui.label("Adaptation for gf by Julien Bernard");
        ui.popupEnd();
      } else {
        showAppAbout = false;
      }
    }

    static gf::UICollapse windowCollapsed = gf::UICollapse::Minimized;

    if (ui.treePush(gf::UITree::Tab, "Window", windowCollapsed)) {
      ui.layoutRowDynamic(30, 2);
      ui.checkbox("Titlebar", title);
      ui.checkbox("Menu", showMenu);
      ui.checkbox("Border", border);
      ui.checkbox("Scalable", scalable);
      ui.checkbox("Movable", movable);
      ui.checkbox("No scrollbar", noScrollbar);
      ui.checkbox("Minimizable", minimizable);
      ui.checkbox("Scale left", scaleLeft);
      ui.treePop();
    }

    static gf::UICollapse widgetsCollapsed = gf::UICollapse::Minimized;

    if (ui.treePush(gf::UITree::Tab, "Widgets", widgetsCollapsed)) {

      static gf::UICollapse textCollapsed = gf::UICollapse::Minimized;

      if (ui.treePush(gf::UITree::Node, "Text", textCollapsed)) {
        ui.layoutRowDynamic(20, 1);
        ui.label("Label aligned left", gf::UIAlignment::Left);
        ui.label("Label aligned center", gf::UIAlignment::Center);
        ui.label("Label aligned right", gf::UIAlignment::Right);
        ui.labelColored(gf::Color::Blue, "Blue text");
        ui.labelColored(gf::Color::Yellow, "Yellow text");

        // TODO: multiline text

        ui.treePop();
      }

      static gf::UICollapse buttonCollapsed = gf::UICollapse::Minimized;

      if (ui.treePush(gf::UITree::Node, "Button", buttonCollapsed)) {
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
        ui.buttonSymbol(gf::UISymbol::CircleSolid);
        ui.buttonSymbol(gf::UISymbol::CircleOutline);
        ui.buttonSymbol(gf::UISymbol::RectSolid);
        ui.buttonSymbol(gf::UISymbol::RectOutline);
        ui.buttonSymbol(gf::UISymbol::TriangleUp);
        ui.buttonSymbol(gf::UISymbol::TriangleDown);
        ui.buttonSymbol(gf::UISymbol::TriangleLeft);
        ui.buttonSymbol(gf::UISymbol::TriangleRight);

        ui.layoutRowStatic(30, 100, 2);
        ui.buttonSymbolLabel(gf::UISymbol::TriangleLeft, "prev", gf::UIAlignment::Right);
        ui.buttonSymbolLabel(gf::UISymbol::TriangleRight, "next", gf::UIAlignment::Left);

        ui.treePop();
      }

      static gf::UICollapse basicCollapsed = gf::UICollapse::Minimized;

      if (ui.treePush(gf::UITree::Node, "Basic", basicCollapsed)) {
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
        ui.checkbox("Checkbox", checkbox);

        ui.layoutRowStatic(30, 80, 3);
        option = ui.option("optionA", option == Option::A) ? Option::A : option;
        option = ui.option("optionB", option == Option::B) ? Option::B : option;
        option = ui.option("optionC", option == Option::C) ? Option::C : option;

        ui.layoutRow(gf::UILayout::Static, 30, ratio);

        ui.label("Slider int");
        ui.sliderInt(0, intSlider, 10, 1);

        ui.label("Slider float");
        ui.sliderFloat(0, floatSlider, 5.0f, 0.5f);

        ui.label("Progressbar");
        ui.progress(progValue, 100);

        ui.layoutRow(gf::UILayout::Static, 25, ratio);

        ui.label("Property float:");
        ui.propertyFloat("Float:", 0, propertyFloat, 64.0f, 0.1f, 0.2f);

        ui.label("Property int:");
        ui.propertyInt("Int:", 0, propertyInt, 100, 1, 1);

        ui.label("Property neg:");
        ui.propertyInt("Neg:", -10, propertyNeg, 10, 1, 1);

        ui.layoutRowDynamic(25, 1);
        ui.label("Range:");

        ui.layoutRowDynamic(25, 3);

        ui.propertyFloat("#min", 0, rangeFloatMin, rangeFloatMax, 1.0f, 0.2f);
        ui.propertyFloat("#float", rangeFloatMin, rangeFloatValue, rangeFloatMax, 1.0f, 0.2f);
        ui.propertyFloat("#max", rangeFloatMin, rangeFloatMax, 100.0f, 1.0f, 0.2f);

        ui.propertyInt("#min", 0, rangeIntMin, rangeIntMax, 1, 10);
        ui.propertyInt("#int", rangeIntMin, rangeIntValue, rangeIntMax, 1, 10);
        ui.propertyInt("#max", rangeIntMin, rangeIntMax, 10000, 1, 10);

        ui.treePop();
      }

      static gf::UICollapse selectableCollapsed = gf::UICollapse::Minimized;

      if (ui.treePush(gf::UITree::Node, "Selectable", selectableCollapsed)) {

        static gf::UICollapse listCollapsed = gf::UICollapse::Minimized;

        if (ui.treePush(gf::UITree::Node, "List", listCollapsed)) {
          static bool selected[4] = { false, false, true, false };

          ui.layoutRowStatic(18, 100, 1);

          ui.selectableLabel("Selectable", gf::UIAlignment::Left, selected[0]);
          ui.selectableLabel("Selectable", gf::UIAlignment::Left, selected[1]);
          ui.label("Not Selectable", gf::UIAlignment::Left);
          ui.selectableLabel("Selectable", gf::UIAlignment::Left, selected[2]);
          ui.selectableLabel("Selectable", gf::UIAlignment::Left, selected[3]);

          ui.treePop();
        }

        static gf::UICollapse gridCollapsed = gf::UICollapse::Minimized;

        if (ui.treePush(gf::UITree::Node, "Grid", gridCollapsed)) {
          static bool selected[16] = {
            true, false, false, false,
            false, true, false, false,
            false, false, true, false,
            false, false, false, true
          };

          ui.layoutRowStatic(50, 50, 4);

          for (int i = 0; i < 16; ++i) {
            if (ui.selectableLabel("+", gf::UIAlignment::Center, selected[i])) {
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

      static gf::UICollapse comboCollapsed = gf::UICollapse::Minimized;

      if (ui.treePush(gf::UITree::Node, "Combo", comboCollapsed)) {
        static int currentWeapon = 0;
        static std::vector<std::string> weapons = { "First", "Pistol", "Shotgun", "Plasma", "BFG" };

        static int comboColor1R = 130;
        static int comboColor1G = 50;
        static int comboColor1B = 50;
        static int comboColor1A = 255;
        static gf::Color4f comboColor1 = gf::Color::fromRgba32(comboColor1R, comboColor1G, comboColor1B, comboColor1A);

        static int comboColor2R = 130;
        static int comboColor2G = 180;
        static int comboColor2B = 50;
        static int comboColor2A = 255;
        static gf::Color4f comboColor2 = gf::Color::fromRgba32(comboColor2R, comboColor2G, comboColor2B, comboColor2A);

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

          ui.layoutRow(gf::UILayout::Dynamic, 30, ratio);
          ui.label("R:");
          ui.sliderInt(0, comboColor1R, 255, 5);
          ui.label("G:");
          ui.sliderInt(0, comboColor1G, 255, 5);
          ui.label("B:");
          ui.sliderInt(0, comboColor1B, 255, 5);
          ui.label("A:");
          ui.sliderInt(0, comboColor1A, 255, 5);

          comboColor1 = gf::Color::fromRgba32(comboColor1R, comboColor1G, comboColor1B, comboColor1A);

          ui.comboEnd();
        }

        // complex color combobox

        if (ui.comboBeginColor(comboColor2, { 200.0f, 400.0f })) {
          static ColorMode colorMode = ColorMode::RGB;

          ui.layoutRowDynamic(120, 1);
          ui.colorPicker(comboColor2);

          gf::Color4u localColor = gf::Color::toRgba32(comboColor2);
          comboColor2R = localColor.r;
          comboColor2G = localColor.g;
          comboColor2B = localColor.b;
          comboColor2A = localColor.a;

          ui.layoutRowDynamic(25, 2);
          colorMode = ui.option("RGB", colorMode == ColorMode::RGB) ? ColorMode::RGB : colorMode;
          colorMode = ui.option("HSV", colorMode == ColorMode::HSV) ? ColorMode::HSV : colorMode;

          ui.layoutRowDynamic(25, 1);
          if (colorMode == ColorMode::RGB) {
            ui.propertyInt("#R", 0, comboColor2R, 255, 1, 1);
            ui.propertyInt("#G", 0, comboColor2G, 255, 1, 1);
            ui.propertyInt("#B", 0, comboColor2B, 255, 1, 1);
            ui.propertyInt("#A", 0, comboColor2A, 255, 1, 1);

            comboColor2 = gf::Color::fromRgba32(comboColor2R, comboColor2G, comboColor2B, comboColor2A);
          } else {
            // TODO: RGB/HSV color conversion
          }

          ui.comboEnd();
        }

        // progressbar combobox

        std::size_t sum = progA + progB + progC + progD;

        if (ui.comboBeginLabel(std::to_string(sum), { 200.0f, 200.0f })) {
          ui.layoutRowDynamic(30, 1);
          ui.progress(progA, 100);
          ui.progress(progB, 100);
          ui.progress(progC, 100);
          ui.progress(progD, 100);
          ui.comboEnd();
        }

        // checkbox combobox

        sum = (checkValues[0] ? 1 : 0) + (checkValues[1] ? 1 : 0) + (checkValues[2] ? 1 : 0) + (checkValues[3] ? 1 : 0);

        if (ui.comboBeginLabel(std::to_string(sum), { 200.0f, 200.0f })) {
          ui.layoutRowDynamic(30, 1);
          ui.checkbox(weapons[0], checkValues[0]);
          ui.checkbox(weapons[1], checkValues[1]);
          ui.checkbox(weapons[2], checkValues[2]);
          ui.checkbox(weapons[3], checkValues[3]);
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

      static gf::UICollapse inputCollapsed = gf::UICollapse::Minimized;

      if (ui.treePush(gf::UITree::Node, "Input", inputCollapsed)) {
        static char text[9][64];
        static std::size_t textLength[9];
        static char box[512];
        static std::size_t boxLength;

        ui.layoutRow(gf::UILayout::Static, 25, { 120.0f, 150.0f });

        ui.label("Default:");
        ui.edit(gf::UIEditType::Simple, text[0], textLength[0], gf::UIEditFilter::Default);
        ui.label("Int:");
        ui.edit(gf::UIEditType::Simple, text[1], textLength[1], gf::UIEditFilter::Decimal);
        ui.label("Float:");
        ui.edit(gf::UIEditType::Simple, text[2], textLength[2], gf::UIEditFilter::Float);
        ui.label("Hex:");
        ui.edit(gf::UIEditType::Simple, text[3], textLength[3], gf::UIEditFilter::Hex);
        ui.label("Octal:");
        ui.edit(gf::UIEditType::Simple, text[4], textLength[4], gf::UIEditFilter::Oct);
        ui.label("Binary:");
        ui.edit(gf::UIEditType::Simple, text[5], textLength[5], gf::UIEditFilter::Binary);

        ui.label("Field:");
        ui.edit(gf::UIEditType::Field, text[6], textLength[6]);

        ui.label("Box:");
        ui.layoutRowStatic(180.0f, 278, 1);
        ui.edit(gf::UIEditType::Box, box, boxLength);
        ui.layoutRow(gf::UILayout::Static, 25, { 120.0f, 150.0f });
        gf::UIEditEventFlags flags = ui.edit(gf::UIEditType::Field | gf::UIEdit::SigEnter, text[7], textLength[7], gf::UIEditFilter::Ascii);

        if (ui.buttonLabel("Submit") || flags.test(gf::UIEditEvent::Commited)) {
          std::size_t length = std::min(textLength[7], 512 - boxLength);
          std::strncat(box, text[7], length);
          boxLength += length;
          textLength[7] = 0;
        }

        ui.treePop();
      }

      ui.treePop();
    }

    // TODO: Chart

    static gf::UICollapse popupCollapsed = gf::UICollapse::Minimized;

    if (ui.treePush(gf::UITree::Tab, "Popup", popupCollapsed)) {
      static gf::Color4f color = gf::Color::Red;
      static bool select[4];
      static bool popupActive;
      static bool fileSelectorActive;

      // menu contextual

      ui.layoutRowStatic(30, 150, 1);
      gf::RectF bounds = ui.getWidgetBounds();
      ui.label("Right click me for menu");

      if (ui.contextualBegin(gf::None, { 100.0f, 300.0f }, bounds)) {
        static std::size_t prog = 40;
        static int slider = 10;

        ui.layoutRowDynamic(25, 1);
        ui.checkbox("Menu", showMenu);
        ui.progress(prog, 100);
        ui.sliderInt(0, slider, 16, 1);

        if (ui.contextualItemLabel("About", gf::UIAlignment::Center)) {
          showAppAbout = true;
        }

        ui.selectableLabel(select[0] ? "Unselect" : "Select", gf::UIAlignment::Left, select[0]);
        ui.selectableLabel(select[1] ? "Unselect" : "Select", gf::UIAlignment::Left, select[1]);
        ui.selectableLabel(select[2] ? "Unselect" : "Select", gf::UIAlignment::Left, select[2]);
        ui.selectableLabel(select[3] ? "Unselect" : "Select", gf::UIAlignment::Left, select[3]);

        ui.contextualEnd();
      }

      // color contextual

      ui.layoutRowBegin(gf::UILayout::Static, 30, 2);
      ui.layoutRowPush(100);
      ui.label("Right click here:");
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

      ui.layoutRowBegin(gf::UILayout::Static, 30, 2);
      ui.layoutRowPush(100);
      ui.label("Popup:");
      ui.layoutRowPush(50);

      if (ui.buttonLabel("Popup")) {
        popupActive = true;
      }

      ui.layoutRowEnd();

      if (popupActive) {
        static gf::RectF bounds(20.0f, 100.0f, 220.0f, 90.0f);

        if (ui.popupBegin(gf::UIPopup::Static, "Error", gf::None, bounds)) {
          ui.layoutRowDynamic(25, 1);
          ui.label("A terrible error has occurred");

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

      ui.layoutRowBegin(gf::UILayout::Static, 30, 2);
      ui.layoutRowPush(100);
      ui.label("File selector:");
      ui.layoutRowPush(50);

      if (ui.buttonLabel("Open")) {
        fileSelectorActive = true;
      }

      ui.layoutRowEnd();

      static gf::UIBrowser browser;

      if (fileSelectorActive) {
        static gf::RectF bounds(20.0f, 100.0f, 500.0f, 500.0f);

        if (!ui.fileSelector(browser, "Choose a file", bounds)) {
          fileSelectorActive = false;
        }

      }

      ui.layoutRowDynamic(30, 2);
      ui.label("Selected file:");
      ui.label(browser.selectedPath.filename().string());

      // tooltip

      ui.layoutRowStatic(30, 150, 1);

      if (ui.isWidgetHovered()) {
        ui.tooltip("This is a tooltip");
      }

      ui.label("Hover me for tooltip");


      ui.treePop();
    }

    static gf::UICollapse layoutCollapsed = gf::UICollapse::Minimized;

    if (ui.treePush(gf::UITree::Tab, "Layout", layoutCollapsed)) {

      static gf::UICollapse widgetCollapsed = gf::UICollapse::Minimized;

      if (ui.treePush(gf::UITree::Node, "Widget", widgetCollapsed)) {
        float ratioTwo[] = { 0.2f, 0.6f, 0.2f };
        float widthTwo[] = { 100.0f, 200.0f, 50.0f };

        ui.layoutRowDynamic(30, 1);
        ui.label("Dynamic fixed column layout with generated position and size:");
        ui.layoutRowDynamic(30, 3);
        ui.buttonLabel("button");
        ui.buttonLabel("button");
        ui.buttonLabel("button");

        ui.layoutRowDynamic(30, 1);
        ui.label("Static fixed column layout with generated position and size:");
        ui.layoutRowStatic(30, 100, 3);
        ui.buttonLabel("button");
        ui.buttonLabel("button");
        ui.buttonLabel("button");

        ui.layoutRowDynamic(30, 1);
        ui.label("Dynamic array-based custom column layout with generated position and custom size:");
        ui.layoutRow(gf::UILayout::Dynamic, 30, ratioTwo);
        ui.buttonLabel("button");
        ui.buttonLabel("button");
        ui.buttonLabel("button");

        ui.layoutRowDynamic(30, 1);
        ui.label("Static array-based custom column layout with generated position and custom size:");
        ui.layoutRow(gf::UILayout::Static, 30, widthTwo);
        ui.buttonLabel("button");
        ui.buttonLabel("button");
        ui.buttonLabel("button");

        ui.layoutRowDynamic(30, 1);
        ui.label("Dynamic immediate mode custom column layout with generated position and custom size:");
        ui.layoutRowBegin(gf::UILayout::Dynamic, 30, 3);
        ui.layoutRowPush(0.2f);
        ui.buttonLabel("button");
        ui.layoutRowPush(0.6f);
        ui.buttonLabel("button");
        ui.layoutRowPush(0.2f);
        ui.buttonLabel("button");
        ui.layoutRowEnd();

        ui.layoutRowDynamic(30, 1);
        ui.label("Static immediate mode custom column layout with generated position and custom size:");
        ui.layoutRowBegin(gf::UILayout::Static, 30, 3);
        ui.layoutRowPush(100.0f);
        ui.buttonLabel("button");
        ui.layoutRowPush(200.0f);
        ui.buttonLabel("button");
        ui.layoutRowPush(50.0f);
        ui.buttonLabel("button");
        ui.layoutRowEnd();

        ui.treePop();
      }

      static gf::UICollapse groupCollapsed = gf::UICollapse::Minimized;

      if (ui.treePush(gf::UITree::Node, "Group", groupCollapsed)) {
        static bool groupTitlebar = false;
        static bool groupBorder = true;
        static bool groupNoScrollbar = false;
        static int groupWidth = 320;
        static int groupHeight = 200;

        gf::UIWindowFlags groupFlags = gf::None;

        if (groupTitlebar) { groupFlags |= gf::UIWindow::Title; }
        if (groupBorder) { groupFlags |= gf::UIWindow::Border; }
        if (groupNoScrollbar) { groupFlags |= gf::UIWindow::NoScrollbar; }

        ui.layoutRowDynamic(30, 3);
        ui.checkbox("Titlebar", groupTitlebar);
        ui.checkbox("Border", groupBorder);
        ui.checkbox("No Scrollbar", groupNoScrollbar);

        ui.layoutRowBegin(gf::UILayout::Static, 22, 3);
        ui.layoutRowPush(50);
        ui.label("Size:");
        ui.layoutRowPush(130);
        ui.propertyInt("Width:", 100, groupWidth, 500, 10, 1);
        ui.layoutRowPush(130);
        ui.propertyInt("Height:", 100, groupHeight, 500, 10, 1);
        ui.layoutRowEnd();

        ui.layoutRowStatic(groupHeight, groupWidth, 2);

        if (ui.groupBegin("Group", groupFlags)) {
          static bool selected[16];

          ui.layoutRowStatic(18, 100, 1);

          for (int i = 0; i < 16; ++i) {
            ui.selectableLabel(selected[i] ? "Selected" : "Unselected", gf::UIAlignment::Center, selected[i]);
          }

          ui.groupEnd();
        }

        ui.treePop();
      }



      ui.treePop();
    }
  }

  ui.end();
}

int main() {
  gf::Window window("40_ui", { 1024, 768 }, ~gf::WindowHints::Resizable);
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
  int selectedStyle = 0;

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

      ui.processEvent(event);
    }

    if (ui.begin("Show", gf::RectF(50, 50, 220, 220), gf::UIWindow::Border | gf::UIWindow::Movable | gf::UIWindow::Scalable | gf::UIWindow::Closable | gf::UIWindow::Minimizable | gf::UIWindow::Title)) {

      /* fixed widget pixel width */

      ui.layoutRowStatic(30, 80, 1);

      if (ui.buttonLabel("button")) {
        std::cout << "Hello!\n";
      }

      /* fixed widget window ratio width */

      ui.layoutRowDynamic(30, 2);

      if (ui.option("easy", op == Difficulty::Easy)) {
        op = Difficulty::Easy;
      }

      if (ui.option("hard", op == Difficulty::Hard)) {
        op = Difficulty::Hard;
      }

      /* custom widget pixel width */

      ui.layoutRowBegin(gf::UILayout::Static, 30, 2);
      ui.layoutRowPush(50);
      ui.label("Volume:");
      ui.layoutRowPush(110);
      if (ui.sliderFloat(0.0f, value, 1.0f, 0.1f)) {
        std::cout << "Value: " << value << '\n';
      }
      ui.layoutRowEnd();


    }

    ui.end();

    if (ui.begin("Style", gf::RectF(50, 300, 220, 100), gf::UIWindow::Border | gf::UIWindow::Movable | gf::UIWindow::Scalable | gf::UIWindow::Closable | gf::UIWindow::Minimizable | gf::UIWindow::Title)) {
      ui.layoutRowDynamic(20, 1);
      ui.label("Style:");
      ui.comboboxSeparator("Default|White|Red|Blue|Dark", '|', selectedStyle, 20, { 220, 220 });

      gf::UIPredefinedStyle style = static_cast<gf::UIPredefinedStyle>(selectedStyle);
      ui.setPredefinedStyle(style);
    }

    ui.end();

    overview(ui);

    renderer.clear();
    renderer.draw(ui);
    renderer.display();
  }

  return 0;
}
