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
#include <gf/Segue.h>

#include <gf/SegueEffect.h>
#include <gf/RenderTarget.h>
#include <gf/Texture.h>

#include <gfpriv/TextureCoords.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Segue::Segue()
  : m_texture0(nullptr)
  , m_texture1(nullptr)
  , m_textureRect(RectF::fromSize({ 1.0f, 1.0f }))
  , m_effect(nullptr)
  , m_easing(Ease::linear)
  , m_totalTime(Time::zero())
  , m_currentTime(Time::zero())
  {
    updateTexCoords();
  }

  void Segue::start(Time time) {
    m_totalTime = time;
    m_currentTime = Time::zero();
  }

  void Segue::stop() {
    m_totalTime = m_currentTime = Time::zero();
  }

  void Segue::update(Time time) {
    m_currentTime += time;

    if (m_effect) {
      float progress = m_currentTime.asSeconds() / m_totalTime.asSeconds();
      m_effect->setProgress(m_easing(progress));
    }
  }

  void Segue::setTextures(const Texture& texture0, const Texture& texture1) {
    m_texture0 = &texture0;
    m_texture1 = &texture1;
    updatePositions();
  }

  void Segue::draw(RenderTarget& target, const RenderStates& states) {
    if (m_texture0 == nullptr || m_texture1 == nullptr) {
      return;
    }

    RenderStates localStates = states;

    localStates.shader = m_effect;
    localStates.texture[0] = m_texture0;
    localStates.texture[1] = m_texture1;
    target.draw(m_vertices, 4, PrimitiveType::TriangleStrip, localStates);
  }

  void Segue::updatePositions() {
    if (m_texture0 == nullptr || m_texture1 == nullptr) {
      return;
    }

    RectF bounds = RectF::fromSize(m_texture0->getSize() * m_textureRect.getSize());

    m_vertices[0].position = bounds.getTopLeft();
    m_vertices[1].position = bounds.getTopRight();
    m_vertices[2].position = bounds.getBottomLeft();
    m_vertices[3].position = bounds.getBottomRight();
  }

  void Segue::updateTexCoords() {
    m_vertices[0].texCoords = gf::priv::computeTextureCoords(m_textureRect.getTopLeft());
    m_vertices[1].texCoords = gf::priv::computeTextureCoords(m_textureRect.getTopRight());
    m_vertices[2].texCoords = gf::priv::computeTextureCoords(m_textureRect.getBottomLeft());
    m_vertices[3].texCoords = gf::priv::computeTextureCoords(m_textureRect.getBottomRight());
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
