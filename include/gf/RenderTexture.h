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
#ifndef GF_RENDER_TEXTURE_H
#define GF_RENDER_TEXTURE_H

#include "Portability.h"
#include "RenderTarget.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief Target for off-screen 2D rendering into a texture
   *
   * gf::RenderTexture is the little brother of gf::RenderWindow.
   * It implements the same 2D drawing and OpenGL-related functions
   * (see their base class gf::RenderTarget for more details),
   * the difference is that the result is stored in an off-screen
   * texture rather than being show in a window.
   *
   * Rendering to a texture can be useful in a variety of situations:
   *
   * - precomputing a complex static texture (like a level's background
   *   from multiple tiles)
   * - applying post-effects to the whole scene with shaders (See
   *   gf::PostProcessing)
   * - creating a sprite from a 3D object rendered with OpenGL
   * - etc.
   *
   * Usage example:
   *
   * ~~~{.cc}
   * // Create a new render-window
   * gf::RenderWindow renderer(window);
   *
   * // Create a new render-texture
   * gf::RenderTexture textureRenderer;
   *
   * if (!textureRenderer.create(500, 500)) {
   *   return -1;
   * }
   *
   * // The main loop
   * while (window.isOpen()) {
   *   // Event processing
   *   // ...
   *
   *   // Activate the texture
   *   textureRenderer.setActive();
   *
   *   // Clear the whole texture with red color
   *   textureRenderer.clear(gf::Color::Red);
   *
   *   // Draw stuff to the texture
   *   textureRenderer.draw(sprite);  // sprite is a gf::Sprite
   *   textureRenderer.draw(shape);   // shape is a gf::Shape
   *   textureRenderer.draw(text);    // text is a gf::Text
   *
   *   // We're done drawing to the texture
   *   textureRenderer.display();
   *
   *   // Now we start rendering to the window
   *   // Activate it first and then clear it
   *   renderer.setActive();
   *   renderer.clear();
   *
   *   // Draw the texture
   *   gf::Sprite sprite(textureRenderer.getTexture());
   *   renderer.draw(sprite);
   *
   *   // End the current frame and display its contents on screen
   *   renderer.display();
   * }
   * ~~~
   *
   * @sa gf::RenderTarget, gf::RenderWindow, gf::View, gf::Texture
   */
  class GF_API RenderTexture : public RenderTarget {
  public:
    /**
     * @brief Default constructor
     *
     * Constructs an empty, invalid render-texture. You must
     * call `create()` to have a valid render-texture.
     *
     * @sa create()
     */
    RenderTexture();

    /**
     * @brief Destructor
     */
    ~RenderTexture();

    /**
     * @brief Create the render-texture
     *
     * Before calling this function, the render-texture is in
     * an invalid state, thus it is mandatory to call it before
     * doing anything with the render-texture.
     *
     * @param size Size of the render-texture
     *
     * @return True if creation has been successful
     */
    bool create(Vector2u size);

    /**
     * @brief Enable or disable texture smoothing
     *
     * This function is similar to `Texture::setSmooth()`.
     * This parameter is disabled by default.
     *
     * @param smooth True to enable smoothing, false to disable it
     *
     * @sa isSmooth()
     */
    void setSmooth(bool smooth = true) {
      m_texture.setSmooth(smooth);
    }

    /**
     * @brief Check if the smooth filtering is enabled or not
     *
     * @return True if texture smoothing is enabled
     *
     * @sa setSmooth()
     */
    bool isSmooth() const {
      return m_texture.isSmooth();
    }

    /**
     * @brief Enable or disable texture repeating
     *
     * This function is similar to `Texture::setRepeated()`.
     * This parameter is disabled by default.
     *
     * @param repeated True to enable repeating, false to disable it
     *
     * @sa isRepeated()
     */
    void setRepeated(bool repeated = true) {
      m_texture.setRepeated(repeated);
    }

    /**
     * @brief Check if the texture is repeated or not
     *
     * @return True if texture is repeated
     *
     * @sa setRepeated()
     */
    bool isRepeated() const {
      return m_texture.isRepeated();
    }

    virtual Vector2u getSize() const override;

    /**
     * @brief Activate the render-texture for rendering
     *
     * This function activates the render-texture so that all draw calls
     * are targeted to the texture. You should call this function before
     * you want to draw something to the target.
     */
    void setActive();

    /**
     * @brief Update the contents of the target texture
     *
     * This function updates the target texture with what
     * has been drawn so far. Like for windows, calling this
     * function is mandatory at the end of rendering. Not calling
     * it may leave the texture in an undefined state.
     */
    void display();

    /**
     * @brief Copy the current contents of the render texture to an image
     *
     * This is a slow operation, whose main purpose is to make screenshots of
     * the application.
     */
    Image capture() const;

    /**
     * @brief Get a read-only reference to the target texture
     *
     * After drawing to the render-texture and calling `display()`,
     * you can retrieve the updated texture using this function,
     * and draw it using a sprite (for example).
     *
     * The internal sf::Texture of a render-texture is always the
     * same instance, so that it is possible to call this function
     * once and keep a reference to the texture even after it is
     * modified.
     *
     * @return Const reference to the texture
     */
    const Texture& getTexture() const {
      return m_texture;
    }

  private:
    unsigned m_name;
    Texture m_texture;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_RENDER_TEXTURE_H
