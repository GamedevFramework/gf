/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#ifndef GF_ARRAY_REF_H
#define GF_ARRAY_REF_H

#include <vector>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  template<typename T>
  class ArrayRef {
  public:
    ArrayRef()
    : m_data(nullptr)
    , m_size(0)
    {

    }

    ArrayRef(const T *data, std::size_t size)
    : m_data(data)
    , m_size(size)
    {

    }


    ArrayRef(const std::vector<T>& values)
    : m_data(values.data())
    , m_size(values.size())
    {

    }

    template<std::size_t N>
    ArrayRef(const T (&data)[N])
    : m_data(data)
    , m_size(N)
    {

    }

    ArrayRef(std::initializer_list<T> values)
    : m_data(values.size() == 0 ? nullptr : values.begin())
    , m_size(values.size())
    {

    }

    const T *getData() const {
      return m_data;
    }

    std::size_t getSize() const {
      return m_size;
    }

  private:
    const T *m_data;
    std::size_t m_size;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_ARRAY_REF_H
