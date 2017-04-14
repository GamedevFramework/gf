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
#ifndef GF_SHADER_H
#define GF_SHADER_H

#include <string>
#include <map>

#include "Matrix.h"
#include "Path.h"
#include "Portability.h"
#include "StringRef.h"
#include "Texture.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class InputStream;

  /**
   * @ingroup graphics
   * @brief An OpenGL vertex and/or fragment shader
   *
   * Shaders are programs written using a specific language,
   * executed directly by the graphics card and allowing
   * to apply real-time operations to the rendered entities.
   *
   * There are two kinds of shaders:
   *
   * - vertex shaders, that process vertices
   * - fragment (or pixel) shaders, that process pixels
   *
   * A gf::Shader can be composed of either a vertex shader
   * alone, a geometry shader alone, a fragment shader alone,
   * or any combination of them. (see the variants of the
   * load functions).
   *
   * Shaders are written in GLSL, which is a C-like
   * language dedicated to OpenGL shaders. You'll probably
   * need to learn its basics before writing your own shaders
   * for gf. See also @ref shader.
   *
   * Like any C/C++ program, a GLSL shader has its own variables
   * called _uniforms_ that you can set from your C++ application.
   * gf::Shader handles different types of uniforms:
   *
   * - scalars: `float`, `int`
   * - vectors (2, 3 or 4 components)
   * - matrices (3x3 or 4x4)
   * - samplers (textures)
   *
   * Every uniform variable in a shader can be set through one of the
   * setUniform() overloads. For example, if you have a shader with the
   * following uniforms:
   *
   * ~~~
   * uniform float offset;
   * uniform vec3 point;
   * uniform vec4 color;
   * uniform mat3 matrix;
   * uniform sampler2D overlay;
   * ~~~
   *
   * You can set their values from C++ code as follows:
   *
   * ~~~
   * shader.setUniform("offset", 2.0f);
   * shader.setUniform("point", gf::Vector3f(0.5f, 0.8f, 0.3f));
   * shader.setUniform("color", color); // color is a gf::Color4f
   * shader.setUniform("matrix", trans); // trans is a gf::Matrix3f
   * shader.setUniform("overlay", texture); // texture is a gf::Texture
   * ~~~
   *
   * To apply a shader to a drawable, you must pass it as part of the
   * gf::RenderStates in the call to @ref RenderTarget::draw() function:
   *
   * ~~~{.cc}
   * gf::RenderStates states;
   * states.shader = &shader;
   * renderer.draw(sprite, states);
   * ~~~
   *
   * In the code above we pass a pointer to the shader, because it may
   * be null (which means "default shader").
   *
   * Shaders can be used on any drawable, but some combinations are
   * not interesting. For example, using a vertex shader on a gf::Sprite
   * is limited because there are only 4 vertices, the sprite would
   * have to be subdivided in order to apply wave effects.
   * Another bad example is a fragment shader with gf::Text: the texture
   * of the text is not the actual text that you see on screen, it is
   * a big texture containing all the characters of the font in an
   * arbitrary order; thus, texture lookups on pixels other than the
   * current one may not give you the expected result.
   */
  class GF_API Shader {
  public:
    /**
     * @brief Type of shaders
     */
    enum Type {
      Vertex,   ///< Type for a vertex shader
      Fragment, ///< Type for a fragment (or pixel) shader
    };

    /**
     * @brief Default constructor
     *
     * This constructor creates an invalid shader.
     */
    Shader();

    /**
     * @brief Destructor
     */
    ~Shader();

    /**
     * @brief Deleted copy constructor
     */
    Shader(const Shader&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    Shader& operator=(const Shader&) = delete;

    /**
     * @name Loading
     * @{
     */

    /**
     * @brief Load the vertex of fragment shader from a file
     *
     * This function loads a single shader, vertex or
     * fragment, identified by the second argument.
     *
     * The source must be a text file containing a valid
     * shader in GLSL language. GLSL is a C-like language
     * dedicated to OpenGL shaders; you'll probably need to
     * read a good documentation for it before writing your
     * own shaders.
     *
     * @param filename Path of the vertex or fragment shader file to load
     * @param type Type of shader (vertex or fragment)
     *
     * @return True if loading succeeded, false if it failed
     *
     * @sa loadFromMemory(), loadFromStream()
     */
    bool loadFromFile(const Path& filename, Type type);

    /**
     * @brief Load both the vertex and fragment shaders from files
     *
     * This function loads both the vertex and the fragment
     * shaders. If one of them fails to load, the shader is left
     * empty (the valid shader is unloaded).
     *
     * The sources must be a text files containing a valid
     * shader in GLSL language. GLSL is a C-like language
     * dedicated to OpenGL shaders; you'll probably need to
     * read a good documentation for it before writing your
     * own shaders.
     *
     * @param vertexShaderFilename   Path of the vertex shader file to load
     * @param fragmentShaderFilename Path of the fragment shader file to load
     *
     * @return True if loading succeeded, false if it failed
     *
     * @sa loadFromMemory(), loadFromStream()
     */
    bool loadFromFile(const Path& vertexShaderFilename, const Path& fragmentShaderFilename);

    /**
     * @brief Load the vertex or fragment shader from a source code in memory
     *
     * This function loads a single shader, vertex
     * or fragment, identified by the second argument.
     *
     * The source code must be a valid shader in GLSL language.
     * GLSL is a C-like language dedicated to OpenGL shaders;
     * you'll probably need to read a good documentation for
     * it before writing your own shaders.
     *
     * @param shader String containing the source code of the shader
     * @param type   Type of shader (vertex or fragment)
     *
     * @return True if loading succeeded, false if it failed
     *
     * @sa loadFromFile(), loadFromStream()
     */
    bool loadFromMemory(StringRef shader, Type type);

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
    *
    * @return True if loading succeeded, false if it failed
    *
    * @sa loadFromFile(), loadFromStream()
    */
    bool loadFromMemory(StringRef vertexShader, StringRef fragmentShader);


    /**
     * @brief Load the vertex or fragment shader from a custom stream
     *
     * This function loads a single shader, vertex
     * or fragment, identified by the second argument.
     *
     * The source code must be a valid shader in GLSL language.
     * GLSL is a C-like language dedicated to OpenGL shaders;
     * you'll probably need to read a good documentation for it
     * before writing your own shaders.
     *
     * @param stream Source stream to read from
     * @param type Type of shader (vertex, geometry or fragment)
     *
     * @return True if loading succeeded, false if it failed
     *
     * @sa loadFromFile(), loadFromMemory()
     */
    bool loadFromStream(InputStream& stream, Type type);

    /**
     * @brief Load both the vertex and fragment shaders from custom streams
     *
     * This function loads both the vertex and the fragment
     * shaders. If one of them fails to load, the shader is left
     * empty (the valid shader is unloaded).
     *
     * The source codes must be valid shaders in GLSL language.
     * GLSL is a C-like language dedicated to OpenGL shaders;
     * you'll probably need to read a good documentation for
     * it before writing your own shaders.
     *
     * @param vertexShaderStream Source stream to read the vertex shader from
     * @param fragmentShaderStream Source stream to read the fragment shader from
     *
     * @return True if loading succeeded, false if it failed
     *
     * @sa loadFromFile(), loadFromMemory()
     */
    bool loadFromStream(InputStream& vertexShaderStream, InputStream& fragmentShaderStream);

    /** @} */

    /**
     * @name Uniform setting
     * @{
     */

    /**
     * @brief Specify value for a `float` uniform
     *
     * @param name Name of the uniform variable in GLSL
     * @param val Value of the `float` scalar
     */
    void setUniform(StringRef name, float val);

    /**
     * @brief Specify value for a `int` uniform
     *
     * @param name Name of the uniform variable in GLSL
     * @param val Value of the `int` scalar
     */
    void setUniform(StringRef name, int val);

    /**
     * @brief Specify value for a `vec2` uniform
     *
     * @param name Name of the uniform variable in GLSL
     * @param vec Value of the `vec2` vector
     */
    void setUniform(StringRef name, const Vector2f& vec);

    /**
     * @brief Specify value for a `vec3` uniform
     *
     * @param name Name of the uniform variable in GLSL
     * @param vec Value of the `vec3` vector
     */
    void setUniform(StringRef name, const Vector3f& vec);

    /**
     * @brief Specify value for a `vec4` uniform
     *
     * @param name Name of the uniform variable in GLSL
     * @param vec Value of the `vec4` vector
     */
    void setUniform(StringRef name, const Vector4f& vec);

    /**
     * @brief Specify value for a `mat3` uniform
     *
     * @param name Name of the uniform variable in GLSL
     * @param mat Value of the `mat3` matrix
     */
    void setUniform(StringRef name, const Matrix3f& mat);

    /**
     * @brief Specify value for a `mat4` uniform
     *
     * @param name Name of the uniform variable in GLSL
     * @param mat Value of the `mat4` matrix
     */
    void setUniform(StringRef name, const Matrix4f& mat);

    /**
     * @brief Specify a texture for a `sampler2D` uniform
     *
     * The corresponding parameter in the shader must be a 2D texture,
     * i.e. a `sampler2D` GLSL type.
     *
     * In the shader code:
     *
     * ~~~
     * uniform sampler2D the_texture; // this is the variable in the shader
     * ~~~
     *
     * In the source code:
     *
     * ~~~{.cc}
     * gf::Texture texture;
     *
     * shader.setUniform("the_texture", texture);
     * ~~~
     *
     * It is important to note that the texture must remain alive as long
     * as the shader uses it, no copy is made internally.
     *
     * @param name Name of the uniform variable in GLSL
     * @param tex Value of the `sampler2D` texture
     */
    void setUniform(StringRef name, const BareTexture& tex);

    /** @} */

    /**
     * @brief Bind a shader for rendering
     *
     * This function is for internal use only.
     *
     * @param shader Shader to bind, can be null to use no shader
     */

    static void bind(const Shader *shader);

  private:
    friend class RenderTarget;
    bool compile(const char *vertexShaderCode, const char *fragmentShaderCode);
    int getUniformLocation(StringRef name);
    int getAttributeLocation(StringRef name);

    struct Guard;

  private:
    unsigned m_program;

    std::map<int, const BareTexture *> m_textures;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SHADER_H
