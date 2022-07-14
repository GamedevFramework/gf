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
#include <gf/Drawable.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Texture.h>
#include <gf/VertexArray.h>

/// [mydrawable]
class MyDrawable : public gf::Drawable {
public:

  // ...

  virtual void draw(gf::RenderTarget& target, const gf::RenderStates& states) override {
    // You can draw other high-level objects
    target.draw(m_sprite, states);

    // ... or use the low-level API
    gf::RenderStates localStates = states;
    localStates.texture[0] = &m_texture;
    target.draw(m_vertices, localStates);
  }

private:
  gf::Sprite m_sprite;
  gf::Texture m_texture;
  gf::VertexArray m_vertices;
};
/// [mydrawable]
