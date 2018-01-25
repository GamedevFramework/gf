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
#ifndef GF_WIDGET_H
#define GF_WIDGET_H

#include "RenderTarget.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief State of a widget
   *
   * @sa gf::Widget
   */
  enum class WidgetState {
    Disabled, ///< The widget is disabled
    Default, ///< The default widget state
    Selected, ///< The widget is selected
  };

  /**
   * @ingroup graphics
   * @brief The widgets abstract class
   */
  class GF_API Widget {
  public:
    /**
     * @brief Render the widget on the target
     *
     * @param target The render target
     * @param states The render states to use for drawing
     */
    virtual void render(RenderTarget &target, const RenderStates &states) = 0;

    /**
     * @brief Abstract method that returns the global bounds of the widget
     *
     * @return the global bounds
     */
    virtual RectF getGlobalBounds() = 0;

    /**
     * @brief Disable the widget.
     */
    void setDisabled();

    /**
     * @brief Set the widget to it's default state
     */
    void setDefault();

    /**
     * @brief Select the widget
     */
    void setSelected();

    /**
     * @brief Set the callback of the widget
     *
     * @param callback The function that will be execute when the widget will be triggered.
     */
    void setCallback(std::function<void()> &callback);

    /**
     * @brief Execute the callback function
     *
     * @sa setCallback()
     */
    void trigger();

  protected:
    WidgetState m_state;
    std::function<void()> m_handler;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_WIDGET_H
