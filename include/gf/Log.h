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
#ifndef GF_LOG_H
#define GF_LOG_H

#include <cstdarg>
#include <cstdlib>
#include <map>

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief Logging functions
   *
   * Logging functions are used to report some events that have happened in
   * the system. These functions print messages to the standard error. It uses
   * `printf`-like format strings.
   *
   * A message is associated to a severity level. The severity level indicates
   * the severity of the message and range from a simple debug message to a
   * fatal message.
   *
   * You can set the minimum severity for which messages are displayed. By
   * default, the minimum severity level is gf::Log::Warning (or
   * gf::Log::Debug if debug mode is activated at build time).
   *
   * Example:
   *
   * ~~~{.cc}
   * int entity = ...;
   * gf::Log::warning("Something weird happened to entity %i!\n", entity);
   * ~~~
   *
   */
  class GF_API Log {
  public:
    /**
     * @brief Deleted constructor
     */
    Log() = delete;

    /**
     * @brief The severity level of the log
     */
    enum Level : int {
      Debug,  ///< The debug level
      Info,   ///< The info level
      Warn,   ///< The warn level
      Error,  ///< The error level
      Fatal,  ///< The fatal level
    };

    /**
     * @brief Set a global severity level
     *
     * @param level The severity level
     */
    static void setLevel(Level level);

    /**
     * @brief Print a debug message
     *
     * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
     */
    static void debug(const char *fmt, ...) GF_FORMAT(1, 2) {
      va_list ap;
      va_start(ap, fmt);
      log(Level::Debug, fmt, ap);
      va_end(ap);
    }

    /**
     * @brief Print an info message
     *
     * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
     */
    static void info(const char *fmt, ...) GF_FORMAT(1, 2) {
      va_list ap;
      va_start(ap, fmt);
      log(Level::Info, fmt, ap);
      va_end(ap);
    }

    /**
     * @brief Print a warning message
     *
     * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
     */
    static void warning(const char *fmt, ...) GF_FORMAT(1, 2) {
      va_list ap;
      va_start(ap, fmt);
      log(Level::Warn, fmt, ap);
      va_end(ap);
    }

    /**
     * @brief Print an error message
     *
     * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
     */
    static void error(const char *fmt, ...) GF_FORMAT(1, 2) {
      va_list ap;
      va_start(ap, fmt);
      log(Level::Error, fmt, ap);
      va_end(ap);
    }

    /**
     * @brief Print a fatal message and quit
     *
     * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
     */
    static void fatal(const char *fmt, ...) GF_FORMAT(1, 2) {
      va_list ap;
      va_start(ap, fmt);
      log(Level::Fatal, fmt, ap);
      va_end(ap);

      std::abort();
    }

  private:
    static Level s_level;

  private:
    static void log(Level level, const char *fmt, va_list ap);
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_LOG_H
