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
#include <gf/Texture.h>

#include <cassert>

#include <glad/glad.h>

#include <gf/Image.h>

#include "priv/Debug.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  BareTexture::BareTexture(Format format)
  : m_format(format)
  , m_name(0)
  , m_size{0, 0}
  , m_smooth(false)
  , m_repeated(false)
  , m_mipmap(false)
  {

  }

  BareTexture::~BareTexture() {
    if (m_name != 0) {
      GLuint name = static_cast<GLuint>(m_name);
      glCheck(glDeleteTextures(1, &name));
    }
  }

  BareTexture::BareTexture(BareTexture&& other)
  : m_format(other.m_format)
  , m_name(other.m_name)
  , m_size(other.m_size)
  , m_smooth(other.m_smooth)
  , m_repeated(other.m_repeated)
  , m_mipmap(other.m_mipmap)
  {
    other.m_name = 0;
  }

  BareTexture& BareTexture::operator=(BareTexture&& other) {
    std::swap(m_format, other.m_format);
    std::swap(m_name, other.m_name);
    std::swap(m_size, other.m_size);
    std::swap(m_smooth, other.m_smooth);
    std::swap(m_repeated, other.m_repeated);
    std::swap(m_mipmap, other.m_mipmap);
    return *this;
  }

  static GLenum getEnum(BareTexture::Format format) {
    switch (format) {
      case BareTexture::Format::Color:
        return GL_RGBA;
      case BareTexture::Format::Alpha:
        return GL_ALPHA;
    }

    assert(false);
    return 0;
  };

  static int getAlignment(BareTexture::Format format) {
    switch (format) {
      case BareTexture::Format::Color:
        return 4;
      case BareTexture::Format::Alpha:
        return 1;
    }

    assert(false);
    return 4;
  };

  static GLenum getMinFilter(bool smooth, bool mipmap) {
    if (mipmap) {
      return smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR;
    }

    return smooth ? GL_LINEAR : GL_NEAREST;
  }

  bool BareTexture::create(Vector2u size, const uint8_t *data) {
    if (size.width == 0 || size.height == 0) {
      return false;
    }

    if (m_name == 0) {
      GLuint name;
      glCheck(glGenTextures(1, &name));
      m_name = static_cast<unsigned>(name);
    }

    m_size = size;

    GLenum textureFormat = getEnum(m_format);

    glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, getAlignment(m_format)));

    glCheck(glBindTexture(GL_TEXTURE_2D, m_name));
    glCheck(glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, m_size.width, m_size.height, 0, textureFormat, GL_UNSIGNED_BYTE, data));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_smooth ? GL_LINEAR : GL_NEAREST));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getMinFilter(m_smooth, m_mipmap)));

    return true;
  }

  void BareTexture::setSmooth(bool smooth) {
    if (m_smooth == smooth) {
      return;
    }

    m_smooth = smooth;

    if (m_name == 0) {
      return;
    }

    glCheck(glBindTexture(GL_TEXTURE_2D, m_name));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_smooth ? GL_LINEAR : GL_NEAREST));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getMinFilter(m_smooth, m_mipmap)));
  }

  void BareTexture::setRepeated(bool repeated) {
    if (m_repeated == repeated) {
      return;
    }

    m_repeated = repeated;

    if (m_name == 0) {
      return;
    }

    glCheck(glBindTexture(GL_TEXTURE_2D, m_name));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
  }

  void BareTexture::update(const uint8_t *data) {
    update(data, RectU({0, 0}, m_size));
  }

  void BareTexture::update(const uint8_t *data, const RectU& rect) {
    assert(rect.left + rect.width <= m_size.width);
    assert(rect.top + rect.height <= m_size.height);

    if (m_name == 0 || data == nullptr) {
      return;
    }

    m_mipmap = false;

    glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, getAlignment(m_format)));

    glCheck(glBindTexture(GL_TEXTURE_2D, m_name));
    glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, rect.left, rect.top, rect.width, rect.height, getEnum(m_format), GL_UNSIGNED_BYTE, data));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getMinFilter(m_smooth, m_mipmap)));
  }

  RectF BareTexture::computeTextureCoords(const RectU& rect) const {
    return {
      static_cast<float>(rect.left) / m_size.width,
      static_cast<float>(rect.top) / m_size.height,
      static_cast<float>(rect.width) / m_size.width,
      static_cast<float>(rect.height) / m_size.height,
    };
  }

  bool BareTexture::generateMipmap() {
    if (m_name == 0) {
      return false;
    }

    m_mipmap = true;

    glCheck(glBindTexture(GL_TEXTURE_2D, m_name));
    glCheck(glGenerateMipmap(GL_TEXTURE_2D));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getMinFilter(m_smooth, m_mipmap)));

    return true;
  }

  void BareTexture::bind(const BareTexture *texture) {
    if (texture && texture->m_name != 0) {
      glCheck(glBindTexture(GL_TEXTURE_2D, texture->m_name));
    } else {
      glCheck(glBindTexture(GL_TEXTURE_2D, 0));
    }
  }

  Texture::Texture()
  : BareTexture(Format::Color)
  {

  }

  bool Texture::create(Vector2u size) {
    return BareTexture::create(size, nullptr);
  }

  bool Texture::loadFromImage(const Image& image) {
    return BareTexture::create(image.getSize(), image.getPixelsPtr());
  }

  bool Texture::loadFromFile(const Path& filename) {
    Image image;
    return image.loadFromFile(filename) && loadFromImage(image);
  }

  bool Texture::loadFromStream(InputStream& stream) {
    Image image;
    return image.loadFromStream(stream) && loadFromImage(image);
  }

  bool Texture::loadFromMemory(const uint8_t *data, std::size_t length) {
    Image image;
    return image.loadFromMemory(data, length) && loadFromImage(image);
  }

  void Texture::update(const Image& image) {
    BareTexture::update(image.getPixelsPtr(), RectU({ 0, 0 }, image.getSize()));
  }


  Image Texture::copyToImage() const {
    if (getName() == 0) {
      return Image();
    }

    auto size = getSize();
    std::vector<uint8_t> pixels(size.width * size.height * 4);

    GLuint frameBuffer = 0;
    glCheck(glGenFramebuffers(1, &frameBuffer));

    if (frameBuffer == 0) {
      return Image();
    }

    GLint boundFrameBuffer;
    glCheck(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFrameBuffer));

    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer));
    glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, getName(), 0));
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    glCheck(glPixelStorei(GL_PACK_ALIGNMENT, 4));
    glCheck(glReadPixels(0, 0, size.width, size.height, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));

    glCheck(glDeleteFramebuffers(1, &frameBuffer));

    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, boundFrameBuffer));

    Image image;
    image.create(size, pixels.data());
    image.flipHorizontally();
    return image;
  }



  AlphaTexture::AlphaTexture()
  : BareTexture(Format::Alpha)
  {

  }

  bool AlphaTexture::create(Vector2u size) {
    return BareTexture::create(size, nullptr);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
