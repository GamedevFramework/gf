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
#ifndef GF_MAP_H
#define GF_MAP_H

#include "Array2D.h"
#include "Flags.h"
#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief A property of a cell
   *
   * @sa gf::CellFlags, gf::SquareMap
   */
  enum class CellProperty : uint8_t {
    Transparent = 0x01, ///< The cell is transparent
    Walkable    = 0x02, ///< The cell is walkable
    Visible     = 0x10, ///< The cell is visible (computed by FoV)
    Explored    = 0x20, ///< The cell has been explored (computed by FoV)
  };

  /**
   * @ingroup game
   * @brief Flags composed of cell properties
   *
   * @sa gf::CellProperty
   */
  using CellFlags = Flags<CellProperty>;

  /**
   * @ingroup game
   * @brief An empty cell
   *
   * An empty cell is transparent and walkable.
   *
   * @sa gf::CellProperty, gf::SquareMap
   */
  constexpr CellFlags EmptyCell = combineFlags(CellProperty::Transparent, CellProperty::Walkable);

  /**
   * @ingroup game
   * @brief Algorithm for computing a field of vision
   *
   * @sa gf::SquareMap
   */
  enum class FieldOfVision {
    Basic,  ///< A basic algorithm based on ray casting
  };

  /**
   * @ingroup game
   * @brief Constant to indicate if the limit is part of the field of vision
   *
   * The limit is generally a wall that is next to a visible ground cell.
   *
   * @sa gf::SquareMap
   */
  enum class FieldOfVisionLimit {
    Included, ///< The limits are included in the field of vision
    Excluded, ///< The limits are *not* included in the field of vision
  };

  /**
   * @ingroup game
   * @brief Algorithm for computing a route
   *
   * @sa gf::SquareMap
   */
  enum class Route {
    AStar,    ///< The A* algorithm
    Dijkstra, ///< The Dijkstra algorithm
  };

  /**
   * @ingroup game
   * @brief A square map
   *
   * A square map is a model of map where cells are organized in a square grid.
   * This type of map is quite common in games. gf provides some useful
   * algorithms related to square maps: field of vision, route finding.
   *
   * A cell can be transparent and/or walkable. By default, all cells are
   * neither transparent nor walkable i.e. they are walls. The transparent
   * property of a cell is used to compute the field of vision. The walkable
   * property of a cell is used to compute routes. A cell can be transparent
   * and not walkable (e.g. lava or water), it can be walkable and not
   * transparent (e.g. a secret passage).
   *
   * @sa gf::CellProperty
   */
  class GF_API SquareMap {
  public:


    /**
     * @brief Constructor
     *
     * @param size The size of the map
     */
    SquareMap(Vector2i size);

    /**
     * @brief Get the size of the map
     *
     * @returns The size of the map
     */
    Vector2i getSize() const;

    /**
     * @brief Get a range of the positions of the map
     *
     * @returns A 2D range of all the positions
     *
     * @sa Array2D::getPositionRange()
     */
    PositionRange<int> getRange() const;

    /**
     * @name Cell properties
     * @{
     */

    /**
     * @brief Set the properties of a cell
     *
     * This function directly set all the properties of a cell. You should use
     * the other functions that set a particular property.
     *
     * @param pos The position of the cell
     * @param flags The properties of the cell
     *
     * @sa setTransparent(), setWalkable(), setEmpty()
     */
    void setCell(Vector2i pos, CellFlags flags);

    /**
     * @brief Initialize the cells with some properties
     *
     * @param flags The properties to set
     */
    void clear(CellFlags flags);

    /**
     * @brief Make a cell transparent
     *
     * @param pos The position of the cell
     *
     * @sa isTransparent()
     */
    void setTransparent(Vector2i pos);

    /**
     * @brief Check if a cell is transparent
     *
     * @returns True if the cell is transparent
     *
     * @sa setTransparent()
     */
    bool isTransparent(Vector2i pos) const;

    /**
     * @brief Make a cell walkable
     *
     * @param pos The position of the cell
     *
     * @sa isWalkable()
     */
    void setWalkable(Vector2i pos);

    /**
     * @brief Check if a cell is walkable
     *
     * @sa setWalkable()
     */
    bool isWalkable(Vector2i pos) const;

    /**
     * @brief Make a cell empty
     *
     * An empty cell is walkable and transparent
     *
     * @param pos The position of the cell
     */
    void setEmpty(Vector2i pos);

    /**
     * @}
     */

    /**
     * @name Field of Vision
     * @{
     */

    /**
     * @brief Make the whole map not visible
     *
     * You should call this function before computing a new field of vision.
     *
     * @sa computeFieldOfVision()
     */
    void clearFieldOfVision();

    /**
     * @brief Make the whole map not explored
     *
     * You should call this function before exploring a new map.
     *
     * @sa computeFieldOfVision()
     */
    void clearExplored();

    /**
     * @brief Compute a field of vision
     *
     * The map is not cleared before computing the field of vision. The
     * algorithm use the transparent property of the cells. After calling this
     * function, some cells are marked visible.
     *
     * This algorithm marks visible cells as explored.
     *
     * @param pos The position of the entity
     * @param maxRadius The maximum radius that the entity can see
     * @param limit Is the limit included in the field of vision?
     * @param algorithm The algorithm to use for computing the field of vision
     *
     * @sa clearFieldOfVision(), isInFieldOfVision()
     */
    void computeFieldOfVision(Vector2i pos, int maxRadius = 0, FieldOfVisionLimit limit = FieldOfVisionLimit::Included, FieldOfVision algorithm = FieldOfVision::Basic);

    /**
     * @brief Compute a local field of vision
     *
     * The map is not cleared before computing the field of vision. The
     * algorithm use the transparent property of the cells. After calling this
     * function, some cells are marked visible.
     *
     * This algorithm does not mark visible cells as explored. It can be used
     * for computing an ennemy field of view without modifying the explored
     * area of the hero.
     *
     * @param pos The position of the entity
     * @param maxRadius The maximum radius that the entity can see
     * @param limit Is the limit included in the field of vision?
     * @param algorithm The algorithm to use for computing the field of vision
     *
     * @sa clearFieldOfVision(), isInFieldOfVision()
     */
    void computeLocalFieldOfVision(Vector2i pos, int maxRadius = 0, FieldOfVisionLimit limit = FieldOfVisionLimit::Included, FieldOfVision algorithm = FieldOfVision::Basic);

    /**
     * @brief Check if a cell is visible
     *
     * Cells can be made visible by computing a field of vision.
     *
     * @returns True if the cell is visible
     *
     * @sa computeFieldOfVision()
     */
    bool isInFieldOfVision(Vector2i pos) const;

    /**
     * @brief Check if a cell is explored
     *
     * Cells are explored if they have been in the field of vision after a
     * call to clearExplored().
     *
     * @sa computeFieldOfVision(), isInFieldOfVision()
     */
    bool isExplored(Vector2i pos) const;

    /**
     * @}
     */

    /**
     * @name Route
     * @{
     */

    /**
     * @brief Compute a route between two points
     *
     * The algorithm use the walkable property of the cells. Diagonal movement
     * can be allowed and its cost can be adjusted (defaults to
     * @f$ \sqrt{2} @f$).
     *
     * @param origin The origin of the route
     * @param target The target of the route
     * @param diagonalCost The cost of going diagonal between two cells (0 means no diagonal movement)
     * @param algorithm The algorithm to use for computing the route
     * @returns The route between the two points (included)
     */
    std::vector<Vector2i> computeRoute(Vector2i origin, Vector2i target, float diagonalCost = Sqrt2, Route algorithm = Route::AStar);

    /**
     * @}
     */

  private:
    Array2D<CellFlags, int> m_cells;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}

template<>
struct EnableBitmaskOperators<CellProperty> {
  static constexpr bool value = true;
};
#endif
}



#endif // GF_MAP_H
