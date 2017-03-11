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
#include <gf/Image.h>

#include <cassert>
#include <algorithm>
#include <locale>
#include <string>

#include <gf/Log.h>
#include <gf/InputStream.h>

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "vendor/stb/stb_image_write.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  static void lower(std::string& str) {
    for (char& c : str) {
      c = std::tolower(c, std::locale::classic());
    }
  }

  static int callbackRead(void *user,char *data,int size) {
    InputStream *stream = static_cast<InputStream *>(user);
    return static_cast<int>(stream->read(data, size));
  }

  static void callbackSkip(void *user, int n) {
    InputStream *stream = static_cast<InputStream *>(user);
    stream->seek(stream->tell() + n);
  }

  static int callbackEof(void *user) {
    InputStream *stream = static_cast<InputStream *>(user);
    return stream->tell() >= static_cast<long>(stream->getSize());
  }

  Image::Image()
  : m_size{0, 0}
  {

  }

  void Image::create(Vector2u size, const Color4u& color) {
    if (size.width == 0 || size.height == 0) {
      m_size = { 0, 0 };
      m_pixels.clear();
      return;
    }

    m_size = size;
    m_pixels.resize(4 * size.width * size.height);

    uint8_t *ptr = m_pixels.data();

    for (unsigned y = 0; y < size.height; ++y) {
      for (unsigned x = 0; x < size.width; ++x) {
        std::copy_n(color.data, 4, ptr);
        ptr += 4;
      }
    }
  }

  void Image::create(Vector2u size, const uint8_t* pixels) {
    if (size.width == 0 || size.height == 0) {
      m_size = { 0, 0 };
      m_pixels.clear();
      return;
    }

    m_size = size;
    m_pixels.resize(4 * size.width * size.height);

    std::copy_n(pixels, 4 * size.width * size.height, m_pixels.data());
  }

  void Image::createRGB(Vector2u size, const uint8_t* pixels) {
    if (size.width == 0 || size.height == 0) {
      m_size = { 0, 0 };
      m_pixels.clear();
      return;
    }

    m_size = size;
    m_pixels.resize(4 * size.width * size.height);

    uint8_t *ptr = m_pixels.data();

    for (unsigned y = 0; y < size.height; ++y) {
      for (unsigned x = 0; x < size.width; ++x) {
        std::copy_n(pixels, 3, ptr);
        ptr[3] = 0xFF; // set alpha to max (opaque)

        ptr += 4;
        pixels += 4;
      }
    }
  }

  bool Image::loadFromFile(const Path& filename) {
    int width = 0;
    int height = 0;
    int n = 0;

    uint8_t *ptr = stbi_load(filename.string().c_str(), &width, &height, &n, STBI_rgb_alpha);

    if (width == 0 || height == 0 || ptr == nullptr) {
      Log::warning("Could not load image from file '%s': %s\n", filename.c_str(), stbi_failure_reason());
      return false;
    }

    Vector2u size;
    size.width = width;
    size.height = height;

    create(size, ptr);
    stbi_image_free(ptr);

    return true;
  }

  bool Image::loadFromMemory(const uint8_t* data, std::size_t length) {
    int width = 0;
    int height = 0;
    int n = 0;

    uint8_t *ptr = stbi_load_from_memory(data, length, &width, &height, &n, STBI_rgb_alpha);

    if (width == 0 || height == 0 || ptr == nullptr) {
      Log::warning("Could not load image from memory: %s\n", stbi_failure_reason());
      return false;
    }

    Vector2u size;
    size.width = width;
    size.height = height;

    create(size, ptr);
    stbi_image_free(ptr);

    return true;
  }

  bool Image::loadFromStream(InputStream& stream) {
    stbi_io_callbacks callbacks;
    callbacks.read = &callbackRead;
    callbacks.skip = &callbackSkip;
    callbacks.eof  = &callbackEof;

    int width = 0;
    int height = 0;
    int n = 0;

    uint8_t *ptr = stbi_load_from_callbacks(&callbacks, &stream, &width, &height, &n, STBI_rgb_alpha);

    if (width == 0 || height == 0 || ptr == nullptr) {
      Log::warning("Could not load image from stream: %s\n", stbi_failure_reason());
      return false;
    }

    Vector2u size;
    size.width = width;
    size.height = height;

    create(size, ptr);
    stbi_image_free(ptr);

    return true;
  }

  bool Image::saveToFile (const Path& filename) const {
    std::string filenameString = filename.string();

    if (m_size.width == 0 || m_size.height == 0 || m_pixels.empty()) {
      Log::warning("Could not save image to file: '%s'\n", filenameString.c_str());
      return false;
    }

    auto len = filenameString.length();
    std::string ext = len >= 4 ? filenameString.substr(len - 4) : "";
    lower(ext);

    if (ext == ".png") {
      stbi_write_png(filenameString.c_str(), m_size.x, m_size.y, 4, m_pixels.data(), 0);
      return true;
    }

    if (ext == ".bmp") {
      stbi_write_bmp(filenameString.c_str(), m_size.x, m_size.y, 4, m_pixels.data());
      return true;
    }

    if (ext == ".tga") {
      stbi_write_tga(filenameString.c_str(), m_size.x, m_size.y, 4, m_pixels.data());
      return true;
    }

    Log::warning("Format not supported: '%s'\n", ext.c_str());
    return false;
  }

  Vector2u Image::getSize() const {
    return m_size;
  }

  void Image::createMaskFromColor(const Color4u& color, uint8_t alpha) {
    if (m_size.width == 0 || m_size.height == 0 || m_pixels.empty()) {
      return;
    }

    uint8_t *ptr = m_pixels.data();

    for (unsigned y = 0; y < m_size.height; ++y) {
      for (unsigned x = 0; x < m_size.width; ++x) {
        if (ptr[0] == color.r && ptr[1] == color.g && ptr[2] == color.b && ptr[3] == color.a) {
          ptr[3] = alpha;
        }

        ptr += 4;
      }
    }
  }

  void Image::setPixel(Vector2u pos, const Color4u& color) {
    if (pos.x >= m_size.width || pos.y >= m_size.height) {
      return;
    }

    uint8_t *ptr = m_pixels.data() + (pos.x + pos.y * m_size.width) * 4;
    ptr[0] = color.r;
    ptr[1] = color.g;
    ptr[2] = color.b;
    ptr[3] = color.a;
  }

  Color4u Image::getPixel(Vector2u pos) const {
    if (pos.x >= m_size.width || pos.y >= m_size.height) {
      return Color4u{0x00, 0x00, 0x00, 0x00};
    }

    const uint8_t *ptr = m_pixels.data() + (pos.x + pos.y * m_size.width) * 4;
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

    unsigned stride = m_size.width * 4;

    uint8_t *src = &m_pixels[0];
    uint8_t *dst = src + (m_size.height - 1) * stride;

    for (unsigned i = 0; i < m_size.height / 2; ++i) {
      std::swap_ranges(src, src + stride, dst);
      src += stride;
      dst -= stride;
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
