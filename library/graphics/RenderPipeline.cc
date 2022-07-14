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
 */
#include <gf/RenderPipeline.h>

#include <cassert>

#include <gf/PostProcessing.h>
#include <gf/Window.h>

#include <gfpriv/GlDebug.h>
#include <gfpriv/GlFwd.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  RenderPipeline::RenderPipeline(Window& window)
  : RenderTarget(window.getFramebufferSize())
  , m_window(window)
  , m_current(0)
  {
    Texture::bind(nullptr);

    Vector2i size = m_window.getFramebufferSize();

    for (auto& buffer : m_buffers) {
      Texture texture(size);
      buffer.texture = std::move(texture);
      buffer.texture.setSmooth();

      GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, buffer.framebuffer));
      GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer.texture.getName(), 0));
      assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_buffers[m_current].framebuffer));
  }

  void RenderPipeline::addEffect(Effect& effect) {
    m_effects.push_back(effect);
  }

  void RenderPipeline::clearEffects() {
    m_effects.clear();
  }

  void RenderPipeline::resized() {
    Texture::bind(nullptr);

    Vector2i size = m_window.getFramebufferSize();

    for (auto& buffer : m_buffers) {
      Texture texture(size);
      buffer.texture = std::move(texture);
      buffer.texture.setSmooth();

      GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, buffer.framebuffer));
      GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer.texture.getName(), 0));
      assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
      GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    onFramebufferResize(size);
  }

  Vector2i RenderPipeline::getSize() const {
    return m_window.getSize();
  }

  void RenderPipeline::display() {
    gf::PostProcessing postProcessing;

    // process the effects

    for (Effect& effect : m_effects) {
      postProcessing.setTexture(m_buffers[m_current].texture);
      postProcessing.setEffect(effect);

      m_current = 1 - m_current;
      GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_buffers[m_current].framebuffer));

      RenderTarget::clear();
      RenderTarget::draw(postProcessing);
      GL_CHECK(glFlush());
    }

    // display to the window

    postProcessing.setTexture(m_buffers[m_current].texture);
    postProcessing.setEffect(m_defaultEffect);

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    RenderTarget::clear();
    RenderTarget::draw(postProcessing);
    m_window.display();

    // prepare for next frame

    m_current = 0;
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_buffers[m_current].framebuffer));
  }

  void RenderPipeline::onFramebufferResize([[maybe_unused]] Vector2i size) {
    // nothing by default
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
