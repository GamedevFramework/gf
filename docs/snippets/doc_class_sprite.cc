/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Unused.h>

void dummySpriteUsage(gf::RenderTarget& renderer) {
  /// [sprite]
  // Declare and load a texture
  gf::Texture texture("texture.png");

  // Create a sprite
  gf::Sprite sprite;
  sprite.setTexture(texture);
  sprite.setTextureRect(gf::RectF::fromPositionSize({ 0.1f, 0.1f }, { 0.5f, 0.3f }));
  sprite.setColor({ 1.0f, 1.0f, 1.0f, 0.8f });
  sprite.setPosition({ 100.0f, 25.0f });

  // Draw it
  renderer.draw(sprite);
  /// [sprite]
}
