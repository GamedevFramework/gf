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

#include <chrono>
#include <utility>

#include "config.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  // default value
  Log::Level Log::s_level =
#if GF_DEBUG
    Log::Debug
#else
    Log::Warn
#endif
  ;

  void Log::setLevel(Level level) {
    s_level = level;
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

  void Log::log(Level level, const char *fmt, va_list ap) {
    if (level < s_level) {
      return;
    }

    auto now = std::chrono::system_clock::now();
    auto epoch = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
    std::time_t integerPart = seconds.count();

    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(epoch) % std::chrono::seconds(1);
    long fractionalPart = microseconds.count();

    static constexpr std::size_t BufferSize = 1024;
    char buffer[BufferSize];
    std::size_t size = std::strftime(buffer, BufferSize, "%F %T", std::localtime(&integerPart));
    std::snprintf(buffer + size, BufferSize - size, ".%06ld", fractionalPart);

    std::fprintf(stderr, "[%s][%s] ", buffer, getStringFromLevel(level));
    std::vfprintf(stderr, fmt, ap);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
