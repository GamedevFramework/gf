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
#ifndef GF_GRAPHICS_HANDLE_H
#define GF_GRAPHICS_HANDLE_H

#include <utility>

#include "Portability.h"
#include "Types.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  enum class GraphicsTag {
    Buffer,
    Framebuffer,
    Texture,
  };

  template<GraphicsTag Tag>
  struct GraphicsTrait;

  /**
   * @ingroup graphics
   * @brief A GL handle
   *
   * This class is a RAII class around an OpenGL name.
   */
  template<GraphicsTag Tag>
  class GraphicsHandle {
  public:
    GraphicsHandle()
    : m_name()
    {
      GraphicsTrait<Tag>::gen(1, &m_name);
    }

    constexpr GraphicsHandle(NoneType) noexcept
    : m_name(0)
    {
    }

    ~GraphicsHandle() noexcept {
      if (m_name != 0) {
        GraphicsTrait<Tag>::del(1, &m_name);
      }
    }

    GraphicsHandle(const GraphicsHandle&) = delete;

    GraphicsHandle& operator=(const GraphicsHandle&) = delete;

    GraphicsHandle(GraphicsHandle&& other) noexcept
    : m_name(std::exchange(other.m_name, 0))
    {
    }

    GraphicsHandle& operator=(GraphicsHandle&& other) noexcept {
      std::swap(m_name, other.m_name);
      return *this;
    }

    bool isValid() const noexcept {
      return m_name != 0;
    }

    unsigned getName() const noexcept {
      return m_name;
    }

    operator unsigned () const noexcept {
      return m_name;
    }

  private:
    unsigned m_name;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_GRAPHICS_HANDLE_H
