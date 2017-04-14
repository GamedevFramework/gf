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
#include <gf/RenderTexture.h>

#include <cassert>

#include <glad/glad.h>

#include "priv/Debug.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  RenderTexture::RenderTexture()
  : m_name(0)
  {

  }

  RenderTexture::~RenderTexture() {
    if (m_name != 0) {
      GLuint name = static_cast<GLuint>(m_name);
      glCheck(glDeleteFramebuffers(1, &name));
    }

  }

  bool RenderTexture::create(Vector2u size) {
    if (!m_texture.create(size)) {
      return false;
    }

    m_texture.setSmooth();
    Texture::bind(nullptr);

    initialize();


    if (m_name == 0) {
      GLuint name;
      glCheck(glGenFramebuffers(1, &name));
      m_name = static_cast<unsigned>(name);
    }


    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_name));
    glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.getName(), 0));
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    return true;
  }

  Vector2u RenderTexture::getSize() const {
    return m_texture.getSize();
  }

  void RenderTexture::setActive() {
    if (m_name != 0) {
      glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_name));
    }
  }

  void RenderTexture::display() {
    glCheck(glFlush());
  }

  Image RenderTexture::capture() const {
    return captureFramebuffer(m_name);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
