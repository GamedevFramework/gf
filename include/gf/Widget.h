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
#ifndef GF_WIDGET_H
#define GF_WIDGET_H

#include <functional>

#include "GraphicsApi.h"
#include "Transformable.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  struct RenderStates;
  class RenderTarget;

  /**
   * @ingroup graphics_widgets
   * @brief State of a widget
   *
   * @sa gf::Widget
   */
  enum class WidgetState {
    Disabled, ///< The widget is disabled
    Default,  ///< The default widget state
    Selected, ///< The widget is selected
  };

  /**
   * @ingroup graphics_widgets
   * @brief The widgets abstract class
   */
  class GF_GRAPHICS_API Widget : public Transformable {
  public:
    /**
     * @brief Constructor
     *
     * The widget is in the default state.
     */
    Widget();

    /**
     * @brief Check if the widget contains the coordinates
     *
     * @param coords The coordinates to check
     * @return True if the coordinates are inside the widget
     */
    virtual bool contains(Vector2f coords) = 0;

    /**
     * @brief Disable the widget.
     */
    void setDisabled();

    /**
     * @brief Check if the widget is disabled
     *
     * @returns True if the widget is disabled
     */
    bool isDisabled() const noexcept {
      return m_state == WidgetState::Disabled;
    }

    /**
     * @brief Set the widget to it's default state
     */
    void setDefault();

    /**
     * @brief Check if the widget is in default state
     *
     * @returns True if the widget is in default state
     */
    bool isDefault() const noexcept {
      return m_state == WidgetState::Default;
    }

    /**
     * @brief Select the widget
     */
    void setSelected();

    /**
     * @brief Check if the widget is selected
     *
     * @returns True if the widget is selected
     */
    bool isSelected() const {
      return m_state == WidgetState::Selected;
    }

    /**
     * @brief Set the state of the widget directly
     *
     * @param state The new state
     * @sa setDisabled(), setDefault(), setSelected()
     */
    void setState(WidgetState state);

    /**
     * @brief Get the state of the widget
     *
     * @returns The current state of the widget
     */
    WidgetState getState() const noexcept {
      return m_state;
    }

    /**
     * @brief Set the callback of the widget
     *
     * @param callback The function that will be execute when the widget will be triggered.
     * @sa triggerCallback()
     */
    void setCallback(std::function<void()> callback);

    /**
     * @brief Execute the callback function
     *
     * @sa setCallback()
     */
    void triggerCallback();

  protected:
    /**
     * @brief Function called when the state changes
     */
    virtual void onStateChanged();

    /**
     * @brief Function called when the callback is triggered
     *
     * This function is called before the callback.
     */
    virtual void triggered();

  private:
    WidgetState m_state;
    std::function<void()> m_callback;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_WIDGET_H
