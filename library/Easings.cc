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
#include <gf/Easings.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    template<Easing Func>
    class GenericEasing {
    public:
      static float easeIn(float t) {
        return Func(t);
      }

      static float easeOut(float t) {
        return 1 - Func(1 - t);
      }

      static float easeInOut(float t) {
        return t < 0.5f ? easeIn(2 * t) / 2 : 0.5f + easeOut(2 * t - 1) / 2;
      }

      static float easeOutIn(float t) {
        return t < 0.5f ? easeOut(2 * t) / 2 : 0.5f + easeIn(2 * t - 1) / 2;
      }
    };

  }

  /*
   * linear
   */

  float Ease::linear(float t) {
    return t;
  }

  /*
   * smooth
   */
  float Ease::smooth(float t) {
    return gf::cubicStep(t);
  }

  /*
   * smoother
   */
  float Ease::smoother(float t) {
    return gf::quinticStep(t);
  }

  /*
   * quad
   */

  namespace {
    constexpr float quad(float t) {
      return t * t;
    }
  }

  float Ease::quadIn(float t) {
    return GenericEasing<quad>::easeIn(t);
  }

  float Ease::quadOut(float t) {
    return GenericEasing<quad>::easeOut(t);
  }

  float Ease::quadInOut(float t) {
    return GenericEasing<quad>::easeInOut(t);
  }

  float Ease::quadOutIn(float t) {
    return GenericEasing<quad>::easeOutIn(t);
  }

  /*
   * cubic
   */

  namespace {
    constexpr float cubic(float t) {
      return t * t * t;
    }
  }

  float Ease::cubicIn(float t) {
    return GenericEasing<cubic>::easeIn(t);
  }

  float Ease::cubicOut(float t) {
    return GenericEasing<cubic>::easeOut(t);
  }

  float Ease::cubicInOut(float t) {
    return GenericEasing<cubic>::easeInOut(t);
  }

  float Ease::cubicOutIn(float t) {
    return GenericEasing<cubic>::easeOutIn(t);
  }

  /*
   * quart
   */

  namespace {
    constexpr float quart(float t) {
      return t * t * t * t;
    }
  }

  float Ease::quartIn(float t) {
    return GenericEasing<quart>::easeIn(t);
  }

  float Ease::quartOut(float t) {
    return GenericEasing<quart>::easeOut(t);
  }

  float Ease::quartInOut(float t) {
    return GenericEasing<quart>::easeInOut(t);
  }

  float Ease::quartOutIn(float t) {
    return GenericEasing<quart>::easeOutIn(t);
  }

  /*
   * quint
   */

  namespace {
    constexpr float quint(float t) {
      return t * t * t * t * t;
    }
  }

  float Ease::quintIn(float t) {
    return GenericEasing<quint>::easeIn(t);
  }

  float Ease::quintOut(float t) {
    return GenericEasing<quint>::easeOut(t);
  }

  float Ease::quintInOut(float t) {
    return GenericEasing<quint>::easeInOut(t);
  }

  float Ease::quintOutIn(float t) {
    return GenericEasing<quint>::easeOutIn(t);
  }

  /*
   * circ
   */

  namespace {
    float circ(float t) {
      return 1 - std::sqrt(1 - t * t);
    }
  }

  float Ease::circIn(float t) {
    return GenericEasing<circ>::easeIn(t);
  }

  float Ease::circOut(float t) {
    return GenericEasing<circ>::easeOut(t);
  }

  float Ease::circInOut(float t) {
    return GenericEasing<circ>::easeInOut(t);
  }

  float Ease::circOutIn(float t) {
    return GenericEasing<circ>::easeOutIn(t);
  }

  /*
   * sine
   */

  namespace {
    inline float sine(float t) {
      return (1 - std::cos(Pi2 * t));
    }
  }
  float Ease::sineIn(float t) {
    return GenericEasing<sine>::easeIn(t);
  }

  float Ease::sineOut(float t) {
    return GenericEasing<sine>::easeOut(t);
  }

  float Ease::sineInOut(float t) {
    return GenericEasing<sine>::easeInOut(t);
  }

  float Ease::sineOutIn(float t) {
    return GenericEasing<sine>::easeOutIn(t);
  }

  /*
   * back
   */

  namespace {
    inline float back(float t) {
      static constexpr float C1 = 1.70158f;
      static constexpr float C2 = 1 + C1;
      return t * t * (C2 * t - C1);
    }
  }

  float Ease::backIn(float t) {
    return GenericEasing<back>::easeIn(t);
  }

  float Ease::backOut(float t) {
    return GenericEasing<back>::easeOut(t);
  }

  float Ease::backInOut(float t) {
    return GenericEasing<back>::easeInOut(t);
  }

  float Ease::backOutIn(float t) {
    return GenericEasing<back>::easeOutIn(t);
  }

  /*
   * bounce
   */

  namespace {
    inline float bounce(float t) {
      static constexpr float C1 = 1.0f / 2.75f;
      static constexpr float C2 = 7.5625f; // = 2.75f * 2.75f

      const float u = 1 - t;

      if (u < 1 * C1) {
        return 1 - C2 * u * u;
      }

      if (u < 2 * C1) {
        return 1 - (C2 * gf::square(u - 1.5f * C1) + 0.75f);
      }

      if (u < 2.5f * C1) {
        return 1 - (C2 * gf::square(u - 2.25f * C1) + 0.9375f);
      }

      return 1 - (C2 * gf::square(u - 2.625f * C1) + 0.984375);
    }
  }

  float Ease::bounceIn(float t) {
    return GenericEasing<bounce>::easeIn(t);
  }

  float Ease::bounceOut(float t) {
    return GenericEasing<bounce>::easeOut(t);
  }

  float Ease::bounceInOut(float t) {
    return GenericEasing<bounce>::easeInOut(t);
  }

  float Ease::bounceOutIn(float t) {
    return GenericEasing<bounce>::easeOutIn(t);
  }

  /*
   * elastic
   */

  namespace {
    inline float elastic(float t) {
      static constexpr float C0 = 0.3f;

      const float u = t - 1;

      return -std::pow(2, 10 * u) * std::sin((u - C0 / 4) * 2 * Pi / C0);
    }
  }

  float Ease::elasticIn(float t) {
    return GenericEasing<elastic>::easeIn(t);
  }

  float Ease::elasticOut(float t) {
    return GenericEasing<elastic>::easeOut(t);
  }

  float Ease::elasticInOut(float t) {
    return GenericEasing<elastic>::easeInOut(t);
  }

  float Ease::elasticOutIn(float t) {
    return GenericEasing<elastic>::easeOutIn(t);
  }

  /*
   * expo
   */

  namespace {
    inline float expo(float t) {
      return t == 0.0f ? 0.0f : std::pow(2, 10 * (t - 1));
    }
  }

  float Ease::expoIn(float t) {
    return GenericEasing<expo>::easeIn(t);
  }

  float Ease::expoOut(float t) {
    return GenericEasing<expo>::easeOut(t);
  }

  float Ease::expoInOut(float t) {
    return GenericEasing<expo>::easeInOut(t);
  }

  float Ease::expoOutIn(float t) {
    return GenericEasing<expo>::easeOutIn(t);
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
