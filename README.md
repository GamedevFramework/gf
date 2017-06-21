# Gamedev Framework (gf)

__Gamedev Framework (gf)__ is a framework to build 2D games in C++11. It is based on [SDL](https://www.libsdl.org/) and [OpenGL ES 2.0](https://www.khronos.org/opengles/2_X/), and presents an API that is very similar to the graphics module of [SFML](http://www.sfml-dev.org/) with additional features. It is _not_ a [game engine](https://en.wikipedia.org/wiki/Game_engine), it is more something like a framework, similar to [libGDX](https://libgdx.badlogicgames.com/) in the Java world.

Gamedev Framework (gf) is licensed under the terms and conditions of the [zlib/libpng license](https://opensource.org/licenses/Zlib).

## Documentation

See the [online documentation for Gamedev Framework (gf)](http://gamedevframework.github.io/).

## Contact

If you want to talk directly with the developpers, you can join the [#gf-devel IRC channel on Freenode](irc://chat.freenode.net/gf-devel).

## Contribution

Gamedev Framework (gf) is very open to contributions. See [the contribution guidelines](CONTRIBUTING.md).

## Build status

| Service | Status |
|---------|--------|
| Travis CI | [![Build Status](https://travis-ci.org/GamedevFramework/gf.svg?branch=develop)](https://travis-ci.org/GamedevFramework/gf) |
| AppVeyor | [![Build status](https://ci.appveyor.com/api/projects/status/orf21jbkpoweigjf?svg=true)](https://ci.appveyor.com/project/jube/gf) |

## Quick install

You have to install the following dependencies:

- [SDL2](https://www.libsdl.org/) >= 2.0.4 (zlib/libpng license)
- [Boost Filesystem](http://www.boost.org/doc/libs/release/libs/filesystem/) >= 1.55 (Boost license)
- [Boost String Algorithms](http://www.boost.org/doc/libs/release/libs/algorithm/string/) >= 1.55 (Boost license)
- [Freetype](http://freetype.org/) >= 2.5.2 (FreeType license)
- [CMake](https://cmake.org/) >= 3.0.2

For Debian/Ubuntu:

```sh
apt-get install libsdl2-dev libboost-filesystem-dev libboost-dev libfreetype6-dev cmake
```

Then:

```sh
git clone https://github.com/GamedevFramework/gf.git
cd gf
git submodule update --init
mkdir build
cd build
cmake ../
make
```

An finally:

```sh
make install # may require root permissions
```
