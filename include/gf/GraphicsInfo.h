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
 */
#ifndef GF_GRAPHICS_INFO_H
#define GF_GRAPHICS_INFO_H

#include <string>
#include <vector>

#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A class for graphics info
   *
   * This class only contains static methods.
   *
   */
  class GF_API GraphicsInfo {
  public:
    /**
     * @brief Deleted default constructor
     */
    GraphicsInfo() = delete;

    /**
     * @brief Get OpenGL vendor string
     */
    static std::string getVendor();

    /**
     * @brief Get OpenGL renderer string
     */
    static std::string getRenderer();

    /**
     * @brief Get OpenGL version string
     */
    static std::string getVersion();

    /**
     * @brief Get OpenGL GLSL version string
     */
    static std::string getShadingLanguageVersion();

    /**
     * @brief Get available OpenGL extensions
     */
    static std::vector<std::string> getExtensions();

    /**
     * @brief An OpenGL format
     */
    struct Format {
      int symbol;       ///< Symbolic constant to represent the format
      std::string name; ///< Name of the constant for the format or "UNKNOWN"
    };

    /**
     * @brief Get a list of supported compressed texture formats
     *
     */
    static std::vector<Format> getCompressedTextureFormats();

    /**
     * @brief Get a list of supported shader binary formats
     */
    static std::vector<Format> getShaderBinaryFormats();

    /**
     * @brief Get the maximum supported texture image units
     */
    static int getMaxCombinedTextureImageUnits();

    /**
     * @brief Get a rough estimate of the largest cube-map texture
     */
    static int getMaxCubeMapTextureSize();

    /**
     * @brief Get the maximum number of four-element vectors that can be held
     * in uniform variable storage for a fragment shader
     */
    static int getMaxFragmentUniformVectors();

    /**
     * @brief Get the largest renderbuffer width and height
     */
    static int getMaxRenderbufferSize();

    /**
     * @brief Get the maximum supported texture image units in a fragment shader
     */
    static int getMaxTextureImageUnits();

    /**
     * @brief Get a rough estimate of the largest texture
     */
    static int getMaxTextureSize();

    /**
     * @brief Get the maximum number four-element vectors available for varying
     * variables used by vertex and fragment shaders
     */
    static int getMaxVaryingVectors();

    /**
     * @brief Get the maximum number of 4-component generic vertex attributes
     * accessible to a vertex shader
     */
    static int getMaxVertexAttribs();

    /**
     * @brief Get the maximum supported texture image units in a vertex shader
     */
    static int getMaxVertexTextureImageUnits();

    /**
     * @brief Get the maximum number of four-element vectors that can be held
     * in uniform variable storage for a vertex shader
     */
    static int getMaxVertexUniformVectors();

    /**
     * @brief Get the maximum supported width and height of the viewport
     */
    static Vector2i getMaxViewportDims();

    /**
     * @brief Get an estimate of the number of bits of subpixel resolution
     */
    static int getSubpixelBits();

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_GRAPHICS_INFO_H
