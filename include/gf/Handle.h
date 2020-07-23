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
#ifndef GF_HANDLE_H
#define GF_HANDLE_H

#include "CoreApi.h"
#include "Id.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class GF_CORE_API Handle {
  public:
    Handle() = default;

    explicit Handle(Id id)
    : m_id(id)
    {
    }

    template<typename T>
    explicit Handle(T& object)
    : m_ptr(&object)
    {
    }

    Id asId() const {
      return m_id;
    }

    template<typename T>
    T& as() {
      return *static_cast<T*>(m_ptr);
    }

    template<typename T>
    const T& as() const {
      return *static_cast<const T*>(m_ptr);
    }

  private:
    union {
      Id m_id;
      void * m_ptr;
    };
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_HANDLE_H
