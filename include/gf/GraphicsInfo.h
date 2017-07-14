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
   * @ingroup graphics
   * @brief A class for graphics info
   *
   * This class only contains static methods. It provides convenient functions
   * for OpenGL configuration variables. The name of the variables are given
   * in the documentation of the functions.
   *
   * @sa [glGetString() - docs.gl](://docs.gl/es2/glGetString)
   * @sa [glGetIntegerv() - docs.gl](http://docs.gl/es2/glGet)
   */
  class GF_API GraphicsInfo {
  public:
    /**
     * @brief Deleted default constructor
     */
    GraphicsInfo() = delete;

    /**
     * @brief Get OpenGL vendor string
     *
     * OpenGL name: `GL_VENDOR`
     */
    static std::string getVendor();

    /**
     * @brief Get OpenGL renderer string
     *
     * OpenGL name: `GL_RENDERER`
     */
    static std::string getRenderer();

    /**
     * @brief Get OpenGL version string
     *
     * OpenGL name: `GL_VERSION`
     */
    static std::string getVersion();

    /**
     * @brief Get OpenGL GLSL version string
     *
     * OpenGL name: `GL_SHADING_LANGUAGE_VERSION`
     */
    static std::string getShadingLanguageVersion();

    /**
     * @brief Get available OpenGL extensions
     *
     * OpenGL name: `GL_EXTENSIONS`
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
     * OpenGL name: `GL_NUM_COMPRESSED_TEXTURE_FORMATS` and `GL_COMPRESSED_TEXTURE_FORMATS`
     */
    static std::vector<Format> getCompressedTextureFormats();

    /**
     * @brief Get a list of supported shader binary formats
     *
     * OpenGL name: `GL_NUM_SHADER_BINARY_FORMATS` and `GL_SHADER_BINARY_FORMATS`
     */
    static std::vector<Format> getShaderBinaryFormats();

    /**
     * @brief Get the maximum supported texture image units
     *
     * OpenGL name: `GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS`
     */
    static int getMaxCombinedTextureImageUnits();

    /**
     * @brief Get a rough estimate of the largest cube-map texture
     *
     * OpenGL name: `GL_MAX_CUBE_MAP_TEXTURE_SIZE`
     */
    static int getMaxCubeMapTextureSize();

    /**
     * @brief Get the maximum number of four-element vectors that can be held
     * in uniform variable storage for a fragment shader
     *
     * OpenGL name: `GL_MAX_FRAGMENT_UNIFORM_VECTORS`
     */
    static int getMaxFragmentUniformVectors();

    /**
     * @brief Get the largest renderbuffer width and height
     *
     * OpenGL name: `GL_MAX_RENDERBUFFER_SIZE`
     */
    static int getMaxRenderbufferSize();

    /**
     * @brief Get the maximum supported texture image units in a fragment shader
     *
     * OpenGL name: `GL_MAX_TEXTURE_IMAGE_UNITS`
     */
    static int getMaxTextureImageUnits();

    /**
     * @brief Get a rough estimate of the largest texture
     *
     * OpenGL name: `GL_MAX_TEXTURE_SIZE`
     */
    static int getMaxTextureSize();

    /**
     * @brief Get the maximum number four-element vectors available for varying
     * variables used by vertex and fragment shaders
     *
     * OpenGL name: `GL_MAX_VARYING_VECTORS`
     */
    static int getMaxVaryingVectors();

    /**
     * @brief Get the maximum number of 4-component generic vertex attributes
     * accessible to a vertex shader
     *
     * OpenGL name: `GL_MAX_VERTEX_ATTRIBS`
     */
    static int getMaxVertexAttribs();

    /**
     * @brief Get the maximum supported texture image units in a vertex shader
     *
     * OpenGL name: `GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS`
     */
    static int getMaxVertexTextureImageUnits();

    /**
     * @brief Get the maximum number of four-element vectors that can be held
     * in uniform variable storage for a vertex shader
     *
     * OpenGL name: `GL_MAX_VERTEX_UNIFORM_VECTORS`
     */
    static int getMaxVertexUniformVectors();

    /**
     * @brief Get the maximum supported width and height of the viewport
     *
     * OpenGL name: `GL_MAX_VIEWPORT_DIMS`
     */
    static Vector2i getMaxViewportDims();

    /**
     * @brief Get an estimate of the number of bits of subpixel resolution
     *
     * OpenGL name: `GL_SUBPIXEL_BITS`
     */
    static int getSubpixelBits();

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_GRAPHICS_INFO_H
