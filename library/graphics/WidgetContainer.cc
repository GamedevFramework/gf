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
#include <gf/WidgetContainer.h>

#include <cassert>

#include <gf/Widgets.h>

#include <gf/Log.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  WidgetContainer::WidgetContainer()
  : m_selectedWidgetIndex(0)
  , m_widgetIsSelected(false)
  {

  }

  void WidgetContainer::pointTo(Vector2f coords) {
    if (m_widgets.empty()) {
      return;
    }

    unselectCurrentlySelected();

    m_widgetIsSelected = false;

    for(size_t i = 0; i < m_widgets.size(); i++) {
      Widget& currentWidget = m_widgets[i];
      if (currentWidget.isDefault() && currentWidget.contains(coords)) {
        m_selectedWidgetIndex = i;
        m_widgetIsSelected = true;
        currentWidget.setSelected();
        break; // to avoid multiple selection
      }
    }
  }

  void WidgetContainer::render(RenderTarget &target, const RenderStates &states) {
    for (Widget& widget : m_widgets) {
      widget.draw(target, states);
    }
  }

  void WidgetContainer::addWidget(Widget& widget) {
    m_widgets.push_back(widget);
  }

  Widget *WidgetContainer::removeWidget(Widget *widget) {
    // erase-remove idiom
    auto it = std::remove(m_widgets.begin(), m_widgets.end(), *widget);

    if (it != m_widgets.end()) {
      m_widgets.erase(it, m_widgets.end());
      return widget;
    }

    return nullptr;
  }

  void WidgetContainer::triggerAction() {
    if (m_widgets.empty()) {
      return;
    }

    if (m_widgetIsSelected) {
      getCurrent().triggerCallback();
      unselectCurrentlySelected();
      m_widgetIsSelected = false;
    }
  }

  void WidgetContainer::selectNextWidget() {
    if (m_widgets.empty()) {
      return;
    }

    unselectCurrentlySelected();

    // select the new one

    if (m_widgetIsSelected) {
      computeNextIndex();
    } else {
      m_widgetIsSelected = true;
    }

    std::size_t count = 0;
    std::size_t maxCount = m_widgets.size();

    while (count < maxCount && !getCurrent().isDefault()) {
      computeNextIndex();
      ++count;
    }

    if (count == maxCount) {
      m_widgetIsSelected = false;
      return;
    }

    getCurrent().setSelected();
  }

  void WidgetContainer::selectPreviousWidget() {
    if (m_widgets.empty()) {
      return;
    }

    unselectCurrentlySelected();

    // select the new one

    if (m_widgetIsSelected) {
      computePreviousIndex();
    } else {
      m_widgetIsSelected = true;
    }

    std::size_t count = 0;
    std::size_t maxCount = m_widgets.size();

    while (count < maxCount && !getCurrent().isDefault()) {
      computePreviousIndex();
      ++count;
    }

    if (count == maxCount) {
      m_widgetIsSelected = false;
      return;
    }

    getCurrent().setSelected();
  }

  void WidgetContainer::clear() {
    m_widgets.clear();
    m_selectedWidgetIndex = 0;
    m_widgetIsSelected =false;
  }

  void WidgetContainer::unselectCurrentlySelected() {
    if (m_widgetIsSelected) {
      assert(getCurrent().isSelected());
      getCurrent().setDefault();
    }
  }

  void WidgetContainer::computePreviousIndex() {
    if (m_selectedWidgetIndex == 0) {
      m_selectedWidgetIndex = m_widgets.size();
    }

    --m_selectedWidgetIndex;
  }

  void WidgetContainer::computeNextIndex() {
    ++m_selectedWidgetIndex;

    if (m_selectedWidgetIndex >= m_widgets.size()) {
      m_selectedWidgetIndex = 0;
    }
  }

  Widget& WidgetContainer::getCurrent() {
    assert(m_widgetIsSelected);
    assert(m_selectedWidgetIndex < m_widgets.size());
    return m_widgets[m_selectedWidgetIndex];
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
