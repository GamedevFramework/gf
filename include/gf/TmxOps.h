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
#ifndef GF_TMX_OPS_H
#define GF_TMX_OPS_H

#include "GraphicsApi.h"
#include "ResourceManager.h"
#include "TileLayer.h"
#include "Tmx.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics_tiles
   * @brief Create a tile layer object from a TMX tile layer
   *
   * @param map The TMX map
   * @param layer The TMX tile layer
   * @param resources The resource manager
   * @returns A tile layer object
   */
  GF_GRAPHICS_API TileLayer makeTileLayer(const TmxLayers& map, const TmxTileLayer& layer, ResourceManager& resources);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TMX_OPS_H
