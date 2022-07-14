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

#include <gf/Color.h>
#include <gf/Console.h>
#include <gf/ConsoleChar.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/VectorOps.h>
#include <gf/Window.h>

int main() {
  static constexpr float Scale = 2.0f;
  static constexpr gf::Vector2i CharacterSize(12, 12);
  static constexpr gf::Vector2i ConsoleSize(80, 50);

  const gf::Vector2i ScreenSize = CharacterSize * ConsoleSize * Scale;

  gf::Window window("27_console", ScreenSize, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  std::cout << "Gamedev Framework (gf) example #27: Console\n";
  std::cout << "This example shows a virtual console.\n";
  std::cout << "How to use:\n";
//   std::cout << "\tM: change mode between field of vision and route finding\n";
//   std::cout << "\tEscape: Close the window\n";

  gf::BitmapConsoleFont font("assets/terminal.png", { gf::ConsoleFontFormat::Grayscale, gf::ConsoleFontFormat::InRow, gf::ConsoleFontFormat::ModifiedCodePage437 });
  assert(CharacterSize == font.getCharacterSize());

  gf::ConsoleStyle style;
  style.background = gf::Color::Gray(0.25f);

  gf::Console console(font, ConsoleSize);

  console.putChar({ 40, 25 }, '@', style);
  console.putChar({ 42, 25 }, gf::ConsoleChar::WhiteSmilingFace, style);

  console.setColorControl(gf::ConsoleColorControl1, gf::Color::Red, gf::Color::Black);
  console.print({ 1, 1 }, style, "String with a %cred%c word.", gf::ConsoleColorControl1, gf::ConsoleColorControlStop);

  style.alignment = gf::ConsoleAlignment::Right;
  console.setColorControl(gf::ConsoleColorControl2, gf::Color::Orange, gf::Color::Azure);
  console.print(ConsoleSize - 2, style, "Made with %cgf%c!", gf::ConsoleColorControl2, gf::ConsoleColorControlStop);

  console.setColorControl(gf::ConsoleColorControl3, gf::Color::Black, gf::Color::Yellow);
  const char *text = "This is a simple but long text with %cmultiple%c lines.";

  style.alignment = gf::ConsoleAlignment::Left;
  console.printRect(gf::RectI::fromPositionSize({ 2,  5 }, { 16, 5 }), style, text, gf::ConsoleColorControl3, gf::ConsoleColorControlStop);
  style.alignment = gf::ConsoleAlignment::Center;
  console.printRect(gf::RectI::fromPositionSize({ 2, 15 }, { 16, 5 }), style, text, gf::ConsoleColorControl3, gf::ConsoleColorControlStop);
  style.alignment = gf::ConsoleAlignment::Right;
  console.printRect(gf::RectI::fromPositionSize({ 2, 25 }, { 16, 5 }), style, text, gf::ConsoleColorControl3, gf::ConsoleColorControlStop);

  console.drawFrame(gf::RectI::fromPositionSize({ 30,  5 }, { 16, 5 }), style);
  console.drawFrame(gf::RectI::fromPositionSize({ 30, 15 }, { 16, 5 }), style, gf::Console::PrintAction::None, "Frame title");

  console.scale(Scale);

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

            default:
              break;
          }
          break;

        default:
          break;
      }
    }

    renderer.clear();

    renderer.draw(console);

    renderer.display();
  }

  return 0;
}
