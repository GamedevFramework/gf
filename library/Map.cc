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
#include <gf/Map.h>

#include <iostream>
#include <limits>

#include <gf/Geometry.h>
#include <gf/VectorOps.h>

#include <boost/heap/binomial_heap.hpp>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  SquareMap::SquareMap(Vector2i size)
  : m_cells(size, None)
  {

  }

  Vector2i SquareMap::getSize() const {
    return m_cells.getSize();
  }

  PositionRange<int> SquareMap::getRange() const {
    return m_cells.getPositionRange();
  }

  void SquareMap::setCell(Vector2i pos, CellFlags flags) {
    m_cells(pos) = flags;
  }

  void SquareMap::clear(CellFlags flags) {
    for (auto& cell : m_cells) {
      cell = flags;
    }
  }

  void SquareMap::setTransparent(Vector2i pos) {
    m_cells(pos).set(CellProperty::Transparent);
  }

  bool SquareMap::isTransparent(Vector2i pos) const {
    return m_cells(pos).test(CellProperty::Transparent);
  }

  void SquareMap::setWalkable(Vector2i pos) {
    m_cells(pos).set(CellProperty::Walkable);
  }

  bool SquareMap::isWalkable(Vector2i pos) const {
    return m_cells(pos).test(CellProperty::Walkable);
  }

  void SquareMap::setEmpty(Vector2i pos) {
    m_cells(pos) = EmptyCell;
  }

  /*
   * FoV
   */

  void SquareMap::clearFieldOfVision() {
    for (auto& cell : m_cells) {
      cell.reset(CellProperty::Visible);
    }
  }

  void SquareMap::clearExplored() {
    for (auto& cell : m_cells) {
      cell.reset(CellProperty::Explored);
    }
  }

  static void postProcessMap(Array2D<CellFlags, int>& cells, Vector2i q0, Vector2i q1, Vector2i step) {
    int xLo, xHi, yLo, yHi;
    std::tie(xLo, xHi) = std::minmax(q0.x, q1.x);
    std::tie(yLo, yHi) = std::minmax(q0.y, q1.y);

    for (int y = yLo; y <= yHi; ++y) {
      for (int x = xLo; x <= xHi; ++x) {
        if (!cells.isValid({ x, y })) {
          continue;
        }

        if (!cells({ x, y }).test(CellProperty::Visible) || !cells({ x, y }).test(CellProperty::Transparent)) {
          continue;
        }

        int x2 = x + step.x;
        int y2 = y + step.y;

        if (xLo <= x2 && x2 <= xHi) {
          gf::Vector2i target = { x2, y };

          if (cells.isValid(target) && !cells(target).test(CellProperty::Transparent)) {
            cells(target).set(CellProperty::Visible);
          }
        }

        if (yLo <= y2 && y2 <= yHi) {
          gf::Vector2i target = { x, y2 };

          if (cells.isValid(target) && !cells(target).test(CellProperty::Transparent)) {
            cells(target).set(CellProperty::Visible);
          }
        }

        if (xLo <= x2 && x2 <= xHi && yLo <= y2 && y2 <= yHi) {
          gf::Vector2i target = { x2, y2 };

          if (cells.isValid(target) && !cells(target).test(CellProperty::Transparent)) {
            cells(target).set(CellProperty::Visible);
          }
        }
      }
    }
  }

  static void castRay(Array2D<CellFlags, int>& cells, Vector2i p0, Vector2i p1, int maxRadius2, FieldOfVisionLimit limit, CellFlags modification) {
    Bresenham bresenham(p0, p1);
    Vector2i curr;
    bool blocked = false;

    while (!bresenham.step(curr)) {
      if (!cells.isValid(curr)) {
        return;
      }

      if (maxRadius2 > 0) {
        int radius2 = gf::squareDistance(p0, curr);

        if (radius2 > maxRadius2) {
          return;
        }
      }

      if (!blocked && !cells(curr).test(CellProperty::Transparent)) {
        blocked = true;
      } else if (blocked) {
        return; // wall
      }

      if (limit == FieldOfVisionLimit::Included || !blocked) {
        cells(curr) |= modification;
      }
    }
  }

  static void computeBasicFov(Array2D<CellFlags, int>& cells, Vector2i pos, int maxRadius, FieldOfVisionLimit limit, CellFlags modification) {
    RangeI xRange = cells.getColRange();
    RangeI yRange = cells.getRowRange();

    int maxRadius2 = maxRadius * maxRadius;

    if (maxRadius > 0) {
      xRange.lo = std::max(xRange.lo, pos.x - maxRadius);
      xRange.hi = std::min(xRange.hi, pos.x + maxRadius);
      yRange.lo = std::max(yRange.lo, pos.y - maxRadius);
      yRange.hi = std::min(yRange.hi, pos.y + maxRadius);
    } else {
      maxRadius2 = 0;
    }

    cells(pos) |= modification;

    for (auto x : xRange) {
      castRay(cells, pos, { x, yRange.lo }, maxRadius2, limit, modification);
      castRay(cells, pos, { x, yRange.hi }, maxRadius2, limit, modification);
    }

    for (auto y : yRange) {
      castRay(cells, pos, { xRange.lo, y }, maxRadius2, limit, modification);
      castRay(cells, pos, { xRange.hi, y }, maxRadius2, limit, modification);
    }

    if (limit == FieldOfVisionLimit::Included) {
      postProcessMap(cells, pos, { xRange.lo, yRange.lo }, { -1, -1 });
      postProcessMap(cells, pos, { xRange.hi, yRange.lo }, {  1, -1 });
      postProcessMap(cells, pos, { xRange.lo, yRange.hi }, { -1,  1 });
      postProcessMap(cells, pos, { xRange.hi, yRange.hi }, {  1,  1 });
    }
  }

  static void computeGenericFieldOfVision(Array2D<CellFlags, int>& cells, Vector2i pos, int maxRadius, FieldOfVisionLimit limit, FieldOfVision algorithm, CellFlags modification) {
    switch (algorithm) {
      case FieldOfVision::Basic:
        computeBasicFov(cells, pos, maxRadius, limit, modification);
        break;

      default:
        break;
    }
  }

  void SquareMap::computeFieldOfVision(Vector2i pos, int maxRadius, FieldOfVisionLimit limit, FieldOfVision algorithm) {
    computeGenericFieldOfVision(m_cells, pos, maxRadius, limit, algorithm, CellProperty::Visible | CellProperty::Explored);
  }

  void SquareMap::computeLocalFieldOfVision(Vector2i pos, int maxRadius, FieldOfVisionLimit limit, FieldOfVision algorithm) {
    computeGenericFieldOfVision(m_cells, pos, maxRadius, limit, algorithm, CellProperty::Visible);
  }

  bool SquareMap::isInFieldOfVision(Vector2i pos) const {
    return m_cells(pos).test(CellProperty::Visible);
  }

  bool SquareMap::isExplored(Vector2i pos) const {
    return m_cells(pos).test(CellProperty::Explored);
  }

  /*
   * Route
   */

  namespace {

    struct DijkstraHeapData {
      Vector2i position;
      float distance;
    };

    static bool operator<(const DijkstraHeapData& lhs, const DijkstraHeapData& rhs) {
      return lhs.distance > rhs.distance;
    }

    using DijkstraHeap = boost::heap::binomial_heap<DijkstraHeapData>;

    struct DijkstraResultData {
      float distance;
      Vector2i previous;
      DijkstraHeap::handle_type handle;
    };
  }

  static std::vector<Vector2i> computeDijkstra(const Array2D<CellFlags, int>& cells, Vector2i origin, Vector2i target, float diagonalCost) {
    DijkstraResultData defaultResult;
    defaultResult.distance = std::numeric_limits<float>::infinity();
    defaultResult.previous = { -1, -1 };

    Array2D<DijkstraResultData, int> results(cells.getSize(), defaultResult);

    results(origin).distance = 0.0f;

    DijkstraHeap heap;

    for (auto position : cells.getPositionRange()) {
      const auto& cell = cells(position);

      if (!cell.test(CellProperty::Walkable)) {
        continue;
      }

      DijkstraHeapData data;
      data.position = position;
      data.distance = results(position).distance;

      results(position).handle = heap.push(data);
    }

    while (!heap.empty()) {
      DijkstraHeapData data = heap.top();
      heap.pop();

      cells.visit8Neighbors(data.position, [&](Vector2i position, const CellFlags& value) {
        if (!value.test(CellProperty::Walkable)) {
          return;
        }

        bool isDiagonal = (gf::manhattanDistance(data.position, position) == 2);

        if (isDiagonal && diagonalCost == 0) {
          return;
        }

        float newDistance = results(data.position).distance + (isDiagonal ? diagonalCost : 1.0f);

        if (newDistance < results(position).distance) {
          auto& result = results(position);
          result.distance = newDistance;
          result.previous = data.position;

          assert((*result.handle).position == position);
          (*result.handle).distance = newDistance;
          heap.increase(result.handle);
        }
      });

    }

    std::vector<Vector2i> route;
    Vector2i curr = target;

    while (curr != origin) {
      assert(curr.x != -1 && curr.y != -1);
      route.push_back(curr);
      curr = results(curr).previous;
    }

    route.push_back(origin);
    std::reverse(route.begin(), route.end());

    assert(!route.empty());

    return route;
  }


  namespace {

    struct AStarHeapData {
      Vector2i position;
      float priority;
    };

    static bool operator<(const AStarHeapData& lhs, const AStarHeapData& rhs) {
      return lhs.priority > rhs.priority;
    }

    using AStarHeap = boost::heap::binomial_heap<AStarHeapData>;

    enum class AStarState {
      None,
      Open,
      Closed,
    };

    struct AStarResultData {
      float distance;
      Vector2i previous;
      AStarState state;
      AStarHeap::handle_type handle;
    };

  }

  static std::vector<Vector2i> computeAStar(const Array2D<CellFlags, int>& cells, Vector2i origin, Vector2i target, float diagonalCost) {
    AStarResultData defaultResult;
    defaultResult.distance = std::numeric_limits<float>::infinity();
    defaultResult.previous = { -1, -1 };
    defaultResult.state = AStarState::None;

    Array2D<AStarResultData, int> results(cells.getSize(), defaultResult);

    results(origin).distance = 0.0f;
    results(origin).state = AStarState::Open;

    AStarHeap heap;
    results(origin).handle = heap.push({ origin, 0.0f });

    // see http://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html#heuristics-for-grid-maps
    auto heuristic = [diagonalCost](Vector2i p0, Vector2i p1) {
      if (diagonalCost == 0) {
        return 1.0f * gf::manhattanDistance(p0, p1);
      }

      Vector2i d = gf::abs(p0 - p1);
      return 1.0f * (d.x + d.y) + (diagonalCost - 2.0f) * std::min(d.x, d.y);
    };

    while (!heap.empty()) {
      AStarHeapData data = heap.top();
      heap.pop();

      assert(results(data.position).state == AStarState::Open);

      if (data.position == target) {
        break;
      }

      results(data.position).state = AStarState::Closed;

      cells.visit8Neighbors(data.position, [&](Vector2i position, const CellFlags& value) {
        if (!value.test(CellProperty::Walkable)) {
          return;
        }

        if (results(position).state == AStarState::Closed) {
          return;
        }

        bool isDiagonal = (gf::manhattanDistance(data.position, position) == 2);

        if (isDiagonal && diagonalCost == 0) {
          return;
        }

        float newDistance = results(data.position).distance + (isDiagonal ? diagonalCost : 1.0f);

        if (newDistance < results(position).distance) {
          auto& result = results(position);
          result.distance = newDistance;
          result.previous = data.position;

          float priority = newDistance + heuristic(position, target) * 1.001;

          if (result.state == AStarState::Open) {
            assert((*result.handle).position == position);

            if ((*result.handle).priority != priority) {
              (*result.handle).priority = priority;
              heap.update(result.handle);
            }
          } else {
            assert(result.state == AStarState::None);
            result.handle = heap.push({ position, priority });
            result.state = AStarState::Open;
          }
        }
      });

    }

    std::vector<Vector2i> route;
    Vector2i curr = target;

    while (curr != origin) {
      assert(curr.x != -1 && curr.y != -1);
      route.push_back(curr);
      curr = results(curr).previous;
    }

    route.push_back(origin);
    std::reverse(route.begin(), route.end());

    assert(!route.empty());

    return route;
  }


  std::vector<Vector2i> SquareMap::computeRoute(Vector2i origin, Vector2i target, float diagonalCost, Route algorithm) {
    switch (algorithm) {
      case Route::Dijkstra:
        return computeDijkstra(m_cells, origin, target, diagonalCost);

      case Route::AStar:
        return computeAStar(m_cells, origin, target, diagonalCost);
    }

    return { };
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
