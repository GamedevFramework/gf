#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/Grid.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>

int main() {
  gf::Window window("GFBattleship", { 500, 500});
  gf::RenderWindow renderer(window);
  gf::SquareGrid grid(gf::Vector2u(50,50),gf::Vector2f(10.0f,10.0f) , gf::Color::White ,1.0f);

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;
      }
    }

    renderer.clear();
    renderer.draw(grid);
    renderer.display();
  }

  return 0;
}
