/*
 * H.O.M.E.
 * Copyright (C) 2019 Hatunruna team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Physics.h"

#include <set>
#include <vector>

#include <gf/Array2D.h>
#include <gf/Color.h>
#include <gf/Curves.h>
#include <gf/Log.h>
#include <gf/Polyline.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>

namespace home {

  namespace {

    constexpr float PhysicsScale = 0.02f;

    b2Vec2 fromVec(gf::Vector2f vec) {
      return { vec.x * PhysicsScale, vec.y * PhysicsScale };
    }

    gf::Vector2f toVec(b2Vec2 vec) {
      return { vec.x / PhysicsScale, vec.y / PhysicsScale };
    }

    struct Segment {
      gf::Vector2i p1;
      gf::Vector2i p2;

      void reverse() {
        std::swap(p1, p2);
      }
    };

    bool operator<(const Segment& lhs, const Segment& rhs) {
      return std::tie(lhs.p1.x, lhs.p1.y, lhs.p2.x, lhs.p2.y) < std::tie(rhs.p1.x, rhs.p1.y, rhs.p2.x, rhs.p2.y);
    }

    template<typename Iterator>
    Iterator findNextSegment(Iterator begin, Iterator end, gf::Vector2i endPoint, bool& needReverse) {
      for (auto it = begin; it != end; ++it) {
        if (it->p1 == endPoint) {
          needReverse = false;
          return it;
        }

        if (it->p2 == endPoint) {
          needReverse = true;
          return it;
        }
      }

      return end;
    }

    std::vector<gf::Polyline> computeAutoCollision(const std::vector<Segment>& segments) {
      // not optimized at all
      std::vector<gf::Polyline> lines;
      std::set<Segment> remaining(segments.begin(), segments.end());

      for (auto it = remaining.begin(); it != remaining.end(); /* not here */) {
        // start a new line
        gf::Polyline polyline(gf::Polyline::Loop);

        Segment first = *it;
        polyline.addPoint(first.p1);

        gf::Vector2i endPoint = first.p2;

        for (;;) {
          polyline.addPoint(endPoint);

          bool needReverse;
          auto next = findNextSegment(std::next(it), remaining.end(), endPoint, needReverse);

          if (next == remaining.end()) {
            // the line is a chain
            polyline.setType(gf::Polyline::Chain);
            break;
          }

          Segment chosen = *next;
          remaining.erase(next);

          if (needReverse) {
            chosen.reverse();
          }

          assert(chosen.p1 == endPoint);
          endPoint = chosen.p2;

          if (endPoint == first.p1) {
            // the line is a loop
            break;
          }
        }

        auto prev = it++;
        remaining.erase(prev);

        polyline.simplify();

        lines.push_back(std::move(polyline));
      }

      return lines;
    }

    class PhysicsMaker : public gf::TmxVisitor {
      static constexpr int Land = 0;
      static constexpr int Void = 1;

    public:
      PhysicsMaker(b2World& world)
      : m_world(world)
      {

      }

      virtual void visitTileLayer(const gf::TmxLayers& map, const gf::TmxTileLayer& layer) override {
        if (!layer.visible) {
          return;
        }

        assert(map.orientation == gf::TileOrientation::Staggered);
        gf::Log::info("Parsing layer '%s'\n", layer.name.c_str());

        gf::Vector2i tileSize = map.tileSize;
        gf::Vector2i tilesetTileSize = tileSize;

        gf::Array2D<int, int> biomes(map.mapSize, Void);

        int k = 0;

        for (auto& cell : layer.cells) {
          int i = k % map.mapSize.width;
          int j = k / map.mapSize.width;
          assert(j < map.mapSize.height);

          int gid = cell.gid;

          if (gid != 0) {
            biomes({ i, j }) = Land;

            auto tileset = map.getTileSetFromGID(gid);
            assert(tileset);
            tilesetTileSize = tileset->tileSize;
          }

          k++;
        }

        std::vector<Segment> segments;

        for (auto pos : biomes.getPositionRange()) {
          int biome = biomes(pos);

          static constexpr std::pair<gf::Vector2i, gf::Vector2i> NeighborsEven[] = {
            { { -1, -1 }, { -1, -1 } }, // NW
            { {  0, -1 }, {  1, -1 } }, // NE
            { { -1,  1 }, { -1,  1 } }, // SW
            { {  0,  1 }, {  1,  1 } }, // SE
          };

          static constexpr std::pair<gf::Vector2i, gf::Vector2i> NeighborsOdd[] = {
           { {  0, -1 }, { -1, -1 } }, // NW
           { {  1, -1 }, {  1, -1 } }, // NE
           { {  0,  1 }, { -1,  1 } }, // SW
           { {  1,  1 }, {  1,  1 } }, // SE
          };

          gf::Vector2i endPoint1, endPoint2;

          if (biome == Void) {
            if (pos.y % 2 == 0) {
              gf::Vector2i base(pos * tileSize);
              base.y /= 2;
              base += tilesetTileSize / 2;

              for (auto& rel : NeighborsEven) {
                gf::Vector2i neighbor = pos + rel.first;

                if (!biomes.isValid(neighbor)) {
                  continue;
                }

                if (biomes(neighbor) == Void) {
                  continue;
                }

                endPoint1 = endPoint2 = base + rel.second * tileSize / 2;
                endPoint1.x = base.x;
                endPoint2.y = base.y;

                segments.push_back({ endPoint1, endPoint2 });

//                 gf::Log::info("For pos (%i,%i) [even], base (%i,%i), add segment (%i, %i) -> (%i, %i)\n", pos.x, pos.y, base.x, base.y, endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y);
              }
            } else {
              gf::Vector2i base(pos * tileSize);
              base.y /= 2;
              base.x += tileSize.width / 2;
              base += tilesetTileSize / 2;

              for (auto& rel : NeighborsOdd) {
                gf::Vector2i neighbor = pos + rel.first;

                if (!biomes.isValid(neighbor)) {
                  continue;
                }

                if (biomes(neighbor) == Void) {
                  continue;
                }

                endPoint1 = endPoint2 = base + rel.second * tileSize / 2;
                endPoint1.x = base.x;
                endPoint2.y = base.y;

                segments.push_back({ endPoint1, endPoint2 });

//                 gf::Log::info("For pos (%i,%i) [odd], base (%i,%i), add segment (%i, %i) -> (%i, %i)\n", pos.x, pos.y, base.x, base.y, endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y);
              }
            }
          }

        }

        gf::Log::info("Number of segments: %zu\n", segments.size());

        std::vector<gf::Polyline> polylines = computeAutoCollision(segments);

        gf::Log::info("Number of polylines: %zu\n", polylines.size());

        for (auto& polyline : polylines) {
          std::vector<b2Vec2> line;

          for (auto& point : polyline) {
            line.push_back(fromVec(point));
          }

          b2ChainShape shape;

          if (polyline.isLoop()) {
            shape.CreateLoop(line.data(), line.size());
          } else {
            assert(polyline.isChain());
            shape.CreateChain(line.data(), line.size());
          }

          b2BodyDef bodyDef;
          bodyDef.type = b2_staticBody;
          bodyDef.position = { 0.0f, 0.0f };
          auto body = m_world.CreateBody(&bodyDef);

          b2FixtureDef fixtureDef;
          fixtureDef.density = 1.0f;
          fixtureDef.friction = 0.0f;
          fixtureDef.restitution = 0.0f;
          fixtureDef.shape = &shape;

          body->CreateFixture(&fixtureDef);
        }

      }

      virtual void visitObjectLayer(const gf::TmxLayers& map, const gf::TmxObjectLayer& layer) override {
        gf::unused(map);

        gf::Log::info("Parsing object layer '%s'\n", layer.name.c_str());

        for (auto& object : layer.objects) {
          if (object->kind != gf::TmxObject::Tile) {
            continue;
          }

          auto tile = static_cast<gf::TmxTileObject *>(object.get());

          if (layer.name == "Trees") {
            gf::Vector2f position = tile->position + gf::Vector2f(384 / 2, -70);

            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.position = fromVec(position);
            auto body = m_world.CreateBody(&bodyDef);

            b2CircleShape shape;
            shape.m_radius = 50.0f * PhysicsScale;

            b2FixtureDef fixtureDef;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.0f;
            fixtureDef.restitution = 0.0f;
            fixtureDef.shape = &shape;

            body->CreateFixture(&fixtureDef);
          }

        }
      }

    private:
      b2World& m_world;
    };

    constexpr int PhysicsMaker::Land;
    constexpr int PhysicsMaker::Void;

  }

  Physics::Physics(const gf::TmxLayers& layers, Hero& hero)
  : m_world({ 0.0f, 0.0f })
  , m_body(nullptr)
  , m_hero(hero)
  {
    PhysicsMaker maker(m_world);
    layers.visitLayers(maker);

    gf::Vector2f initialPosition = m_hero.getDynamics().position;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = fromVec(initialPosition);
    m_body = m_world.CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = 20.0f * PhysicsScale; // same constants as in Player.cc

    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.shape = &shape;

    m_body->CreateFixture(&fixtureDef);
  }

  void Physics::update(gf::Time time) {
    auto dynamics = m_hero.getDynamics();
    m_body->SetTransform(fromVec(dynamics.position), 0.0f);
    m_body->SetLinearVelocity(fromVec(dynamics.velocity));

    static constexpr int32 VelocityIterations = 10; // 6;
    static constexpr int32 PositionIterations = 8; // 2;
    m_world.Step(time.asSeconds(), VelocityIterations, PositionIterations);

    m_hero.setDynamics({ toVec(m_body->GetPosition()), toVec(m_body->GetLinearVelocity()) });
  }

}
