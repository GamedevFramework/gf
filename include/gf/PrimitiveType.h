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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#ifndef GF_PRIMITIVE_TYPE_H
#define GF_PRIMITIVE_TYPE_H

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief Kind of primitives to render
   *
   * gf::PrimitiveType represents a kind of primitive to render. The primitives
   * are mapped to their OpenGL equivalents, specified by
   * [glDrawArrays](http://docs.gl/es2/glDrawArrays) or
   * [glDrawElements](http://docs.gl/es2/glDrawElements).
   *
   * @sa gf::RenderTarget::draw
   */
  enum class PrimitiveType {
    Points,         ///< List of individual points
    LineStrip,      ///< List of connected lines, a point uses the previous point to form a line
    LineLoop,       ///< List of connected lines, a point uses the previous point to form a line, the last point is connected to the first
    Lines,          ///< List of individual lines
    TriangleStrip,  ///< List of connected triangles, a point uses the two previous points to form a triangle
    TriangleFan,    ///< List of connected triangles, a point uses the common center and the previous point to form a triangle
    Triangles,      ///< List of individual triangles
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_PRIMITIVE_TYPE_H
