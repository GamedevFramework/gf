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
#ifndef GF_FLAGS_H
#define GF_FLAGS_H

#include <type_traits>

#include "Types.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief Bitfield relying on an enumeration
   *
   * ~~~
   * enum class AnimalProperties {
   *   HasClaws     = 0x01,
   *   CanFly       = 0x02,
   *   EatsFish     = 0x04,
   *   IsEndangered = 0x08,
   * };
   *
   * namespace gf {
   *   template<>
   *   struct EnableBitmaskOperators<AnimalProperties> {
   *     static constexpr bool value = true;
   *   };
   * }
   *
   * gf::Flags<AnimalProperties> seahawk = AnimalProperties::CanFly | AnimalProperties::EatsFish;
   * seahawk |= AnimalProperties::IsEndangered;
   *
   * bool b = seahawk.test(AnimalProperties::HasClaws); // false
   * ~~~
   *
   * If you do not initialize flags, then the state is undefined. You can use
   * semantic constants gf::All and gf::None to set all the flags or none.
   *
   * ~~~{.cc}
   * gf::Flags<AnimalProperties> unicorn(gf::All);
   *
   * gf::Flags<AnimalProperties> rat(gf::None);
   * ~~~
   */
  template<typename E>
  class Flags {
  public:
    /**
     * @brief Default constructor
     */
    Flags() = default;

    /**
     * @brief Constructor with no flag set
     */
    constexpr Flags(NoneType)
    : m_data(0)
    {

    }

    /**
     * @brief Constructor with all flags set
     */
    constexpr Flags(AllType)
    : m_data(~0)
    {

    }

    /**
     * @brief Constructor with an enum value
     *
     * @param e An enum value
     */
    constexpr Flags(E e)
    : m_data(static_cast<Type>(e))
    {

    }

    /**
     * @brief Binary NOT operator
     *
     * @returns A bitfield where all flags have been inverted
     */
    constexpr Flags<E> operator~() const {
      return Flags(~m_data);
    }

    /**
     * @brief Binary OR between two bitfields
     *
     * @param flags Another bitfield
     * @return The bitfield with a binary OR of the two bitfields
     */
    constexpr Flags operator|(Flags flags) const {
      return Flags(m_data | flags.m_data);
    }

    /**
     * @brief Binary OR and assignment
     *
     * @param flags Another bitfield
     * @return The bitfield with a binary OR of the flags and the other's flags
     */
    Flags<E>& operator|=(Flags<E> flags) {
      m_data |= flags.m_data;
      return *this;
    }

    /**
     * @brief Binary AND between two bitfields
     *
     * @param flags Another bitfield
     * @return The bitfield with a binary AND of the two bitfields
     */
    constexpr Flags operator&(Flags flags) const {
      return Flags(m_data & flags.m_data);
    }

    /**
     * @brief Binary AND and assignment
     *
     * @param flags Another bitfield
     * @return The bitfield with a binary AND of the flags and the other's flags
     */
    Flags<E>& operator&=(Flags<E> flags) {
      m_data &= flags.m_data;
      return *this;
    }

    /**
     * @brief Test if any flag is set
     *
     * @returns True if any flag is set
     */
    constexpr operator bool() const {
      return m_data != 0;
    }

    /**
     * @brief Test if a specified flag is set
     *
     * @param flag The flag to test
     * @return True if the flag is set
     */
    constexpr bool test(E flag) const {
      return (m_data & static_cast<Type>(flag)) != 0;
    }

    /**
     * @brief Set a flag
     *
     * @param flag The flag to set
     */
    void set(E flag) {
      m_data |= static_cast<Type>(flag);
    }

    /**
     * @brief Reset a flag
     *
     * @param flag The flag to reset
     */
    void reset(E flag) {
      m_data &= ~static_cast<Type>(flag);
    }

    using Type = typename std::underlying_type<E>::type;

    /**
     * @brief Get the underlying value of the flags
     *
     * This function should not be used in normal cases.
     *
     * @return The value of the flags
     */
    Type getValue() const {
      return m_data;
    }

  private:
    constexpr Flags(Type data)
    : m_data(data)
    {

    }

    Type m_data;
  };

  /**
   * @relates Flags
   * @brief Binary OR between a bitfield and a flag
   *
   * @param lhs The bitfield
   * @param rhs The flag
   * @return The bitfield with a binary OR of the bitfield and the flag
   */
  template<typename E>
  constexpr
  Flags<E> operator|(Flags<E> lhs, E rhs) {
    return lhs | Flags<E>(rhs);
  }

  /**
   * @relates Flags
   * @brief Binary OR between a flag and a bitfield
   *
   * @param lhs The flag
   * @param rhs The bitfield
   * @return The bitfield with a binary OR of the flag and the bitfield
   */
  template<typename E>
  constexpr
  Flags<E> operator|(E lhs, Flags<E> rhs) {
    return Flags<E>(lhs) | rhs;
  }

  /**
   * @relates Flags
   * @brief Binary AND between a bitfield and a flag
   *
   * @param lhs The bitfield
   * @param rhs The flag
   * @return The bitfield with a binary AND of the bitfield and the flag
   */
  template<typename E>
  constexpr
  Flags<E> operator&(Flags<E> lhs, E rhs) {
    return lhs & Flags<E>(rhs);
  }

  /**
   * @relates Flags
   * @brief Binary AND between a flag and a bitfield
   *
   * @param lhs The flag
   * @param rhs The bitfield
   * @return The bitfield with a binary AND of the flag and the bitfield
   */
  template<typename E>
  constexpr
  Flags<E> operator&(E lhs, Flags<E> rhs) {
    return Flags<E>(lhs) & rhs;
  }


  /**
   * @relates Flags
   * @brief Combine a single enum value into a flag value
   *
   * @param flag An enum value
   * @returns The enum value turned into a flag value
   */
  template<typename E>
  constexpr Flags<E> combineFlags(E flag) {
    return Flags<E>(flag);
  }

  /**
   * @relates Flags
   * @brief Combine several enum values into a flag value
   *
   * @param flag An enum value
   * @param others The other enum values
   * @returns The enum values turned into a flag value
   */
  template<typename E, typename ... F>
  constexpr Flags<E> combineFlags(E flag, F ... others) {
    return Flags<E>(flag) | combineFlags(others ...);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}


// this traits is not versioned

template<typename E>
struct EnableBitmaskOperators {
  static constexpr bool value = false;
};

// these overloads are only available to gf enum types and gf flags
// unless you add: "using gf::operator|;"

template<typename E>
constexpr
typename std::enable_if<EnableBitmaskOperators<E>::value, gf::Flags<E>>::type
operator|(E lhs, E rhs) {
  return gf::Flags<E>(lhs) | gf::Flags<E>(rhs);
}

template<typename E>
constexpr
typename std::enable_if<EnableBitmaskOperators<E>::value, gf::Flags<E>>::type
operator&(E lhs, E rhs) {
  return gf::Flags<E>(lhs) & gf::Flags<E>(rhs);
}

template<typename E>
constexpr
typename std::enable_if<EnableBitmaskOperators<E>::value, gf::Flags<E>>::type
operator~(E val) {
  return ~gf::Flags<E>(val);
}

#endif
}

#endif // GF_FLAGS_H
