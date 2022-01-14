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
#ifndef GF_GRAPHICS_HANDLE_H
#define GF_GRAPHICS_HANDLE_H

#include <utility>

#include "Types.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics_gpu
   * @brief A tag to represent various GPU resources
   */
  enum class GraphicsTag {
    Buffer,       ///< A GPU buffer
    Framebuffer,  ///< A GPU framebuffer
    Texture,      ///< A GPU texture
  };

  /**
   * @ingroup graphics_gpu
   * @brief A trait to handle creation and deletion of GPU resources
   */
  template<GraphicsTag Tag>
  struct GraphicsTrait;

  /**
   * @ingroup graphics_gpu
   * @brief A GL handle
   *
   * This class is a RAII class around an OpenGL name.
   */
  template<GraphicsTag Tag>
  class GraphicsHandle {
  public:
    /**
     * @brief Constructor
     *
     * Generate a valid handle.
     */
    GraphicsHandle()
    : m_name()
    {
      GraphicsTrait<Tag>::gen(1, &m_name);
    }

    /**
     * @brief Constructor
     *
     * Generate an invalid handle.
     */
    constexpr GraphicsHandle(NoneType) noexcept
    : m_name(0)
    {
    }

    /**
     * @brief Destructor
     */
    ~GraphicsHandle() noexcept {
      if (m_name != 0) {
        GraphicsTrait<Tag>::del(1, &m_name);
      }
    }

    /**
     * @brief Deleted copy constructor
     */
    GraphicsHandle(const GraphicsHandle&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    GraphicsHandle& operator=(const GraphicsHandle&) = delete;

    /**
     * @brief Move constructor
     *
     * @param other Another handle
     */
    GraphicsHandle(GraphicsHandle&& other) noexcept
    : m_name(std::exchange(other.m_name, 0))
    {
    }

    /**
     * @brief Move assignment
     *
     * @param other Another handle
     * @returns A reference to this handle
     */
    GraphicsHandle& operator=(GraphicsHandle&& other) noexcept {
      std::swap(m_name, other.m_name);
      return *this;
    }

    /**
     * @brief Check if the handle is valid
     *
     * @returns True if the handle is valid
     */
    bool isValid() const noexcept {
      return m_name != 0;
    }

    /**
     * @brief Get the underlying name of the handle
     *
     * @returns A GL name
     */
    unsigned getName() const noexcept {
      return m_name;
    }

    /**
     * @brief Conversion operator to the underlying name
     *
     * @sa getName()
     */
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
