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
#include <gf/Monitor.h>

#include <cassert>

#include <SDL.h>

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  static void sdlCheckError(int err) {
    if (err == 0) {
      return;
    }

    const char *message = SDL_GetError();

    if (*message) {
      Log::error("SDL error: %s\n", message);
      SDL_ClearError();
    }
  }

  Monitor Monitor::getPrimaryMonitor() {
    Library lib;
    return Monitor(0); // 0 is just a guess
  }

  std::vector<Monitor> Monitor::getAvailableMonitors() {
    Library lib;
    std::vector<Monitor> list;

    int count = SDL_GetNumVideoDisplays();

    for (int i = 0; i < count; ++i) {
      list.push_back(Monitor(i));
    }

    return list;
  }

  Monitor::Monitor(int index)
  : m_index(index)
  {

  }

  std::string Monitor::getName() const {
    return std::string(SDL_GetDisplayName(m_index));
  }

  Vector2i Monitor::getPosition() const {
    SDL_Rect rect;
    int err = SDL_GetDisplayBounds(m_index, &rect);
    sdlCheckError(err);
    return { rect.x, rect.y };
  }

  Vector2u Monitor::getPhysicalSize() const {
    SDL_Rect rect;
    int err = SDL_GetDisplayBounds(m_index, &rect);
    sdlCheckError(err);
    return { static_cast<unsigned>(rect.w), static_cast<unsigned>(rect.h) };
  }

  std::vector<VideoMode> Monitor::getAvailableVideoModes() const {
    std::vector<VideoMode> list;

    int count = SDL_GetNumDisplayModes(m_index);

    for (int i = 0; i < count; ++i) {
      SDL_DisplayMode mode;
      int err = SDL_GetDisplayMode(m_index, i, &mode);
      sdlCheckError(err);

      if (err == 0) {
        list.push_back({
          { static_cast<unsigned>(mode.w), static_cast<unsigned>(mode.h) },
          SDL_BITSPERPIXEL(mode.format),
          mode.refresh_rate
        });
      }
    }

    return list;
  }

  VideoMode Monitor::getCurrentVideoMode() const {
    SDL_DisplayMode mode;
    int err = SDL_GetCurrentDisplayMode(m_index, &mode);
    sdlCheckError(err);

    return VideoMode{
      { static_cast<unsigned>(mode.w), static_cast<unsigned>(mode.h) },
      SDL_BITSPERPIXEL(mode.format),
      mode.refresh_rate
    };
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
