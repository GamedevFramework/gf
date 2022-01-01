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
#ifndef GF_DICE_H
#define GF_DICE_H

#include <cassert>
#include <climits>
#include <string_view>

#include "CoreApi.h"
#include "Random.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_utilities
   * @brief A set of dice that can be rolled
   *
   * The set of dice is composed of several dice with the same number of faces.
   * After a roll a modifier can be added to the sum.
   */
  class GF_CORE_API Dice {
  public:
    /**
     * @brief Constructor
     *
     * With this constructor, you can set the number of faces, the count of
     * dice and the modifier directly.
     *
     * @param faces The number of faces of each dice
     * @param count The count of dice
     * @param modifier The modifier to apply after a roll
     */
    constexpr explicit Dice(int faces, int count = 1, int modifier = 0)
    : m_faces(faces)
    , m_count(count)
    , m_modifier(modifier)
    {

    }

    /**
     * @brief Constructor
     *
     * @param str The string representing a dice notation
     */
    constexpr Dice(std::string_view str)
    : m_faces(0)
    , m_count(0)
    , m_modifier(0)
    {
      std::size_t i = 0;
      std::size_t sz = str.size();

      while (i < sz && '0' <= str[i] && str[i] <= '9') {
        m_count = m_count * 10 + (str[i] - '0');
        ++i;
      }

      if (i == sz) {
        return;
      }

      if (str[i] != 'd' && str[i] != 'D') {
        return;
      }

      if (m_count == 0) {
        m_count = 1;
      }

      ++i; // eat 'd'

      while (i < sz && '0' <= str[i] && str[i] <= '9') {
        m_faces = m_faces * 10 + (str[i] - '0');
        ++i;
      }

      if (i == sz) {
        return;
      }

      if (str[i] != '+') {
        return;
      }

      ++i; // eat '+'

      while (i < sz && '0' <= str[i] && str[i] <= '9') {
        m_modifier = m_modifier * 10 + (str[i] - '0');
        ++i;
      }
    }

    /**
     * @brief Get the number of faces
     *
     * @returns The number of faces
     */
    constexpr int getFaces() const {
      return m_faces;
    }

    /**
     * @brief Get the count of dice
     *
     * @returns The count of dice
     */
    constexpr int getCount() const {
      return m_count;
    }

    /**
     * @brief Get the modifier
     *
     * @returns The modifier of the roll
     */
    constexpr int getModifier() const {
      return m_modifier;
    }

    /**
     * @brief Roll the dice
     *
     * @return The result of dice rolling
     */
    int roll(gf::Random& random) const;

  private:
    int m_faces;
    int m_count;
    int m_modifier;
  };


  namespace literals {

    /**
     * @relates gf::Dice
     * @brief User defined operator for creating dice with common dice notation
     *
     * The notation is the classical @f$ XdY+Z @f$ where @f$ X @f$ is the count
     * of dice (1 if omitted), @f$ Y @f$ is the number of faces (mandatory),
     * @f$ Z @f$ is the modifier (0 if omitted).
     *
     * @sa [Dice notation - Wikipedia](https://en.wikipedia.org/wiki/Dice_notation)
     */
    constexpr gf::Dice operator"" _dice(const char *str, std::size_t sz) {
      return gf::Dice(std::string_view(str, sz));
    }

    /**
     * @relates gf::Dice
     * @brief User defined operator for creating some d4
     *
     * @sa [Dice notation - Wikipedia](https://en.wikipedia.org/wiki/Dice_notation)
     */
    constexpr gf::Dice operator"" _d4(unsigned long long int count) {
      assert(count <= INT_MAX);
      return gf::Dice(4, static_cast<int>(count));
    }

    /**
     * @relates gf::Dice
     * @brief User defined operator for creating some d6
     *
     * @sa [Dice notation - Wikipedia](https://en.wikipedia.org/wiki/Dice_notation)
     */
    constexpr gf::Dice operator"" _d6(unsigned long long int count) {
      assert(count <= INT_MAX);
      return gf::Dice(6, static_cast<int>(count));
    }

    /**
     * @relates gf::Dice
     * @brief User defined operator for creating some d6
     *
     * @sa [Dice notation - Wikipedia](https://en.wikipedia.org/wiki/Dice_notation)
     */
    constexpr gf::Dice operator"" _d8(unsigned long long int count) {
      assert(count <= INT_MAX);
      return gf::Dice(8, static_cast<int>(count));
    }

    /**
     * @relates gf::Dice
     * @brief User defined operator for creating some d10
     *
     * @sa [Dice notation - Wikipedia](https://en.wikipedia.org/wiki/Dice_notation)
     */
    constexpr gf::Dice operator"" _d10(unsigned long long int count) {
      assert(count <= INT_MAX);
      return gf::Dice(10, static_cast<int>(count));
    }

    /**
     * @relates gf::Dice
     * @brief User defined operator for creating some d12
     *
     * @sa [Dice notation - Wikipedia](https://en.wikipedia.org/wiki/Dice_notation)
     */
    constexpr gf::Dice operator"" _d12(unsigned long long int count) {
      assert(count <= INT_MAX);
      return gf::Dice(12, static_cast<int>(count));
    }

     /**
     * @relates gf::Dice
     * @brief User defined operator for creating some d20
     *
     * @sa [Dice notation - Wikipedia](https://en.wikipedia.org/wiki/Dice_notation)
     */
    constexpr gf::Dice operator"" _d20(unsigned long long int count) {
      assert(count <= INT_MAX);
      return gf::Dice(20, static_cast<int>(count));
    }

    /**
     * @relates gf::Dice
     * @brief User defined operator for creating some d100
     *
     * @sa [Dice notation - Wikipedia](https://en.wikipedia.org/wiki/Dice_notation)
     */
    constexpr gf::Dice operator"" _d100(unsigned long long int count) {
      assert(count <= INT_MAX);
      return gf::Dice(100, static_cast<int>(count));
    }
 }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_DICE_H
