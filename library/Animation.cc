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
#include <gf/Animation.h>

#include <stdexcept>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Texture;

  Animation::Animation()
  : m_currentFrame(0)
  , m_currentDurationInFrame(Time::zero())
  {

  }

  void Animation::addFrame(const Texture &texture, const RectF& bounds, Time duration) {
    if (m_frames.empty()) {
      m_currentDurationInFrame = duration;
      m_currentFrame = 0;
    }

    m_frames.push_back({ &texture, bounds, duration });
  }

  const Texture& Animation::getCurrentTexture() const {
    if (m_frames.empty()) {
      throw std::runtime_error("No frame in the animation");
    }

    return *m_frames[m_currentFrame].texture;
  }

  RectF Animation::getCurrentBounds() const {
    if (m_frames.empty()) {
      throw std::runtime_error("No frame in the animation");
    }

    return m_frames[m_currentFrame].bounds;
  }

  bool Animation::update(Time time) {
    if (m_frames.empty()) {
      return false;
    }

    auto prevFrame = m_currentFrame;
    m_currentDurationInFrame -= time;

    while (m_currentDurationInFrame < Time::zero()) {
      m_currentFrame = (m_currentFrame + 1) % m_frames.size();
      m_currentDurationInFrame += m_frames[m_currentFrame].duration;
    }

    return (prevFrame != m_currentFrame);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
