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
#include <gf/Log.h>

#include <cassert>
#include <cstdio>
#include <ctime>
#include <utility>

#include "config.h"

namespace gf {
inline namespace v1 {

  // default values
  std::map<Log::Category, Log::Level> Log::s_levels = {
#if GF_DEBUG
    { Log::General, Log::Debug },
    { Log::Graphics, Log::Debug },
    { Log::Network, Log::Debug },
    { Log::Physics, Log::Debug },
    { Log::Resources, Log::Debug },
#else
    { Log::General, Log::Warn },
    { Log::Graphics, Log::Warn },
    { Log::Network, Log::Warn },
    { Log::Physics, Log::Warn },
    { Log::Resources, Log::Warn },
#endif
  };

  void Log::setLevel(Level level) {
    for (auto& item : s_levels) {
      item.second = level;
    }
  }

  void Log::setLevel(Category category, Level level) {
    s_levels[category] = level;
  }

  static const char *getStringFromLevel(Log::Level level) {
    switch (level) {
      case Log::Debug:
        return "Debug";

      case Log::Info:
        return "Info";

      case Log::Warn:
        return "Warn";

      case Log::Error:
        return "Error";

      case Log::Fatal:
        return "Fatal";
    }

    assert(false);
    return "?";
  }

  static const char *getStringFromCategory(Log::Category category) {
    switch (category) {
      case Log::General:
        return "General";

      case Log::Graphics:
        return "Graphics";

      case Log::Network:
        return "Network";

      case Log::Physics:
        return "Physics";

      case Log::Resources:
        return "Resources";
    }

    assert(false);
    return "?";
  }

  void Log::printDebug(const char *fmt, ...) {
#ifdef GF_DEBUG
    va_list ap;
    va_start(ap, fmt);
    std::vfprintf(stderr, fmt, ap);
    va_end(ap);
#endif
  }

  void Log::log(Level level, Category category, const char *fmt, va_list ap) {
    if (level < s_levels[category]) {
      return;
    }

    unsigned long t = std::time(nullptr);
    std::fprintf(stderr, "[%lu][%s][%s] ", t, getStringFromLevel(level), getStringFromCategory(category));

    std::vfprintf(stderr, fmt, ap);
  }

}
}
