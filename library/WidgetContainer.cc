/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Lilian Franchi
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
#include <gf/WidgetContainer.h>

#include <gf/Widgets.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  WidgetContainer::WidgetContainer()
    : m_currentWidgetPos(0)
  {
  }

  void WidgetContainer::pointTo(Vector2f coords)
  {
    for(size_t i = 0; i < m_widgets.size(); i++) {
      m_widgets[i]->setDefault();
      if (m_widgets[i]->getGlobalBounds().contains(coords)) {
        m_currentWidgetPos = i;
        m_widgets[i]->setSelected();
      }
    }
  }

  void WidgetContainer::render(RenderTarget &target, const RenderStates &states) {
    for (auto widget : m_widgets) {
      widget->render(target, states);
    }
  }

  void WidgetContainer::addWidget(Widget& widget) {
    m_widgets.push_back(&widget);
  }

  Widget *WidgetContainer::removeWidget(Widget *widget) {
    // erase-remove idiom
    auto it = std::remove(m_widgets.begin(), m_widgets.end(), widget);

    if (it != m_widgets.end()) {
      m_widgets.erase(it, m_widgets.end());
      return widget;
    }

    return nullptr;
  }

  void WidgetContainer::triggerAction() {
    if (m_widgets[m_currentWidgetPos] != nullptr) {
      m_widgets[m_currentWidgetPos]->trigger();
    }
  }

  void WidgetContainer::selectNextWidget() {
    if (m_widgets[m_currentWidgetPos] != nullptr) {
      m_widgets[m_currentWidgetPos]->setDefault();
    }
    m_currentWidgetPos++;
    if (m_currentWidgetPos >= m_widgets.size()) {
      m_currentWidgetPos = 0;
    }
    m_widgets[m_currentWidgetPos]->setSelected();
  }

  void WidgetContainer::selectPreviousWidget() {
    if (m_widgets[m_currentWidgetPos] != nullptr) {
      m_widgets[m_currentWidgetPos]->setDefault();
    }
    if (m_currentWidgetPos <= 0) {
      m_currentWidgetPos = m_widgets.size();
    }
    m_currentWidgetPos--;
    m_widgets[m_currentWidgetPos]->setSelected();
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
