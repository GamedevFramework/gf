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
#ifndef GF_SPATIAL_H
#define GF_SPATIAL_H

#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <vector>

#include <boost/container/static_vector.hpp>

#include "Box.h"
#include "Math.h"
#include "Unused.h"

#include "Log.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  enum class SpatialStructureType {
    Object,
    Node,
  };

  template<typename U, std::size_t N>
  struct SpatialStructure {
    Box<U, N> bounds;
    SpatialStructureType type;
    int level;
  };

  /**
   * @ingroup core
   * @brief A kind of spatial query
   */
  enum class SpatialQuery {
    Contain, ///< Search for all objects that contain the given bounds
    Intersect, ///< Search for all objects that intersect the given bounds
  };

  /**
   * @ingroup core
   * @brief A callback for spatial query
   */
  template<typename T>
  using SpatialQueryCallback = std::function<void(const T&)>;

  /**
   * @ingroup core
   * @brief An implementation of quadtree
   *
   * @sa gf::RStarTree
   * @sa [Quadtree - Wikipedia](https://en.wikipedia.org/wiki/Quadtree)
   */
  template<typename T, typename U = float, std::size_t Size = 16>
  class QuadTree {
    static_assert(Size > 0, "Size can not be 0");
  public:
    /**
     * @brief Constructor
     *
     * @param bounds The global bounds for objects in the tree
     */
    QuadTree(const Box<U, 2>& bounds)
    : m_root(bounds)
    {

    }

    /**
     * @brief Insert an object in the tree
     *
     * @param value The object to insert
     * @param bounds The bounds of the object
     * @returns True if the object has been inserted
     */
    bool insert(T value, const Box<U, 2>& bounds) {
      Node *node = m_root.chooseNode(bounds);

      if (node == nullptr) {
        return false;
      }

      return node->insert(std::move(value), bounds);
    }

    /**
     * @brief Query objects in the tree
     *
     * @param bounds The bounds of the query
     * @param callback The callback to apply to found objects
     * @param kind The kind of spatial query
     * @returns The number of objects found
     */
    std::size_t query(const Box<U, 2>& bounds, SpatialQueryCallback<T> callback, SpatialQuery kind = SpatialQuery::Intersect) const {
      return m_root.query(bounds, callback, kind);
    }

    /**
     * @brief Remove all the objects from the tree
     */
    void clear() {
      m_root.clear();
    }


    std::vector<SpatialStructure<U, 2>> getStructure() const {
      std::vector<SpatialStructure<U, 2>> structures;
      m_root.appendToStructure(structures, 0);
      return structures;
    }

  private:
    struct Entry {
      T value;
      Box<U, 2> bounds;
    };

    class Node {
    public:
      Node()
      : m_children(nullptr)
      {
        m_entries.reserve(Size);
      }

      Node(const Box<U, 2>& bounds)
      : m_bounds(bounds)
      , m_children(nullptr)
      {
        m_entries.reserve(Size);
      }

      bool isLeaf() const {
        return m_children == nullptr;
      }

      Node *chooseNode(const Box<U, 2>& bounds) {
        if (!m_bounds.contains(bounds)) {
          return nullptr;
        }

        if (isLeaf()) {
          if (m_entries.size() < Size) {
            return this;
          }

          subdivide();

          // try again
          if (m_entries.size() < Size) {
            return this;
          }
        }

        for (std::size_t i = 0; i < 4; ++i) {
          if (m_children[i].chooseNode(bounds) != nullptr) {
            return &m_children[i];
          }
        }

        clearChildrenIfEmpty();

        return this;
      }

      bool insert(T&& value, const Box<U, 2>& bounds) {
        m_entries.push_back({ std::move(value), bounds });
        return true;
      }

      std::size_t query(const Box<U, 2>& bounds, SpatialQueryCallback<T>& callback, SpatialQuery kind) const {
        if (!m_bounds.intersects(bounds)) {
          return 0;
        }

        std::size_t found = 0;

        for (auto& entry : m_entries) {
          switch (kind) {
            case SpatialQuery::Contain:
              if (bounds.contains(entry.bounds)) {
                callback(entry.value);
                ++found;
              }
              break;

            case SpatialQuery::Intersect:
              if (bounds.intersects(entry.bounds)) {
                callback(entry.value);
                ++found;
              }
              break;
          }
        }

        if (!isLeaf()) {
          for (std::size_t i = 0; i < 4; ++i) {
            found += m_children[i].query(bounds, callback, kind);
          }
        }

        return found;
      }

      void clear() {
        m_entries.clear();

        // no need to explicitly clear children
        m_children = nullptr;
      }

      void appendToStructure(std::vector<SpatialStructure<U, 2>>& structures, int level) const {
        structures.push_back({ m_bounds, SpatialStructureType::Node, level });

        for (auto& entry : m_entries) {
          structures.push_back( { entry.bounds, SpatialStructureType::Object, level });
        }

        if (m_children) {
          for (std::size_t i = 0; i < 4; ++i) {
            m_children[i].appendToStructure(structures, level + 1);
          }
        }
      }

    private:
      void subdivide() {
        m_children = std::make_unique<Node[]>(4);

        m_children[0].m_bounds = computeBoxQuadrant(m_bounds, Quadrant::UpperLeft);
        m_children[1].m_bounds = computeBoxQuadrant(m_bounds, Quadrant::UpperRight);
        m_children[2].m_bounds = computeBoxQuadrant(m_bounds, Quadrant::LowerRight);
        m_children[3].m_bounds = computeBoxQuadrant(m_bounds, Quadrant::LowerLeft);

        std::vector<Entry> entries;

        for (auto& entry : m_entries) {
          bool inserted = false;

          for (std::size_t i = 0; i < 4; ++i) {
            if (m_children[i].m_bounds.contains(entry.bounds)) {
              m_children[i].m_entries.push_back(std::move(entry));
              inserted = true;
              break;
            }
          }

          if (!inserted) {
            entries.push_back(std::move(entry));
          }
        }

        std::swap(m_entries, entries);
      }

      void clearChildrenIfEmpty() {
        assert(!isLeaf());

        for (std::size_t i = 0; i < 4; ++i) {
          if (!m_children[i].m_entries.empty()) {
            return;
          }
        }

        m_children = nullptr;
      }

    private:
      Box<U, 2> m_bounds;
      std::vector<Entry> m_entries;
      std::unique_ptr<Node[]> m_children;
    };

  private:
    Node m_root;
  };

  /**
   * @ingroup core
   * @brief An implemntation of a R* tree
   *
   * More precisely, this class implements the [Revised R* tree](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.367.7273&rep=rep1&type=pdf).
   *
   * @sa gf::QuadTree
   * @sa [R* tree - Wikipedia](https://en.wikipedia.org/wiki/R*_tree)
   */
  template<typename T, typename U = float, std::size_t N = 2, std::size_t MaxSize = 16, std::size_t MinSize = 4>
  class RStarTree {
    static_assert(N > 0, "N can not be 0");
    static_assert(2 <= MinSize, "MinSize must be at least 2");
    static_assert(MinSize <= MaxSize / 2, "MinSize must be less than MaxSize/2");
  public:
    /**
     * @brief Constructor
     */
    RStarTree()
    : m_root(new Leaf)
    {
      auto branch = std::make_unique<Branch>();
    }

    /**
     * @brief Deleted copy constructor
     */
    RStarTree(const RStarTree&) = delete;

    /**
     * @brief Deleted copy assignement
     */
    RStarTree& operator=(const RStarTree&) = delete;

    /**
     * @brief Move constructor
     */
    RStarTree(RStarTree&& other) noexcept
    {
      m_root = std::exchange(other.m_root, nullptr);
    }

    /**
     * @brief Move assignement
     */
    RStarTree& operator=(RStarTree&& other) noexcept {
      std::swap(m_root, other.m_root);
      return *this;
    }

    /**
     * @brief Destructor
     */
    ~RStarTree() {
      delete m_root;
    }

    /**
     * @brief Insert an object in the tree
     *
     * @param value The object to insert
     * @param bounds The bounds of the object
     * @returns True if the object has been inserted
     */
    bool insert(T value, const Box<U, N>& bounds) {
      Leaf *leaf = m_root->chooseSubtree(bounds);

      Node *splitted = leaf->tryInsert(std::move(value), bounds);
      Node *current = leaf;

      while (splitted != nullptr) {
        auto splittedBounds = splitted->computeBounds();
        splitted->updateOriginalBounds(splittedBounds);

        auto currentBounds = current->computeBounds();
        current->updateOriginalBounds(currentBounds);

        if (current == m_root) {
          auto branch = new Branch;

          current->setParent(branch);
          branch->tryInsert(current, currentBounds);

          splitted->setParent(branch);
          branch->tryInsert(splitted, splittedBounds);

          branch->updateOriginalBounds(branch->computeBounds());

          m_root = branch;
          current = m_root;
          splitted = nullptr;
        } else {
          Branch *parent = current->getParent();
          assert(parent != nullptr);

          parent->updateBoundsForChild(currentBounds, current);

          splitted->setParent(parent);
          splitted = parent->tryInsert(splitted, splittedBounds);

          current = parent;
        }
      }

      while (current != m_root) {
        auto currentBounds = current->computeBounds();

        Branch *parent = current->getParent();
        assert(parent != nullptr);

        parent->updateBoundsForChild(currentBounds, current);

        current = parent;
      }

      return true;
    }

    /**
     * @brief Query objects in the tree
     *
     * @param bounds The bounds of the query
     * @param callback The callback to apply to found objects
     * @param kind The kind of spatial query
     * @returns The number of objects found
     */
    std::size_t query(const Box<U, N>& bounds, SpatialQueryCallback<T> callback, SpatialQuery kind = SpatialQuery::Intersect) const {
      return m_root->query(bounds, callback, kind);
    }

    /**
     * @brief Remove all the objects from the tree
     */
    void clear() {
      delete m_root;
      m_root = new Leaf;
    }

    std::vector<SpatialStructure<U, N>> getStructure() const {
      std::vector<SpatialStructure<U, N>> structures;
      m_root->appendToStructure(structures, 0);
      return structures;
    }

  private:
    static constexpr std::size_t Size = MaxSize + 1;

    class Leaf;
    class Branch;

    /*
     * Node
     */

    struct Entry {
      Box<U, N> bounds;
    };

    class Node {
    public:
      Node()
      : m_parent(nullptr)
      {

      }

      Branch *getParent() noexcept {
        return m_parent;
      }

      void setParent(Branch *parent) {
        m_parent = parent;
      }

      bool hasOriginalBounds() const {
        return !m_orig.isEmpty();
      }

      U getOriginalCenterAxisValue(std::size_t axis) const {
        return (m_orig.min[axis] + m_orig.max[axis]) / 2;
      }

      void updateOriginalBounds(Box<U, N> orig) {
        m_orig = orig;
      }

      virtual ~Node() = default;
      virtual std::size_t query(const Box<U, N>& bounds, SpatialQueryCallback<T>& callback, SpatialQuery kind) const = 0;
      virtual Leaf *chooseSubtree(const Box<U, N>& bounds) = 0;

      virtual Box<U, N> computeBounds() const = 0;

      virtual void appendToStructure(std::vector<SpatialStructure<U, N>>& structures, int level) const = 0;

    protected:
      enum SplitOrder {
        Min,
        Max,
      };

      struct SplitResult {
        std::size_t index;
        std::size_t axis;
        SplitOrder order;
      };

    private:
      Branch *m_parent;
      Box<U, N> m_orig;
    };

    /*
     * Leaf
     */

    struct LeafEntry : Entry {
      T value;
    };

    class Leaf : public Node {
    public:
      using typename Node::SplitOrder;
      using typename Node::SplitResult;

      virtual std::size_t query(const Box<U, N>& bounds, SpatialQueryCallback<T>& callback, SpatialQuery kind) const override {
        std::size_t found = 0;

        switch (kind) {
          case SpatialQuery::Contain:
            for (auto& entry : m_entries) {
              if (bounds.contains(entry.bounds)) {
                callback(entry.value);
                ++found;
              }
            }
            break;

          case SpatialQuery::Intersect:
            for (auto& entry : m_entries) {
              if (bounds.intersects(entry.bounds)) {
                callback(entry.value);
                ++found;
              }
            }
            break;
        }

        return found;
      }


      virtual Leaf *chooseSubtree(const Box<U, N>& bounds) override {
        gf::unused(bounds);
        return this;
      }

      virtual Box<U,N> computeBounds() const override {
        assert(!m_entries.empty());
        Box<U,N> res(m_entries[0].bounds);

        for (auto& entry : m_entries) {
          res.extend(entry.bounds);
        }

        return res;
      }

      virtual void appendToStructure(std::vector<SpatialStructure<U, N>>& structures, int level) const override {
        for (auto& entry : m_entries) {
          structures.push_back({ entry.bounds, SpatialStructureType::Object, level });
        }
      }

      Leaf *tryInsert(T&& value, const Box<U, N>& bounds) {
        LeafEntry entry;
        entry.bounds = bounds;
        entry.value = std::move(value);
        m_entries.push_back(std::move(entry));

        if (m_entries.size() < Size) {
          return nullptr;
        }

        std::vector<Box<U, N>> boxes;

        std::transform(m_entries.begin(), m_entries.end(), std::back_inserter(boxes), [](const Entry& entry) {
          return entry.bounds;
        });

        SplitResult split = computeSplit(boxes);

        switch (split.order) {
          case SplitOrder::Min:
            std::sort(m_entries.begin(), m_entries.end(), EntryMinAxisComparator<LeafEntry>(split.axis));
            break;
          case SplitOrder::Max:
            std::sort(m_entries.begin(), m_entries.end(), EntryMaxAxisComparator<LeafEntry>(split.axis));
            break;
        }

        auto leaf = new Leaf;

        auto splitIteratorBegin = std::next(m_entries.begin(), split.index + 1);
        auto splitIteratorEnd = m_entries.end();

        std::move(splitIteratorBegin, splitIteratorEnd, std::back_inserter(leaf->m_entries));
        m_entries.erase(splitIteratorBegin, splitIteratorEnd);

        return leaf;
      }

    private:
      SplitResult computeSplit(std::vector<Box<U, N>>& boxes) {
        SplitResult split;
        std::tie(split.axis, split.order) = computeSplitAxis(boxes);

        switch (split.order) {
          case SplitOrder::Min:
            std::sort(boxes.begin(), boxes.end(), MinAxisComparator(split.axis));
            break;
          case SplitOrder::Max:
            std::sort(boxes.begin(), boxes.end(), MaxAxisComparator(split.axis));
            break;
        }

        split.index = computeSplitIndex(boxes, split.axis);
        return split;
      }

      std::tuple<std::size_t, SplitOrder> computeSplitAxis(std::vector<Box<U, N>>& boxes) {
        std::size_t currentAxis = 0;
        U currentValue = std::numeric_limits<U>::max();
        SplitOrder currentOrder = SplitOrder::Min;

        for (std::size_t axis = 0; axis < N; ++axis) {
          std::sort(boxes.begin(), boxes.end(), MinAxisComparator(axis));

          U value = computeAxisValue(boxes);

          if (value < currentValue) {
            currentAxis = axis;
            currentValue = value;
            currentOrder = SplitOrder::Min;
          }

          std::sort(boxes.begin(), boxes.end(), MaxAxisComparator(axis));

          value = computeAxisValue(boxes);

          if (value < currentValue) {
            currentAxis = axis;
            currentValue = value;
            currentOrder = SplitOrder::Max;
          }
        }

        return std::make_tuple(currentAxis, currentOrder);
      }

      U computeAxisValue(const std::vector<Box<U, N>>& boxes) {
        std::vector<Box<U, N>> firstBounds;

        std::partial_sum(boxes.begin(), boxes.end(), std::back_inserter(firstBounds), [](const Box<U, N>& lhs, const Box<U, N>& rhs) {
          return lhs.getExtended(rhs);
        });

        std::vector<Box<U, N>> secondBounds;

        std::partial_sum(boxes.rbegin(), boxes.rend(), std::back_inserter(secondBounds), [](const Box<U, N>& lhs, const Box<U, N>& rhs) {
          return lhs.getExtended(rhs);
        });

        U value = { };

        for (std::size_t j = MinSize; j <= MaxSize - MinSize + 1; ++j) {
          value += firstBounds[j].getExtentDistance() + secondBounds[Size - j].getExtentDistance();
        }

        return value;
      }

      std::size_t computeSplitIndex(const std::vector<Box<U, N>>& boxes, std::size_t axis) {
        std::vector<Box<U, N>> firstBounds;

        std::partial_sum(boxes.begin(), boxes.end(), std::back_inserter(firstBounds), [](const Box<U, N>& lhs, const Box<U, N>& rhs) {
          return lhs.getExtended(rhs);
        });

        std::vector<Box<U, N>> secondBounds;

        std::partial_sum(boxes.rbegin(), boxes.rend(), std::back_inserter(secondBounds), [](const Box<U, N>& lhs, const Box<U, N>& rhs) {
          return lhs.getExtended(rhs);
        });


        // define wf

        const auto& bounds = firstBounds.back();
        float asym = 0.0f;

        if (Node::hasOriginalBounds()) {
          asym = 2.0f * ((bounds.max[axis] + bounds.min[axis]) / 2 - Node::getOriginalCenterAxisValue(axis)) / (bounds.max[axis] - bounds.min[axis]);
        }

        assert(-1.0f <= asym && asym <= 1.0f);

        static constexpr float S = 0.5f;
        const float mu = (1 - 2.0f * MinSize / (MaxSize + 1)) * asym;
        const float rho = S * (1 + std::abs(mu));
        const float y1 = std::exp(- 1 / (S * S));
        const float ys = 1 / (1  - y1);

        auto wf = [mu,rho,y1,ys](std::size_t i) {
          const float xi = 2.0f * i / (MaxSize + 1.0f) - 1.0f;
          return ys * (std::exp(- gf::square((xi - mu) / rho) - y1));
        };

        // compute weight

        std::size_t currentIndex = 0;
        U currentValue = std::numeric_limits<U>::max();

        U (Box<U,N>::* overlapFn)(const Box<U, N>&) const noexcept = nullptr;

        if (firstBounds[MinSize].getVolume() == 0 || secondBounds[MinSize].getVolume() == 0) {
          // perimeter based strategy
          overlapFn = &Box<U,N>::getIntersectionExtentDistance;
        } else {
          // volume based strategy
          overlapFn = &Box<U,N>::getIntersectionVolume;
        }

        bool overlapFree = false;

        U perimeterMax = 2 * bounds.getExtentDistance() - bounds.getMinimumEdge();

        for (std::size_t index = MinSize; index <= MaxSize - MinSize + 1; ++index) {
          auto weight = (firstBounds[index].*overlapFn)(secondBounds[Size - index - 1]);

          if (!overlapFree) {
            if (weight == 0) {
              overlapFree = true;
            } else {
              auto value = weight * wf(index);

              if (value < currentValue) {
                currentValue = value;
                currentIndex = index;
              }
            }
          }

          if (overlapFree && weight == 0) {
            weight = firstBounds[index].getExtentDistance() + secondBounds[Size - index - 1].getExtentDistance() - perimeterMax;

            if (weight > 0) {
              gf::Log::debug("weight: %f\n", static_cast<double>(weight));
            }

            assert(weight <= 0);

            auto value = weight / wf(index);

            if (value < currentValue) {
              currentValue = value;
              currentIndex = index;
            }
          }
        }

        return currentIndex;
      }

    private:
      boost::container::static_vector<LeafEntry, Size> m_entries;
    };

    /*
     * Branch
     */

    struct BranchEntry : Entry {
      Node *child;
    };

    class Branch : public Node {
    public:
      using typename Node::SplitOrder;
      using typename Node::SplitResult;

      virtual ~Branch() {
        for (auto& entry : m_entries) {
          delete entry.child;
        }
      }

      virtual std::size_t query(const Box<U, N>& bounds, SpatialQueryCallback<T>& callback, SpatialQuery kind) const override {
        std::size_t found = 0;

        for (auto& entry : m_entries) {
          if (bounds.intersects(entry.bounds)) {
            found += entry.child->query(bounds, callback, kind);
          }
        }

        return found;
      }

      virtual Leaf *chooseSubtree(const Box<U, N>& bounds) override {
        return chooseNode(bounds)->chooseSubtree(bounds);
      }

      Node *searchForCoveringNode(const Box<U, N>& bounds) {
        Node *bestNodeForVolume = nullptr;
        U bestVolume = std::numeric_limits<U>::max();

        Node *bestNodeForExtentDistance = nullptr;
        U bestExtentDistance = std::numeric_limits<U>::max();

        for (auto& entry : m_entries) {
          if (entry.bounds.getIntersection(bounds) == bounds) {
            U volume = entry.bounds.getVolume();

            if (bestNodeForVolume == nullptr || volume < bestVolume) {
              bestVolume = volume;
              bestNodeForVolume = entry.child;
            }

            U extentDistance = entry.bounds.getExtentDistance();

            if (bestNodeForExtentDistance == nullptr || extentDistance < bestExtentDistance) {
              bestExtentDistance = extentDistance;
              bestNodeForExtentDistance = entry.child;
            }
          }
        }

        if (bestNodeForVolume != nullptr) {
          if (bestVolume > 0) {
            return bestNodeForVolume;
          }

          assert(bestNodeForExtentDistance);
          return bestNodeForExtentDistance;
        }

        return nullptr;
      }

      Node *chooseNode(const Box<U, N>& bounds) {
        Node *covering = searchForCoveringNode(bounds);

        if (covering != nullptr) {
          return covering;
        }

        std::sort(m_entries.begin(), m_entries.end(), ExtentDistanceEnlargement(bounds));

        OverlapExtentDistanceEnlargement extentDistanceEnlargement(bounds, m_entries.front());

        std::size_t p;

        for (p = m_entries.size() - 1; p > 0; --p) {
          if (extentDistanceEnlargement(m_entries[p]) != 0) {
            break;
          }
        }

        if (p == 0) {
          return m_entries[0].child;
        }

        std::vector<Candidate> candidates(p + 1, { 0, false, U() });
        Node *node = nullptr;

        if (existsEmptyVolumeExtension(bounds)) {
          node = findCandidates<OverlapExtentDistanceEnlargement>(0, p, bounds, candidates);
        } else {
          node = findCandidates<OverlapVolumeEnlargement>(0, p, bounds, candidates);
        }

        if (node != nullptr) {
          return node;
        }

        candidates.erase(std::remove_if(candidates.begin(), candidates.end(), [](const Candidate& candidate) {
          return !candidate.isCandidate;
        }), candidates.end());

        assert(!candidates.empty());

        auto it = std::min_element(candidates.begin(), candidates.end(), [](const Candidate& lhs, const Candidate& rhs) {
          return lhs.overlap < rhs.overlap;
        });

        return m_entries[it->index].child;
      }

      virtual Box<U,N> computeBounds() const override {
        assert(!m_entries.empty());
        Box<U,N> res(m_entries[0].bounds);

        for (auto& entry : m_entries) {
          res.extend(entry.bounds);
        }

        return res;
      }

      virtual void appendToStructure(std::vector<SpatialStructure<U, N>>& structures, int level) const override {
        for (auto& entry : m_entries) {
          structures.push_back({ entry.bounds, SpatialStructureType::Node, level });
          entry.child->appendToStructure(structures, level + 1);
        }
      }

      void updateBoundsForChild(const Box<U, N>& bounds, Node *child) {
        for (auto& entry : m_entries) {
          if (entry.child == child) {
            entry.bounds = bounds;
            return;
          }
        }

        assert(false);
      }

      Branch *tryInsert(Node * node, const Box<U, N>& bounds) {
        BranchEntry entry;
        entry.bounds = bounds;
        entry.child = node;
        m_entries.push_back(std::move(entry));

        if (m_entries.size() < Size) {
          return nullptr;
        }

        std::vector<Box<U, N>> boxes;

        std::transform(m_entries.begin(), m_entries.end(), std::back_inserter(boxes), [](const Entry& entry) {
          return entry.bounds;
        });

        SplitResult split = computeSplit(boxes);

        switch (split.order) {
          case SplitOrder::Min:
            std::sort(m_entries.begin(), m_entries.end(), EntryMinAxisComparator<BranchEntry>(split.axis));
            break;
          case SplitOrder::Max:
            std::sort(m_entries.begin(), m_entries.end(), EntryMaxAxisComparator<BranchEntry>(split.axis));
            break;
        }

        auto branch = new Branch;

        auto splitIteratorBegin = std::next(m_entries.begin(), split.index + 1);
        auto splitIteratorEnd = m_entries.end();

        std::move(splitIteratorBegin, splitIteratorEnd, std::back_inserter(branch->m_entries));
        m_entries.erase(splitIteratorBegin, splitIteratorEnd);

        for (auto& entry : branch->m_entries) {
          entry.child->setParent(branch);
        }

        return branch;
      }

    private:
      struct Candidate {
        std::size_t index;
        bool isCandidate;
        U overlap;
      };

      bool existsEmptyVolumeExtension(const Box<U, N>& bounds) {
        for (auto& entry : m_entries) {
          if (entry.bounds.getExtended(bounds).getVolume() == 0) {
            return true;
          }
        }

        return false;
      }

      template<typename OverlapEnlargement>
      Node *findCandidates(std::size_t t, std::size_t p, const Box<U, N>& bounds, std::vector<Candidate>& candidates) {
        candidates[t].index = t;
        candidates[t].isCandidate = true;

        OverlapEnlargement enlargement(bounds, m_entries[t]);

        U overlap = { };

        for (std::size_t i = 0; i <= p; ++i) {
          if (i == t) {
            continue;
          }

          U localOverlap = enlargement(m_entries[i]);
          overlap += localOverlap;

          if (localOverlap == 0 && !candidates[i].isCandidate) {
            Node *node = findCandidates<OverlapEnlargement>(i, p, bounds, candidates);

            if (node != nullptr) {
              return node;
            }
          }
        }

        if (overlap == 0) {
          return m_entries[t].child;
        }

        candidates[t].overlap = overlap;
        return nullptr;
      }

      struct SplitStatus {
        bool overlapFree = false;
        U currentValue = std::numeric_limits<U>::max();
      };

      SplitResult computeSplit(std::vector<Box<U, N>>& boxes) {
        SplitResult result;
        SplitStatus status;

        for (std::size_t axis = 0; axis < N; ++axis) {
          std::sort(boxes.begin(), boxes.end(), MinAxisComparator(axis));
          computeBestSplitValue(boxes, result, status, axis, SplitOrder::Min);

          std::sort(boxes.begin(), boxes.end(), MaxAxisComparator(axis));
          computeBestSplitValue(boxes, result, status, axis, SplitOrder::Max);
        }

        return result;
      }

      void computeBestSplitValue(std::vector<Box<U, N>>& boxes, SplitResult& result, SplitStatus& status, std::size_t axis, SplitOrder order) {
        std::vector<Box<U, N>> firstBounds;

        std::partial_sum(boxes.begin(), boxes.end(), std::back_inserter(firstBounds), [](const Box<U, N>& lhs, const Box<U, N>& rhs) {
          return lhs.getExtended(rhs);
        });

        std::vector<Box<U, N>> secondBounds;

        std::partial_sum(boxes.rbegin(), boxes.rend(), std::back_inserter(secondBounds), [](const Box<U, N>& lhs, const Box<U, N>& rhs) {
          return lhs.getExtended(rhs);
        });

        // define wf

        const auto& bounds = firstBounds.back();
        float asym = 0.0f;

        if (Node::hasOriginalBounds()) {
          asym = 2.0f * ((bounds.max[axis] + bounds.min[axis]) / 2 - Node::getOriginalCenterAxisValue(axis)) / (bounds.max[axis] - bounds.min[axis]);
        }

        assert(-1.0f <= asym && asym <= 1.0f);

        static constexpr float S = 0.5f;
        const float mu = (1 - 2.0f * MinSize / (MaxSize + 1)) * asym;
        const float rho = S * (1 + std::abs(mu));
        const float y1 = std::exp(- 1 / (S * S));
        const float ys = 1 / (1  - y1);

        auto wf = [mu,rho,y1,ys](std::size_t i) {
          const float xi = 2.0f * i / (MaxSize + 1.0f) - 1.0f;
          return ys * (std::exp(- gf::square((xi - mu) / rho) - y1));
        };

        // compute weight

        U (Box<U,N>::* overlapFn)(const Box<U, N>&) const noexcept = nullptr;

        if (firstBounds[MinSize].getVolume() == 0 || secondBounds[MinSize].getVolume() == 0) {
          // perimeter based strategy
          overlapFn = &Box<U,N>::getIntersectionExtentDistance;
        } else {
          // volume based strategy
          overlapFn = &Box<U,N>::getIntersectionVolume;
        }

        U perimeterMax = 2 * bounds.getExtentDistance() - bounds.getMinimumEdge();

        for (std::size_t index = MinSize; index <= MaxSize - MinSize + 1; ++index) {
          auto weight = (firstBounds[index].*overlapFn)(secondBounds[Size - index - 1]);

          if (!status.overlapFree) {
            if (weight == 0) {
              status.overlapFree = true;
            } else {
              auto value = weight * wf(index);

              if (value < status.currentValue) {
                status.currentValue = value;
                result.index = index;
                result.axis = axis;
                result.order = order;
              }
            }
          }

          if (status.overlapFree && weight == 0) {
            weight = firstBounds[index].getExtentDistance() + secondBounds[Size - index - 1].getExtentDistance() - perimeterMax;

            if (weight > 0) {
              gf::Log::debug("weight: %f\n", static_cast<double>(weight));
            }

            assert(weight <= 0);

            auto value = weight / wf(index);

            if (value < status.currentValue) {
              status.currentValue = value;
              result.index = index;
              result.axis = axis;
              result.order = order;
            }
          }
        }
      }

    private:
      boost::container::static_vector<BranchEntry, Size> m_entries;
    };

    /*
     * Comparators and Functors
     */

    class ExtentDistanceEnlargement {
    public:
      ExtentDistanceEnlargement(const Box<U, N>& bounds)
      : m_bounds(bounds)
      {

      }

      bool operator()(const Entry& lhs, const Entry& rhs) const {
        return getExtentDistanceEnlargement(lhs) < getExtentDistanceEnlargement(rhs);
      }

      U getExtentDistanceEnlargement(const Entry& entry) const {
        return entry.bounds.getExtended(m_bounds).getExtentDistance() - entry.bounds.getExtentDistance();
      }

    private:
      const Box<U, N>& m_bounds;
    };

    class OverlapExtentDistanceEnlargement {
    public:
      OverlapExtentDistanceEnlargement(const Box<U, N>& bounds, const Entry& reference)
      : m_bounds(bounds)
      , m_reference(reference)
      , m_extended(reference.bounds.getExtended(bounds))
      {

      }

      U operator()(const Entry& entry) const {
        return getOverlapExtentDistanceEnlargement(entry);
      }

      U getOverlapExtentDistanceEnlargement(const Entry& entry) const {
        return m_extended.getIntersectionExtentDistance(entry.bounds) - m_reference.bounds.getIntersectionExtentDistance(entry.bounds);
      }

    private:
      const Box<U, N>& m_bounds;
      const Entry& m_reference;
      Box<U, N> m_extended;
    };


    class OverlapVolumeEnlargement {
    public:
      OverlapVolumeEnlargement(const Box<U, N>& bounds, const Entry& reference)
      : m_bounds(bounds)
      , m_reference(reference)
      , m_extended(reference.bounds.getExtended(bounds))
      {

      }

      U operator()(const Entry& entry) const {
        return getOverlapVolumeEnlargement(entry);
      }

      U getOverlapVolumeEnlargement(const Entry& entry) const {
        return m_extended.getIntersectionVolume(entry.bounds) - m_reference.bounds.getIntersectionVolume(entry.bounds);
      }

    private:
      const Box<U, N>& m_bounds;
      const Entry& m_reference;
      Box<U, N> m_extended;
    };

    class MinAxisComparator {
    public:
      MinAxisComparator(std::size_t axis)
      : m_axis(axis)
      {

      }

      bool operator()(const Box<U, N>& lhs, const Box<U, N>& rhs) {
        return std::make_tuple(lhs.min[m_axis], lhs.max[m_axis]) < std::make_tuple(rhs.min[m_axis], rhs.max[m_axis]);
      }

    private:
      std::size_t m_axis;
    };

    template<typename Entry>
    class EntryMinAxisComparator {
    public:
      EntryMinAxisComparator(std::size_t axis)
      : m_axis(axis)
      {

      }

      bool operator()(const Entry& lhs, const Entry& rhs) {
        return std::make_tuple(lhs.bounds.min[m_axis], lhs.bounds.max[m_axis]) < std::make_tuple(rhs.bounds.min[m_axis], rhs.bounds.max[m_axis]);
      }

    private:
      std::size_t m_axis;
    };

    class MaxAxisComparator {
    public:
      MaxAxisComparator(std::size_t axis)
      : m_axis(axis)
      {

      }

      bool operator()(const Box<U, N>& lhs, const Box<U, N>& rhs) {
        return std::make_tuple(lhs.max[m_axis], lhs.min[m_axis]) < std::make_tuple(rhs.max[m_axis], rhs.min[m_axis]);
      }

    private:
      std::size_t m_axis;
    };

    template<typename Entry>
    class EntryMaxAxisComparator {
    public:
      EntryMaxAxisComparator(std::size_t axis)
      : m_axis(axis)
      {

      }

      bool operator()(const Entry& lhs, const Entry& rhs) {
        return std::make_tuple(lhs.bounds.max[m_axis], lhs.bounds.min[m_axis]) < std::make_tuple(rhs.bounds.max[m_axis], rhs.bounds.min[m_axis]);
      }

    private:
      std::size_t m_axis;
    };

  private:
    void updateBounds(Node *node) {
      while (node != nullptr) {
        Box<U, N> bounds = node->computeBounds();

        Branch *parent = node->getParent();

        if (parent != nullptr) {
          parent->updateBoundsForChild(bounds, node);
        }

        node = parent;
      }
    }

  private:
    Node *m_root;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SPATIAL_H
