/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
 * Copyright (C) 2018 Lilian Franchi
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
#ifndef GF_WIDGET_CONTAINER_H
#define GF_WIDGET_CONTAINER_H

#include <vector>

#include "GraphicsApi.h"
#include "Ref.h"
#include "RenderStates.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Widget;
  class RenderTarget;

  /**
   * @ingroup graphics_widgets
   * @brief A collection of widgets
   *
   * @sa gf::Widget
   */
  class GF_GRAPHICS_API WidgetContainer {
  public:
    /**
     * @brief Construct a widget wontainer with a given view.
     */
    WidgetContainer();

    /**
     * @brief Render the widgets on the target
     *
     * Widgets are rendered by the order they appears on the list.
     *
     * @param target The render target
     * @param states The render states to use for drawing
     * @sa gf::Entity::render()
     */
    void render(RenderTarget &target, const RenderStates& states = RenderStates());

    /**
     * @brief Check if the given coords are hover a widget and set it selected if so.
     *
     * @param coords The coords to check
     */
    void pointTo(Vector2f coords);

    /**
     * @brief Add a widget to the list of widgets
     *
     * @param widget A widget to add to the list
     */
    void addWidget(Widget& widget);

    /**
     * @brief Remove a widget from the list
     *
     * @param widget The widget to remove
     * @return The removed widget or `nullptr` if the widget was not present
     * @sa addWidget()
     */
    Widget *removeWidget(Widget *widget);

    /**
     * @brief Trigger the callback of the current selected widget.
     */
    void triggerAction();

    /**
     * @brief Select the next widget in the list as the current selected widget.
     */
    void selectNextWidget();

    /**
     * @brief Select the previous widget in the list as the current selected widget.
     */
    void selectPreviousWidget();

    /**
     * @brief Remove all widgets
     */
    void clear();

  private:
    void unselectCurrentlySelected();

    void computePreviousIndex();
    void computeNextIndex();

    Widget& getCurrent();

  private:
    std::vector<Ref<Widget>> m_widgets;
    std::size_t m_selectedWidgetIndex;
    bool m_widgetIsSelected;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // WIDGETS_WIDGET_CONTAINER_H
