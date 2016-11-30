/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016 Julien Bernard
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
#ifndef GF_UI_H
#define GF_UI_H

#include <cstdint>

#include "Drawable.h"
#include "Event.h"
#include "Flags.h"
#include "Font.h"
#include "Types.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  enum class UIWindow : uint32_t {
    Border          = 0x0001, ///< Draws a border around the window to visually separate the window from the background
    Movable         = 0x0002, ///< The movable flag indicates that a window can be moved by user input or by dragging the window header
    Scalable        = 0x0004, ///< The scalable flag indicates that a window can be scaled by user input by dragging a scaler icon at the button of the window
    Closable        = 0x0008, ///< Adds a closable icon into the header
    Minimizable     = 0x0010, ///< Adds a minimize icon into the header
    NoScrollbar     = 0x0020, ///< Removes the scrollbar from the window
    Title           = 0x0040, ///< Forces a header at the top at the window showing the title
    ScrollAutoHide  = 0x0080, ///< Automatically hides the window scrollbar if no user interaction: also requires delta time in `nk_context` to be set each frame
    Background      = 0x0100, ///< Always keep window in the background
  };

  using UIWindowFlags = Flags<UIWindow>;

  class UI : public Drawable {
  public:
    static constexpr unsigned DefaultCharacterSize = 13;

    UI(Font& font, unsigned characterSize = DefaultCharacterSize);
    ~UI();

    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;

    UI(UI&& other);
    UI& operator=(UI&& other);

    /**
     * @brief Update the internal state with an event
     *
     * This function must be called for every event that occur in a frame.
     *
     * @param event An event
     */
    void update(const Event& event);

    bool begin(const std::string& title, const RectF& bounds, UIWindowFlags flags = None);
    void end();

    void layoutRowStatic(float height, int itemWidth, int cols);

    bool buttonLabel(const std::string& title);

    virtual void draw(RenderTarget &target, RenderStates states) override;

  private:
    enum class State {
      Start,
      Input,
      Setup,
      Draw,
    };

    void setState(State state);

  private:
    struct Impl;

    Impl *m_impl;
    State m_state;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template<>
struct EnableBitmaskOperators<UIWindow> {
  static constexpr bool value = true;
};
#endif

}

#endif // GF_UI_H
