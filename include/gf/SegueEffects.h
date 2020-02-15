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
#ifndef GF_SEGUE_EFFECTS_H
#define GF_SEGUE_EFFECTS_H

#include "Portability.h"
#include "SegueEffect.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class GF_API FadeSegueEffect : public SegueEffect {
  public:
    /**
     * @brief Default constructor
     */
    FadeSegueEffect();
  };

  class GF_API SlideSegueEffect : public SegueEffect {
  public:
    enum Orientation {
      Horizontal  = 0,
      Vertical    = 1,
    };

    /**
     * @brief Default constructor
     */
    SlideSegueEffect();

    void setStripes(int stripes);

    void setStripeOrientation(Orientation orientation);
  };

  class GF_API GlitchSegueEffect : public SegueEffect {
  public:
    /**
     * @brief Default constructor
     */
    GlitchSegueEffect();
  };


  class GF_API CheckerboardSegueEffect : public SegueEffect {
  public:
    /**
     * @brief Default constructor
     */
    CheckerboardSegueEffect();

    void setBoardSize(Vector2i size);

    void setSmoothness(float smoothness);
  };

  class GF_API CircleSegueEffect : public SegueEffect {
  public:
    enum Type {
      Open  =  1,
      Close = -1,
    };

    /**
     * @brief Default constructor
     */
    CircleSegueEffect();

    void setType(Type type);

    void setFramebufferSize(Vector2i size);

  };

  class GF_API PixelateSegueEffect : public SegueEffect {
  public:
    /**
     * @brief Default constructor
     */
    PixelateSegueEffect();

    void setFramebufferSize(Vector2i size);
  };

  class GF_API RadialSegueEffect : public SegueEffect {
  public:
    /**
     * @brief Default constructor
     */
    RadialSegueEffect();
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SEGUE_EFFECTS_H
