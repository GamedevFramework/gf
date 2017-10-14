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
#ifndef GF_UI_H
#define GF_UI_H

#include <cstdint>
#include <memory>

#include "ArrayRef.h"
#include "BufferRef.h"
#include "Drawable.h"
#include "Event.h"
#include "Flags.h"
#include "Font.h"
#include "Path.h"
#include "Portability.h"
#include "StringRef.h"
#include "Types.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Texture;

  /**
   * @ingroup graphics
   * @brief Properties for windows and window-like elements
   *
   * @sa gf::UIWindowFlags
   */
  enum class UIWindow : uint32_t {
    Border          = 0x0001, ///< The window has a border
    Movable         = 0x0002, ///< The window can be moved by the user
    Scalable        = 0x0004, ///< The window is scalable by the user
    Closable        = 0x0008, ///< The window can be closed with an icon in the header
    Minimizable     = 0x0010, ///< The window can be minimized with an icon in the header
    NoScrollbar     = 0x0020, ///< The window has no scrollbar
    Title           = 0x0040, ///< The window has a title in the header
    ScrollAutoHide  = 0x0080, ///< The window's scrollbar can hide automatically
    Background      = 0x0100, ///< The window is in the background
    ScaleLeft       = 0x0200, ///< The window's scaler is on the left
    NoInput         = 0x0400, ///< The window can not scale, move or get focus
  };

  /**
   * @ingroup graphics
   * @brief Flags composed of window properties
   *
   * @sa gf::UIWindow
   */
  using UIWindowFlags = Flags<UIWindow>;

  /**
   * @ingroup graphics
   * @brief Collapse property of a tree
   */
  enum class UICollapse : bool {
    Minimized = false,  ///< The tree is minimized
    Maximized = true,   ///< The tree is maximized
  };

  /**
   * @ingroup graphics
   * @brief Layout property for rows
   */
  enum class UILayout {
    Dynamic = 0,  ///< The row has a dynamic layout
    Static  = 1,  ///< The row has a static layout
  };

  /**
   * @ingroup graphics
   * @brief The type of tree
   */
  enum class UITree {
    Node  = 0,  ///< The tree is a node, generally an internal node of the tree
    Tab   = 1,  ///< The tree is a tabulation, generally the root of the tree
  };

  /**
   * @ingroup graphics
   * @brief The alignment of the text
   *
   * @sa gf::Alignment
   */
  enum class UIAlignment {
    Left    = 0x10 | 0x01,  ///< Left alignment
    Center  = 0x10 | 0x02,  ///< Centered alignment
    Right   = 0x10 | 0x04,  ///< Right alignment
  };

  /**
   * @ingroup graphics
   * @brief State for scrollbar in groups
   */
  using UIScroll = Vector2u;

  /**
   * @ingroup graphics
   * @brief Behavior for buttons
   */
  enum class UIButtonBehavior {
    Default   = 0,  ///< The button is active once
    Repeater  = 1,  ///< The button is active as long as it is pressed
  };

  /**
   * @ingroup graphics
   * @brief State of the progress bar
   */
  enum class UIProgress : bool {
    Fixed = false,      ///< The progress bar is fixed
    Modifyable = true,  ///< The progress bar can be modified
  };

  /**
   * @ingroup graphics
   * @brief A representative symbol
   */
  enum class UISymbol {
    None,           ///< No symbol
    X,              ///< X symbol
    Underscore,     ///< Underscore
    CircleSolid,    ///< Solid circle
    CircleOutline,  ///< Outline circle
    RectSolid,      ///< Solid rectangle
    RectOutline,    ///< Outline rectangle
    TriangleUp,     ///< Triangle up
    TriangleDown,   ///< Triangle down
    TriangleLeft,   ///< Triangle left
    TriangleRight,  ///< Triangle right
    Plus,           ///< Plus
    Minus,          ///< Minus
  };

  /**
   * @ingroup graphics
   * @brief Properties for edit widgets
   */
  enum class UIEdit : uint32_t {
    Default             = 0x0000,
    ReadOnly            = 0x0001,
    AutoSelect          = 0x0002,
    SigEnter            = 0x0004,
    AllowTab            = 0x0008,
    NoCursor            = 0x0010,
    Selectable          = 0x0020,
    Clipboard           = 0x0040,
    CtrlEnterNewline    = 0x0080,
    NoHorizontalScroll  = 0x0100,
    AlwaysInsertMode    = 0x0200,
    Multiline           = 0x0400,
    GotoEndOnActivate   = 0x0800,
  };

  /**
   * @ingroup graphics
   * @brief Flags composed of edit properties
   *
   * @sa gf::UIEdit
   */
  using UIEditFlags = Flags<UIEdit>;

  /**
   * @ingroup graphics
   * @brief Predefined flags for edit
   */
  struct GF_API UIEditType {
    static const UIEditFlags Simple;
    static const UIEditFlags Field;
    static const UIEditFlags Box;
    static const UIEditFlags Editor;
  };

  /**
   * @ingroup graphics
   * @brief Properties for edit events
   */
  enum class UIEditEvent : uint32_t {
    Active      = 0x0001, ///< Edit widget is currently being modified
    Inactive    = 0x0002, ///< Edit widget is not active and is not being modified
    Activated   = 0x0004, ///< Edit widget went from state inactive to state active
    Deactivated = 0x0008, ///< Edit widget went from state active to state inactive
    Commited    = 0x0010, ///< Edit widget has received an enter and lost focus
  };

  /**
   * @ingroup graphics
   * @brief Flags composed of edit events properties
   *
   * @sa gf::UIEditEvent
   */
  using UIEditEventFlags = Flags<UIEditEvent>;

  /**
   * @ingroup graphics
   * @brief Filters for edit
   */
  enum class UIEditFilter {
    Default,
    Ascii,
    Float,
    Decimal,
    Hex,
    Oct,
    Binary,
  };

  /**
   * @ingroup graphics
   * @brief Data for file selector
   *
   * @sa gf::UI::fileSelector()
   */
  struct UIBrowser {
    Path currentPath;   ///< The current path for searching
    Path selectedPath;  ///< The selected path
  };

  /**
   * @ingroup graphics
   * @brief Type of popup
   */
  enum class UIPopup {
    Static  = 0,  ///< The popup is static
    Dynamic = 1,  ///< The popup is dynamic
  };

  /**
   * @ingroup graphics
   * @brief A predefined style
   */
  enum class UIPredefinedStyle {
    Default,  ///< The default style
    White,    ///< A light gray style
    Red,      ///< A red and gray style
    Blue,     ///< A blue and light gray style
    Dark,     ///< A dark gray and dark blue style
  };

  /**
   * @ingroup graphics
   * @brief Context for an immediate mode graphical interface
   *
   * When building a graphical interfaces, you have basically two choices:
   * either the retained mode where you build a hierarchy of widgets, or the
   * immediate mode where widgets are built from function calls. Choosing the
   * best mode between these two may be
   * [controversial](http://gamedev.stackexchange.com/questions/24103/immediate-gui-yae-or-nay).
   * gf offers an immediate mode graphical interface through this class.
   *
   * The immediate mode graphical interface (or imgui) of gf is based on
   * [Nuklear](https://github.com/vurtun/nuklear), a C library with the same
   * purpose. gf does not provide (yet?) all the features of Nuklear, but a
   * fair amount of the main features. Nuklear is totally abstracted in gf so
   * you won't see any interface from Nuklear and you won't be able to
   * interact directly with Nuklear. If you need a feature that is not
   * implemented yet, just ask.
   *
   * This imgui library can be used to create tools for the game developpers.
   * It's easy and quick to build a functional interface with an imgui
   * library. You can see some examples in gf itself. You can also use this
   * library to build in-game interfaces, even if it's not its primary
   * purpose. If your game is heavily based on a graphical interface, it
   * may be a good option.
   *
   *
   * # Creating a context
   *
   * An instance of gf::UI is a context for all the windows that will be
   * created. You must provide a font and a character size that will be
   * used in the interface. Generally, you create the instance of gf::UI
   * at the beginning of your application and then you use this instance
   * throughout your application.
   *
   * @snippet snippets/doc_class_ui.cc context
   *
   * By default, the character size is 13.
   *
   *
   * # Managing events
   *
   * The imgui needs to know the input events that occurred in the frame time.
   * So when polling the events in your main loop, you must pass the events
   * to the context so that it can be updated and take the good decision. For
   * example, if you press on a button, the mouse event will be transmitted
   * and the library will know that you pressed a button and act accordingly.
   *
   * @snippet snippets/doc_class_ui.cc events
   *
   *
   * # Creating a window
   *
   * Then, you have to create a window. The window has a title that may appear
   * in the titlebar. It also identifies the window so two windows must have
   * two different titles. The window also has bounds, i.e. a position and a
   * size, that indicates where to put the window in the screen. There is no
   * automatic placement of windows. Finally, the window can have many
   * properties (see gf::UIWindow) that modify the interaction with the window.
   * They are defined at the window creation.
   *
   * @snippet snippets/doc_class_ui.cc window
   *
   * For each begin(), you have to call end() to finish the window.
   * You can create as many windows as you want as long as they have different
   * titles.
   *
   *
   * # Determining a layout
   *
   * Before drawing any widget, you have to define a layout. Think of it as a
   * sort of specification for how to organize the following widgets. There
   * are three types of layouts: rows, groups, trees.
   *
   * ## Rows
   *
   * Rows are the most common type of layout. The row layout is very flexible.
   *
   * A row can be either static (it does not adapt to the width of the window)
   * of dynamic (it adapts to the width of the window). It can be fixed (all
   * columns have the same width) or custom (columns' widths are specified,
   * either relatively or absolutely), and in the latter case the
   * specification can be global or in immediate mode. Here is a summary of
   * the functions to use in each case:
   *
   * - dynamic row
   *   - fixed column layout: layoutRowDynamic()
   *   - custom column layout
   *     - array-based: layoutRow() with gf::UILayout::Dynamic
   *     - immediate-mode: layoutRowBegin() with gf::UILayout::Dynamic, layoutRowPush(), layoutRowEnd()
   * - static row
   *   - fixed column layout: layoutRowStatic()
   *   - custom column layout
   *     - array-based: layoutRow() with gf::UILayout::Static
   *     - immediate-mode: layoutRowBegin() with gf::UILayout::Static, layoutRowPush(), layoutRowEnd()
   *
   * ## Trees
   *
   * A tree is a tree-like layout. It can be collapsed to hide its content
   * except its title. There are two types of trees: tabs and nodes (see
   * gf::UITree). A tab tree is generally put at the root of the tree, it is
   * represented with a border around the title. A node tree is generally the
   * son of a tab tree or another node tree. It is represented without any
   * border.
   *
   * In both cases, you need a variable of type gf::UICollapse to handle the
   * current state of the tree: minimized or maximized.
   *
   * @snippet snippets/doc_class_ui.cc tree
   *
   * ## Groups
   *
   * A group is a kind of window inside a window. It can have a title, a
   * border and a scrollbar. However, a group needs to be put in a row.
   * See groupBegin() and groupEnd().
   *
   * @snippet snippets/doc_class_ui.cc group
   *
   *
   * # Adding widgets and more
   *
   * The library offers common kinds of widgets:
   * - [labels](https://en.wikipedia.org/wiki/Label_%28control%29): label(), selectableLabel()
   * - [buttons](https://en.wikipedia.org/wiki/Button_%28computing%29): buttonLabel(), buttonColor(), buttonSymbol(), buttonSymbolLabel()
   * - [checkboxes](https://en.wikipedia.org/wiki/Checkbox): checkbox(), checkboxFlags()
   * - [radio buttons](https://en.wikipedia.org/wiki/Radio_button): option(), radio()
   * - [sliders](https://en.wikipedia.org/wiki/Slider_%28computing%29): sliderFloat(), sliderInt()
   * - [progress bars](https://en.wikipedia.org/wiki/Progress_bar): progress()
   * - [color pickers](https://en.wikipedia.org/wiki/Color_tool): colorPicker()
   *
   * Widgets often manipulate a reference to a user-provided variable that
   * represents the state of the widget. This way, you can pass directly your
   * model variables to the graphical interface without duplicating the data.
   * Some widgets return a boolean that indicates a state change. For example,
   * a button returns true if it has been pressed.
   *
   * The library also offers common graphical interface elements:
   * - [popups](https://en.wikipedia.org/wiki/Popover_%28GUI%29): popupBegin(), popupEnd()
   * - [combo boxes](https://en.wikipedia.org/wiki/Combo_box): combobox(), comboboxSeparator()
   * - [contextuals](https://en.wikipedia.org/wiki/Context_menu): contextualBegin(), contextualEnd()
   * - [tooltips](https://en.wikipedia.org/wiki/Tooltip): tooltip()
   * - [menus](https://en.wikipedia.org/wiki/Menu_28computing%29): menubarBegin(), menubarEnd()
   *
   * Many of these elements have a begin/end API. The end part must be called
   * if the begin part succeeded, i.e. returned true. Some elements offer a
   * simplified version without begin/end for common cases.
   *
   */
  class GF_API UI : public Drawable {
  public:
    /**
     * @brief The default size for the font
     */
    static constexpr unsigned DefaultCharacterSize = 13;

    /**
     * @brief Constructor
     *
     * @param font The font used in the interface
     * @param characterSize The size of the font used in the interface
     */
    UI(Font& font, unsigned characterSize = DefaultCharacterSize);

    /**
     * @brief Destructor
     */
    ~UI();

    /**
     * @brief Deleted copy constructor
     */
    UI(const UI&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    UI& operator=(const UI&) = delete;

    /**
     * @brief Move constructor
     */
    UI(UI&& other);

    /**
     * @brief Move assignment
     */
    UI& operator=(UI&& other);

    /**
     * @brief Update the internal state with an event
     *
     * This function must be called for every event that occur in a frame.
     *
     * @param event An event
     */
    void processEvent(const Event& event);

    /**
     * @name Window
     * @{
     */

    /**
     * @brief Create a window
     *
     * @snippet snippets/doc_class_ui.cc window
     *
     * @param title The title of the window
     * @param bounds The area of the window
     * @param flags The properties of the window
     * @returns True is the content of the window is visible
     *
     * @sa end(), gf::UIWindow
     */
    bool begin(const std::string& title, const RectF& bounds, UIWindowFlags flags = None);

    /**
     * @brief Finish a window
     *
     * @snippet snippets/doc_class_ui.cc window
     *
     * @sa begin()
     */
    void end();

    /**
     * @brief Get the window bounds
     *
     * This function must be called between begin() and end().
     *
     * @returns The bounds of the current window
     */
    RectF windowGetBounds();

    /**
     * @}
     */

    /**
     * @name Layout
     * @{
     */

    /**
     * @brief Dynamic row with fixed column layout
     *
     * @param height The height of the row
     * @param cols The number of columns in the row
     *
     * @sa layoutRowStatic()
     */
    void layoutRowDynamic(float height, int cols);

    /**
     * @brief Static row with fixed column layout
     *
     * @param height The height of the row
     * @param itemWidth The width of each item in the row
     * @param cols The number of columns in the row
     *
     * @sa layoutRowDynamic()
     */
    void layoutRowStatic(float height, int itemWidth, int cols);

    /**
     * @brief Start an immediate mode custom column layout
     *
     * @param format The format of the row: static or dynamic
     * @param height The height of the row
     * @param cols The number of columns in the row
     *
     * @sa layoutRowPush(), layoutRowEnd()
     */
    void layoutRowBegin(UILayout format, float height, int cols);

    /**
     * @brief Specify the width of the next column
     *
     * @param width The width of the next column
     *
     * @sa layoutRowBegin(), layoutRowEnd()
     */
    void layoutRowPush(float width);

    /**
     * @brief Finish an immediate mode custom column layout
     *
     * @sa layoutRowBegin(), layoutRowPush()
     */
    void layoutRowEnd();

    /**
     * @brief Array-based custom column layout
     *
     * @param format The format of the row: static or dynamic
     * @param height The height of the row
     * @param ratio A reference to an array of ratios for the columns
     */
    void layoutRow(UILayout format, float height, ArrayRef<float> ratio);

    /**
     * @brief An empty separator
     *
     * This function creates an empty row so you have to specify a new row.
     *
     * @param height The height of the row
     */
    void separator(float height);

    /**
     * @}
     */

    /**
     * @name Layout: Group
     * @{
     */

    /**
     * @brief Start a group
     *
     * The only allowed flags are UIWindow::Title, UIWindow::Border,
     * UIWindow::NoScrollbar.
     *
     * @snippet snippets/doc_class_ui.cc group
     *
     * @param title The title of the group
     * @param flags The properties of the group
     * @returns True if the group is visible
     *
     * @sa groupEnd()
     */
    bool groupBegin(const std::string& title, UIWindowFlags flags = None);

    /**
     * @brief Finish a group
     *
     * @snippet snippets/doc_class_ui.cc group
     *
     * @sa groupBegin()
     */
    void groupEnd();

    /**
     * @brief Start a scrolled group
     *
     * The only allowed flags are UIWindow::Title, UIWindow::Border,
     * UIWindow::NoScrollbar.
     *
     * @param scroll A state for representing the scroll
     * @param title The title of the group
     * @param flags The properties of the group
     * @returns True if the group is visible
     *
     * @sa groupScrolledEnd()
     */
    bool groupScrolledBegin(UIScroll& scroll, const std::string& title, UIWindowFlags flags = None);

    /**
     * @brief Finish a scrolled group
     *
     * @sa groupScrolledBegin()
     */
    void groupScrolledEnd();

    /**
     * @}
     */

    /**
     * @name Layout: Tree
     * @{
     */

    /**
     * @brief Start a tree layout
     *
     * @snippet snippets/doc_class_ui.cc tree
     *
     * @param type The type of tree: tab or node
     * @param title The title of the tree
     * @param state The collapse state
     * @returns True if the tree is maximized
     *
     * @sa treePop()
     */
    bool treePush(UITree type, const std::string& title, UICollapse& state);

    /**
     * @brief Finish a tree layout
     *
     * @snippet snippets/doc_class_ui.cc tree
     *
     * @sa treePush()
     */
    void treePop();

    /**
     * @}
     */

    /**
     * @name Widgets: Labels
     * @{
     */

    /**
     * @brief A simple label with text
     *
     * @param title The title of the label
     * @param align The alignment of the text in the label
     *
     * @sa labelColored()
     */
    void label(StringRef title, UIAlignment align = UIAlignment::Left);

    /**
     * @brief A label with colored text
     *
     * @param color The color of the text
     * @param title The title of the label
     * @param align The alignment of the text in the label
     *
     * @sa label()
     */
    void labelColored(const Color4f& color, StringRef title, UIAlignment align = UIAlignment::Left);

    /**
     * @brief A simple label that can wrap
     *
     * @param title The title of the label
     *
     * @sa labelWrapColored()
     */
    void labelWrap(StringRef title);

    /**
     * @brief A label with colored text that can wrap
     *
     * @param color The color of the text
     * @param title The title of the label
     *
     * @sa labelWrap()
     */
    void labelWrapColored(const Color4f& color, StringRef title);

    /**
     * @brief An image
     *
     * @param texture A texture
     * @param textureRect A sub-rectangle of the texture to show
     */
    void image(const Texture& texture, const RectF& textureRect);

    /**
     * @}
     */

    /**
     * @name Widgets: Buttons
     * @{
     */

    /**
     * @brief Change the behavior of buttons
     *
     * @param behavior The new behavior of buttons
     *
     * @sa buttonPushBehavior(), buttonPopBehavior()
     */
    void buttonSetBehavior(UIButtonBehavior behavior);

    /**
     * @brief Push a new behavior of buttons
     *
     * @param behavior The new behavior of buttons
     *
     * @sa buttonSetBehavior(), buttonPopBehavior()
     */
    bool buttonPushBehavior(UIButtonBehavior behavior);

    /**
     * @brief Pop the previous behavior of buttons
     *
     * @sa buttonSetBehavior(), buttonPushBehavior()
     */
    bool buttonPopBehavior();

    /**
     * @brief A button with a centered label
     *
     * @param title The title of the button
     * @returns True if the button was pressed
     *
     * @sa buttonColor(), buttonSymbol(), buttonSymbolLabel()
     */
    bool buttonLabel(StringRef title);

    /**
     * @brief A button with a color
     *
     * @param color The color of the button
     * @returns True if the button was pressed
     *
     * @sa buttonLabel(), buttonSymbol(), buttonSymbolLabel()
     */
    bool buttonColor(const Color4f& color);

    /**
     * @brief A button with a symbol
     *
     * @param symbol A symbol
     * @returns True if the button was pressed
     *
     * @sa buttonLabel(), buttonColor(), buttonSymbolLabel()
     */
    bool buttonSymbol(UISymbol symbol);

    /**
     * @brief A button with a symbol and a centered label
     *
     * @param symbol A symbol
     * @param title The title of the button
     * @param align The alignment of the symbol
     * @returns True if the button was pressed
     *
     * @sa buttonLabel(), buttonColor(), buttonSymbol()
     */
    bool buttonSymbolLabel(UISymbol symbol, StringRef title, UIAlignment align = UIAlignment::Left);

    /**
     * @}
     */

    /**
     * @name Widgets: Checkbox
     * @{
     */

    /**
     * @brief A checkbox with a title
     *
     * @param title The title of the checkbox
     * @param active A reference to a boolean that indicates the state of the checkbox
     * @returns True if the checkbox has changed its state
     *
     * @sa checkboxFlags()
     */
    bool checkbox(StringRef title, bool& active);

    /**
     * @brief A checkbox with a title for flags
     *
     * @param title The title of the checkbox
     * @param flags A reference to the state of flags
     * @param value The flag value for this checkbox
     * @returns True if the checkbox has changed its state
     *
     * @sa checkbox()
     */
    bool checkboxFlags(StringRef title, unsigned& flags, unsigned value);

    /**
     * @}
     */

    /**
     * @name Widgets: Radio
     * @{
     */

    /**
     * @brief A radio button without state
     *
     * @param title The title of the radio button
     * @param active A boolean that indicates if the radio is chosen
     * @returns True if the radio is pressed
     *
     * @sa radio()
     */
    bool option(StringRef title, bool active);

    /**
     * @brief A radio button with a reference state
     *
     * @param title The title of the radio button
     * @param active A reference to the state of the radio
     * @returns True if the radio is pressed
     *
     * @sa option()
     */
    bool radio(StringRef title, bool& active);

    /**
     * @}
     */

    /**
     * @name Widgets: Selectable
     * @{
     */

    /**
     * @brief A selectable label
     *
     * @param title The title of the label
     * @param align The alignment of the text in the label
     * @param value A reference to the state of the selection
     * @returns True if the state has changed
     *
     * @sa label()
     */
    bool selectableLabel(StringRef title, UIAlignment align, bool& value);

    /**
     * @}
     */

    /**
     * @name Widgets: Slider
     * @{
     */

    /**
     * @brief A slider for a float value
     *
     * @param min The minimum for the value
     * @param val A reference to the value
     * @param max The maximum for the value
     * @param step A step when changing the value
     * @returns True if the slider has changed
     *
     * @sa sliderInt()
     */
    bool sliderFloat(float min, float& val, float max, float step);

    /**
     * @brief A slider for an int value
     *
     * @param min The minimum for the value
     * @param val A reference to the value
     * @param max The maximum for the value
     * @param step A step when changing the value
     * @returns True if the slider has changed
     *
     * @sa sliderFloat()
     */
    bool sliderInt(int min, int& val, int max, int step);

    /**
     * @}
     */

    /**
     * @name Widgets: Progressbar
     * @{
     */

    /**
     * @brief A progress bar
     *
     * @param current A reference to the state of the progress bar
     * @param max The maximum of the progress bar (the minimum is zero)
     * @param modifyable A property to indicate if the progress bar can be modified by the user or not
     * @returns True if the progress bar has changed
     */
    bool progress(std::size_t& current, std::size_t max, UIProgress modifyable = UIProgress::Modifyable);

    /**
     * @}
     */

    /**
     * @name Widgets: Color picker
     * @{
     */

    /**
     * @brief A color picker
     *
     * @param color A reference to the state of the color picker, i.e. the current color
     * @returns True if the color has changed
     */
    bool colorPicker(Color4f& color);

    /**
     * @}
     */

    /**
     * @name Widgets: Property
     * @{
     */

    /**
     * @brief A property for an integer
     *
     * @param name The name of the property
     * @param min The minimum for the value
     * @param val A reference to the value
     * @param max The maximum for the value
     * @param step A step when changing the value
     * @param incPerPixel A step when modifying the value with the mouse
     *
     * @sa propertyFloat(), propertyDouble()
     */
    void propertyInt(const std::string& name, int min, int& val, int max, int step, float incPerPixel);

    /**
     * @brief A property for a float
     *
     * @param name The name of the property
     * @param min The minimum for the value
     * @param val A reference to the value
     * @param max The maximum for the value
     * @param step A step when changing the value
     * @param incPerPixel A step when modifying the value with the mouse
     *
     * @sa propertyInt(), propertyDouble()
     */
    void propertyFloat(const std::string& name, float min, float& val, float max, float step, float incPerPixel);

    /**
     * @brief A property for a double
     *
     * @param name The name of the property
     * @param min The minimum for the value
     * @param val A reference to the value
     * @param max The maximum for the value
     * @param step A step when changing the value
     * @param incPerPixel A step when modifying the value with the mouse
     *
     * @sa propertyInt(), propertyFloat()
     */
    void propertyDouble(const std::string& name, double min, double& val, double max, double step, float incPerPixel);

    /**
     * @}
     */

    /**
     * @name Widgets: TextEdit
     * @{
     */

    UIEditEventFlags edit(UIEditFlags flags, BufferRef<char> buffer, std::size_t& length, UIEditFilter filter = UIEditFilter::Default);

    /**
     * @}
     */

    /**
     * @name Dialogs
     * @{
     */

    /**
     * @brief File selector
     *
     * @param browser State of the file selector
     * @param title The title of the file selector
     * @param bounds The area of the file selector
     * @returns True if the file selector is open
     */
    bool fileSelector(UIBrowser& browser, const std::string& title, const RectF& bounds);

    /**
     * @}
     */

    /**
     * @name Popups
     * @{
     */

    /**
     * @brief Start a popup window
     *
     * @param type The type of popup (static or dynamic)
     * @param title The title of the popup
     * @param flags The properties of the popup window
     * @param bounds The area of the popup window
     * @returns True if the popup is open
     *
     * @sa popupClose(), popupEnd()
     */
    bool popupBegin(UIPopup type, const std::string& title, UIWindowFlags flags, const RectF& bounds);

    /**
     * @brief Close a popup window
     *
     * @sa popupBegin(), popupEnd()
     */
    void popupClose();

    /**
     * @brief Finish a popup window
     *
     * @sa popupBegin(), popupClose()
     */
    void popupEnd();

    /**
     * @}
     */

    /**
     * @name Combo box
     * @{
     */

    /**
     * @brief A simple combo box with a list of item
     *
     * @param items The list of items in the combo box
     * @param selected A reference to the selected item in the combo box
     * @param itemHeight The height of the items in the list
     * @param size The size of the combo box
     *
     * @sa comboboxSeparator()
     */
    void combobox(const std::vector<std::string>& items, int& selected, int itemHeight, Vector2f size);

    /**
     * @brief A simple combo box with a string and a separator
     *
     * @param itemsSeparatedBySeparator A string with all the items separated by a separator
     * @param separator The separator used in the string
     * @param selected A reference to the selected item in the combo box
     * @param itemHeight The height of the items in the list
     * @param size The size of the combo box
     *
     * @sa combobox()
     */
    void comboboxSeparator(const std::string& itemsSeparatedBySeparator, char separator, int& selected, int itemHeight, Vector2f size);

    /**
     * @brief Start a combo box with a label
     *
     * @param selected The selected item that appears in the combo box
     * @param size The size of the combo box
     * @returns True if the combo box is open
     *
     * @sa comboClose(), comboEnd()
     */
    bool comboBeginLabel(StringRef selected, Vector2f size);

    /**
     * @brief Start a combo box with a color
     *
     * @param color The color that appears in the combo box
     * @param size The size of the combo box
     * @returns True if the combo box is open
     *
     * @sa comboClose(), comboEnd()
     */
    bool comboBeginColor(const Color4f& color, Vector2f size);

    /**
     * @brief Start a combo box with a symbol
     *
     * @param symbol The symbol that appears in the combo box
     * @param size The size of the combo box
     * @returns True if the combo box is open
     *
     * @sa comboClose(), comboEnd()
     */
    bool comboBeginSymbol(UISymbol symbol, Vector2f size);

    /**
     * @brief Start a combo box with a symbol and a label
     *
     * @param symbol The symbol that appears in the combo box
     * @param selected The selected item that appears in the combo box
     * @param size The size of the combo box
     * @returns True if the combo box is open
     *
     * @sa comboClose(), comboEnd()
     */
    bool comboBeginSymbolLabel(UISymbol symbol, StringRef selected, Vector2f size);

    /**
     * @brief A label inside the combo box
     *
     * @param title The title of the label
     * @param align The alignment of the text in the label
     * @returns True if the item is selected
     */
    bool comboItemLabel(StringRef title, UIAlignment align = UIAlignment::Left);

    /**
     * @brief A symbol and a label inside the combo box
     *
     * @param symbol The symbol of the item
     * @param title The title of the label
     * @param align The alignment of the text in the label
     * @returns True if the item is selected
     */
    bool comboItemSymbolLabel(UISymbol symbol, StringRef title, UIAlignment align = UIAlignment::Left);

    /**
     * @brief Close a combo box
     *
     * @sa comboEnd()
     */
    void comboClose();

    /**
     * @brief Finish a combo box
     *
     * @sa comboBeginLabel(), comboBeginColor(), comboBeginSymbol(), comboBeginSymbolLabel()
     */
    void comboEnd();

    /**
     * @}
     */

    /**
     * @name Contextual
     * @{
     */

    /**
     * @brief Start a contextual window
     *
     * @param flags The properties of the contextual window
     * @param size The size of the contextual window
     * @param triggerBounds The bounds when the contextual should appear
     * @returns True if the contextual is open
     *
     * @sa contextualClose(), contextualEnd()
     */
    bool contextualBegin(UIWindowFlags flags, Vector2f size, const RectF& triggerBounds);

    /**
     * @brief A label inside the contextual window
     *
     * @param title The title of the label
     * @param align The alignment of the text in the label
     * @returns True if the item is selected
     */
    bool contextualItemLabel(StringRef title, UIAlignment align = UIAlignment::Left);

    /**
     * @brief A symbol and a label inside the contextual window
     *
     * @param symbol The symbol of the item
     * @param title The title of the label
     * @param align The alignment of the text in the label
     * @returns True if the item is selected
     */
    bool contextualItemSymbolLabel(UISymbol symbol, StringRef title, UIAlignment align = UIAlignment::Left);

    /**
     * @brief Close the contextual window
     *
     * @sa contextualBegin(), contextualEnd()
     */
    void contextualClose();

    /**
     * @brief Finish the contextual window
     *
     * @sa contextualBegin(), contextualClose()
     */
    void contextualEnd();

    /**
     * @}
     */

    /**
     * @name Tooltip
     * @{
     */

    /**
     * @brief A simple tooltip with a text
     *
     * @param text The text of the tooltip
     */
    void tooltip(const std::string& text);

    /**
     * @brief Start a tooltip
     *
     * @param width The with of the tooltip
     * @returns True if the tooltip is open
     *
     * @sa tooltipEnd()
     */
    bool tooltipBegin(float width);

    /**
     * @brief Finish a tooltip
     *
     * @sa tooltipBegin()
     */
    void tooltipEnd();

    /**
     * @}
     */

    /**
     * @name Menu
     * @{
     */

    /**
     * @brief Start a menu bar
     *
     * @sa menubarEnd()
     */
    void menubarBegin();

    /**
     * @brief Finish a menu bar
     *
     * @sa menubarBegin()
     */
    void menubarEnd();

    /**
     * @brief Start a menu with a label
     *
     * @param title The title of the label
     * @param align The alignment of the text in the label
     * @param size The size of the menu
     * @returns True if the menu is open
     *
     * @sa menuClose(), menuEnd()
     */
    bool menuBeginLabel(StringRef title, UIAlignment align, Vector2f size);

    /**
     * @brief Start a menu with a symbol
     *
     * @param id A unique identifier for the menu
     * @param symbol A symbol for the menu
     * @param size The size of the menu
     * @returns True if the menu is open
     *
     * @sa menuClose(), menuEnd()
     */
    bool menuBeginSymbol(const std::string& id, UISymbol symbol, Vector2f size);

    /**
     * @brief Start a menu with a symbol and a label
     *
     * @param symbol A symbol for the menu
     * @param title The title of the label
     * @param align The alignment of the text in the label
     * @param size The size of the menu
     * @returns True if the menu is open
     *
     * @sa menuClose(), menuEnd()
     */
    bool menuBeginSymbolLabel(UISymbol symbol, StringRef title, UIAlignment align, Vector2f size);

    /**
     * @brief A label inside the menu
     *
     * @param title The title of the label
     * @param align The alignment of the text in the label
     * @returns True if the item is selected
     */
    bool menuItemLabel(StringRef title, UIAlignment align = UIAlignment::Left);

    /**
     * @brief A symbol and a label inside the menu
     *
     * @param symbol The symbol of the item
     * @param title The title of the label
     * @param align The alignment of the text in the label
     * @returns True if the item is selected
     */
    bool menuItemSymbolLabel(UISymbol symbol, StringRef title, UIAlignment align = UIAlignment::Left);

    /**
     * @brief Close a menu
     *
     * @sa menuEnd()
     */
    void menuClose();

    /**
     * @brief Finish a menu
     *
     * @sa menuClose(), menuBeginLabel(), menuBeginSymbol(), menuBeginSymbolLabel()
     */
    void menuEnd();

    /**
     * @}
     */

    /**
     * @name Utilities
     * @{
     */

    /**
     * @brief Get the current widget bounds
     *
     * @returns The bounds of the widget
     */
    RectF getWidgetBounds();

    /**
     * @brief Check if the widget is hovered by the mouse
     *
     * You must do this check just after the layout specification
     *
     * @returns True if the mouse hovers the widget
     */
    bool isWidgetHovered();

    /**
     * @brief Create a horizontal spacing to fill some columns
     *
     * @param cols The number of columns.
     */
    void spacing(int cols);

    /**
     * @}
     */

    /**
     * @name Style
     * @{
     */

    /**
     * @brief Set a predefined style for all the windows and the widgets
     *
     * @param style The predefined style
     *
     * @sa gf::UIPredefinedStyle
     */
    void setPredefinedStyle(UIPredefinedStyle style);

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
    struct UIImpl;

    std::unique_ptr<UIImpl> m_impl;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template<>
struct EnableBitmaskOperators<UIWindow> {
  static constexpr bool value = true;
};

template<>
struct EnableBitmaskOperators<UIEdit> {
  static constexpr bool value = true;
};

template<>
struct EnableBitmaskOperators<UIEditEvent> {
  static constexpr bool value = true;
};
#endif

}

#endif // GF_UI_H
