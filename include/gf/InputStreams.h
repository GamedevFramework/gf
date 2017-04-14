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
#ifndef GF_INPUT_STREAMS_H
#define GF_INPUT_STREAMS_H

#include <cstddef>
#include <cstdio>

#include "InputStream.h"
#include "Path.h"
#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief File based input stream
   */
  class GF_API FileInputStream : public InputStream {
  public:
    /**
     * @brief Constructor
     *
     * The constructor opens the file. If the file is not opened, the
     * operations will return default values.
     *
     * @param path The path to the file
     */
    explicit FileInputStream(const Path& path);

    /**
     * @brief Destructor
     *
     * The destructor closes the file.
     */
    ~FileInputStream();

    virtual std::size_t read(void* data, std::size_t size) override;

    virtual long seek(long position) override;

    virtual long tell() override;

    virtual std::size_t getSize() override;

  private:
    std::FILE *m_file;
    std::size_t m_size;
  };


  /**
   * @ingroup core
   * @brief Memory based input stream
   */
  class GF_API MemoryInputStream : public InputStream {
  public:
    /**
     * @brief Constructor
     *
     * This class does not own the memory. The user must keep the memory
     * available when the stream is active.
     *
     * @param data Pointer to the data
     * @param size Size of the data
     */
    MemoryInputStream(const void *data, std::size_t size);

    virtual std::size_t read(void* data, std::size_t size) override;

    virtual long seek(long position) override;

    virtual long tell() override;

    virtual std::size_t getSize() override;

  private:
    const char *m_data;
    std::size_t m_size;
    long m_offset;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_INPUT_STREAMS_H
