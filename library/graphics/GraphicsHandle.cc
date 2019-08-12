/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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
#include <gf/GraphicsHandle.h>

#include <cassert>
#include <utility>

#include <gf/Log.h>

#include "priv/OpenGLFwd.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  static_assert(std::is_same<GLsizei, int>::value, "GLsizei and int should be the same.");
  static_assert(std::is_same<GLuint, unsigned>::value, "GLuint and unsigned should be the same.");

  GraphicsHandle::GraphicsHandle(Generator gen, Deletor del)
  : m_del(del)
  , m_name(0)
  {
    gen(1, &m_name);

    if (m_name == 0) {
      Log::error("A handle could not be generated.\n");
    }
  }

  GraphicsHandle::~GraphicsHandle() noexcept {
    if (m_name != 0) {
      m_del(1, &m_name);
    }
  }

  GraphicsHandle::GraphicsHandle(GraphicsHandle&& other) noexcept
  : m_name(std::exchange(other.m_name, 0))
  {
    assert(m_del == other.m_del);
  }

  GraphicsHandle& GraphicsHandle::operator=(GraphicsHandle&& other) noexcept {
    std::swap(m_name, other.m_name);
    assert(m_del == other.m_del);
    return *this;
  }

  unsigned GraphicsHandle::getName() const noexcept {
    return m_name;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


