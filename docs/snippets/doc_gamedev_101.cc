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

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/Texture.h>

#include "doc_utils.h"

/// [entity]
class MyEntity : public gf::Entity {
public:
  virtual void update(gf::Time dt) override;

  virtual void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

private:
  // data of my entity
};
/// [entity]


int main() {

  {
  /// [init]
  gf::Font font("path/to/Arial.ttf");
  gf::Texture texture("path/to/image.png");
  /// [init]
  }

  {
  /// [resources]
  gf::ResourceManager resources;
  resources.addSearchDir("path/to/data");

  gf::Font& font = resources.getFont("Arial.ttf");
  gf::Texture& texture = resources.getTexture("image.png");

  gf::Font& anotherFont = resources.getFont("Arial.ttf"); // the font is not loaded a second time
  /// [resources]

  unused(font, texture, anotherFont);
  }


  return 0;
}
