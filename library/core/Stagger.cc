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
#include <gf/Stagger.h>

#include <gf/VectorOps.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Vector2f StaggerHelper::computeCenter(Vector2i coords, Vector2f size) const {
    Vector2f base = coords * size;

    switch (m_axis) {
      case MapCellAxis::Y:
        base.y /= 2;

        switch (m_index) {
          case MapCellIndex::Odd:
            if (coords.y % 2 == 0) {
              base += size / 2;
            } else {
              base.x += size.width;
              base.y += size.height / 2;
            }
            break;
          case MapCellIndex::Even:
            if (coords.y % 2 == 0) {
              base.x += size.width;
              base.y += size.height / 2;
            } else {
              base += size / 2;
            }
            break;
        }
        break;
      case MapCellAxis::X:
        base.x /= 2;

        switch (m_index) {
          case MapCellIndex::Odd:
            if (coords.x % 2 == 0) {
              base += size / 2;
            } else {
              base.y += size.height;
              base.x += size.width / 2;
            }
            break;
          case MapCellIndex::Even:
            if (coords.x % 2 == 0) {
              base.y += size.height;
              base.x += size.width / 2;
            } else {
              base += size / 2;
            }
            break;
        }
        break;
    }

    return base;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
