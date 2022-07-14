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
#include <gf/Spatial_RStarTree.h>

#include <iterator>
#include <numeric>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {
    /*
     * Functors
     */

    class MinAxisComparator {
    public:
      MinAxisComparator(std::size_t axis)
      : m_axis(axis)
      {
      }

      bool operator()(const RectF& lhs, const RectF& rhs) {
        return std::make_tuple(lhs.min[m_axis], lhs.max[m_axis]) < std::make_tuple(rhs.min[m_axis], rhs.max[m_axis]);
      }

    private:
      std::size_t m_axis;
    };

    template<typename Member>
    class MemberMinAxisComparator {
    public:
      MemberMinAxisComparator(std::size_t axis)
      : m_axis(axis)
      {
      }

      bool operator()(const Member& lhs, const Member& rhs) {
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

      bool operator()(const RectF& lhs, const RectF& rhs) {
        return std::make_tuple(lhs.max[m_axis], lhs.min[m_axis]) < std::make_tuple(rhs.max[m_axis], rhs.min[m_axis]);
      }

    private:
      std::size_t m_axis;
    };

    template<typename Member>
    class MemberMaxAxisComparator {
    public:
      MemberMaxAxisComparator(std::size_t axis)
      : m_axis(axis)
      {
      }

      bool operator()(const Member& lhs, const Member& rhs) {
        return std::make_tuple(lhs.bounds.max[m_axis], lhs.bounds.min[m_axis]) < std::make_tuple(rhs.bounds.max[m_axis], rhs.bounds.min[m_axis]);
      }

    private:
      std::size_t m_axis;
    };


    template<typename Member>
    class ExtentLengthEnlargement {
    public:
      ExtentLengthEnlargement(const RectF& bounds)
      : m_bounds(bounds)
      {

      }

      bool operator()(const Member& lhs, const Member& rhs) const {
        return getExtentLengthEnlargement(lhs) < getExtentLengthEnlargement(rhs);
      }

      float getExtentLengthEnlargement(const Member& member) const {
        return member.bounds.getExtended(m_bounds).getExtentLength() - member.bounds.getExtentLength();
      }

    private:
      const RectF& m_bounds;
    };

    template<typename Member>
    class OverlapExtentLengthEnlargement {
    public:
      OverlapExtentLengthEnlargement(const RectF& bounds, const Member& reference)
      : m_bounds(bounds)
      , m_reference(reference)
      , m_extended(reference.bounds.getExtended(bounds))
      {

      }

      float operator()(const Member& member) const {
        return getOverlapExtentLengthEnlargement(member);
      }

      float getOverlapExtentLengthEnlargement(const Member& member) const {
        return m_extended.getIntersectionExtentLength(member.bounds) - m_reference.bounds.getIntersectionExtentLength(member.bounds);
      }

    private:
      const RectF& m_bounds;
      const Member& m_reference;
      RectF m_extended;
    };

    template<typename Member>
    class OverlapVolumeEnlargement {
    public:
      OverlapVolumeEnlargement(const RectF& bounds, const Member& reference)
      : m_bounds(bounds)
      , m_reference(reference)
      , m_extended(reference.bounds.getExtended(bounds))
      {

      }

      float operator()(const Member& member) const {
        return getOverlapVolumeEnlargement(member);
      }

      float getOverlapVolumeEnlargement(const Member& member) const {
        return m_extended.getIntersectionVolume(member.bounds) - m_reference.bounds.getIntersectionVolume(member.bounds);
      }

    private:
      const RectF& m_bounds;
      const Member& m_reference;
      RectF m_extended;
    };

    /*
     * Types
     */

    enum class SplitOrder {
      Min,
      Max,
    };

    struct SplitResult {
      std::size_t index = static_cast<std::size_t>(-1);
      std::size_t axis = static_cast<std::size_t>(-1);
      SplitOrder order;
    };

    struct SplitStatus {
      bool overlapFree = false;
      float currentValue = std::numeric_limits<float>::max();
    };

    /*
     * Functions for leafs
     */

    std::size_t computeSplitIndex(const std::vector<RectF>& boxes, std::size_t axis, const RectF& nodeBounds) {
      const std::size_t size = boxes.size();

      std::vector<RectF> firstBounds;

      std::partial_sum(boxes.begin(), boxes.end(), std::back_inserter(firstBounds), [](const RectF& lhs, const RectF& rhs) {
        return lhs.getExtended(rhs);
      });

      std::vector<RectF> secondBounds;

      std::partial_sum(boxes.rbegin(), boxes.rend(), std::back_inserter(secondBounds), [](const RectF& lhs, const RectF& rhs) {
        return lhs.getExtended(rhs);
      });


      // define wf

      const auto& bounds = firstBounds.back();
      float asym = 0.0f;

      if (!nodeBounds.isEmpty()) {
        asym = 2.0f * ((bounds.min[axis] + bounds.max[axis]) / 2.0f - (nodeBounds.min[axis] + nodeBounds.max[axis]) / 2.0f) / (bounds.max[axis] - bounds.min[axis]);
      }

      assert(-1.0f <= asym && asym <= 1.0f);

      static constexpr float S = 0.5f;
      const float mu = (1 - 2.0f * RStarTree::MinSize / (RStarTree::MaxSize + 1)) * asym;
      const float rho = S * (1 + std::abs(mu));
      const float y1 = std::exp(- 1 / (S * S));
      const float ys = 1 / (1  - y1);

      auto wf = [mu,rho,y1,ys](std::size_t i) {
        const float xi = 2.0f * i / (RStarTree::MaxSize + 1.0f) - 1.0f;
        return ys * (std::exp(- gf::square((xi - mu) / rho) - y1));
      };

      // compute weight

      std::size_t currentIndex = 0;
      float currentValue = std::numeric_limits<float>::max();

      float (RectF::* overlapFn)(const RectF&) const noexcept = nullptr;

      if (firstBounds[RStarTree::MinSize].getVolume() == 0 || secondBounds[RStarTree::MinSize].getVolume() == 0) {
        // perimeter based strategy
        overlapFn = &RectF::getIntersectionExtentLength;
      } else {
        // volume based strategy
        overlapFn = &RectF::getIntersectionVolume;
      }

      bool overlapFree = false;

      float perimeterMax = 2 * bounds.getExtentLength() - bounds.getMinimumEdge();

      for (std::size_t index = RStarTree::MinSize; index <= RStarTree::MaxSize - RStarTree::MinSize; ++index) {
        auto weight = (firstBounds[index].*overlapFn)(secondBounds[size - index - 1]);

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
          weight = firstBounds[index].getExtentLength() + secondBounds[size - index - 1].getExtentLength() - perimeterMax;
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

    float computeAxisValue(const std::vector<RectF>& boxes) {
      const std::size_t size = boxes.size();

      std::vector<RectF> firstBounds;

      std::partial_sum(boxes.begin(), boxes.end(), std::back_inserter(firstBounds), [](const RectF& lhs, const RectF& rhs) {
        return lhs.getExtended(rhs);
      });

      std::vector<RectF> secondBounds;

      std::partial_sum(boxes.rbegin(), boxes.rend(), std::back_inserter(secondBounds), [](const RectF& lhs, const RectF& rhs) {
        return lhs.getExtended(rhs);
      });

      float value = 0.0f;

      for (std::size_t j = RStarTree::MinSize; j <= RStarTree::MaxSize - RStarTree::MinSize; ++j) {
        value += firstBounds[j].getExtentLength() + secondBounds[size - j].getExtentLength();
      }

      return value;
    }

    std::tuple<std::size_t, SplitOrder> computeSplitAxis(std::vector<RectF>& boxes) {
      std::size_t currentAxis = 0;
      float currentValue = std::numeric_limits<float>::max();
      SplitOrder currentOrder = SplitOrder::Min;

      for (std::size_t axis = 0; axis < 2; ++axis) {
        std::sort(boxes.begin(), boxes.end(), MinAxisComparator(axis));

        float value = computeAxisValue(boxes);

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

    SplitResult computeSplitForLeaf(std::vector<RectF>& boxes, const RectF& nodeBounds) {
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

      split.index = computeSplitIndex(boxes, split.axis, nodeBounds);
      return split;
    }


    /*
     * Functions for branches
     */

    void computeBestSplitValue(std::vector<RectF>& boxes, SplitResult& split, SplitStatus& status, std::size_t axis, SplitOrder order, const RectF& nodeBounds) {
      const std::size_t size = boxes.size();

      std::vector<RectF> firstBounds;

      std::partial_sum(boxes.begin(), boxes.end(), std::back_inserter(firstBounds), [](const RectF& lhs, const RectF& rhs) {
        return lhs.getExtended(rhs);
      });

      std::vector<RectF> secondBounds;

      std::partial_sum(boxes.rbegin(), boxes.rend(), std::back_inserter(secondBounds), [](const RectF& lhs, const RectF& rhs) {
        return lhs.getExtended(rhs);
      });

      // define wf

      const auto& bounds = firstBounds.back();
      float asym = 0.0f;

      if (!nodeBounds.isEmpty()) {
        asym = 2.0f * ((bounds.min[axis] + bounds.max[axis]) / 2 - (nodeBounds.min[axis] + nodeBounds.max[axis]) / 2) / (bounds.max[axis] - bounds.min[axis]);
      }

      assert(-1.0f <= asym && asym <= 1.0f);

      static constexpr float S = 0.5f;
      const float mu = (1 - 2.0f * RStarTree::MinSize / (RStarTree::MaxSize + 1)) * asym;
      const float rho = S * (1 + std::abs(mu));
      const float y1 = std::exp(- 1 / (S * S));
      const float ys = 1 / (1  - y1);

      auto wf = [mu,rho,y1,ys](std::size_t i) {
        const float xi = 2.0f * i / (RStarTree::MaxSize + 1.0f) - 1.0f;
        return ys * (std::exp(- gf::square((xi - mu) / rho) - y1));
      };

      // compute weight

      float (RectF::* overlapFn)(const RectF&) const noexcept = nullptr;

      if (firstBounds[RStarTree::MinSize].getVolume() == 0 || secondBounds[RStarTree::MinSize].getVolume() == 0) {
        // perimeter based strategy
        overlapFn = &RectF::getIntersectionExtentLength;
      } else {
        // volume based strategy
        overlapFn = &RectF::getIntersectionVolume;
      }

      float perimeterMax = 2 * bounds.getExtentLength() - bounds.getMinimumEdge();

      for (std::size_t index = RStarTree::MinSize; index <= RStarTree::MaxSize - RStarTree::MinSize; ++index) {
        auto weight = (firstBounds[index].*overlapFn)(secondBounds[size - index - 1]);

        if (!status.overlapFree) {
          if (weight == 0) {
            status.overlapFree = true;
          } else {
            auto value = weight * wf(index);

            if (value < status.currentValue) {
              status.currentValue = value;
              split.index = index;
              split.axis = axis;
              split.order = order;
            }
          }
        }

        if (status.overlapFree && weight == 0) {
          weight = firstBounds[index].getExtentLength() + secondBounds[size - index - 1].getExtentLength() - perimeterMax;
          assert(weight <= 0);

          auto value = weight / wf(index);

          if (value < status.currentValue) {
            status.currentValue = value;
            split.index = index;
            split.axis = axis;
            split.order = order;
          }
        }
      }
    }

    SplitResult computeSplitForBranch(std::vector<RectF>& boxes, const RectF& nodeBounds) {
      SplitResult split;
      SplitStatus status;

      for (std::size_t axis = 0; axis < 2; ++axis) {
        std::sort(boxes.begin(), boxes.end(), MinAxisComparator(axis));
        computeBestSplitValue(boxes, split, status, axis, SplitOrder::Min, nodeBounds);

        std::sort(boxes.begin(), boxes.end(), MaxAxisComparator(axis));
        computeBestSplitValue(boxes, split, status, axis, SplitOrder::Max, nodeBounds);
      }

      return split;
    }

  }


  RStarTree::RStarTree()
  : m_root(NullIndex)
  {
    m_root = allocateNode();
    Node& node = m_nodes[m_root];
    node.parent = NullIndex;
    node.type = Node::Leaf;
  }

  SpatialId RStarTree::insert(Handle handle, const RectF& bounds) {
    std::size_t entryIndex = allocateEntry();
    m_entries[entryIndex].handle = handle;
    m_entries[entryIndex].bounds = bounds;

    doInsert(entryIndex, bounds);
//     validate();

    return static_cast<SpatialId>(entryIndex);
  }

  void RStarTree::modify(SpatialId id, RectF bounds) {
    std::size_t entryIndex = static_cast<std::size_t>(id);
    doRemove(entryIndex);
    m_entries[entryIndex].bounds = bounds;
    doInsert(entryIndex, bounds);

//     validate();
  }

  std::size_t RStarTree::query(const RectF& bounds, SpatialQueryCallback callback, SpatialQuery kind) {
    return doQuery(m_root, bounds, callback, kind);
  }

  void RStarTree::remove(SpatialId id) {
    std::size_t entryIndex = static_cast<std::size_t>(id);
    doRemove(entryIndex);
    m_entries.dispose(entryIndex);

//     validate();
  }

  void RStarTree::clear() {
    m_entries.clear();
    m_nodes.clear();
    m_root = NullIndex;
  }

  Handle RStarTree::operator[](SpatialId id) {
    std::size_t index = static_cast<std::size_t>(id);
    return m_entries[index].handle;
  }


  std::size_t RStarTree::allocateEntry() {
    return m_entries.allocate();
  }

  void RStarTree::disposeEntry(std::size_t index) {
    m_entries.dispose(index);
  }

  std::size_t RStarTree::allocateNode() {
    return m_nodes.allocate();
  }

  void RStarTree::disposeNode(std::size_t index) {
    m_nodes[index].members.clear();
    m_nodes.dispose(index);
  }

  RectF RStarTree::computeBounds(std::size_t nodeIndex) {
    Node& node = m_nodes[nodeIndex];

    assert(!node.members.empty());
    RectF bounds = RectF::empty();

    for (auto& member : node.members) {
      bounds.extend(member.bounds);
    }

    node.bounds = bounds;
    return bounds;
  }

  void RStarTree::updateBoundsForChild(std::size_t parentIndex, const RectF& bounds, std::size_t childIndex) {
    assert(m_nodes[parentIndex].type == Node::Branch);
    assert(m_nodes[childIndex].parent == parentIndex);

    for (auto& member : m_nodes[parentIndex].members) {
      if (member.index == childIndex) {
        member.bounds = bounds;
        return;
      }
    }

    assert(false);
  }

  void RStarTree::doInsert(std::size_t entryIndex, const RectF& bounds) {
    assert(m_root != NullIndex);

    std::size_t leafIndex = chooseSubtree(m_root, bounds);
    assert(m_nodes[leafIndex].type == Node::Leaf);

    std::size_t splittedIndex = doInsertInLeaf(leafIndex, entryIndex, bounds);
    std::size_t currentIndex = leafIndex;

    while (splittedIndex != NullIndex) {
      RectF splittedBounds = computeBounds(splittedIndex);
      RectF currentBounds = computeBounds(currentIndex);

      if (currentIndex == m_root) {
        std::size_t branchIndex = allocateNode();
        Node& branch = m_nodes[branchIndex];
        branch.parent = NullIndex;
        branch.type = Node::Branch;

        m_nodes[currentIndex].parent = branchIndex;
        doInsertInBranch(branchIndex, currentIndex, currentBounds);

        m_nodes[splittedIndex].parent = branchIndex;
        doInsertInBranch(branchIndex, splittedIndex, splittedBounds);

        branch.bounds = computeBounds(branchIndex);
        assert(m_nodes[branchIndex].members.size() == 2);

        m_root = branchIndex;
        currentIndex = m_root;
        splittedIndex = NullIndex;
      } else {
        std::size_t parentIndex = m_nodes[currentIndex].parent;
        assert(parentIndex != NullIndex);
        assert(m_nodes[parentIndex].type == Node::Branch);

        updateBoundsForChild(parentIndex, currentBounds, currentIndex);

        m_nodes[splittedIndex].parent = parentIndex;
        splittedIndex = doInsertInBranch(parentIndex, splittedIndex, splittedBounds);

        currentIndex = parentIndex;
      }
    }

    while (currentIndex != m_root) {
      RectF currentBounds = computeBounds(currentIndex);

      std::size_t parentIndex = m_nodes[currentIndex].parent;
      assert(parentIndex != NullIndex);
      assert(m_nodes[parentIndex].type == Node::Branch);

      updateBoundsForChild(parentIndex, currentBounds, currentIndex);

      currentIndex = parentIndex;
    }

  }

  std::size_t RStarTree::chooseSubtree(std::size_t nodeIndex, const RectF& bounds) {
    assert(nodeIndex != NullIndex);

    while (m_nodes[nodeIndex].type != Node::Leaf) {
      nodeIndex = chooseNode(nodeIndex, bounds);
    }

    return nodeIndex;
  }

  std::size_t RStarTree::chooseNode(std::size_t nodeIndex, const RectF& bounds) {
    Node& node = m_nodes[nodeIndex];
    assert(node.type == Node::Branch);

    std::size_t coveringIndex = searchForCoveringNode(nodeIndex, bounds);

    if (coveringIndex != NullIndex) {
      return coveringIndex;
    }

    std::sort(node.members.begin(), node.members.end(), ExtentLengthEnlargement<Member>(bounds));

    OverlapExtentLengthEnlargement<Member> extentDistanceEnlargement(bounds, node.members.front());

    std::size_t p;

    for (p = node.members.size() - 1; p > 0; --p) {
      if (extentDistanceEnlargement(node.members[p]) != 0) {
        break;
      }
    }

    if (p == 0) {
      return node.members[0].index;
    }

    std::vector<Candidate> candidates(p + 1, { 0, 0.0f, false });
    std::size_t candidateIndex = NullIndex;

    if (existsEmptyVolumeExtension(nodeIndex, bounds)) {
      candidateIndex = findCandidates<OverlapExtentLengthEnlargement<Member>>(nodeIndex, 0, p, bounds, candidates);
    } else {
      candidateIndex = findCandidates<OverlapVolumeEnlargement<Member>>(nodeIndex, 0, p, bounds, candidates);
    }

    if (candidateIndex != NullIndex) {
      return candidateIndex;
    }

    candidates.erase(std::remove_if(candidates.begin(), candidates.end(), [](const Candidate& candidate) {
      return !candidate.isCandidate;
    }), candidates.end());

    assert(!candidates.empty());

    auto it = std::min_element(candidates.begin(), candidates.end(), [](const Candidate& lhs, const Candidate& rhs) {
      return lhs.overlap < rhs.overlap;
    });

    return node.members[it->index].index;
  }

  bool RStarTree::existsEmptyVolumeExtension(std::size_t nodeIndex, const RectF& bounds) {
    Node& node = m_nodes[nodeIndex];

    for (auto& member : node.members) {
      if (member.bounds.getExtended(bounds).getVolume() == 0.0f) {
        return true;
      }
    }

    return false;
  }

  std::size_t RStarTree::searchForCoveringNode(std::size_t nodeIndex, const RectF& bounds) {
    std::size_t bestNodeForVolumeIndex = NullIndex;
    float bestVolume = std::numeric_limits<float>::max();

    std::size_t bestNodeForExtentLengthIndex = NullIndex;
    float bestExtentLength = std::numeric_limits<float>::max();

    Node& node = m_nodes[nodeIndex];

    for (auto& member : node.members) {
      if (member.bounds.getIntersection(bounds) == bounds) {
        float volume = member.bounds.getVolume();

        if (bestNodeForVolumeIndex == NullIndex || volume < bestVolume) {
          bestVolume = volume;
          bestNodeForVolumeIndex = member.index;
        }

        float extentLength = member.bounds.getExtentLength();

        if (bestNodeForExtentLengthIndex == NullIndex || extentLength < bestExtentLength) {
          bestExtentLength = extentLength;
          bestNodeForExtentLengthIndex = member.index;
        }
      }
    }

    if (bestNodeForVolumeIndex != NullIndex) {
      if (bestVolume > 0) {
        return bestNodeForExtentLengthIndex;
      }

      assert(bestNodeForExtentLengthIndex != NullIndex);
      return bestNodeForExtentLengthIndex;
    }

    return NullIndex;
  }

  template<typename OverlapEnlargement>
  std::size_t RStarTree::findCandidates(std::size_t nodeIndex, std::size_t t, std::size_t p, const RectF& bounds, std::vector<Candidate>& candidates) {
    Node& node = m_nodes[nodeIndex];

    candidates[t].index = t;
    candidates[t].isCandidate = true;

    OverlapEnlargement enlargement(bounds, node.members[t]);
    float overlap = 0.0f;

    for (std::size_t i = 0; i <= p; ++i) {
      if (i == t) {
        continue;
      }

      float localOverlap = enlargement(node.members[i]);
      overlap += localOverlap;

      if (localOverlap == 0.0f && !candidates[i].isCandidate) {
        std::size_t index = findCandidates<OverlapEnlargement>(nodeIndex, i, p, bounds, candidates);

        if (index != NullIndex) {
          return index;
        }
      }
    }

    if (overlap == 0.0f) {
      return node.members[t].index;
    }

    candidates[t].overlap = overlap;
    return NullIndex;
  }

  std::size_t RStarTree::doInsertInLeaf(std::size_t nodeIndex, std::size_t entryIndex, const RectF& entryBounds) {
    Node& node = m_nodes[nodeIndex];
    assert(node.type == Node::Leaf);

    node.members.push_back({ entryBounds, entryIndex });
    m_entries[entryIndex].node = nodeIndex;

    if (node.members.size() < Size) {
      return NullIndex;
    }

    std::vector<RectF> boxes;
    std::transform(node.members.begin(), node.members.end(), std::back_inserter(boxes), [](const auto& member) { return member.bounds; });

    SplitResult split = computeSplitForLeaf(boxes, node.bounds);

    switch (split.order) {
      case SplitOrder::Min:
        std::sort(node.members.begin(), node.members.end(), MemberMinAxisComparator<Member>(split.axis));
        break;
      case SplitOrder::Max:
        std::sort(node.members.begin(), node.members.end(), MemberMaxAxisComparator<Member>(split.axis));
        break;
    }

    std::size_t leafIndex = allocateNode();
    // from here, node may be a dangling reference
    m_nodes[leafIndex].type = Node::Leaf;
    m_nodes[leafIndex].parent = NullIndex;

    auto splitIteratorBegin = std::next(m_nodes[nodeIndex].members.begin(), split.index + 1);
    auto splitIteratorEnd = m_nodes[nodeIndex].members.end();

    std::move(splitIteratorBegin, splitIteratorEnd, std::back_inserter(m_nodes[leafIndex].members));
    m_nodes[nodeIndex].members.erase(splitIteratorBegin, splitIteratorEnd);

    for (auto& member : m_nodes[leafIndex].members) {
      m_entries[member.index].node = leafIndex;
    }

    return leafIndex;
  }

  std::size_t RStarTree::doInsertInBranch(std::size_t nodeIndex, std::size_t childIndex, const RectF& childBounds) {
    Node& node = m_nodes[nodeIndex];
    assert(node.type == Node::Branch);

    node.members.push_back({ childBounds, childIndex });
    m_nodes[childIndex].parent = nodeIndex;

    if (node.members.size() < Size) {
      return NullIndex;
    }

    std::vector<RectF> boxes;
    std::transform(node.members.begin(), node.members.end(), std::back_inserter(boxes), [](const auto& member) { return member.bounds; });

    SplitResult split = computeSplitForBranch(boxes, node.bounds);

    switch (split.order) {
      case SplitOrder::Min:
        std::sort(node.members.begin(), node.members.end(), MemberMinAxisComparator<Member>(split.axis));
        break;
      case SplitOrder::Max:
        std::sort(node.members.begin(), node.members.end(), MemberMaxAxisComparator<Member>(split.axis));
        break;
    }

    std::size_t branchIndex = allocateNode();
    // from here, node may be a dangling reference
    m_nodes[branchIndex].type = Node::Branch;
    m_nodes[branchIndex].parent = NullIndex;

    auto splitIteratorBegin = std::next(m_nodes[nodeIndex].members.begin(), split.index + 1);
    auto splitIteratorEnd = m_nodes[nodeIndex].members.end();

    std::move(splitIteratorBegin, splitIteratorEnd, std::back_inserter(m_nodes[branchIndex].members));
    m_nodes[nodeIndex].members.erase(splitIteratorBegin, splitIteratorEnd);

    for (auto& member : m_nodes[branchIndex].members) {
      m_nodes[member.index].parent = branchIndex;
    }

    return branchIndex;
  }


  std::size_t RStarTree::doQuery(std::size_t nodeIndex, const RectF& bounds, SpatialQueryCallback callback, SpatialQuery kind) {
    std::size_t found = 0;
    Node& node = m_nodes[nodeIndex];

    switch (node.type) {
      case Node::Leaf:
        switch (kind) {
          case SpatialQuery::Contain:
            for (auto& member : node.members) {
              if (bounds.contains(member.bounds)) {
                callback(m_entries[member.index].handle);
                ++found;
              }
            }
            break;

          case SpatialQuery::Intersect:
            for (auto& member : node.members) {
              if (bounds.intersects(member.bounds)) {
                callback(m_entries[member.index].handle);
                ++found;
              }
            }
            break;
        }
        break;
      case Node::Branch:
        for (auto& member : node.members) {
          if (bounds.intersects(member.bounds)) {
            found += doQuery(member.index, bounds, callback, kind);
          }
        }
        break;
    }

    return found;
  }

  void RStarTree::getEntriesAndDispose(std::size_t nodeIndex, std::vector<std::size_t>& eliminated) {
    Node& node = m_nodes[nodeIndex];

    if (node.type == Node::Leaf) {
      for (auto& member : node.members) {
        assert(m_entries[member.index].node == nodeIndex);
        m_entries[member.index].node = NullIndex;
        eliminated.push_back(member.index);
      }
    } else {
      for (auto& member : node.members) {
        getEntriesAndDispose(member.index, eliminated);
      }
    }

    disposeNode(nodeIndex);
  }

  void RStarTree::doRemove(std::size_t entryIndex) {
    Entry& entry = m_entries[entryIndex];

    std::size_t nodeIndex = entry.node;
    Node& node = m_nodes[nodeIndex];
    assert(node.type == Node::Leaf);

    {
      auto it = std::find_if(node.members.begin(), node.members.end(), [entryIndex](auto& member) { return member.index == entryIndex; });
      assert(it != node.members.end());
      node.members.erase(it);
    }

    // condense tree

    std::vector<std::size_t> eliminated;
    std::size_t currentIndex = nodeIndex;

    while (m_nodes[currentIndex].parent != NullIndex) {
      Node& current = m_nodes[currentIndex];
      std::size_t parentIndex = current.parent;

      if (current.members.size() < MinSize) {
        Node& parent = m_nodes[parentIndex];
        assert(parent.type == Node::Branch);
        auto it = std::find_if(parent.members.begin(), parent.members.end(), [currentIndex](auto& member) { return member.index == currentIndex; });
        assert(it != parent.members.end());
        parent.members.erase(it);

        getEntriesAndDispose(currentIndex, eliminated);
      } else {
        RectF bounds = computeBounds(currentIndex);
        updateBoundsForChild(parentIndex, bounds, currentIndex);
      }

      currentIndex = parentIndex;
    }

    for (auto index : eliminated) {
      doInsert(index, m_entries[index].bounds);
    }
  }

  void RStarTree::validate() const {
    [[maybe_unused]] std::size_t entries = validateNode(m_root);
    assert(entries == m_entries.getAllocated());
  }

  std::size_t RStarTree::validateNode(std::size_t nodeIndex) const {
    const Node& node = m_nodes[nodeIndex];
    std::size_t entries = 0;

    assert(nodeIndex == m_root || (MinSize <= node.members.size() && node.members.size() <= MaxSize));

    if (node.type == Node::Leaf) {
      entries += node.members.size();

      for (auto& member : node.members) {
        [[maybe_unused]] const Entry& entry = m_entries[member.index];
        assert(entry.bounds == member.bounds);
        assert(entry.node == nodeIndex);
      }
    } else {
      assert(node.type == Node::Branch);

      for (auto& member : node.members) {
        [[maybe_unused]] const Node& child = m_nodes[member.index];
        assert(child.bounds == member.bounds);
        assert(child.parent == nodeIndex);
        entries += validateNode(member.index);
      }
    }

    return entries;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
