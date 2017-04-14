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
#ifndef GF_DRAWABLE_H
#define GF_DRAWABLE_H

#include "Portability.h"
#include "RenderStates.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class RenderTarget;

  /**
   * @ingroup graphics
   * @brief Abstract base class for objects that can be drawn to a render window
   *
   * gf::Drawable is a very simple base class that allows objects
   * of derived classes to be drawn to a sf::RenderTarget.
   *
   * All you have to do in your derived class is to override the
   * `draw()` virtual function.
   *
   * Note that inheriting from `gf::Drawable` is not mandatory,
   * but it allows this nice syntax `target.draw(object)` rather
   * than `object.draw(target)`, which is more consistent with other
   * classes.
   *
   * Example:
   *
   * ~~~{.cc}
   * class MyDrawable : public gf::Drawable {
   * public:
   *
   *    ...
   *
   *   virtual void draw(gf::RenderTarget& target, gf::RenderStates states) override {
   *     // You can draw other high-level objects
   *     target.draw(m_sprite, states);
   *
   *     // ... or use the low-level API
   *     states.texture = &m_texture;
   *     target.draw(m_vertices, states);
   *
   *   }
   *
   *
   * private:
   *   gf::Sprite m_sprite;
   *   gf::Texture m_texture;
   *   gf::VertexArray m_vertices;
   * };
   * ~~~
   *
   */
  class GF_API Drawable {
  public:
    /**
     * @brief Virtual desctructor
     */
    virtual ~Drawable();

    /**
     * @brief Draw the object to a render target
     *
     * This is a pure virtual function that has to be implemented
     * by the derived class to define how the drawable should be
     * drawn.
     *
     * @param target Render target to draw to
     * @param states Current render states
     */
    virtual void draw(RenderTarget& target, RenderStates states) = 0;

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_DRAWABLE_H
