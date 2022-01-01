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
#ifndef GF_SPATIAL_BLOCK_ALLOCATOR_H
#define GF_SPATIAL_BLOCK_ALLOCATOR_H

#include <cassert>
#include <vector>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_container
   * @brief A null index in a block allocator
   *
   * @sa gf::BlockAllocator
   */
  constexpr std::size_t NullIndex = static_cast<std::size_t>(-1);

  /**
   * @ingroup core_container
   * @brief An allocator of objects referenced by an index
   */
  template<typename T>
  class BlockAllocator {
  public:
    /**
     * @brief Default constructor
     */
    BlockAllocator()
    : m_firstFreeBlock(NullIndex)
    , m_allocated(0)
    {
    }

    /**
     * @brief Allocate an object
     *
     * @returns The index representing the object
     */
    std::size_t allocate() {
      std::size_t index = NullIndex;

      if (m_firstFreeBlock != NullIndex) {
        index = m_firstFreeBlock;
        m_firstFreeBlock = m_blocks[index].next;
        m_blocks[index].next = NullIndex;
      } else {
        index = m_blocks.size();
        m_blocks.push_back(Block());
      }

      assert(index < m_blocks.size());
      assert(m_blocks[index].next == NullIndex);

      ++m_allocated;
      return index;
    }

    /**
     * @brief Deallocate the object at the given index
     *
     * After this function call, the index is not valid anymore.
     *
     * @param index A valid index
     */
    void dispose(std::size_t index) {
      assert(index < m_blocks.size());
      m_blocks[index].next = m_firstFreeBlock;
      m_firstFreeBlock = index;
      --m_allocated;
    }

    /**
     * @brief Access the object at a given index
     *
     * @param index A valid index
     */
    T& operator[](std::size_t index) {
      assert(index < m_blocks.size());
      assert(m_blocks[index].next == NullIndex);
      return m_blocks[index].data;
    }

    /**
     * @brief Access the object at a given index
     *
     * @param index A valid index
     */
    const T& operator[](std::size_t index) const {
      assert(index < m_blocks.size());
      assert(m_blocks[index].next == NullIndex);
      return m_blocks[index].data;
    }

    /**
     * @brief Remove all objects at once
     */
    void clear() {
      m_firstFreeBlock = NullIndex;
      m_blocks.clear();
    }

    /**
     * @brief Get the number of allocated objects
     *
     * @returns The number of allocated objects
     */
    std::size_t getAllocated() const {
      return m_allocated;
    }

  private:
    struct Block {
      T data;
      std::size_t next = NullIndex;
    };

    std::size_t m_firstFreeBlock;
    std::vector<Block> m_blocks;
    std::size_t m_allocated;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


#endif // GF_SPATIAL_BLOCK_ALLOCATOR_H
