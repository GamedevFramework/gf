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
#ifndef GF_POST_PROCESSING_H
#define GF_POST_PROCESSING_H

#include "Drawable.h"
#include "Portability.h"
#include "Rect.h"
#include "Vertex.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Effect;
  class Texture;

  /**
   * @ingroup graphics
   * @brief A post-processing drawable
   *
   * This class is a light-weight object to apply a post-processing effect to
   * a texture. The texture generally comes froma gf::RenderTexture.
   *
   * @sa gf::RenderTexture
   */
  class GF_API PostProcessing : public Drawable {
  public:
    /**
     * @brief Default constructor
     */
    PostProcessing();

    /**
     * @brief Set the source texture
     *
     * @param texture The source texture
     */
    void setTexture(const Texture& texture);

    /**
     * @brief Set the texture rectangle
     *
     * @param textureRect The texture rectangle
     */
    void setTextureRect(const RectF& textureRect);

    /**
     * @brief Set the effect to apply
     *
     * @param effect The post-processing effect
     */
    void setEffect(Effect& effect) {
      m_effect = &effect;
    }

    virtual void draw(RenderTarget& target, RenderStates states) override;

  private:
    void updatePositions();
    void updateTexCoords();

  private:
    const Texture *m_texture;
    RectF m_textureRect;
    Effect *m_effect;
    Vertex m_vertices[4];
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_POST_PROCESSING_H
