/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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
#ifndef GF_BINARY_FILE_H
#define GF_BINARY_FILE_H

#include <cstdio>

#include "ArrayRef.h"
#include "BufferRef.h"
#include "Path.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A binary file that can be read or written
   *
   */
  class BinaryFile {
  public:
    /**
     * @brief Open mode for the file
     */
    enum class Mode {
      Read,   ///< Open the file in read only
      Write,  ///< Open the file in write only
      Append, ///< Open the file in append only
    };

    /**
     * @brief Constructor
     *
     * @param filename The filename of the binary file
     * @param mode The open mode
     */
    BinaryFile(const Path& filename, Mode mode);

    /**
     * @brief Deleted copy constructor
     */
    BinaryFile(const BinaryFile&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    BinaryFile& operator=(const BinaryFile&) = delete;

    /**
     * @brief Move constructor
     */
    BinaryFile(BinaryFile&& other);

    /**
     * @brief Move assignment
     */
    BinaryFile& operator=(BinaryFile&& other);

    /**
     * @brief Destructor
     *
     * The file is closed in the destructor.
     */
    ~BinaryFile();

    /**
     * @brief Write a buffer in the file
     *
     * @param buffer The buffer to write in the file
     * @returns The number of bytes written in the file
     */
    std::size_t write(ArrayRef<uint8_t> buffer) const;

    /**
     * @brief Write a single byte in the file
     *
     * @param byte The byte to write in the file
     * @returns The number of bytes written in the file
     */
    std::size_t write(uint8_t byte) const;

    /**
     * @brief Read from the file into a buffer
     *
     * @param buffer The buffer to read from the file
     * @returns The number of bytes read from the file
     */
    std::size_t read(BufferRef<uint8_t> buffer) const;

    /**
     * @brief Read a single byte from the file
     *
     * @param byte The byte to read from the file
     * @returns The number of bytes read from the file
     */
    std::size_t read(uint8_t& byte) const;

    /**
     * @brief Tell if the file is at the end
     *
     * @returns True if the file reached the end
     */
    bool isEof() const;

  private:
    std::FILE *m_file;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_BINARY_FILE_H
