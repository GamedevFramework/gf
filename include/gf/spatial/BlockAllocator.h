/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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

  constexpr std::size_t NullIndex = -1;

  template<typename T>
  class BlockAllocator {
  public:
    BlockAllocator()
    : m_firstFreeBlock(NullIndex)
    , m_allocated(0)
    {
    }

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

    void dispose(std::size_t index) {
      assert(index < m_blocks.size());
      m_blocks[index].next = m_firstFreeBlock;
      m_firstFreeBlock = index;
      --m_allocated;
    }

    T& operator[](std::size_t index) {
      assert(index < m_blocks.size());
      assert(m_blocks[index].next == NullIndex);
      return m_blocks[index].data;
    }

    const T& operator[](std::size_t index) const {
      assert(index < m_blocks.size());
      assert(m_blocks[index].next == NullIndex);
      return m_blocks[index].data;
    }

    void clear() {
      m_firstFreeBlock = NullIndex;
      m_blocks.clear();
    }

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
