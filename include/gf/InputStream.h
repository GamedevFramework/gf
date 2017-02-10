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
#ifndef GF_INPUT_STREAM_H
#define GF_INPUT_STREAM_H

#include <cstddef>

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief Abstract class for custom file input streams
   *
   * This class allows users to define their own file input sources from
   * which Gamedev Framework can load resources.
   *
   * Gamedev Framework resource classes like gf::Texture and gf::Shader
   * provide `loadFromFile()` and `loadFromMemory()` functions, which read
   * data from conventional sources. However, if you have data coming from
   * a different source (over a network, embedded, encrypted, compressed,
   * etc) you can derive your own class from gf::InputStream and load SFML
   * resources with their `loadFromStream()` function.
   *
   */
  class GF_API InputStream {
  public:
    /**
     * @brief Default constructor
     */
    InputStream() = default;

    /**
     * @brief Deleted copy constructor
     */
    InputStream(const InputStream&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    InputStream& operator=(const InputStream&) = delete;

    /**
     * @brief Destructor
     */
    virtual ~InputStream();

    /**
     * @brief Read data from the stream
     *
     * After reading, the stream's reading position must be
     * advanced by the amount of bytes read.
     *
     * @param data Buffer where to copy the read data
     * @param size Desired number of bytes to read
     *
     * @return The number of bytes actually read
     */
    virtual std::size_t read(void* data, std::size_t size) = 0;

    /**
     * @brief Change the current reading position
     *
     * @param position The position to seek to, from the beginning
     *
     * @return The position actually sought to
     */
    virtual long seek(long position) = 0;

    /**
     * @brief Get the current reading position in the stream
     *
     * @return The current position
     */
    virtual long tell() = 0;

    /**
     * @brief Return the size of the stream
     *
     * @return The total number of bytes available in the stream
     */
    virtual std::size_t getSize() = 0;

  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_INPUT_STREAM_H
