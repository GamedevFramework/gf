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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#include <gf/LightSystem.h>

#include <cassert>
#include <cmath>
#include <algorithm>
#include <optional>

#include <gf/Color.h>
#include <gf/Easings.h>
#include <gf/Log.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>

#include <gfpriv/GlFwd.h>
#include <gfpriv/GlDebug.h>
#include <gfpriv/TextureCoords.h>

#include "generated/default.vert.h"
#include "generated/light_shape.frag.h"
#include "generated/light_unshadow.frag.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {
    void drawRawTexture(RenderTarget& target, const Texture& texture, BlendMode mode) {
      Sprite sprite(texture);

      RenderStates states;
      states.mode = mode;
      states.texture[0] = &texture;

      target.draw(sprite, states);
    }

    Image createPenumbraImage(int size) {
      Image image(gf::vec(size, size));

      for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
          if (x > size - y) {
            image.setPixel({ x, y }, gf::Color::toRgba32(gf::Color::Black));
          } else {
            image.setPixel({ x, y }, gf::Color::toRgba32(gf::Color::Gray(1.0f - static_cast<float>(x) / (static_cast<float>(size - y)))));
          }
        }
      }

      image.saveToFile("penumbra.png");

      return image;
    }


    struct Ray {
      Vector2f origin;
      Vector2f direction;
    };

    bool computeRayIntersection(const Ray& r0, const Ray& r1, Vector2f& intersection) {
      Vector w = r0.origin - r1.origin;

      float det = gf::cross(r0.direction, r1.direction);

      if (std::abs(det) < Epsilon) {
        return false;
      }

      float sI = gf::cross(r1.direction, w) / det;

      if (sI < 0.0f) {
        return false;
      }

      float tI = gf::cross(r0.direction, w) / det;

      if (tI < 0.0f) {
        return false;
      }

      intersection = r0.origin + sI * r0.direction;
      return true;
    }

    struct Penumbra {
      std::optional<Ray> outside1 = std::nullopt;
      std::optional<Ray> outside2 = std::nullopt;
      std::optional<Ray> inside1 = std::nullopt;
      std::optional<Ray> inside2 = std::nullopt;
    };

    Penumbra computePenumbraData(const LightPointEmission& light, const LightShape& shape) {
      std::size_t pointCount = shape.getPointCount();

      Vector2f sourceCenter = gf::transform(light.getTransform(), gf::transform(gf::translation(light.getOrigin()), light.getLocalCastCenter()));
      CircF source(sourceCenter, light.getSourceRadius());

//       gf::Log::debug("Source: (%g, %g) x %g\n", source.getCenter().x, source.getCenter().y, source.getRadius());
//       gf::Log::debug("Light: (%g, %g)\n", light.getPosition().x, light.getPosition().y);

      const Matrix3f transform = shape.getTransform();

      auto computeEdgeRays = [source,&transform](Vector2f localPoint) -> std::optional<std::tuple<Vector2f, Vector2f, Vector2f>> {
        Vector2f point = gf::transform(transform, localPoint);

        // https://math.stackexchange.com/a/3190374

        Vector2f direction = point - source.getCenter();
        Vector2f perp = gf::perp(direction);
        float distance = gf::euclideanLength(direction);

        if (distance < source.getRadius()) {
          return std::nullopt;
        }

        float rho = source.getRadius() / distance;
        float ad = gf::square(rho);
        float bd = rho * std::sqrt(1 - gf::square(rho));

        Vector2f t1 = source.getCenter() + ad * direction + bd * perp;
        Vector2f t2 = source.getCenter() + ad * direction - bd * perp;

//         gf::Log::debug("t1: (%g, %g), t2: (%g, %g)\n", t1.x, t1.y, t2.x, t2.y);

        Vector2f ray1 = point - t1;
        Vector2f ray2 = point - t2;

        return std::make_optional(std::make_tuple(point, ray1, ray2));
      };

      gf::Log::debug("----\n");

      Penumbra penumbra;

      for (std::size_t i = 0; i < pointCount; ++i) {
        auto opt = computeEdgeRays(shape.getPoint(i));

        if (!opt) {
          continue;
        }

        auto [ point, ray1, ray2 ] = opt.value();

        Vector2f prevPoint = gf::transform(transform, shape.getPrevPoint(i));
        Vector2f prevEdge = point - prevPoint;
        Vector2f nextPoint = gf::transform(transform, shape.getNextPoint(i));
        Vector2f nextEdge = nextPoint - point;


        float ray1VersusPrev = gf::cross(ray1, prevEdge);
        float ray1VersusNext = gf::cross(ray1, nextEdge);

        bool ray1Outside = ray1VersusPrev > 0 && ray1VersusNext < 0;

        if (ray1Outside) {
          gf::Log::debug("Ray1: [%zu] (%g, %g) + k * (%g, %g)  (outside)\n", i, point.x, point.y, ray1.x, ray1.y);
          assert(!penumbra.outside1.has_value());
          penumbra.outside1 = { point, ray1 };
        }

        bool ray1Inside = ray1VersusPrev < 0 && ray1VersusNext > 0;

        if (ray1Inside) {
          gf::Log::debug("Ray1: [%zu] (%g, %g) + k * (%g, %g)  (inside)\n", i, point.x, point.y, ray1.x, ray1.y);
          assert(!penumbra.inside1.has_value());
          penumbra.inside1 = { point, ray1 };
        }

        float ray2VersusPrev = gf::cross(ray2, prevEdge);
        float ray2VersusNext = gf::cross(ray2, nextEdge);

        bool ray2Inside = ray2VersusPrev > 0 && ray2VersusNext < 0;

        if (ray2Inside) {
          gf::Log::debug("Ray2: [%zu] (%g, %g) + k * (%g, %g)  (inside)\n", i, point.x, point.y, ray2.x, ray2.y);
          assert(!penumbra.inside2.has_value());
          penumbra.inside2 = { point, ray2 };
        }

        bool ray2Outside = ray2VersusPrev < 0 && ray2VersusNext > 0;

        if (ray2Outside) {
          gf::Log::debug("Ray2: [%zu] (%g, %g) + k * (%g, %g)  (outside)\n", i, point.x, point.y, ray2.x, ray2.y);
          assert(!penumbra.outside2.has_value());
          penumbra.outside2 = { point, ray2 };
        }
      }

      assert(penumbra.outside1.has_value() == penumbra.outside2.has_value());
      assert(penumbra.inside1.has_value() == penumbra.inside2.has_value());

      return penumbra;
    }

  }

  LightSystem::LightSystem(Vector2i size)
  : m_ambientColor(Color::Gray(0.1f))
  , m_penumbraTexture(createPenumbraImage(512))
  , m_unshadowShader(default_vert, light_unshadow_frag)
  , m_lightOverShapeShader(default_vert, light_shape_frag)
  , m_size(size)
  , m_view(RectF::fromSize(size))
  , m_lightTexture(size)
  , m_emissionTexture(size)
  , m_antumbraTexture(size)
  , m_compositionTexture(size)
  {
    m_penumbraTexture.setSmooth();
    m_lightOverShapeShader.setUniform("u_size", size);
  }

  LightId LightSystem::addLightShape(LightShape& shape) {
    LightId id = { LightType::Shape, static_cast<uint32_t>(m_shapes.size()) };
    m_shapes.push_back(shape);
    return id;
  }

  LightId LightSystem::addLightPoint(LightPointEmission& light) {
    LightId id = { LightType::Point, static_cast<uint32_t>(m_points.size()) };
    m_points.push_back(light);
    return id;
  }

//   LightId LightSystem::addLightDirection(Vector2f direction, float angle, float radius, float distance) {
//     LightId id = { LightType::Direction, static_cast<uint32_t>(m_directions.size()) };
//     return id;
//   }

  void LightSystem::removeLight(LightId id) {
    switch (id.type) {
      case LightType::Shape:
        assert(id.index < m_shapes.size() && m_shapes[id.index].get().isActive());
        m_shapes[id.index].get().setActive(false);
        break;
      case LightType::Point:
        assert(id.index < m_points.size() && m_points[id.index].get().isActive());
        m_points[id.index].get().setActive(false);
        break;
      case LightType::Direction:
        assert(id.index < m_directions.size() && m_directions[id.index].get().active);
        m_directions[id.index].get().active = false;
        break;
    }
  }

  void LightSystem::draw(RenderTarget& target, const RenderStates& states) {
    float currentClearColor[4];
    GL_CHECK(glGetFloatv(GL_COLOR_CLEAR_VALUE, currentClearColor));

    Vector2i size = target.getSize();

    if (size != m_size) {
      resize(size);
    }

    View view = target.getView();

    m_compositionTexture.setActive();
    m_compositionTexture.setView(m_view);
    m_compositionTexture.clear(m_ambientColor);

    // points


    for (LightPointEmission& point : m_points) {
      if (!point.isActive()) {
        continue;
      }

      renderLightPointEmission(point, view);

      m_compositionTexture.setActive();
      drawRawTexture(m_compositionTexture, m_lightTexture.getTexture(), BlendAdd);
    }





    m_compositionTexture.display();

    target.setActive();
    target.setView(m_view);
    drawRawTexture(target, m_compositionTexture.getTexture(), BlendMultiply);
    target.setView(view);

    if (m_dump) {
      dumpTextures();
      m_dump = false;
    }

    GL_CHECK(glClearColor(currentClearColor[0], currentClearColor[1], currentClearColor[2], currentClearColor[3]));
  }

  void LightSystem::dump() {
    m_dump = true;
  }

  void LightSystem::resize(Vector2i size) {
    m_size = size;
    m_view.reset(RectF::fromSize(size));
    m_lightTexture.resize(size);
    m_emissionTexture.resize(size);
    m_antumbraTexture.resize(size);
    m_compositionTexture.resize(size);
    m_lightOverShapeShader.setUniform("u_size", size);
  }

  void LightSystem::dumpTextures() {
    m_lightTexture.capture().saveToFile("debug_light.png");
    m_emissionTexture.capture().saveToFile("debug_emission.png");
    m_antumbraTexture.capture().saveToFile("debug_antumbra.png");
    m_compositionTexture.capture().saveToFile("debug_composition.png");
  }

  void LightSystem::renderLightPointEmission(LightPointEmission& light, const View& view) {
    RectF aabb = gf::transform(light.getTransform(), light.getLocalBounds());
    float shadowExtension = light.getShadowOverExtendMultiplier() * aabb.getExtentLength();

    m_emissionTexture.setActive();
    m_emissionTexture.clear(Color::Black);
    m_emissionTexture.setView(view);
    m_emissionTexture.draw(light);
    m_emissionTexture.display();

    m_lightTexture.setActive();
    m_lightTexture.setView(view);
    m_lightTexture.clear(Color::Black);
    m_lightTexture.draw(light);

    // shapes

    for (LightShape& shape : m_shapes) {
      if (!shape.isActive()) {
        continue;
      }

      auto penumbra = computePenumbraData(light, shape);

      if (!penumbra.outside1.has_value() || !penumbra.outside2.has_value()) {
        gf::Log::debug("No umbra!\n");
        continue;
      }

      m_antumbraTexture.setActive();
      m_antumbraTexture.setView(view);
      m_antumbraTexture.clear(Color::White);

      Vector2f intersection;

      if (computeRayIntersection(*penumbra.outside1, *penumbra.outside2, intersection)) {
        gf::Log::debug("Intersection!\n");
        ConvexShape mask(3);
        mask.setPoint(0, penumbra.outside1->origin);
        mask.setPoint(1, penumbra.outside2->origin);
        mask.setPoint(2, intersection);
        mask.setColor(Color::Black);
        m_antumbraTexture.draw(mask);

        for (std::size_t i = 0; i < 3; ++i) {
          gf::Log::debug("\t-> (%g, %g)\n", mask.getPoint(i).x, mask.getPoint(i).y);
        }
      } else {
        gf::Log::debug("No intersection!\n");
        ConvexShape mask(4);
        mask.setPoint(0, penumbra.outside1->origin);
        mask.setPoint(1, penumbra.outside2->origin);
        mask.setPoint(2, penumbra.outside2->origin + gf::normalize(penumbra.outside2->direction) * shadowExtension);
        mask.setPoint(3, penumbra.outside1->origin + gf::normalize(penumbra.outside1->direction) * shadowExtension);
        mask.setColor(Color::Black);
        m_antumbraTexture.draw(mask);

        for (std::size_t i = 0; i < 4; ++i) {
          gf::Log::debug("\t-> (%g, %g)\n", mask.getPoint(i).x, mask.getPoint(i).y);
        }
      }

//       if (penumbra.inside1.has_value() && penumbra.inside2.has_value()) {
//
//         auto displayPenumbra = [&](const Ray& inside, const Ray& outside) {
//           if (inside.origin == outside.origin) {
//             gf::Log::debug("Penumbra same origin!\n");
//
//             VertexArray vertices(PrimitiveType::Triangles, 3);
//
//             RenderStates states;
//             states.mode = BlendMultiply;
//             states.texture[0] = &m_penumbraTexture;
//
//             vertices[0].position = inside.origin;
//             vertices[1].position = outside.origin + gf::normalize(outside.direction) * shadowExtension;
//             vertices[2].position = inside.origin + gf::normalize(inside.direction) * shadowExtension;
//
//             vertices[0].texCoords = gf::priv::computeTextureCoords({ 0.0f, 1.0f });
//             vertices[1].texCoords = gf::priv::computeTextureCoords({ 1.0f, 0.0f });
//             vertices[2].texCoords = gf::priv::computeTextureCoords({ 0.0f, 0.0f });
//
//             m_antumbraTexture.draw(vertices, states);
//
//           } else {
//             Ray reverseOutside = { outside.origin, -outside.direction };
//
//             [[maybe_unused]] bool intersects = computeRayIntersection(inside, reverseOutside, intersection);
//             assert(intersects);
//
//             gf::Log::debug("Penumbra intersection!\n");
//
//             VertexArray vertices(PrimitiveType::Triangles, 3);
//
//             RenderStates states;
//             states.mode = BlendMultiply;
//             states.texture[0] = &m_penumbraTexture;
//
//             vertices[0].position = inside.origin;
//             vertices[1].position = inside.origin + gf::normalize(inside.direction) * shadowExtension;
//             vertices[2].position = outside.origin + gf::normalize(outside.direction) * shadowExtension;
//
//             vertices[0].texCoords = gf::priv::computeTextureCoords({ 0.0f, 1.0f });
//             vertices[1].texCoords = gf::priv::computeTextureCoords({ 0.0f, 0.0f });
//             vertices[2].texCoords = gf::priv::computeTextureCoords({ 1.0f, 0.0f });
//
//             m_antumbraTexture.draw(vertices, states);
//           }
//         };
//
//
//         displayPenumbra(*penumbra.inside1, *penumbra.outside2);
//         displayPenumbra(*penumbra.inside2, *penumbra.outside1);
//
//
//       } else {
//         gf::Log::debug("No penumbra!\n");
//       }

      m_antumbraTexture.display();

      m_lightTexture.setActive();
      m_lightTexture.setView(m_view);
      drawRawTexture(m_lightTexture, m_antumbraTexture.getTexture(), BlendMultiply);
      m_lightTexture.setView(view);
    }


    for (LightShape& shape : m_shapes) {
      if (!shape.isActive()) {
        continue;
      }

      if (shape.getVisibility() == LightShapeVisibility::Apparent) {
        shape.setColor(Color::White);

        RenderStates states;
        states.shader = &m_lightOverShapeShader;
        states.texture[0] = &m_emissionTexture.getTexture();
        m_lightTexture.draw(shape, states);
      } else {
        shape.setColor(Color::Black);
        m_lightTexture.draw(shape);
      }
    }

    m_lightTexture.display();
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
