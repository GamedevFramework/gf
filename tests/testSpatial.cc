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
#include <gf/Spatial.h>

#include <iostream>

#include <gf/Clock.h>
#include <gf/Random.h>

#include "gtest/gtest.h"

namespace {

  struct NullCallback {
    void operator()([[maybe_unused]] const gf::Handle& handle) {
    }
  };

  struct Callback {
    void operator()(const gf::Handle& handle) {
      set.insert(handle.asId());
    }

    std::set<gf::Id> set;
  };

  constexpr gf::RectF Bounds = gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 100.0f, 100.0f });
  constexpr std::size_t SampleSize = 10000;
  constexpr std::size_t QuerySize = 20;

  gf::RectF getRandomBox(gf::Random& random) {
    gf::RectF rect;

    for (std::size_t j = 0; j < 2; ++j) {
      rect.min[j] = random.computeUniformFloat(0.0f, 90.0f);
      rect.max[j] = rect.min[j] + random.computeUniformFloat(5.0f, 10.0f);
    }

    return rect;
  }

  std::vector<gf::RectF> getRandomBoxes(gf::Random& random) {
    std::vector<gf::RectF> res;

    for (std::size_t i = 0; i < SampleSize; ++i) {
      res.push_back(getRandomBox(random));
    }

    return res;
  }

  gf::RectF getRandomQueryBox(gf::Random& random) {
    gf::RectF rect;

    for (std::size_t j = 0; j < 2; ++j) {
      rect.min[j] = random.computeUniformFloat(0.0f, 60.0f);
      rect.max[j] = rect.min[j] + random.computeUniformFloat(20.0f, 40.0f);
    }

    return rect;
  }

  template<typename T>
  void testInsertSimple(T& spatial) {
    gf::RectF rect = gf::RectF::fromPositionSize({ 10.0f, 10.0f }, { 10.0f, 10.0f });
    gf::Handle handle(1);
    auto id = spatial.insert(handle, rect);

    EXPECT_EQ(spatial[id].asId(), handle.asId());
  }

  template<typename T>
  void testInsertRandom(T& spatial) {
    gf::Random random(23);

    auto boxes = getRandomBoxes(random);

    std::map<gf::SpatialId, gf::Handle> handles;

    gf::Clock clock;

    for (std::size_t i = 0; i < SampleSize; ++i) {
      auto id = spatial.insert(gf::Handle(i), boxes[i]);
      handles[id] = gf::Handle(i);
    }

    gf::Time insertionTime = clock.restart();
    std::cout << "Insertion time: " << insertionTime.asMilliseconds() << "ms\n";

    EXPECT_EQ(handles.size(), SampleSize);

    for (auto& kv : handles) {
      ASSERT_EQ(spatial[kv.first].asId(), kv.second.asId());
    }
  }

  template<typename T>
  void testQueryRandom(T& spatial) {
    gf::Random random(42);
    gf::SimpleSpatialIndex reference;

    auto boxes = getRandomBoxes(random);

    for (std::size_t i = 0; i < SampleSize; ++i) {
      reference.insert(gf::Handle(i), boxes[i]);
    }

    for (std::size_t i = 0; i < SampleSize; ++i) {
      spatial.insert(gf::Handle(i), boxes[i]);
    }

    gf::Clock clock;

    gf::Time queryIntersectTime = gf::Time::zero();

    for (std::size_t i = 0; i < QuerySize; ++i) {
      auto queryBox = getRandomQueryBox(random);

      Callback referenceResult;
      reference.query(queryBox, std::ref(referenceResult), gf::SpatialQuery::Intersect);

      Callback spatialResult;
      clock.restart();
      spatial.query(queryBox, std::ref(spatialResult), gf::SpatialQuery::Intersect);
      queryIntersectTime += clock.restart();

      EXPECT_EQ(referenceResult.set.size(), spatialResult.set.size());
      EXPECT_EQ(referenceResult.set, spatialResult.set);
    }

    std::cout << "Query time (Intersect): " << queryIntersectTime.asMicroseconds() / QuerySize << "us\n";

    gf::Time queryContainTime = gf::Time::zero();

    for (std::size_t i = 0; i < QuerySize; ++i) {
      auto queryBox = getRandomQueryBox(random);

      Callback referenceResult;
      reference.query(queryBox, std::ref(referenceResult), gf::SpatialQuery::Contain);

      Callback spatialResult;
      clock.restart();
      spatial.query(queryBox, std::ref(spatialResult), gf::SpatialQuery::Contain);
      queryContainTime += clock.restart();

      EXPECT_EQ(referenceResult.set.size(), spatialResult.set.size());
      EXPECT_EQ(referenceResult.set, spatialResult.set);
    }

    std::cout << "Query time (Contain): " << queryContainTime.asMicroseconds() / QuerySize << "us\n";
  }

  template<typename T>
  void testRemoveRandom(T& spatial) {
    gf::Random random(69);

    auto boxes = getRandomBoxes(random);

    std::vector<gf::SpatialId> removed;

    for (std::size_t i = 0; i < SampleSize; ++i) {
      auto id = spatial.insert(gf::Handle(i), boxes[i]);

      if (i % 3 == 1) {
        removed.push_back(id);
      }
    }

    std::size_t remaining = SampleSize - removed.size();

    gf::Clock clock;

    for (auto id : removed) {
      spatial.remove(id);
    }

    gf::Time removalTime = clock.restart();
    std::cout << "Remove time: " << removalTime.asMicroseconds() / removed.size() << "us\n";

    Callback result;
    spatial.query(Bounds, std::ref(result), gf::SpatialQuery::Contain);

    EXPECT_EQ(result.set.size(), remaining);
  }

  template<typename T>
  void testModifyRandom(T& spatial) {
    gf::Random random(99);

    auto boxes = getRandomBoxes(random);

    std::vector<gf::SpatialId> modified;

    for (std::size_t i = 0; i < SampleSize; ++i) {
      auto id = spatial.insert(gf::Handle(i), boxes[i]);

      if (i % 3 == 1) {
        modified.push_back(id);
      }
    }

    gf::Clock clock;

    for (auto id : modified) {
      auto box = getRandomBox(random);
      spatial.modify(id, box);
    }

    gf::Time modifyTime = clock.restart();
    std::cout << "Modify time: " << modifyTime.asMicroseconds() / modified.size() << "us\n";

    Callback result;
    spatial.query(Bounds, std::ref(result), gf::SpatialQuery::Contain);

    EXPECT_EQ(result.set.size(), SampleSize);
  }
}

/*
 * SimpleSpatialIndex
 */

TEST(SpatialTest, SimpleSpatialIndexInsertSimple) {
  gf::SimpleSpatialIndex spatial;
  testInsertSimple(spatial);
}

TEST(SpatialTest, SimpleSpatialIndexInsertRandom) {
  gf::SimpleSpatialIndex spatial;
  testInsertRandom(spatial);
}

TEST(SpatialTest, SimpleSpatialIndexRemoveRandom) {
  gf::SimpleSpatialIndex spatial;
  testRemoveRandom(spatial);
}

TEST(SpatialTest, SimpleSpatialIndexModifyRandom) {
  gf::SimpleSpatialIndex spatial;
  testModifyRandom(spatial);
}

/*
 * QuadTree
 */

TEST(SpatialTest, QuadtreeInsertSimple) {
  gf::Quadtree spatial(Bounds);
  testInsertSimple(spatial);
}

TEST(SpatialTest, QuadtreeInsertRandom) {
  gf::Quadtree spatial(Bounds);
  testInsertRandom(spatial);
}

TEST(SpatialTest, QuadtreeQueryRandom) {
  gf::Quadtree spatial(Bounds);
  testQueryRandom(spatial);
}

TEST(SpatialTest, QuadtreeRemoveRandom) {
  gf::Quadtree spatial(Bounds);
  testRemoveRandom(spatial);
}

TEST(SpatialTest, QuadtreeModifyRandom) {
  gf::Quadtree spatial(Bounds);
  testModifyRandom(spatial);
}

/*
 * DynamicTree
 */

TEST(SpatialTest, DynamicTreeInsertSimple) {
  gf::DynamicTree spatial;
  testInsertSimple(spatial);
}

TEST(SpatialTest, DynamicTreeInsertRandom) {
  gf::DynamicTree spatial;
  testInsertRandom(spatial);
}

TEST(SpatialTest, DynamicTreeQueryRandom) {
  gf::DynamicTree spatial;
  testQueryRandom(spatial);
}

TEST(SpatialTest, DynamicTreeRemoveRandom) {
  gf::DynamicTree spatial;
  testRemoveRandom(spatial);
}

TEST(SpatialTest, DynamicTreeModifyRandom) {
  gf::DynamicTree spatial;
  testModifyRandom(spatial);
}

/*
 * RStartTree
 */

TEST(SpatialTest, RStarTreeInsertSimple) {
  gf::RStarTree spatial;
  testInsertSimple(spatial);
}

TEST(SpatialTest, RStarTreeInsertRandom) {
  gf::RStarTree spatial;
  testInsertRandom(spatial);
}

TEST(SpatialTest, RStarTreeQueryRandom) {
  gf::RStarTree spatial;
  testQueryRandom(spatial);
}

TEST(SpatialTest, RStarTreeRemoveRandom) {
  gf::RStarTree spatial;
  testRemoveRandom(spatial);
}

TEST(SpatialTest, RStarTreeModifyRandom) {
  gf::RStarTree spatial;
  testModifyRandom(spatial);
}
