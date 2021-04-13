/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2021 Julien Bernard
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
#ifndef GF_LIGHT_SYSTEM_H
#define GF_LIGHT_SYSTEM_H

#include <cstdint>
#include <vector>

#include "Drawable.h"
#include "GraphicsApi.h"
#include "LightPointEmission.h"
#include "LightShape.h"
#include "Polygon.h"
#include "Ref.h"
#include "RenderTexture.h"
#include "Shader.h"
#include "Shapes.h"
#include "Texture.h"
#include "Vector.h"
#include "View.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif
  struct RenderStates;
  class Texture;

  enum class LightType : uint32_t {
    Shape,
    Point,
    Direction,
  };

  struct GF_GRAPHICS_API LightId {
    LightType type;
    uint32_t index;
  };

  class GF_GRAPHICS_API LightSystem : public Drawable {
  public:
    LightSystem(Vector2i size);

    void setAmbientColor(const Color4f& color) {
      m_ambientColor = color;
    }

    LightId addLightShape(LightShape& shape);
    LightId addLightPoint(LightPointEmission& light);

//     LightId addLightDirection(LightDirectionEmission& light);

    void removeLight(LightId id);

    void draw(RenderTarget& target, const RenderStates& states) override;

    void dump();

  private:

    struct LightDirection {

      bool active = true;
    };

  private:
    void resize(Vector2i size);

    void renderLightPointEmission(LightPointEmission& light, const View& view);

    void dumpTextures();

  private:
    std::vector<Ref<LightShape>> m_shapes;
    std::vector<Ref<LightPointEmission>> m_points;
    std::vector<Ref<LightDirection>> m_directions;

    Color4f m_ambientColor;

    Texture m_penumbraTexture;
    Shader m_unshadowShader;
    Shader m_lightOverShapeShader;

    Vector2i m_size;
    View m_view;
    RenderTexture m_lightTexture;
    RenderTexture m_emissionTexture;
    RenderTexture m_antumbraTexture;
    RenderTexture m_compositionTexture;

    bool m_dump = false;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_LIGHT_SYSTEM_H
