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
#ifndef GF_SINGLETON_H
#define GF_SINGLETON_H

#include <cassert>
#include <utility>

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  template<typename T>
  class SingletonStorage;

  /**
   * @ingroup core
   * @brief A singleton that wraps a pointer provided by a storage.
   *
   * This class should be used with a global variable. It acts as an access to
   * a singleton which storage is provided by a gf::SingletonStorage<T>.
   *
   * ~~~{.cc}
   * gf::Singleton<Foo> gFoo;
   *
   * int main() {
   *    gf::SingletonStorage<Foo> fooStorage(gFoo);
   *
   *    gFoo().bar();
   * }
   * ~~~
   *
   * @sa gf::SingletonStorage
   */
  template<typename T>
  class Singleton {
  public:
    /**
     * @brief Default constructor.
     *
     * It initialize the underlying pointer to `nullptr`. The actual
     * initialization is done when the storage is allocated.
     */
    Singleton()
    : m_single(nullptr)
    {
    }

    /**
     * @brief Deleted copy constructor.
     */
    Singleton(const Singleton&) = delete;

    /**
     * @brief Deleted move constructor.
     */
    Singleton(Singleton&&) = delete;
    /**
     * @brief Deleted copy assignment.
     */
    Singleton& operator=(const Singleton&) = delete;

    /**
     * @brief Deleted move assignment.
     */
    Singleton& operator=(Singleton&&) = delete;

    /**
     * @brief Access the singleton.
     *
     * The access of the singleton must be done after a storage has been
     * provided. Otherwise, the behaviour is undefined (and may result in a
     * crash).
     *
     * @returns a reference to the stored singleton
     */
    T& operator()() {
      assert(m_single);
      return *m_single;
    }

    /**
     * @brief Reset the singleton
     *
     * After a call to this function, the singleton must be accessed anymore.
     */
    void reset() noexcept {
      m_single = nullptr;
    }

    /**
     * @brief Check if the singleton has been initialized
     *
     * @return True if the singleton is valid
     */
    bool isValid() const noexcept {
      return m_single != nullptr;
    }

  private:
    friend class SingletonStorage<T>;

    T *m_single;
  };

  /**
   * @ingroup core
   * @brief A storage for a singleton.
   *
   * This class is a wrapper around an object of class `T` that should be
   * unique in the system. It works in cooperation with a global variable of
   * type gf::Singleton<T>. The global variable is for the access of the object
   * while this class is for the storage of the object. The global variable is
   * initialized at the storage construction.
   *
   * ~~~{.cc}
   * gf::Singleton<Foo> gFoo;
   *
   * int main() {
   *    gf::SingletonStorage<Foo> fooStorage(gFoo);
   *
   *    gFoo().bar();
   * }
   * ~~~
   *
   * @sa gf::Singleton
   */
  template<typename T>
  class SingletonStorage {
  public:
    /**
     * @brief Construct a storage for a singleton.
     *
     *
     * @param ref a reference to a singleton (a global variable)
     * @param args arguments for the constructor of class `T`
     */
    template<typename ... Args>
    SingletonStorage(Singleton<T>& ref, Args&&... args)
    : m_storage(std::forward<Args>(args)...) {
      assert(ref.m_single == nullptr);
      ref.m_single = &m_storage;
    }

  private:
    T m_storage;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SINGLETON_H
