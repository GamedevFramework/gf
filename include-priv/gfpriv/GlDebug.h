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
#ifndef GF_GL_DEBUG_H
#define GF_GL_DEBUG_H

#ifdef GF_DEBUG
  #define GL_CHECK_EXPR(expr) gf::priv::checkedGlCall(expr, __FILE__, __LINE__, #expr)
  #define GL_CHECK(expr) do { (expr); gf::priv::loggedGlCall(__FILE__, __LINE__, #expr); } while (false)
#else
  #define GL_CHECK_EXPR(expr) (expr)
  #define GL_CHECK(expr) (expr)
#endif

namespace gf {
namespace priv {

  void loggedGlCall(const char* file, unsigned int line, const char* expr);

  template<typename T>
  T checkedGlCall(T value, const char* file, unsigned int line, const char* expr) {
    loggedGlCall(file, line, expr);
    return value;
  }

}
}


#endif // GF_GL_DEBUG_H
