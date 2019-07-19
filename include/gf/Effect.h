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
#ifndef GF_EFFECT_H
#define GF_EFFECT_H

#include "Portability.h"
#include "Shader.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief A post-processing effect
   *
   * @sa gf::PostProcessing
   */
  class GF_API Effect : public Shader {

  protected:
    /**
    * @brief Load both the vertex and fragment shaders from source codes
    * in memory
    *
    * This function loads both the vertex and the fragment
    * shaders. If one of them fails to load, the shader is left
    * empty (the valid shader is unloaded).
    *
    * The source codes must be a valid shaders in GLSL language.
    * GLSL is a C-like language dedicated to OpenGL shaders;
    * you'll probably need to read a good documentation for
    * it before writing your own shaders.
    *
    * @param vertexShader String containing the source code of the vertex shader
    * @param fragmentShader String containing the source code of the fragment shader
    */
    Effect(const char *vertexShader, const char *fragmentShader)
    : Shader(vertexShader, fragmentShader)
    {
    }

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_EFFECT_H
