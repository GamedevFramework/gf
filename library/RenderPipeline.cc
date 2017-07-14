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
 */
#include <gf/RenderPipeline.h>

#include <cassert>

#include <glad/glad.h>

#include <gf/PostProcessing.h>
#include <gf/Unused.h>
#include <gf/Window.h>

#include "priv/Debug.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  RenderPipeline::RenderPipeline(Window& window)
  : m_window(window)
  , m_current(0)
  {
    initialize();
    Texture::bind(nullptr);

    Vector2u size = m_window.getFramebufferSize();

    for (auto& buffer : m_buffers) {
      buffer.name = 0;

      if (!buffer.texture.create(size)) {
        continue;
      }

      buffer.texture.setSmooth();

      GLuint name;
      glCheck(glGenFramebuffers(1, &name));
      buffer.name = static_cast<unsigned>(name);

      glCheck(glBindFramebuffer(GL_FRAMEBUFFER, name));
      glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer.texture.getName(), 0));
      assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_buffers[m_current].name));
  }

  RenderPipeline::~RenderPipeline() {
    for (auto& buffer : m_buffers) {
      if (buffer.name != 0) {
        GLuint name = static_cast<GLuint>(buffer.name);
        glCheck(glDeleteFramebuffers(1, &name));
      }
    }

  }

  void RenderPipeline::addEffect(Effect& effect) {
    m_effects.push_back(&effect);
  }

  void RenderPipeline::clearEffects() {
    m_effects.clear();
  }

  void RenderPipeline::resized() {
    Texture::bind(nullptr);

    Vector2u size = m_window.getFramebufferSize();

    for (auto& buffer : m_buffers) {
      if (!buffer.texture.create(size)) {
        continue;
      }

      buffer.texture.setSmooth();

      glCheck(glBindFramebuffer(GL_FRAMEBUFFER, buffer.name));
      glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer.texture.getName(), 0));
      assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    onFramebufferResize(size);
  }

  Vector2u RenderPipeline::getSize() const {
    return m_window.getSize();
  }

  void RenderPipeline::display() {
    gf::PostProcessing postProcessing;

    // process the effects

    for (auto effect : m_effects) {
      postProcessing.setTexture(m_buffers[m_current].texture);
      postProcessing.setEffect(*effect);

      m_current = 1 - m_current;
      glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_buffers[m_current].name));

      RenderTarget::clear();
      RenderTarget::draw(postProcessing);
      glCheck(glFlush());
    }

    // display to the window

    postProcessing.setTexture(m_buffers[m_current].texture);
    postProcessing.setEffect(m_defaultEffect);

    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    RenderTarget::clear();
    RenderTarget::draw(postProcessing);
    m_window.display();

    // prepare for next frame

    m_current = 0;
    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_buffers[m_current].name));
  }

  void RenderPipeline::onFramebufferResize(Vector2u size) {
    gf::unused(size);
    // nothing by default
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
