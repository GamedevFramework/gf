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
#ifndef GF_IMAGE_H
#define GF_IMAGE_H

#include <cstddef>
#include <cstdint>
#include <vector>

#include "Path.h"
#include "Portability.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class InputStream;

  /**
   * @ingroup graphics
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
   * functions (such as `loadFromMemory()`) must use this
   * representation as well.
   *
   * A gf::Image can be copied, but it is a heavy resource and
   * if possible you should always use (const) references to
   * pass or return them to avoid useless copies.
   *
   * Usage example:
   *
   * ~~~{.cc}
   * // Load an image file from a file
   * gf::Image background;
   *
   * if (!background.loadFromFile("background.jpg")) {
   *   return -1;
   * }
   *
   * // Create a 20x20 image filled with black color
   * gf::Image image;
   * image.create({ 20, 20 }, sf::Color4u{0xFF, 0xFF, 0xFF, 0xFF});
   *
   * // Make the top-left pixel transparent
   * gf::Color4u color = image.getPixel({ 0, 0 });
   * color.a = 0;
   * image.setPixel({ 0, 0 }, color);
   *
   * // Save the image to a file
   * if (!image.saveToFile("result.png")) {
   *   return -1;
   * }
   * ~~~
   *
   * @sa gf::Texture
   */
  class GF_API Image {
  public:
    /**
     * @brief Default constructor
     *
     * Creates an empty image.
     */
    Image();

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
     * @brief Create the image and fill it with a unique color
     *
     * @param size  Size of the image
     * @param color Fill color
     */
    void create(Vector2u size, const Color4u& color = Color4u{0x00, 0x00, 0x00, 0xFF});

    /**
     * @brief Create the image from an array of pixels
     *
     * The @a pixel array is assumed to contain 32-bits RGBA pixels,
     * and have the given @a size. If not, this is an undefined behavior.
     * If @a pixels is null, an empty image is created.
     *
     * @param size   Size of the image
     * @param pixels Array of pixels to copy to the image
     */
    void create(Vector2u size, const uint8_t* pixels);

    /**
     * @brief Create the image from an array of pixels
     *
     * The @a pixel array is assumed to contain 24-bits RGB pixels,
     * and have the given @a size. If not, this is an undefined behavior.
     * If @a pixels is null, an empty image is created.
     *
     * @param size   Size of the image
     * @param pixels Array of pixels to copy to the image
     */
    void createRGB(Vector2u size, const uint8_t* pixels);

    /**
     * @brief Load the image from a file on disk
     *
     * The supported image formats are bmp, png, tga, jpg, gif,
     * psd, hdr and pic. Some format options are not supported,
     * like progressive jpeg.
     * If this function fails, the image is left unchanged.
     *
     * @param filename Path of the image file to load
     *
     * @return true if loading was successful
     *
     * @sa loadFromMemory(), loadFromStream(), saveToFile()
     */
    bool loadFromFile(const Path& filename);

    /**
     * @brief Load the image from a file in memory
     *
     * The supported image formats are bmp, png, tga, jpg, gif,
     * psd, hdr and pic. Some format options are not supported,
     * like progressive jpeg.
     * If this function fails, the image is left unchanged.
     *
     * @param data Pointer to the file data in memory
     * @param length Length of the data to load, in bytes
     *
     * @return true if loading was successful
     *
     * @sa loadFromFile(), loadFromStream(), saveToFile()
     */
    bool loadFromMemory(const uint8_t* data, std::size_t length);

    /**
     * @brief Load the image from a custom stream
     *
     * The supported image formats are bmp, png, tga, jpg, gif,
     * psd, hdr and pic. Some format options are not supported,
     * like progressive jpeg.
     * If this function fails, the image is left unchanged.
     *
     * @param stream Source stream to read from
     *
     * @return True if loading was successful
     *
     * @sa loadFromFile(), loadFromMemory()
    */
    bool loadFromStream(InputStream& stream);

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
     *
     * @sa create, loadFromFile(), loadFromMemory(), loadFromStream()
     */
    bool saveToFile(const Path& filename) const;

    /**
     * @brief Return the size (width and height) of the image
     *
     * @return Size of the image, in pixels
     */
    Vector2u getSize() const;

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
    void setPixel(Vector2u pos, const Color4u& color);

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
    Color4u getPixel(Vector2u pos) const;

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
    Vector2u m_size;
    std::vector<uint8_t> m_pixels;

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_IMAGE_H
