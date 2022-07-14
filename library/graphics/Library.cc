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
#include <gf/Library.h>

#include <atomic>
#include <cassert>
#include <cstdio>

#include <SDL.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <boost/version.hpp>

#include <gf/Log.h>

#include <gfpriv/SdlDebug.h>

#include "config.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  static std::atomic_int g_loaded{0};

  Library::Library()
  {

    if (g_loaded.fetch_add(1) == 0) { // we are the first
      if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        // do not use SDL_CHECK here, it has a bug with udev on Linux
        Log::error("Unable to initialize SDL: '%s'\n", SDL_GetError());
        return;
      }

      SDL_ClearError();

#ifdef GF_OPENGL3
      SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));
      SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
      SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3));
#else
      SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES));
      SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2));
      SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0));
#endif
    }
  }

  Library::Library([[maybe_unused]] Library&& other) noexcept
  {
    g_loaded.fetch_add(1);
  }

  Library& Library::operator=([[maybe_unused]] Library&& other) noexcept {
    g_loaded.fetch_add(1);
    return *this;
  }

  Library::~Library() {
    if (g_loaded.fetch_sub(1) == 1) { // we are the last
      SDL_Quit();
    }
  }

  Library::Version Library::getVersion() {
    Version version;
    version.major = GF_VERSION_MAJOR;
    version.minor = GF_VERSION_MINOR;
    version.patch = GF_VERSION_PATCH;
    return version;
  }

  namespace {

    Library::Version getSDLCompiledVersion() {
      SDL_version compiled;
      SDL_VERSION(&compiled);

      Library::Version version;
      version.major = compiled.major;
      version.minor = compiled.minor;
      version.patch = compiled.patch;
      return version;
    }

    Library::Version getSDLLinkedVersion() {
      SDL_version linked;
      SDL_GetVersion(&linked);

      Library::Version version;
      version.major = linked.major;
      version.minor = linked.minor;
      version.patch = linked.patch;
      return version;
    }

    Library::Version getFreeTypeCompiledVersion() {
      Library::Version version;
      version.major = FREETYPE_MAJOR;
      version.minor = FREETYPE_MINOR;
      version.patch = FREETYPE_PATCH;
      return version;
    }

    Library::Version getFreeTypeLinkedVersion() {
      FT_Library library;
      FT_Init_FreeType(&library);
      FT_Int major, minor, patch;
      FT_Library_Version(library, &major, &minor, &patch);
      FT_Done_FreeType(library);

      Library::Version version;
      version.major = major;
      version.minor = minor;
      version.patch = patch;
      return version;
    }

    Library::Version getBoostVersion() {
      Library::Version version;
      version.major = BOOST_VERSION / 100000;
      version.minor = BOOST_VERSION / 100 % 1000;
      version.patch = BOOST_VERSION % 100;
      return version;
    }

  } // anonymous namespace

  std::tuple<Library::Version, Library::Version> Library::getDependencyVersion(Dependency dep) {
    switch (dep) {
      case Boost:
        return std::make_tuple(getBoostVersion(), getBoostVersion());
      case SimpleDirectMediaLayer:
        return std::make_tuple(getSDLCompiledVersion(), getSDLLinkedVersion());
      case FreeType:
        return std::make_tuple(getFreeTypeCompiledVersion(), getFreeTypeLinkedVersion());
    }

    assert(false);
    return std::make_tuple<Version, Version>({ 0, 0, 0 }, { 0, 0, 0 });
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
