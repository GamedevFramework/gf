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
#include <gf/Image.h>

#include <cassert>
#include <climits>
#include <algorithm>
#include <locale>
#include <string>

#include <gf/Log.h>
#include <gf/Stream.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    std::size_t computeImageSize(Vector2i size) {
      return 4 * static_cast<std::size_t>(size.width) * static_cast<std::size_t>(size.height);
    }

    void lower(std::string& str) {
      for (char& c : str) {
        c = std::tolower(c, std::locale::classic());
      }
    }

    int callbackRead(void *user,char *data,int size) {
      InputStream *stream = static_cast<InputStream *>(user);
      return static_cast<int>(stream->read(Span<uint8_t>(reinterpret_cast<uint8_t*>(data), size)));
    }

    void callbackSkip(void *user, int n) {
      InputStream *stream = static_cast<InputStream *>(user);
      stream->skip(n);
    }

    int callbackEof(void *user) {
      InputStream *stream = static_cast<InputStream *>(user);
      return static_cast<int>(stream->isFinished());
    }

  } // anonymous namespace

  Image::Image()
  : m_size{0, 0}
  {
  }

  Image::Image(Vector2i size)
  : m_size(size)
  , m_pixels(computeImageSize(size), 0xFF)
  {
  }

  Image::Image(Vector2i size, Color4u color)
  : m_size(size)
  , m_pixels(computeImageSize(size))
  {
    uint8_t *ptr = m_pixels.data();

    for (int y = 0; y < m_size.height; ++y) {
      for (int x = 0; x < m_size.width; ++x) {
        ptr[0] = color.r;
        ptr[1] = color.g;
        ptr[2] = color.b;
        ptr[3] = color.a;
        ptr += 4;
      }
    }
  }

  Image::Image(Vector2i size, Color3u color)
  : m_size(size)
  , m_pixels(computeImageSize(size))
  {
    uint8_t *ptr = m_pixels.data();

    for (int y = 0; y < m_size.height; ++y) {
      for (int x = 0; x < m_size.width; ++x) {
        ptr[0] = color.r;
        ptr[1] = color.g;
        ptr[2] = color.b;
        ptr[3] = 0xFF; // set alpha to max (opaque)
        ptr += 4;
      }
    }
  }

  Image::Image(Vector2i size, const uint8_t *pixels, PixelFormat format)
  : m_size(size)
  , m_pixels(computeImageSize(size))
  {
    assert(pixels != nullptr);

    switch (format) {
      case PixelFormat::Rgb24: {
        uint8_t *ptr = m_pixels.data();

        for (int y = 0; y < size.height; ++y) {
          for (int x = 0; x < size.width; ++x) {
            ptr[0] = pixels[0];
            ptr[1] = pixels[1];
            ptr[2] = pixels[2];
            ptr[3] = 0xFF; // set alpha to max (opaque)

            ptr += 4;
            pixels += 3;
          }
        }

        break;
      }

      case PixelFormat::Rgba32:
        std::copy_n(pixels, m_pixels.size(), m_pixels.data());
        break;
    }

    flipHorizontally();
  }

  Image::Image(const Path& filename)
  : m_size({ 0, 0 })
  {
    int n = 0;
    uint8_t *pixels = stbi_load(filename.string().c_str(), &m_size.width, &m_size.height, &n, STBI_rgb_alpha);

    if (m_size.width == 0 || m_size.height == 0 || pixels == nullptr) {
      Log::warning("Could not load image from file '%s': %s\n", filename.c_str(), stbi_failure_reason());
      throw std::runtime_error("Could not load image from file");
    }

    m_pixels.resize(computeImageSize(m_size));
    std::copy_n(pixels, m_pixels.size(), m_pixels.data());
    stbi_image_free(pixels);

    flipHorizontally();
  }

  Image::Image(Span<const uint8_t> content)
  : m_size({ 0, 0 })
  {
    int n = 0;
    assert(content.getSize() < INT_MAX);
    uint8_t *pixels = stbi_load_from_memory(content.getData(), static_cast<int>(content.getSize()), &m_size.width, &m_size.height, &n, STBI_rgb_alpha);

    if (m_size.width == 0 || m_size.height == 0 || pixels == nullptr) {
      Log::warning("Could not load image from memory: %s\n", stbi_failure_reason());
      throw std::runtime_error("Could not load image from memory");
    }

    m_pixels.resize(computeImageSize(m_size));
    std::copy_n(pixels, m_pixels.size(), m_pixels.data());
    stbi_image_free(pixels);

    flipHorizontally();
  }

  Image::Image(InputStream& stream)
  : m_size({ 0, 0 })
  {
    stbi_io_callbacks callbacks;
    callbacks.read = &callbackRead;
    callbacks.skip = &callbackSkip;
    callbacks.eof  = &callbackEof;

    int n = 0;
    uint8_t *pixels = stbi_load_from_callbacks(&callbacks, &stream, &m_size.width, &m_size.height, &n, STBI_rgb_alpha);

    if (m_size.width == 0 || m_size.height == 0 || pixels == nullptr) {
      Log::warning("Could not load image from stream: %s\n", stbi_failure_reason());
      throw std::runtime_error("Could not load image from stream");
    }

    m_pixels.resize(computeImageSize(m_size));
    std::copy_n(pixels, m_pixels.size(), m_pixels.data());
    stbi_image_free(pixels);

    flipHorizontally();
  }

  bool Image::saveToFile (const Path& filename) const {
    std::string filenameString = filename.string();

    if (m_size.width == 0 || m_size.height == 0 || m_pixels.empty()) {
      Log::warning("Could not save image to file: '%s'\n", filenameString.c_str());
      return false;
    }

    Image flipped(*this);
    flipped.flipHorizontally();

    auto len = filenameString.length();
    std::string ext = len >= 4 ? filenameString.substr(len - 4) : "";
    lower(ext);

    if (ext == ".png") {
      stbi_write_png(filenameString.c_str(), m_size.x, m_size.y, 4, flipped.m_pixels.data(), 0);
      return true;
    }

    if (ext == ".bmp") {
      stbi_write_bmp(filenameString.c_str(), m_size.x, m_size.y, 4, flipped.m_pixels.data());
      return true;
    }

    if (ext == ".tga") {
      stbi_write_tga(filenameString.c_str(), m_size.x, m_size.y, 4, flipped.m_pixels.data());
      return true;
    }

    Log::warning("Format not supported: '%s'\n", ext.c_str());
    return false;
  }

  Image Image::subImage(const RectI& area) const {
    RectI currArea = RectI::fromSize(m_size);
    RectI newArea;

    if (!area.intersects(currArea, newArea)) {
      return Image();
    }

    Vector2i newPos = newArea.getPosition();
    Vector2i newSize = newArea.getSize();
    Image resultImage(newSize);

    const uint8_t* pixels = m_pixels.data() + (newPos.x + (m_size.height - newPos.y - 1) * m_size.width) * 4;
    pixels -= 4 * m_size.width * (newSize.height - 1);
    uint8_t *ptr = resultImage.m_pixels.data();

    for (int y = 0; y < newSize.height; ++y) {
      std::copy_n(pixels, newSize.width * 4, ptr);
      ptr += 4 * newSize.width;
      pixels += 4 * m_size.width;
    }

    return resultImage;
  }

  Vector2i Image::getSize() const {
    return m_size;
  }

  void Image::createMaskFromColor(const Color4u& color, uint8_t alpha) {
    if (m_size.width == 0 || m_size.height == 0 || m_pixels.empty()) {
      return;
    }

    uint8_t *ptr = m_pixels.data();

    for (int y = 0; y < m_size.height; ++y) {
      for (int x = 0; x < m_size.width; ++x) {
        if (ptr[0] == color.r && ptr[1] == color.g && ptr[2] == color.b && ptr[3] == color.a) {
          ptr[3] = alpha;
        }

        ptr += 4;
      }
    }
  }

  void Image::setPixel(Vector2i pos, const Color4u& color) {
    if (pos.x < 0 || pos.x >= m_size.width || pos.y < 0 || pos.y >= m_size.height) {
      return;
    }

    uint8_t *ptr = m_pixels.data() + (pos.x + (m_size.height - pos.y - 1) * m_size.width) * 4;
    ptr[0] = color.r;
    ptr[1] = color.g;
    ptr[2] = color.b;
    ptr[3] = color.a;
  }

  Color4u Image::getPixel(Vector2i pos) const {
    if (pos.x < 0 || pos.x >= m_size.width || pos.y < 0 || pos.y >= m_size.height) {
      return Color4u{0x00, 0x00, 0x00, 0x00};
    }

    const uint8_t *ptr = m_pixels.data() + (pos.x + (m_size.height - pos.y - 1) * m_size.width) * 4;
    return Color4u{ptr[0], ptr[1], ptr[2], ptr[3]};
  }

  const uint8_t* Image::getPixelsPtr() const {
    if (m_pixels.empty()) {
      return nullptr;
    }

    return m_pixels.data();
  }

  void Image::flipHorizontally() {
    if (m_pixels.empty()) {
      return;
    }

    int stride = m_size.width * 4;

    uint8_t *src = &m_pixels[0];
    uint8_t *dst = src + (m_size.height - 1) * stride;

    for (int i = 0; i < m_size.height / 2; ++i) {
      std::swap_ranges(src, src + stride, dst);
      src += stride;
      dst -= stride;
    }
  }


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
