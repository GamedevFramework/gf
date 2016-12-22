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

#include "ArrayRef.h"
#include "Drawable.h"
#include "Event.h"
#include "Flags.h"
#include "Font.h"
#include "StringRef.h"
#include "Types.h"
#include "Vector.h"

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

  enum class UICollapse : bool {
    Minimized = false,
    Maximized = true,
  };

  enum class UILayout {
    Dynamic = 0,
    Static  = 1,
  };

  enum class UITree {
    Node  = 0,
    Tab   = 1,
  };

  enum class UIAlignment {
    Left    = 0x10 | 0x01,
    Center  = 0x10 | 0x02,
    Right   = 0x10 | 0x04,
  };

  using UIScroll = Vector<unsigned short, 2>;

  enum class UIButtonBehavior {
    Default   = 0,
    Repeater  = 1,
  };

  enum class UISymbol {
    None,
    X,
    Underscore,
    CircleSolid,
    CircleOutline,
    RectSolid,
    RectOutline,
    TriangleUp,
    TriangleDown,
    TriangleLeft,
    TriangleRight,
    Plus,
    Minus,
  };

  enum class UIPopup {
    Static  = 0,
    Dynamic = 1,
  };

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

    /**
     * @name Window
     * @{
     */

    bool begin(const std::string& title, const RectF& bounds, UIWindowFlags flags = None);
    void end();

    /**
     * @}
     */

    /**
     * @name Layout
     * @{
     */

    void layoutRowDynamic(float height, int cols);
    void layoutRowStatic(float height, int itemWidth, int cols);

    void layoutRowBegin(UILayout format, float rowHeight, int cols);
    void layoutRowPush(float value);
    void layoutRowEnd();
    void layoutRow(UILayout format, float height, ArrayRef<float> ratio);

    /**
     * @}
     */

    /**
     * @name Layout: Group
     * @{
     */

    bool groupBegin(const std::string& title, UIWindowFlags flags = None);
    void groupEnd();

    bool groupScrolledBegin(UIScroll& scroll, const std::string& title, UIWindowFlags flags = None);
    void groupScrolledEnd();

    /**
     * @}
     */

    /**
     * @name Layout: Tree
     * @{
     */

    bool treePush(UITree type, const std::string& title, UICollapse& state);
    void treePop();

    /**
     * @}
     */

    /**
     * @name Widgets: Labels
     * @{
     */

    void label(StringRef title, UIAlignment align = UIAlignment::Left);
    void labelColored(const Color4f& color, StringRef title, UIAlignment align = UIAlignment::Left);
    void labelWrap(StringRef title);
    void labelWrapColored(const Color4f& color, StringRef title);

    /**
     * @}
     */

    /**
     * @name Widgets: Buttons
     * @{
     */

    void buttonSetBehavior(UIButtonBehavior behavior);
    bool buttonPushBehavior(UIButtonBehavior behavior);
    bool buttonPopBehavior();

    bool buttonLabel(StringRef title);
    bool buttonColor(const Color4f& color);
    bool buttonSymbol(UISymbol symbol);
    bool buttonSymbolLabel(UISymbol symbol, StringRef title, UIAlignment align = UIAlignment::Left);

    /**
     * @}
     */

    /**
     * @name Widgets: Checkbox
     * @{
     */

    bool checkbox(StringRef title, bool& active);
    bool checkboxFlags(StringRef title, unsigned& flags, unsigned value);

    /**
     * @}
     */

    /**
     * @name Widgets: Radio
     * @{
     */

    bool option(StringRef title, bool active);

    bool radio(StringRef title, bool& active);

    /**
     * @}
     */

    /**
     * @name Widgets: Selectable
     * @{
     */

    bool selectableLabel(StringRef title, UIAlignment align, bool& value);

    /**
     * @}
     */

    /**
     * @name Widgets: Slider
     * @{
     */

    bool sliderFloat(float min, float& val, float max, float step);
    bool sliderInt(int min, int& val, int max, int step);

    /**
     * @}
     */

    /**
     * @name Widgets: Progressbar
     * @{
     */

    bool progress(std::size_t& current, std::size_t max, bool modifyable);

    /**
     * @}
     */

    /**
     * @name Widgets: Color picker
     * @{
     */

    bool colorPicker(Color4f& color);

    /**
     * @}
     */

    /**
     * @name Widgets: Property
     * @{
     */

    void propertyInt(const std::string& name, int min, int& val, int max, int step, float incPerPixel);
    void propertyFloat(const std::string& name, float min, float& val, float max, float step, float incPerPixel);
    void propertyDouble(const std::string& name, double min, double& val, double max, double step, float incPerPixel);

    /**
     * @}
     */

    /**
     * @name Popups
     * @{
     */

    bool popupBegin(UIPopup type, const std::string& title, UIWindowFlags flags, const RectF& bounds);
    void popupClose();
    void popupEnd();

    /**
     * @}
     */

    /**
     * @name Combobox
     * @{
     */

    void combobox(const std::vector<std::string>& items, int& selected, int itemHeight, Vector2f size);
    void comboboxSeparator(const std::string& itemsSeparatedBySeparator, char separator, int& selected, int itemHeight, Vector2f size);

    bool comboBeginLabel(StringRef selected, Vector2f size);
    bool comboBeginColor(const Color4f& color, Vector2f size);
    bool comboBeginSymbol(UISymbol symbol, Vector2f size);
    bool comboBeginSymbolLabel(UISymbol symbol, StringRef selected, Vector2f size);

    bool comboItemLabel(StringRef title, UIAlignment align = UIAlignment::Left);
    bool comboItemSymbolLabel(UISymbol symbol, StringRef title, UIAlignment align = UIAlignment::Left);

    void comboClose();
    void comboEnd();

    /**
     * @}
     */

    /**
     * @name Contextual
     * @{
     */

    bool contextualBegin(UIWindowFlags flags, Vector2f size, const RectF& triggerBounds);

    bool contextualItemLabel(StringRef title, UIAlignment align = UIAlignment::Left);
    bool contextualItemSymbolLabel(UISymbol symbol, StringRef title, UIAlignment align = UIAlignment::Left);

    void contextualClose();
    void contextualEnd();

    /**
     * @}
     */

    /**
     * @name Tooltip
     * @{
     */

    void tooltip(const std::string& text);
    bool tooltipBegin(float width);
    void tooltipEnd();

    /**
     * @}
     */

    /**
     * @name Menu
     * @{
     */

    void menubarBegin();
    void menubarEnd();

    bool menuBeginLabel(StringRef title, UIAlignment align, Vector2f size);
    bool menuBeginSymbol(const std::string& id, UISymbol symbol, Vector2f size);
    bool menuBeginSymbolLabel(UISymbol symbol, StringRef title, UIAlignment align, Vector2f size);

    bool menuItemLabel(StringRef title, UIAlignment align = UIAlignment::Left);
    bool menuItemSymbolLabel(UISymbol symbol, StringRef title, UIAlignment align = UIAlignment::Left);

    void menuClose();
    void menuEnd();

    /**
     * @}
     */




    /**
     * @name Utilities
     * @{
     */

    RectF getWidgetBounds();
    bool isWidgetHovered();

    /**
     * @}
     */

    /**
     * @name Input
     * @{
     */

    bool isMouseHoveringRect(const RectF& bounds);

    /**
     * @}
     */


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
