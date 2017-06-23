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
#ifndef GF_ANIMATION_H
#define GF_ANIMATION_H

#include <cstddef>
#include <vector>

#include "Portability.h"
#include "Rect.h"
#include "Time.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Texture;

  /**
   * @ingroup graphics
   * @brief An animation
   *
   * An animation is a collection of frames that are displayed
   * consecutively during a predefined amount of time.
   *
   * To display an animation, you need a gf::AnimatedSprite.
   *
   * @sa gf::AnimatedSprite
   */
  class GF_API Animation {
  public:
    /**
     * @brief Default constructor
     */
    Animation();

    /**
     * @brief Add a frame to the animation
     *
     * The frame of an animation is defined by the texture of the frame, the
     * texture rectangle (in texture coordinates) and an amount of time.
     *
     * @param texture The texture where the sprite is
     * @param bounds The texture rectangle where the sprite is
     * @param duration The amount of time to display the frame
     */
    void addFrame(const Texture& texture, const RectF& bounds, Time duration);

    /**
     * @brief Get the current texture
     *
     * @return The current texture
     * @throw std::runtime_error If the animation is empty
     */
    const Texture& getCurrentTexture() const;

    /**
     * @brief Get the current texture rectangle
     *
     * @return The current texture rectangle
     * @throw std::runtime_error If the animation is empty
     */
    RectF getCurrentBounds() const;

    /**
     * @brief Update the state of the animation
     *
     * @param time The time since the last update
     * @return True if the current frame has changed
     */
    bool update(Time time);

  private:
    struct Frame {
      const Texture *texture;
      RectF bounds;
      Time duration;
    };

    std::size_t m_currentFrame;
    Time m_currentDurationInFrame;
    std::vector<Frame> m_frames;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_ANIMATION_H
