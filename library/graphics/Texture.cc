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
#include <gf/Texture.h>

#include <cassert>

#include <gf/Image.h>
#include <gf/RenderTarget.h>

#include <gfpriv/GlDebug.h>
#include <gfpriv/GlFwd.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  void GraphicsTrait<GraphicsTag::Texture>::gen(int n, unsigned* resources) {
    GL_CHECK(glGenTextures(n, resources));
  }

  void GraphicsTrait<GraphicsTag::Texture>::del(int n, const unsigned* resources) {
    GL_CHECK(glDeleteTextures(n, resources));
  }

  namespace {

    GLenum getEnum(BareTexture::Format format) {
      switch (format) {
        case BareTexture::Format::Color:
          return GL_RGBA;
        case BareTexture::Format::Alpha:
#ifdef GF_OPENGL3
          return GL_RED;
#else
          return GL_ALPHA;
#endif
      }

      assert(false);
      return 0;
    }

    int getAlignment(BareTexture::Format format) {
      switch (format) {
        case BareTexture::Format::Color:
          return 4;
        case BareTexture::Format::Alpha:
          return 1;
      }

      assert(false);
      return 4;
    }

    GLenum getMinFilter(bool smooth, bool mipmap) {
      if (mipmap) {
        return smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR;
      }

      return smooth ? GL_LINEAR : GL_NEAREST;
    }

  } // anonymous namespace

  BareTexture::BareTexture(Format format)
  : m_format(format)
  , m_handle(gf::None)
  , m_size{0, 0}
  , m_smooth(false)
  , m_repeated(false)
  , m_mipmap(false)
  {
  }

  BareTexture::BareTexture(Format format, Vector2i size, const uint8_t *data)
  : m_format(format)
  , m_size(size)
  , m_smooth(false)
  , m_repeated(false)
  , m_mipmap(false)
  {
    assert(m_size.width > 0 && m_size.height > 0);

    GLenum textureFormat = getEnum(m_format);

    GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, getAlignment(m_format)));

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, m_size.width, m_size.height, 0, textureFormat, GL_UNSIGNED_BYTE, data));

    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); // m_repeated == false
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)); // m_repeated == false
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)); // m_smooth == false
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)); // m_smooth == false && m_mipmap == false
  }

  void BareTexture::setSmooth(bool smooth) {
    if (m_smooth == smooth) {
      return;
    }

    m_smooth = smooth;

    if (!m_handle.isValid()) {
      return;
    }

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_smooth ? GL_LINEAR : GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getMinFilter(m_smooth, m_mipmap)));
  }

  void BareTexture::setRepeated(bool repeated) {
    if (m_repeated == repeated) {
      return;
    }

    m_repeated = repeated;

    if (!m_handle.isValid()) {
      return;
    }

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
  }

  void BareTexture::update(const uint8_t *data) {
    update(data, RectI::fromPositionSize({0, 0}, m_size));
  }

  void BareTexture::update(const uint8_t *data, const RectI& rect) {
    assert(0 <= rect.min.x);
    assert(0 <= rect.min.y);
    assert(rect.max.x <= m_size.width);
    assert(rect.max.y <= m_size.height);

    if (!m_handle.isValid() || data == nullptr) {
      return;
    }

    m_mipmap = false;

    GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, getAlignment(m_format)));

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
    GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, rect.min.x, rect.min.y, rect.getWidth(), rect.getHeight(), getEnum(m_format), GL_UNSIGNED_BYTE, data));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getMinFilter(m_smooth, m_mipmap)));

#ifdef GF_OPENGL3
    if (m_format == Format::Alpha) {
      GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED));
    }
#endif
  }

  void BareTexture::resize(Vector2i size, const uint8_t *data) {
    assert(size.width > 0 && size.height > 0);
    m_size = size;

    GLenum textureFormat = getEnum(m_format);

    GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, getAlignment(m_format)));

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, m_size.width, m_size.height, 0, textureFormat, GL_UNSIGNED_BYTE, data));
  }

  RectF BareTexture::computeTextureCoords(const RectI& rect) const {
    Vector2f size = m_size;
    return RectF::fromMinMax(rect.min / size, rect.max / size);
  }

  bool BareTexture::generateMipmap() {
    if (!m_handle.isValid()) {
      return false;
    }

    m_mipmap = true;

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));
    GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getMinFilter(m_smooth, m_mipmap)));

    return true;
  }

  void BareTexture::bind(const BareTexture *texture) {
    if (texture != nullptr && texture->m_handle.getName() != 0) {
      GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture->m_handle));
    } else {
      GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
    }
  }

  Texture::Texture()
  : BareTexture(Format::Color)
  {
  }

  Texture::Texture(Vector2i size)
  : BareTexture(Format::Color, size, nullptr)
  {
  }

  Texture::Texture(const Image& image)
  : BareTexture(Format::Color, image.getSize(), image.getPixelsPtr())
  {
  }

  Texture::Texture(const Image& image, const RectI& area)
  : Texture(image.subImage(area))
  {
  }

  Texture::Texture(const Path& filename)
  : Texture(Image(filename))
  {
  }

  Texture::Texture(const Path& filename, const RectI& area)
  : Texture(Image(filename), area)
  {
  }

  Texture::Texture(InputStream& stream)
  : Texture(Image(stream))
  {
  }

  Texture::Texture(InputStream& stream, const RectI& area)
  : Texture(Image(stream), area)
  {
  }

  Texture::Texture(Span<const uint8_t> content)
  : Texture(Image(content))
  {
  }

  Texture::Texture(Span<const uint8_t> content, const RectI& area)
  : Texture(Image(content), area)
  {
  }

  void Texture::update(const Image& image) {
    BareTexture::update(image.getPixelsPtr(), RectI::fromSize(image.getSize()));
  }

  Image Texture::copyToImage() const {
    if (getName() == 0) {
      return Image();
    }

    auto size = getSize();
    std::vector<uint8_t> pixels(static_cast<std::size_t>(size.width) * static_cast<std::size_t>(size.height) * 4);

    GraphicsHandle<GraphicsTag::Framebuffer> framebuffer;

    GLint boundFramebuffer;
    GL_CHECK(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFramebuffer));

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
    GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, getName(), 0));
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    GL_CHECK(glPixelStorei(GL_PACK_ALIGNMENT, 4));
    GL_CHECK(glReadPixels(0, 0, size.width, size.height, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, boundFramebuffer));

    Image image(size, pixels.data());
    return image;
  }



  AlphaTexture::AlphaTexture()
  : BareTexture(Format::Alpha)
  {
  }

  AlphaTexture::AlphaTexture(Vector2i size)
  : BareTexture(Format::Alpha, size, nullptr)
  {
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
