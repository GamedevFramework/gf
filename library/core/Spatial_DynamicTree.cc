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
#include <gf/Spatial_DynamicTree.h>

#include <stack>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  DynamicTree::DynamicTree()
  : m_root(NullIndex)
  {
  }

  SpatialId DynamicTree::insert(Handle handle, const RectF& bounds) {
    std::size_t index = allocateNode();
    Node& node = m_nodes[index];
    node.handle = handle;
    node.bounds = bounds;
    node.height = 0;
    doInsert(index);
    return static_cast<SpatialId>(index);
  }

  void DynamicTree::modify(SpatialId id, RectF bounds) {
    std::size_t index = static_cast<std::size_t>(id);
    doRemove(index);
    m_nodes[index].bounds = bounds;
    doInsert(index);
  }

  std::size_t DynamicTree::query(const RectF& bounds, SpatialQueryCallback callback, SpatialQuery kind) {
    std::size_t found = 0;

    std::stack<std::size_t> stack;
    stack.push(m_root);

    while (!stack.empty()) {
      std::size_t index = stack.top();
      stack.pop();

      if (index == NullIndex) {
        continue;
      }

      Node *node = &m_nodes[index];

      if (node->isLeaf()) {
        switch (kind) {
          case SpatialQuery::Contain:
            if (bounds.contains(node->bounds)) {
              callback(node->handle);
              ++found;
            }
            break;

          case SpatialQuery::Intersect:
            if (bounds.intersects(node->bounds)) {
              callback(node->handle);
              ++found;
            }
            break;
        }
      } else if (bounds.intersects(node->bounds)) {
        stack.push(node->child1);
        stack.push(node->child2);
      }
    }

    return found;
  }

  void DynamicTree::remove(SpatialId id) {
    std::size_t index = static_cast<std::size_t>(id);
    doRemove(index);
    disposeNode(index);
  }

  void DynamicTree::clear() {
    m_nodes.clear();
    m_root = NullIndex;
  }

  Handle DynamicTree::operator[](SpatialId id) {
    std::size_t index = static_cast<std::size_t>(id);
    return m_nodes[index].handle;
  }

  std::size_t DynamicTree::allocateNode() {
    std::size_t index = m_nodes.allocate();
    Node& node = m_nodes[index];
    node.parent = NullIndex;
    node.child1 = NullIndex;
    node.child2 = NullIndex;
    node.height = 0;
    return index;
  }

  void DynamicTree::disposeNode(std::size_t index) {
    Node& node = m_nodes[index];
    node.height = -1;
    m_nodes.dispose(index);
  }

  void DynamicTree::doInsert(std::size_t leaf) {
    if (m_root == NullIndex) {
      m_root = leaf;
      m_nodes[m_root].parent = NullIndex;
      return;
    }

    // Find the best sibling for this node
    RectF leafBounds = m_nodes[leaf].bounds;
    std::size_t index = m_root;

    while (!m_nodes[index].isLeaf()) {
      std::size_t child1 = m_nodes[index].child1;
      std::size_t child2 = m_nodes[index].child2;

      float perimeter = m_nodes[index].bounds.getExtentLength() * 2.0f;

      RectF combinedBounds = leafBounds.getExtended(m_nodes[index].bounds);
      float combinedPerimeter = combinedBounds.getExtentLength() * 2.0f;

      // Cost of creating a new parent for this node and the new leaf
      float cost = 2.0f * combinedPerimeter;

      // Minimum cost of pushing the leaf further down the tree
      float inheritanceCost = 2.0f * (combinedPerimeter - perimeter);

      // Cost of descending into child
      auto childCost = [&](std::size_t child) {
        RectF bounds = leafBounds.getExtended(m_nodes[child].bounds);

        if (m_nodes[child].isLeaf()) {
          return bounds.getExtentLength() * 2.0f + inheritanceCost;
        }

        float oldPerimeter = m_nodes[child].bounds.getExtentLength() * 2.0f;
        float newPerimeter = bounds.getExtentLength() * 2.0f;
        return (newPerimeter - oldPerimeter) + inheritanceCost;
      };

      float cost1 = childCost(child1);
      float cost2 = childCost(child2);

      // Descend according to the minimum cost.
      if (cost < cost1 && cost < cost2) {
        break;
      }

      // Descend
      if (cost1 < cost2) {
        index = child1;
      } else {
        index = child2;
      }
    }

    std::size_t sibling = index;

    // Create a new parent.
    std::size_t oldParent = m_nodes[sibling].parent;
    std::size_t newParent = allocateNode();

    m_nodes[newParent].parent = oldParent;
    m_nodes[newParent].bounds = leafBounds.getExtended(m_nodes[sibling].bounds);
    m_nodes[newParent].height = m_nodes[sibling].height + 1;

    if (oldParent != NullIndex) {
      // The sibling was not the root.
      if (m_nodes[oldParent].child1 == sibling) {
        m_nodes[oldParent].child1 = newParent;
      } else {
        m_nodes[oldParent].child2 = newParent;
      }
    } else {
      // The sibling was the root.
      m_root = newParent;
    }

    m_nodes[newParent].child1 = sibling;
    m_nodes[newParent].child2 = leaf;
    m_nodes[sibling].parent = newParent;
    m_nodes[leaf].parent = newParent;

    // Walk back up the tree fixing heights and AABBs
    index = m_nodes[leaf].parent;

    while (index != NullIndex) {
      index = balance(index);

      std::size_t child1 = m_nodes[index].child1;
      assert(child1 != NullIndex);

      std::size_t child2 = m_nodes[index].child2;
      assert(child2 != NullIndex);

      m_nodes[index].height = std::max(m_nodes[child1].height, m_nodes[child2].height);
      m_nodes[index].bounds = m_nodes[child1].bounds.getExtended(m_nodes[child2].bounds);

      index = m_nodes[index].parent;
    }
  }

  void DynamicTree::doRemove(std::size_t leaf) {
    if (leaf == m_root) {
      m_root = NullIndex;
      return;
    }

    std::size_t parent = m_nodes[leaf].parent;
    std::size_t grandParent = m_nodes[parent].parent;
    std::size_t sibling;

    if (m_nodes[parent].child1 == leaf) {
      sibling = m_nodes[parent].child2;
    } else {
      sibling = m_nodes[parent].child1;
    }

    if (grandParent != NullIndex) {
      // Destroy parent and connect sibling to grandParent.
      if (m_nodes[grandParent].child1 == parent) {
        m_nodes[grandParent].child1 = sibling;
      } else {
        m_nodes[grandParent].child2 = sibling;
      }

      m_nodes[sibling].parent = grandParent;
      disposeNode(parent);

      // Adjust ancestor bounds.
      std::size_t index = grandParent;
      while (index != NullIndex) {
        index = balance(index);

        std::size_t child1 = m_nodes[index].child1;
        std::size_t child2 = m_nodes[index].child2;

        m_nodes[index].bounds = m_nodes[child1].bounds.getExtended(m_nodes[child2].bounds);
        m_nodes[index].height = 1 + std::max(m_nodes[child1].height, m_nodes[child2].height);

        index = m_nodes[index].parent;
      }
    }	else  {
      m_root = sibling;
      m_nodes[sibling].parent = NullIndex;
      disposeNode(parent);
    }
  }

  std::size_t DynamicTree::balance(std::size_t iA) {
    assert(iA != NullIndex);

    Node *A = &m_nodes[iA];

    if (A->isLeaf() || A->height < 2) {
      return iA;
    }

    std::size_t iB = A->child1;
    Node *B = &m_nodes[iB];

    std::size_t iC = A->child2;
    Node *C = &m_nodes[iC];

    int32_t balance = C->height - B->height;

    // Rotate C up
    if (balance > 1) {
      std::size_t iF = C->child1;
      Node *F = &m_nodes[iF];

      std::size_t iG = C->child2;
      Node *G = &m_nodes[iG];

      // Swap A and C
      C->child1 = iA;
      C->parent = A->parent;
      A->parent = iC;

      // A's old parent should point to C
      if (C->parent != NullIndex) {
        if (m_nodes[C->parent].child1 == iA) {
          m_nodes[C->parent].child1 = iC;
        } else {
          assert(m_nodes[C->parent].child2 == iA);
          m_nodes[C->parent].child2 = iC;
        }
      } else {
        m_root = iC;
      }

      // Rotate
      if (F->height > G->height) {
        C->child2 = iF;
        A->child2 = iG;
        G->parent = iA;
        A->bounds = B->bounds.getExtended(G->bounds);
        C->bounds = A->bounds.getExtended(F->bounds);

        A->height = 1 + std::max(B->height, G->height);
        C->height = 1 + std::max(A->height, F->height);
      }	else {
        C->child2 = iG;
        A->child2 = iF;
        F->parent = iA;
        A->bounds = B->bounds.getExtended(F->bounds);
        C->bounds = A->bounds.getExtended(G->bounds);

        A->height = 1 + std::max(B->height, F->height);
        C->height = 1 + std::max(A->height, G->height);
      }

      return iC;
    }

    // Rotate B up
    if (balance < -1) {
      std::size_t iD = B->child1;
      Node *D = &m_nodes[iD];

      std::size_t iE = B->child2;
      Node *E = &m_nodes[iE];

      // Swap A and B
      B->child1 = iA;
      B->parent = A->parent;
      A->parent = iB;

      // A's old parent should point to B
      if (B->parent != NullIndex) {
        if (m_nodes[B->parent].child1 == iA) {
          m_nodes[B->parent].child1 = iB;
        } else {
          assert(m_nodes[B->parent].child2 == iA);
          m_nodes[B->parent].child2 = iB;
        }
      } else {
        m_root = iB;
      }

      // Rotate
      if (D->height > E->height) {
        B->child2 = iD;
        A->child1 = iE;
        E->parent = iA;
        A->bounds = C->bounds.getExtended(E->bounds);
        B->bounds = A->bounds.getExtended(D->bounds);

        A->height = 1 + std::max(C->height, E->height);
        B->height = 1 + std::max(A->height, D->height);
      } else {
        B->child2 = iE;
        A->child1 = iD;
        D->parent = iA;
        A->bounds = C->bounds.getExtended(D->bounds);
        B->bounds = A->bounds.getExtended(E->bounds);

        A->height = 1 + std::max(C->height, D->height);
        B->height = 1 + std::max(A->height, E->height);
      }

      return iB;
    }

    return iA;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
