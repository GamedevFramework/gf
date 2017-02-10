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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#ifndef GF_RENDER_STATES_H
#define GF_RENDER_STATES_H

#include "Blend.h"
#include "Matrix.h"
#include "Portability.h"
#include "Transform.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class BareTexture;
  class Shader;

  /**
   * @ingroup graphics
   * @brief Define the states used for drawing to a RenderTarget
   *
   * There are four global states that can be applied to
   * the drawn objects:
   *
   * - the blending mode: how pixels of the object are blended with the background
   * - the transform matrix: how the object is positioned/rotated/scaled
   * - the texture: what image is mapped to the object
   * - the shader: what custom effect is applied to the object
   *
   * High-level objects such as sprites or text force some of
   * these states when they are drawn. For example, a sprite
   * will set its own texture, so that you don't have to care
   * about it when drawing the sprite.
   *
   * The transform is a special case: sprites, texts and shapes
   * (and it's a good idea to do it with your own drawable classes
   * too) combine their transform with the one that is passed in the
   * gf::RenderStates structure. So that you can use a "global" transform
   * on top of each object's transform.
   *
   * Most objects, especially high-level drawables, can be drawn
   * directly without defining render states explicitly -- the
   * default set of states is ok in most cases.
   *
   * ~~~{.cc}
   * window.draw(sprite);
   * ~~~
   *
   * When you're inside the `draw()` function of a drawable
   * object (inherited from gf::Drawable), you can
   * either pass the render states unmodified, or change
   * some of them.
   *
   * For example, a transformable object will combine the
   * current transform with its own transform. A sprite will
   * set its texture. Etc.
   *
   * @sa gf::RenderTarget, gf::Drawable
   */
  struct GF_API RenderStates {
    BlendMode mode = BlendAlpha; ///< The blending mode
    Matrix3f transform = identityTransform(); ///< The transform matrix
    const BareTexture *texture = nullptr; ///< The texture
    Shader *shader = nullptr; ///< The shader
    float lineWidth = 0.0f; ///< The line width
  };

  /**
   * @ingroup graphics
   * @relates RenderStates
   * @brief Check render states equality
   *
   * Two render states are equals if their blend mode, their transform
   * matrix, their texture and their shader are the same.
   */
  inline
  bool operator==(const RenderStates& lhs, const RenderStates& rhs) {
    return lhs.mode == rhs.mode && lhs.transform == rhs.transform && lhs.texture == rhs.texture && lhs.shader == rhs.shader;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_RENDER_STATES_H
