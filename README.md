# Gamedev Framework (gf)

[![Travis](https://img.shields.io/travis/GamedevFramework/gf.svg?style=flat-square)](https://travis-ci.org/GamedevFramework/gf)
[![AppVeyor](https://img.shields.io/appveyor/ci/jube/gf.svg?style=flat-square)](https://ci.appveyor.com/project/jube/gf)
[![GitHub license](https://img.shields.io/badge/license-zlib-blue.svg?style=flat-square)](https://raw.githubusercontent.com/GamedevFramework/gf/master/LICENSE)
[![GitHub tag](https://img.shields.io/github/tag/GamedevFramework/gf.svg?style=flat-square)](https://github.com/GamedevFramework/gf/tags)

__Gamedev Framework (gf)__ is a framework to build 2D games in C++14. It is based on [SDL](https://www.libsdl.org/) and [OpenGL ES 2.0](https://www.khronos.org/opengles/2_X/), and presents an API that is very similar to the graphics module of [SFML](http://www.sfml-dev.org/) with additional features. It is _not_ a [game engine](https://en.wikipedia.org/wiki/Game_engine), it is more something like a framework, similar to [libGDX](https://libgdx.badlogicgames.com/) in the Java world.

Gamedev Framework (gf) is licensed under the terms and conditions of the [zlib/libpng license](https://opensource.org/licenses/Zlib).

![Gamedev Framework (gf)](gf_logo.png)

## Quick example

```cpp
#include <gf/Event.h>
#include <gf/Font.h>
#include <gf/RenderWindow.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Window.h>

int main() {
  // Create the main window and the renderer

  gf::Window window("Example", { 640, 480 });
  gf::RenderWindow renderer(window);

  // Load a sprite to display

  gf::Texture texture("sprite.png");

  gf::Sprite sprite(texture);
  sprite.setPosition({ 300, 200 });

  // Create a graphical text to display

  gf::Font font("DroidSans.ttf");

  gf::Text text("Hello gf!", font, 50);
  text.setPosition({ 100, 100 });

  renderer.clear(gf::Color::White);

  // Start the game loop

  while (window.isOpen()) {

    // Process events

    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        default:
          break;
      }
    }

    // Draw the entities

    renderer.clear();
    renderer.draw(sprite);
    renderer.draw(text);
    renderer.display();
  }

  return 0;
}
```

## Documentation

See the [online documentation for Gamedev Framework (gf)](http://gamedevframework.github.io/).

## Packaging

[![Packaging status](https://repology.org/badge/vertical-allrepos/gamedev-framework.svg)](https://repology.org/project/gamedev-framework/versions)

## Contact

If you want to talk directly with the developpers, you can

- chat on the [Discord server of gf](https://discord.gg/2fXM3T4)
- post a message on [/r/GamedevFramework](https://www.reddit.com/r/GamedevFramework/).
- join the [#gf-devel IRC channel on Freenode](irc://chat.freenode.net/gf-devel)

## Contribution

Gamedev Framework (gf) is very open to contributions. See [the contribution guidelines](CONTRIBUTING.md).

## Dependencies

You have to install the following dependencies:

- [SDL2](https://www.libsdl.org/) >= 2.0.8 (zlib/libpng license)
- [Boost Filesystem](http://www.boost.org/doc/libs/release/libs/filesystem/) >= 1.65 (Boost license)
- [Boost String Algorithms](http://www.boost.org/doc/libs/release/libs/algorithm/string/) >= 1.65 (Boost license)
- [Boost Heap](http://www.boost.org/doc/libs/release/libs/heap/) >= 1.65 (Boost license)
- [Freetype](http://freetype.org/) >= 2.9.1 (FreeType license)
- [zlib](https://zlib.net/) >= 1.2.11 (zlib/libpng license)
- [pugixml](https://pugixml.org/) >= 1.8.1 (MIT licence)
- [CMake](https://cmake.org/) >= 3.10.2

See the [documentation](http://gamedevframework.github.io/latest/build_and_install.html) for more information on how to build and install gf.

## Screenshots

These screenshots are from games included in the repository.

### Games

![gf Pong!](games/gf_pong/gf_pong.png)
![gf 2048!](games/gf_2048/gf_2048.png)

## Code Quality

- LGTM: [![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/GamedevFramework/gf.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/GamedevFramework/gf/context:cpp)
- CodeFactor: [![CodeFactor](https://www.codefactor.io/repository/github/gamedevframework/gf/badge)](https://www.codefactor.io/repository/github/gamedevframework/gf)
- Codacy: [![Codacy Badge](https://api.codacy.com/project/badge/Grade/991cd949d3d74b3a9052be89d7b42541)](https://www.codacy.com/app/jube/gf?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=GamedevFramework/gf&amp;utm_campaign=Badge_Grade)

