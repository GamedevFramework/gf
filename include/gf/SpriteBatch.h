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
#ifndef GF_SPRITE_BATCH_H
#define GF_SPRITE_BATCH_H

#include <cstddef>
#include <array>

#include "Portability.h"
#include "RenderStates.h"
#include "Vertex.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class RenderTarget;
  class Sprite;
  class Texture;

  /**
   * @ingroup graphics
   * @brief A sprite batch
   *
   * A sprite batch is responsible for minimizing the number of draw calls by
   * concatenating the different calls for sprites.
   *
   * Before using a sprite batch, you have to call `begin()`, then call
   * `draw()` for each sprite you want to draw, and finally call `end()`.
   *
   * ~~~{.cc}
   * gf::SpriteBatch batch(renderer);
   *
   * batch.begin();
   * batch.draw(sprite1);
   * batch.draw(sprite2);
   * batch.draw(sprite3);
   * batch.end();
   * ~~~
   *
   * @sa gf::Sprite
   */
  class GF_API SpriteBatch {
  public:
    /**
     * @brief Constructor
     *
     * @param target A render target where the sprites will be drawn
     */
    SpriteBatch(RenderTarget& target);

    /**
     * @brief Begin the batch
     */
    void begin();

    /**
     * @brief Add a sprite to the batch
     *
     * You must call `begin()` before calling this function, and `end()` once
     * you have finished to draw the sprites.
     *
     * @param sprite The sprite to draw
     * @param states The render states
     */
    void draw(Sprite& sprite, const RenderStates& states = RenderStates());

    /**
     * @brief Add a raw texture to the batch
     *
     * You must call `begin()` before calling this function, and `end()` once
     * you have finished to draw the sprites.
     *
     * @param texture The texture to draw
     * @param position The position of the texture
     * @param states The render states
     */
    void draw(const Texture& texture, Vector2f position, const RenderStates& states = RenderStates());

    /**
     * @brief Add a portion of a raw texture to the batch
     *
     * You must call `begin()` before calling this function, and `end()` once
     * you have finished to draw the sprites.
     *
     * @param texture The texture to draw
     * @param textureRect The sub-rectangle of the texture to draw
     * @param position The position of the texture
     * @param states The render states
     */
    void draw(const Texture& texture, const RectF& textureRect, Vector2f position, const RenderStates& states = RenderStates());

    /**
     * @brief End the batch
     */
    void end();

  private:
    void renderBatch();

  private:
    static constexpr std::size_t MaxSpriteCount = 1024;
    static constexpr std::size_t VerticesPerSprite = 6;
    static constexpr std::size_t MaxVertexCount = MaxSpriteCount * VerticesPerSprite;

    RenderTarget& m_target;
    RenderStates m_currentRenderStates;
    std::size_t m_count;
    std::array<Vertex, MaxVertexCount> m_vertices;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SPRITE_BATCH_H
