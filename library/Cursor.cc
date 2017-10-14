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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#include <gf/Cursor.h>

#include <cassert>

#include <SDL.h>

#include <gf/Image.h>
#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif


  Cursor::Cursor()
  : m_cursor(nullptr)
  {

  }

  Cursor::Cursor(Cursor&& other)
  : m_cursor(other.m_cursor)
  {
    other.m_cursor = nullptr;
  }

  Cursor& Cursor::operator=(Cursor&& other) {
    m_cursor = other.m_cursor;
    other.m_cursor = nullptr;
    return *this;
  }

  Cursor::~Cursor() {
    if (m_cursor != nullptr) {
      SDL_FreeCursor(m_cursor);
    }
  }

  bool Cursor::loadFromPixels(const uint8_t* pixels, Vector2u size, Vector2u hotspot) {
    if (pixels == nullptr || size.width == 0 || size.height == 0) {
      return false;
    }

    if (m_cursor != nullptr) {
      SDL_FreeCursor(m_cursor);
      m_cursor = nullptr;
    }

    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xFF000000;
    gmask = 0x00FF0000;
    bmask = 0x0000FF00;
    amask = 0x000000FF;
#else // little endian, like x86
    rmask = 0x000000FF;
    gmask = 0x0000FF00;
    bmask = 0x00FF0000;
    amask = 0xFF000000;
#endif

    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(const_cast<uint8_t*>(pixels), size.width, size.height, 32, 4 * size.width,
        rmask, gmask, bmask, amask);

    if (surface == nullptr) {
      Log::error("Could not create surface for cursor: %s", SDL_GetError());
      return false;
    }

    m_cursor = SDL_CreateColorCursor(surface, hotspot.x, hotspot.y);
    SDL_FreeSurface(surface);

    if (m_cursor == nullptr) {
      Log::error("Could not load cursor: '%s'\n", SDL_GetError());
      return false;
    }

    return true;
  }

  bool Cursor::loadFromImage(const Image& image, Vector2u hotspot) {
    return loadFromPixels(image.getPixelsPtr(), image.getSize(), hotspot);
  }

  static SDL_SystemCursor getSDLSystemCursor(Cursor::Type type) {
    switch (type) {
      case Cursor::Arrow:
        return SDL_SYSTEM_CURSOR_ARROW;
      case Cursor::ArrowWait:
        return SDL_SYSTEM_CURSOR_WAITARROW;
      case Cursor::Wait:
        return SDL_SYSTEM_CURSOR_WAIT;
      case Cursor::Text:
        return SDL_SYSTEM_CURSOR_IBEAM;
      case Cursor::Hand:
        return SDL_SYSTEM_CURSOR_HAND;
      case Cursor::SizeHorizontal:
        return SDL_SYSTEM_CURSOR_SIZEWE;
      case Cursor::SizeVertical:
        return SDL_SYSTEM_CURSOR_SIZENS;
      case Cursor::SizeTopLeftBottomRight:
        return SDL_SYSTEM_CURSOR_SIZENWSE;
      case Cursor::SizeBottomLeftTopRight:
        return SDL_SYSTEM_CURSOR_SIZENESW;
      case Cursor::SizeAll:
        return SDL_SYSTEM_CURSOR_SIZEALL;
      case Cursor::Cross:
        return SDL_SYSTEM_CURSOR_CROSSHAIR;
      case Cursor::NotAllowed:
        return SDL_SYSTEM_CURSOR_NO;
    }

    assert(false);
    return SDL_SYSTEM_CURSOR_ARROW;
  }

  bool Cursor::loadFromSystem(Type type) {
    if (m_cursor != nullptr) {
      SDL_FreeCursor(m_cursor);
      m_cursor = nullptr;
    }

    m_cursor = SDL_CreateSystemCursor(getSDLSystemCursor(type));

    if (m_cursor == nullptr) {
      Log::error("Could not load system cursor: '%s'\n", SDL_GetError());
      return false;
    }

    return true;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


