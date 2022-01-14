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
#ifndef GF_IMAGE_H
#define GF_IMAGE_H

#include <cstddef>
#include <cstdint>
#include <vector>

#include "CoreApi.h"
#include "Path.h"
#include "Span.h"
#include "Vector.h"
#include "Rect.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class InputStream;

  /**
   * @ingroup core_color
   * @brief Pixel format
   */
  enum PixelFormat {
    Rgba32, ///< Four 8-bit channels
    Rgb24,  ///< Three 8-bit channels
  };

  /**
   * @ingroup core_color
   * @brief Class for loading, manipulating and saving images
   *
   * gf::Image is an abstraction to manipulate images
   * as bidimensional arrays of pixels. The class provides
   * functions to load, read, write and save pixels, as well
   * as many other useful functions.
   *
   * gf::Image can handle a unique internal representation of
   * pixels, which is RGBA 32 bits. This means that a pixel
   * must be composed of 8 bits red, green, blue and alpha
   * channels -- just like a gf::Color4u.
   *
   * All the functions that return an array of pixels follow
   * this rule, and all parameters that you pass to gf::Image
   * constructors must use this representation as well.
   *
   * A gf::Image can be copied, but it is a heavy resource and
   * if possible you should always use (const) references to
   * pass or return them to avoid useless copies.
   *
   * Usage example:
   *
   * @snippet snippets/doc_class_image.cc image
   *
   * @sa gf::Texture
   */
  class GF_CORE_API Image {
  public:
    /**
     * @brief Default constructor
     *
     * Creates an empty image.
     */
    Image();

    /**
     * @brief Create the image
     *
     * The initial color of the pixels is opaque white
     *
     * @param size Size of the image
     */
    Image(Vector2i size);

    /**
     * @brief Create the image and fill it with a unique color
     *
     * @param size Size of the image
     * @param color Fill color
     */
    Image(Vector2i size, Color4u color);

    /**
     * @brief Create the image and fill it with a unique color
     *
     * @param size Size of the image
     * @param color Fill color
     */
    Image(Vector2i size, Color3u color);

    /**
     * @brief Create the image from an array of pixels
     *
     * The @a pixels array is assumed to contain pixels with format given by
     * @a format pixel format, and have the given @a size. If not, this is an
     * undefined behavior.
     *
     * @param size Size of the image
     * @param pixels Array of pixels to copy to the image
     * @param format Format of the pixels
     */
    Image(Vector2i size, const uint8_t *pixels, PixelFormat format = PixelFormat::Rgba32);

    /**
     * @brief Load the image from a file on disk
     *
     * The supported image formats are bmp, png, tga, jpg, gif,
     * psd, hdr and pic. Some format options are not supported,
     * like progressive jpeg.
     * If this function fails, the image is left unchanged.
     *
     * @param filename Path of the image file to load
     */
    Image(const Path& filename);

    /**
     * @brief Load the image from a file in memory
     *
     * The supported image formats are bmp, png, tga, jpg, gif,
     * psd, hdr and pic. Some format options are not supported,
     * like progressive jpeg.
     * If this function fails, the image is left unchanged.
     *
     * @param content Content of the file data in memory
     */
    Image(Span<const uint8_t> content);

    /**
     * @brief Load the image from a custom stream
     *
     * The supported image formats are bmp, png, tga, jpg, gif,
     * psd, hdr and pic. Some format options are not supported,
     * like progressive jpeg.
     * If this function fails, the image is left unchanged.
     *
     * @param stream Source stream to read from
    */
    Image(InputStream& stream);

    /**
     * @brief Default copy constructor
     */
    Image(const Image&) = default;

    /**
     * @brief Default copy assignment
     */
    Image& operator=(const Image&) = default;

    /**
     * @brief Default move constructor
     */
    Image(Image&&) = default;

    /**
     * @brief Default move assignment
     */
    Image& operator=(Image&&) = default;

    /**
     * @brief Save the image to a file on disk
     *
     * The format of the image is automatically deduced from
     * the extension. The supported image formats are bmp, png
     * and tga. The destination file is overwritten if it already
     * exists. This function fails if the image is empty.
     *
     * @param filename Path of the file to save
     *
     * @return true if saving was successful
     */
    bool saveToFile(const Path& filename) const;

    /**
     * @brief Create a sub-image of the image from a defined area
     *
     * If the area doesn't contain the image, an empty image is returned.
     * If the area is bigger than the size of the image, the image itself is returned.
     *
     * @param area Sub-area of the image
     *
     * @return The sub-image of the image
     */
    Image subImage(const RectI& area) const;

    /**
     * @brief Return the size (width and height) of the image
     *
     * @return Size of the image, in pixels
     */
    Vector2i getSize() const;

    /**
     * @brief Create a transparency mask from a specified color-key
     *
     * This function sets the alpha value of every pixel matching
     * the given color to @a alpha (0 by default), so that they
     * become transparent.
     *
     * @param color %Color to make transparent
     * @param alpha Alpha value to assign to transparent pixels
     */
    void createMaskFromColor(const Color4u& color, uint8_t alpha = 0);

    /**
     * @brief Change the color of a pixel
     *
     * This function doesn't check the validity of the pixel
     * coordinates, using out-of-range values will result in
     * an undefined behavior.
     *
     * @param pos   Coordinate of pixel to change
     * @param color New color of the pixel
     *
     * @sa getPixel
     */
    void setPixel(Vector2i pos, const Color4u& color);

    /**
     * @brief Get the color of a pixel
     *
     * This function doesn't check the validity of the pixel
     * coordinates, using out-of-range values will result in
     * an undefined behavior.
     *
     * @param pos Coordinate of pixel to get
     *
     * @return %Color of the pixel at coordinates (x, y)
     *
     * @sa setPixel
     */
    Color4u getPixel(Vector2i pos) const;

    /**
     * @brief Get a read-only pointer to the array of pixels
     *
     * The returned value points to an array of RGBA pixels made of
     * 8 bits integers components. The size of the array is
     * width * height * 4.
     * Warning: the returned pointer may become invalid if you
     * modify the image, so you should never store it for too long.
     * If the image is empty, a null pointer is returned.
     *
     * @return Read-only pointer to the array of pixels
     */
    const uint8_t* getPixelsPtr() const;

    /**
     * @brief Flip the pixels horizontally
     *
     * This function is needed internally. But you can use it if you want.
     */
    void flipHorizontally();

  private:
    Vector2i m_size;
    std::vector<uint8_t> m_pixels;

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_IMAGE_H
