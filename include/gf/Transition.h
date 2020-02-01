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
#ifndef GF_TRANSITION_H
#define GF_TRANSITION_H

#include "Drawable.h"
#include "Portability.h"
#include "Rect.h"
#include "Vertex.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class TransitionEffect;
  class Texture;

  class GF_API Transition : public Drawable {
  public:

    Transition();

    void start(float time);
    void stop(void);
    void update(float time);

    bool isActive() {
      return m_time < m_totalTime;
    }

    void setTextures(const Texture& texture1, const Texture& texture2);

    /**
     * @brief Set the effect to apply
     *
     * @param effect The post-processing effect
     */
    void setEffect(TransitionEffect& effect) {
      m_effect = &effect;
    }

    virtual void draw(RenderTarget& target, const RenderStates& states) override;

  private:
    void updatePositions();
    void updateTexCoords();

  private:
    const Texture *m_texture1;
    const Texture *m_texture2;
    RectF m_textureRect;
    TransitionEffect *m_effect;
    Vertex m_vertices[4];

    float m_totalTime;
    float m_time;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TRANSITION_H
