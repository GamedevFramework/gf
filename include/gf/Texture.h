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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#ifndef GF_TEXTURE_H
#define GF_TEXTURE_H

#include <cstdint>

#include "GraphicsApi.h"
#include "GraphicsHandle.h"
#include "Path.h"
#include "Rect.h"
#include "Span.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Image;
  class InputStream;

  /**
   * @ingroup graphics_gpu
   * @brief Trait for texture
   */
  template<>
  struct GF_GRAPHICS_API GraphicsTrait<GraphicsTag::Texture> {
    static void gen(int n, unsigned* resources);
    static void del(int n, const unsigned* resources);
  };

  /**
   * @ingroup graphics_gpu
   * @brief An image that lives in the graphic memory that can be used for drawing
   *
   * gf::BareTexture stores pixels that can be drawn, with a sprite
   * for example. A texture lives in the graphics card memory,
   * therefore it is very fast to draw a texture to a render target,
   * or copy a render target to a texture (the graphics card can
   * access both directly).
   *
   * Being stored in the graphics card memory has some drawbacks.
   * A texture cannot be manipulated as freely as a gf::Image,
   * you need to prepare the pixels first and then upload them
   * to the texture in a single operation (see BareTexture::update()).
   *
   * gf::BareTexture can handle two types of texture:
   *
   * - colored texture that stores RGBA channels (see gf::Texture)
   * - alpha texture that stores a single alpha channel (see gf::AlphaTexture)
   *
   * Generally, you do not manipulate a gf::BareTexture directly but you can
   * use a gf::Texture.
   *
   * @sa gf::Texture, gf::AlphaTexture
   */
  class GF_GRAPHICS_API BareTexture {
  public:
    /**
     * @brief Format of the texture
     *
     * @sa gf::BareTexture
     */
    enum class Format {
      Color, ///< RGBA format
      Alpha, ///< Alpha format
    };

    /**
     * @brief Constructor

     * Once set, the format can not be changed.
     *
     * @param format Format of the texture
     */
    BareTexture(Format format);

    /**
     * @brief Create the texture
     *
     * @param format Format of the texture
     * @param size Size of the texture
     * @param data Initial pixels of the texture (can be `nullptr`)
     */
    BareTexture(Format format, Vector2i size, const uint8_t *data);

    /**
     * @brief Get the format of the texture
     *
     * @return The format of the texture
     */
    Format getFormat() const {
      return m_format;
    }

    /**
     * @brief Get the internal representation of the texture
     *
     * This function is for internal use only.
     *
     * @return The OpenGL name of the texture
     */
    unsigned getName() const {
      return m_handle.getName();
    }

    /**
     * @brief Return the size of the texture
     *
     * @return Size in pixels
     */
    Vector2i getSize() const {
      return m_size;
    }

    /**
     * @brief Enable or disable the smooth filter
     *
     * When the filter is activated, the texture appears smoother
     * so that pixels are less noticeable. However if you want
     * the texture to look exactly the same as its source file,
     * you should leave it disabled.
     *
     * The smooth filter is disabled by default.
     *
     * @param smooth True to enable smoothing, false to disable it
     *
     * @see isSmooth()
     */
    void setSmooth(bool smooth = true);

    /**
     * @brief Check if the smooth filter is enabled or not
     *
     * @return True if smoothing is enabled, false if it is disabled
     *
     * @see setSmooth()
     */
    bool isSmooth() const noexcept {
      return m_smooth;
    }

    /**
     * @brief Enable or disable repeating
     *
     * Repeating is involved when using texture coordinates
     * outside the texture rectangle @f$ [0, 1] \times [0, 1] @f$.
     * In this case, if repeat mode is enabled, the whole texture
     * will be repeated as many times as needed to reach the
     * coordinate (for example, if the @f$ u @f$ texture coordinate is
     * 3, the texture will be repeated 3 times).
     *
     * If repeat mode is disabled, the "extra space" will instead
     * be filled with border pixels.
     *
     * Repeating is disabled by default.
     *
     * @param repeated True to repeat the texture, false to disable repeating
     *
     * @see isRepeated()
     */
    void setRepeated(bool repeated = true);

    /**
     * @brief Check if the texture is repeated or not
     *
     * @return True if repeat mode is enabled, false if it is disabled
     *
     * @see setRepeated()
     */
    bool isRepeated() const noexcept {
      return m_repeated;
    }

    /**
     * @brief Update the whole texture from an array of pixels
     *
     * The `data` array is assumed to be in the right format (4 channels
     * for colored texture and 1 channel for alpha texture) and have the
     * right size.
     *
     * No additional check is performed on the size of the pixel
     * array, passing invalid arguments will lead to an undefined
     * behavior.
     *
     * This function does nothing if `data` is `nullptr` or if the
     * texture was not previously created.
     *
     * @param data An array of pixels to copy to the texture
     */
    void update(const uint8_t *data);

    /**
     * @brief Update a part of the texture from an array of pixels
     *
     * The `data` array is assumed to be in the right format (4 channels
     * for colored texture and 1 channel for alpha texture) and its size
     * must match the size of the `rect` argument.
     *
     * No additional check is performed on the size of the pixel
     * array or the bounds of the area to update, passing invalid
     * arguments will lead to an undefined behavior.
     *
     * This function does nothing if `data` is `nullptr` or if the
     * texture was not previously created.
     *
     * @param data An array of pixels to copy to the texture
     * @param rect The region of the texture to update
     */
    void update(const uint8_t *data, const RectI& rect);

    /**
     * @brief Resize a texture
     *
     * @param size The new size of the texture
     * @param data An array of pixels (may be `nullptr`)
     */
    void resize(Vector2i size, const uint8_t *data = nullptr);

    /**
     * @brief Compute normalized texture coordinates
     *
     * @param rect The rectangle in the texture, in pixels
     * @return Normalized texture coordinates
     */
    RectF computeTextureCoords(const RectI& rect) const;

    /**
     * @brief Generate a mipmap using the current texture data
     *
     * Mipmaps are pre-computed chains of optimized textures. Each
     * level of texture in a mipmap is generated by halving each of
     * the previous level's dimensions. This is done until the final
     * level has the size of 1x1. The textures generated in this process may
     * make use of more advanced filters which might improve the visual quality
     * of textures when they are applied to objects much smaller than they are.
     * This is known as minification. Because fewer texels (texture elements)
     * have to be sampled from when heavily minified, usage of mipmaps
     * can also improve rendering performance in certain scenarios.
     *
     * Mipmap data is only valid from the time it is generated until the next
     * time the base level image is modified, at which point this function
     * will have to be called again to regenerate it.
     *
     * @return True if mipmap generation was successful, false if unsuccessful
     */
    bool generateMipmap();

    /**
     * @brief Bind a texture for rendering
     *
     * This function is for internal use only.
     *
     * @param texture Pointer to the texture to bind, can be `nullptr` to use no texture
     */
    static void bind(const BareTexture *texture);

  private:
    Format m_format;
    GraphicsHandle<GraphicsTag::Texture> m_handle;
    Vector2i m_size;
    bool m_smooth;
    bool m_repeated;
    bool m_mipmap;
  };


  /**
   * @ingroup graphics_gpu
   * @brief A texture for colored images
   *
   * A texture can be loaded from an image, but also directly
   * from a file/memory/stream. The necessary shortcuts are defined
   * so that you don't need an image first for the most common cases.
   * However, if you want to perform some modifications on the pixels
   * before creating the final texture, you can load your file to a
   * gf::Image, do whatever you need with the pixels, and then call
   * the appropriate constructor.
   *
   * Like gf::Image, gf::Texture can handle a unique internal
   * representation of pixels, which is RGBA. This means
   * that a pixel must be composed of 8 bits red, green, blue and
   * alpha channels.
   *
   * Usage example:
   *
   * @snippet snippets/doc_class_texture.cc texture
   *
   * @sa gf::Sprite, gf::Image, gf::RenderTexture
   */
  class GF_GRAPHICS_API Texture : public BareTexture {
  public:
    /**
     * @brief Constructor
     *
     * No texture is created.
     */
    Texture();

    /**
     * @brief Create the texture
     *
     * @param size Size of the texture
     */
    Texture(Vector2i size);

    /**
     * @brief Load the texture from an image
     *
     * @param image Image to load into the texture
     */
    Texture(const Image& image);

    /**
     * @brief Load the texture from a sub-area of an image
     *
     * @param image Image to load into the texture
     * @param area Sub-area of the image
     */
    Texture(const Image& image, const RectI& area);

    /**
     * @brief Load the texture from a file on disk
     *
     * @param filename Path of the image file to load
     */
    Texture(const Path& filename);

    /**
     * @brief Load the texture from a sub-area of a file on disk
     *
     * @param filename Path of the image file to load
     * @param area Sub-area of the image
     */
    Texture(const Path& filename, const RectI& area);

    /**
     * @brief Load the texture from a custom stream
     *
     * @param stream Source stream to read from
     */
    Texture(InputStream& stream);

    /**
     * @brief Load the texture from a sub-area of a custom stream
     *
     * @param stream Source stream to read from
     * @param area Sub-area of the image
     */
    Texture(InputStream& stream, const RectI& area);

    /**
     * @brief Load the texture from a file in memory
     *
     * @param content Content of the file data in memory
     */
    Texture(Span<const uint8_t> content);

    /**
     * @brief Load the texture from a sub-area of a file in memory
     *
     * @param content Content of the file data in memory
     * @param area Sub-area of the image
     */
    Texture(Span<const uint8_t> content, const RectI& area);

    /**
     * @brief Update the texture from an image
     *
     * Although the source image can be smaller than the texture,
     * this function is usually used for updating the whole texture.
     *
     * No additional check is performed on the size of the image,
     * passing an image bigger than the texture will lead to an
     * undefined behavior.
     *
     * This function does nothing if the texture was not
     * previously created.
     *
     * @param image Image to copy to the texture
     */
    void update(const Image& image);

    /**
     * @brief Copy the texture pixels to an image
     *
     * This function performs a slow operation that downloads
     * the texture's pixels from the graphics card and copies
     * them to a new image, potentially applying transformations
     * to pixels if necessary (texture may be padded or flipped).
     *
     * @return An image containing the texture's pixels
     */
    Image copyToImage() const;

  };


  /**
   * @ingroup graphics_gpu
   * @brief A texture with a single alpha channel
   *
   * This texture is used internally by gf::Font
   */
  class GF_GRAPHICS_API AlphaTexture : public BareTexture {
  public:
    /**
     * @brief Constructor
     *
     * No texture is created
     */
    AlphaTexture();

    /**
     * @brief Create the texture
     *
     * @param size Size of the texture
     */
    AlphaTexture(Vector2i size);
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TEXTURE_H
