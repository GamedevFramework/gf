#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>

int main() {
  gf::Window window0("window0", {640, 480}, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer0(window0);

  gf::Window window1("window1", {640, 480}, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer1(window1);

  while (window0.isOpen() || window1.isOpen()) {
    if(window0.isOpen()) {
      gf::Event event;

      while (window0.pollEvent(event)) {
        switch (event.type) {
          case gf::EventType::Closed:
            window0.close();
            break;
          case gf::EventType::KeyPressed:
            if (event.key.scancode == gf::Scancode::Escape) {
              window0.close();
            }
            break;
          default:
            break;
        }
      }

      renderer0.setActive();
      renderer0.clear(gf::Color::Red);
      renderer0.display();
    }

    if(window1.isOpen()) {
      gf::Event event;

      while (window1.pollEvent(event)) {
        switch (event.type) {
          case gf::EventType::Closed:
            window1.close();
            break;
          case gf::EventType::KeyPressed:
            if (event.key.scancode == gf::Scancode::Escape) {
              window1.close();
            }
            break;
          default:
            break;
        }
      }

      renderer1.setActive();
      renderer1.clear(gf::Color::Blue);
      renderer1.display();
    }
  }

  return 0;
}
