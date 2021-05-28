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
#ifndef GF_TILE_PROPERTIES_H
#define GF_TILE_PROPERTIES_H

#include <functional>

#include "GraphicsApi.h"
#include "Polyline.h"
#include "Rect.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics_tiles
   * @brief The properties of tiles
   *
   * These properties depend on the orientation of the tiles in the map.
   *
   * @sa gf::TileOrientation
   */
  class GF_GRAPHICS_API TileProperties {
  public:
    virtual ~TileProperties();

    virtual RectF computeBounds(Vector2i layerSize) const noexcept = 0;

    virtual RectI computeVisibleArea(const RectF& local) const noexcept = 0;

    virtual RectF computeCellBounds(Vector2i coords) const noexcept = 0;

    virtual Vector2i computeCoordinates(Vector2f position) const noexcept = 0;

    virtual Polyline computePolyline(Vector2i coords) const = 0;

    virtual void forEachNeighbor(Vector2i coords, Vector2i layerSize, std::function<void(Vector2i)> func) = 0;
  };


  /**
   * @ingroup graphics_tiles
   * @brief The generic properties of tiles
   *
   * For tile orientation that have a helper, they can be instantiated directly
   * thanks to this class.
   *
   * @sa gf::OrthogonalHelper, gf::StaggerHelper, gf::HexagonHelper
   */
  template<typename Helper>
  class GenericTileProperties : public TileProperties {
  public:
    GenericTileProperties(Helper helper)
    : m_helper(std::move(helper))
    {
    }

    RectF computeBounds(Vector2i layerSize) const noexcept override {
      return m_helper.computeBounds(layerSize);
    }

    RectI computeVisibleArea(const RectF& local) const noexcept override {
      return m_helper.computeVisibleArea(local);
    }

    RectF computeCellBounds(Vector2i coords) const noexcept override {
      return m_helper.computeCellBounds(coords);
    }

    Vector2i computeCoordinates(Vector2f position) const noexcept override {
      return m_helper.computeCoordinates(position);
    }

    Polyline computePolyline(Vector2i coords) const override {
      return m_helper.computePolyline(coords);
    }

    void forEachNeighbor(Vector2i coords, Vector2i layerSize, std::function<void(Vector2i)> func) override {
      m_helper.forEachNeighbor(coords, layerSize, std::move(func));
    }

  private:
    Helper m_helper;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TILE_PROPERTIES_H
