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
#include <gf/Spatial.h>

#include <gf/Log.h>
#include <gf/Random.h>
#include <gf/Unused.h>

#include "gtest/gtest.h"

namespace {

  template<typename T, typename U = float, std::size_t N = 2>
  class NaiveSpatialIndex {
  public:
    bool insert(const T& value, const gf::Box<U, N>& bounds) {
      m_entries.push_back({ value, bounds });
      return true;
    }

    std::size_t query(const gf::Box<U, N>& bounds, gf::SpatialQueryCallback<T> callback, gf::SpatialQuery kind) {
      std::size_t found = 0;

      for (auto& entry : m_entries) {
        switch (kind) {
          case gf::SpatialQuery::Contain:
            if (bounds.contains(entry.bounds)) {
              callback(entry.value);
              ++found;
            }
            break;

          case gf::SpatialQuery::Intersect:
            if (bounds.intersects(entry.bounds)) {
              callback(entry.value);
              ++found;
            }
            break;
        }
      }

      return found;
    }

  private:
    struct Entry {
      T value;
      gf::Box<U, N> bounds;
    };

    std::vector<Entry> m_entries;
  };

  template<typename T>
  struct NullCallback {
    void operator()(const T& value) {
      gf::unused(value);
    }
  };

  template<typename T>
  struct Callback {
    void operator()(const T& value) {
      set.insert(value);
    }

    std::set<T> set;
  };

  constexpr std::size_t SampleSize = 10000;


  template<std::size_t N = 2>
  std::vector<gf::Box<int, N>> getRandomBoxes(gf::Random& random) {
    std::vector<gf::Box<int, N>> res;

    for (std::size_t i = 0; i < SampleSize; ++i) {
      gf::Box<int, N> box;

      for (std::size_t j = 0; j < N; ++j) {
        box.min[j] = random.computeUniformInteger(0, 100);
        box.max[j] = box.min[j] + random.computeUniformInteger(1, 20);
      }

      res.push_back(box);
    }

    return res;
  }

  template<std::size_t N>
  gf::Box<int, N> getRandomQueryBox(gf::Random& random) {
    gf::Box<int, N> box;

    for (std::size_t j = 0; j < N; ++j) {
      box.min[j] = random.computeUniformInteger(0, 80);
      box.max[j] = box.min[j] + random.computeUniformInteger(20, 40);
    }

    return box;
  }

}

TEST(SpatialTest, QuadTreeMoveOnlyType) {
  gf::QuadTree<std::unique_ptr<std::size_t>, int> qtree(gf::Box2i({ 0, 0 }, { 100, 100 }));

  qtree.insert(std::make_unique<std::size_t>(1), gf::Box2i({ 10, 10 }, { 20, 20 }));
  std::size_t count = qtree.query(gf::Box2i({ 15, 15 }, { 25, 25 }), NullCallback<std::unique_ptr<std::size_t>>());

  EXPECT_EQ(1u, count);
}

TEST(SpatialTest, QuadTreeQuery) {
  gf::Random random(42);

  NaiveSpatialIndex<std::size_t, int> naive;
  gf::QuadTree<std::size_t, int> qtree(gf::Box2i({ 0, 0 }, { 120, 120 }));

  auto boxes = getRandomBoxes<2>(random);

  for (std::size_t i = 0; i < SampleSize; ++i) {
    naive.insert(i, boxes[i]);
    qtree.insert(i, boxes[i]);
  }

  auto queryBox = getRandomQueryBox<2>(random);

  Callback<std::size_t> naiveResult;
  Callback<std::size_t> qtreeResult;

  naive.query(queryBox, std::ref(naiveResult), gf::SpatialQuery::Intersect);
  qtree.query(queryBox, std::ref(qtreeResult), gf::SpatialQuery::Intersect);

  EXPECT_EQ(naiveResult.set, qtreeResult.set);

  naiveResult.set.clear();
  qtreeResult.set.clear();

  naive.query(queryBox, std::ref(naiveResult), gf::SpatialQuery::Contain);
  qtree.query(queryBox, std::ref(qtreeResult), gf::SpatialQuery::Contain);

  EXPECT_EQ(naiveResult.set, qtreeResult.set);
}

TEST(SpatialTest, RStarTreeQuery) {
  gf::Random random(42);

  NaiveSpatialIndex<std::size_t, int> naive;
  gf::RStarTree<std::size_t, int> rstar;

  auto boxes = getRandomBoxes<2>(random);

  for (std::size_t i = 0; i < SampleSize; ++i) {
    naive.insert(i, boxes[i]);
    rstar.insert(i, boxes[i]);
  }

  auto queryBox = getRandomQueryBox<2>(random);

  Callback<std::size_t> naiveResult;
  Callback<std::size_t> rstarResult;

  std::size_t foundNaiveIntersect = naive.query(queryBox, std::ref(naiveResult), gf::SpatialQuery::Intersect);
  std::size_t foundRStarIntersect = rstar.query(queryBox, std::ref(rstarResult), gf::SpatialQuery::Intersect);

  EXPECT_EQ(foundNaiveIntersect, foundRStarIntersect);
  EXPECT_EQ(naiveResult.set, rstarResult.set);

  naiveResult.set.clear();
  rstarResult.set.clear();

  std::size_t foundNaiveContain = naive.query(queryBox, std::ref(naiveResult), gf::SpatialQuery::Contain);
  std::size_t foundRStarContain = rstar.query(queryBox, std::ref(rstarResult), gf::SpatialQuery::Contain);

  EXPECT_EQ(foundNaiveContain, foundRStarContain);
  EXPECT_EQ(naiveResult.set, rstarResult.set);
}

TEST(SpatialTest, RStarTreeMoveOnlyType) {
  gf::RStarTree<std::unique_ptr<std::size_t>, int> rstar;

  rstar.insert(std::make_unique<std::size_t>(1), gf::Box2i({ 10, 10 }, { 20, 20 }));
  std::size_t count = rstar.query(gf::Box2i({ 15, 15 }, { 25, 25 }), NullCallback<std::unique_ptr<std::size_t>>());

  EXPECT_EQ(1u, count);
}
