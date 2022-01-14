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
#ifndef GF_COLOR_RAMP_H
#define GF_COLOR_RAMP_H

#include <cassert>
#include <map>
#include <type_traits>
#include <utility>

#include "Color.h"
#include "CoreApi.h"
#include "Vector.h"
#include "VectorOps.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_color
   * @brief A color ramp
   *
   * A color ramp (or color gradient) specifies a range of colors that depends
   * on a position.
   *
   * @sa gf::ColorBase, gf::Color4f
   */
  template<typename T>
  struct ColorRampBase {
    static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
  public:
    /**
     * @brief Check if the color ramp is empty
     *
     * @return True if the ramp is empty
     */
    bool isEmpty() const {
      return m_map.empty();
    }

    /**
     * @brief Add a color stop
     *
     * @param offset The offset of the color
     * @param color The color
     */
    void addColorStop(T offset, const Color4<T>& color) {
      if (isEmpty()) {
        m_min = m_max = offset;
      } else {
        if (offset < m_min) {
          m_min = offset;
        }

        if (offset > m_max) {
          m_max = offset;
        }
      }

      m_map.insert(std::make_pair(offset, color));
    }

    /**
     * @brief Compute a color from an offset
     *
     * @param offset The offset of the wanted color
     * @return A color
     */
    Color4<T> computeColor(T offset) const {
      if (m_map.empty()) {
        return ColorBase<T>::White;
      }

      if (offset < m_min) {
        return m_map.begin()->second;
      }

      if (offset > m_max) {
        return m_map.rbegin()->second;
      }

      auto it = m_map.lower_bound(offset);
      assert(it != m_map.end());

      T t2 = it->first;
      Color4<T> c2 = it->second;

      if (it == m_map.begin()) {
        return c2;
      }

      --it;
      T t1 = it->first;
      Color4<T> c1 = it->second;

      return gf::lerp(c1, c2, (offset - t1) / (t2 - t1));
    }

  private:
    T m_min;
    T m_max;
    std::map<T, Color4<T>> m_map;
  };

  /**
   * @ingroup core_color
   * @brief Instantiation of ColoRampBase for `float`
   */
  using ColorRampF = ColorRampBase<float>;

  /**
   * @ingroup core_color
   * @brief Instantiation of ColoRampBase for `double`
   */
  using ColorRampD = ColorRampBase<double>;

  /**
   * @ingroup core_color
   * @brief Instantiation of ColoRampBase for `float`
   */
  using ColorRamp = ColorRampF;

// MSVC does not like extern template
#ifndef _MSC_VER
  extern template struct GF_CORE_API ColorRampBase<float>;
  extern template struct GF_CORE_API ColorRampBase<double>;
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_COLOR_RAMP_H
