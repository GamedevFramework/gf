/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2021 Lilian Franchi
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
#include <gf/Widget.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif
  Widget::Widget()
  : m_state(WidgetState::Default)
  , m_callback(nullptr)
  {

  }

  void Widget::setDisabled() {
    setState(WidgetState::Disabled);
  }

  void Widget::setDefault() {
    setState(WidgetState::Default);
  }

  void Widget::setSelected() {
    setState(WidgetState::Selected);
  }

  void Widget::setState(WidgetState state) {
    m_state = state;
    onStateChanged();
  }

  void Widget::setCallback(std::function<void()> callback) {
    m_callback = std::move(callback);
  }

  void Widget::triggerCallback() {
    triggered();

    if (m_callback) {
      m_callback();
    }
  }

  void Widget::onStateChanged() {
    // nothing to do
  }

  void Widget::triggered() {
    // nothing to do
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
