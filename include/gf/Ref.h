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
#ifndef GF_REF_H
#define GF_REF_H

#include <memory>
#include <type_traits>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_vocabulary
   * @brief A reference wrapper
   *
   * This class is similar to [std::reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper).
   */
  template<typename T>
  class Ref {
  public:
    /**
     * @brief Constructor with a lvalue reference
     *
     * @param ref A reference
     */
    Ref(T& ref) noexcept
    : m_ptr(std::addressof(ref))
    {

    }

    /**
     * @brief Deleted constructor with rvalue reference
     */
    Ref(T&&) = delete;

    /**
     * @brief Copy constructor
     */
    Ref(const Ref& other) noexcept = default;

    /**
     * @brief Copy constructor
     */
    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    Ref(const Ref<U>& other) noexcept
    : m_ptr(std::addressof(other.get()))
    {
    }

    /**
     * @brief Copy assignment
     */
    Ref& operator=(const Ref& other) noexcept = default;

    /**
     * @brief Copy assignment
     */
    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    Ref& operator=(const Ref<U>& other) noexcept {
      m_ptr = std::addressof(other.get());
    }

    /**
     * @brief Get the reference
     *
     * @returns A reference to the object
     */
    T& get() const noexcept {
      return *m_ptr;
    }

    /**
     * @brief Conversion to a reference
     *
     * @returns A reference to the object
     */
    operator T& () const noexcept {
      return *m_ptr;
    }

  private:
    T *m_ptr;
  };

  /**
   * @relates Ref
   * @brief Referece comparison
   *
   * The reference are compared by address, not by value.
   */
  template<typename T>
  constexpr bool operator==(Ref<T> lhs, Ref<T> rhs) noexcept {
    return std::addressof(lhs.get()) == std::addressof(rhs.get());
  }

  /**
   * @relates Ref
   * @brief Referece comparison
   *
   * The reference are compared by address, not by value.
   */
  template<typename T>
  constexpr bool operator==(Ref<T> lhs, const T& rhs) noexcept {
    return std::addressof(lhs.get()) == std::addressof(rhs);
  }

  /**
   * @relates Ref
   * @brief Referece comparison
   *
   * The reference are compared by address, not by value.
   */
  template<typename T>
  constexpr bool operator==(const T& lhs, Ref<T> rhs) noexcept {
    return std::addressof(lhs) == std::addressof(rhs.get());
  }

  /**
   * @relates Ref
   * @brief Reference creation
   */
  template<typename T>
  Ref<T> ref(T& object) {
    return Ref<T>(object);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template<typename T>
  Ref<T> ref(const T&&) = delete;
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_REF_H
