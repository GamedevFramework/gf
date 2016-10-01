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
#include <string>
#include <vector>

#include "Alignment.h"
#include "Drawable.h"
#include "Event.h"
#include "Flags.h"
#include "Portability.h"
#include "Rect.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Font;

  /**
   * @ingroup graphics
   * @brief Properties of a user interface
   *
   * @sa gf::UI, gf::UIFlags
   */
  enum class UIProperties : uint64_t {
    Enabled         = 0x0001, ///< The widget is enabled
    Hot             = 0x0002, ///< The widget is hot
    Active          = 0x0004, ///< The widget is active

    Selectable      = 0x0100, ///< The widget is selectable
    Draggable       = 0x0200, ///< The widget is draggable
    Reactive        = 0x0400, ///< The widget is reactive
    Underlying      = 0x0800, ///< The widget is underlying
    Useless         = 0x1000, ///< The widget is useless
  };

  /**
   * @ingroup graphics
   * @brief The flags corresponding to user interface properties
   *
   * @sa gf::UI, gf::Flags, gf::UIProperties
   */
  using UIFlags = Flags<UIProperties>;

  /**
   * @ingroup graphics
   * @brief Layout of a user interface
   *
   * The layout contains several metrics that are used to compute the size
   * and the shape of the widgets.
   *
   * @sa gf::UI
   */
  struct GF_API UILayout {
    // general
    float widgetHeight = 20.0f;       ///< The height of the widgets
    float textHeight = 12.0f;         ///< The size of the text
    float defaultSpacing = 4.0f;      ///< The default spacing between widgets
    float areaHeader = 20.0f;         ///< The height of the area header
    float scrollAreaPadding = 6.0f;   ///< The padding of the scroll area
    float scrollAreaCorner = 6.0f;    ///< The corner size of the scroll area
    // widget specific
    float buttonCorner = 9.0f;        ///< The corner size of a button
    float itemCorner = 2.0f;          ///< The corner size of an item
    float checkSize = 11.0f;          ///< The size of a check
    float checkCorner = 4.0f;         ///< The corner size of a check
    float sliderCorner = 4.0f;        ///< The corner size of a slider
    float sliderMarkerWidth = 10.0f;  ///< The width of a marker
    float cycleCorner = 4.0f;         ///< The corner size of a cycle
    // misc
    float indentSize = 16.0f;         ///< The indent size
  };

  /**
   * @ingroup graphics
   * @brief An icon of a user interface
   *
   * @sa gf::UI, gf::UIRenderer
   */
  enum class UIIcon {
    Check,      ///< A check icon (generally a cross)
    Collapsed,  ///< A collapsed icon (generally a triangle pointing sideways)
    Expanded,   ///< An expanded icon (generally a triangle pointing downwards)
    Loop,       ///< A loop icon
  };

  /**
   * @ingroup graphics
   * @brief A renderer for a user interface
   *
   * The renderer is responsible for displaying the elements of the widgets.
   * An element is a rectangle, a text or an icon. The flags indicate the
   * status of the element that can be represented by a color.
   *
   * @sa gf::UI, gf::DefaultUIRenderer, gf::UIProperties
   */
  class GF_API UIRenderer {
  public:
    /**
     * @brief Virtual destructor
     */
    virtual ~UIRenderer();

    /**
     * @brief Draw a rectangle
     *
     * @param target The target to draw on
     * @param rect The coordinates of the rectangle to draw
     * @param corner The corner size (or no corner if 0)
     * @param flags The status of the widget
     */
    virtual void drawRect(RenderTarget& target, const RectF& rect, float corner, UIFlags flags) const = 0;

    /**
     * @brief Draw a text
     *
     * @param target The target to draw on
     * @param text The text to draw
     * @param size The character size of the text
     * @param pos The position of the text (top-left corner)
     * @param width The availabe width for the text
     * @param alignment The alignment of the text
     * @param flags The status of the widget
     */
    virtual void drawText(RenderTarget& target, const std::string& text, unsigned size, Vector2f pos, float width, Alignment alignment, UIFlags flags) const = 0;

    /**
     * @brief Draw an icon
     *
     * @param target The target to draw on
     * @param pos The position of the icon (center)
     * @param icon The icon to draw
     * @param flags The status of the widget
     */
    virtual void drawIcon(RenderTarget& target, Vector2f pos, UIIcon icon, UIFlags flags) const = 0;
  };

  /**
   * @ingroup graphics
   * @brief The default renderer for user interface
   *
   * @sa gf::UI, gf::UIRenderer
   */
  class GF_API DefaultUIRenderer : public UIRenderer {
  public:
    /**
     * @brief Constructor
     *
     * @param font The font to display the text
     */
    DefaultUIRenderer(gf::Font& font);

    virtual void drawRect(RenderTarget& target, const RectF& rect, float corner, UIFlags flags) const override;

    virtual void drawText(RenderTarget& target, const std::string& text, unsigned size, Vector2f pos, float width, Alignment alignment, UIFlags flags) const override;

    virtual void drawIcon(RenderTarget& target, Vector2f pos, UIIcon icon, UIFlags flags) const override;
  private:
    gf::Font *m_font;
  };

  /**
   * @ingroup graphics
   * @brief An immediate mode user interface class
   *
   * ~~~{.cc}
   * // initialisation
   *
   * gf::DefaultUIRenderer uiRenderer(font);
   * gf::UILayout uiLayout;
   * gf::UI ui(uiRenderer, uiLayout);
   *
   * // state
   *
   * bool checked1 = false;
   * bool checked2 = false;
   * bool checked3 = true;
   * bool checked4 = false;
   * float scrollArea = 0;
   * float value1 = 50.0f;
   * float value2 = 30.0f;
   *
   * std::vector<std::string> choices = { "First Choice", "Next Choice", "Last Choice" };
   * std::size_t choice = 0;
   *
   * // ...
   *
   * // in the main loop
   * ui.beginScrollArea("Scroll area", { 10.0f, 10.0f, 200.0f, 400.0f }, &scrollArea);
   *
   * ui.separatorLine();
   * ui.separator();
   *
   * ui.button("Button");
   * ui.button("Disabled Button", false);
   * ui.item("Item");
   * ui.item("Disabled item", false);
   *
   * if (ui.check("Checkbox", checked1)) {
   *   checked1 = !checked1;
   * }
   *
   * if (ui.check("Disabled checkbox", checked2, false)) {
   *   checked2 = !checked2;
   * }
   *
   * if (ui.collapse("Collapse", checked3)) {
   *   checked3 = !checked3;
   * }
   *
   * if (checked3) {
   *  ui.indent();
   *  ui.label("Collapsible element");
   *  ui.unindent();
   * }
   *
   *
   * if (ui.collapse("Disabled collapse", checked4, false)) {
   *   checked4 = !checked4;
   * }
   *
   * ui.label("Label");
   * ui.value("Value");
   *
   * ui.slider("Slider", &value1, 0.0f, 100.0f, 1.0f);
   * ui.slider("Disabled slider", &value2, 0.0f, 100.0f, 1.0f, false);
   *
   * ui.indent();
   * ui.label("Indented");
   * ui.unindent();
   * ui.label("Unindented");
   *
   * if (ui.cycle(choices, choice)) {
   *   choice = (choice + 1) % choices.size();
   * }
   *
   * ui.endScrollArea();
   * ~~~
   *
   * The result of the previous code is given in the following figure.
   *
   * ![UI example](@ref ui.png)
   *
   * @sa gf::UIRenderer, gf::UILayout, gf::UIFlags, gf::UIIcon
   */
  class GF_API UI : public Drawable {
  public:
    /**
     * @brief Constructor
     *
     * @param renderer A renderer for user interface
     * @param layout A layout for user interface
     */
    UI(const UIRenderer& renderer, const UILayout& layout);

    /**
     * @brief Update the internal state with an event
     *
     * This function must be called for every event that occur in a frame.
     *
     * @param event An event
     */
    void update(const Event& event);

    /**
     * @brief Clear the internal state for this frame
     *
     * This function must be called at the beginning of every frame, before
     * any other call to any function.
     */
    void clear();

    /**
     * @brief Define the main widget
     *
     * A scroll area is the main widget. It is the container for the other
     * widgets. It may have a scrollbar on the right side if the widgets are
     * too high.
     *
     * @param name The name of the scroll area
     * @param area The area of the scroll area
     * @param scroll A pointer to the value of the scroll
     *
     * @sa endScrollArea()
     * @sa [Scrollbar - Wikipedia](https://en.wikipedia.org/wiki/Scrollbar)
     */
    bool beginScrollArea(const std::string& name, const RectF& area, float *scroll);

    /**
     * @brief End the main widget
     *
     * @sa beginScrollArea()
     */
    void endScrollArea();

    /**
     * @brief Increase the indent size
     *
     * After this call, all the widgets will be indented. It is possible to
     * call this function multiple times.
     *
     * @sa unindent()
     */
    void indent();

    /**
     * @brief Decrease the indent size
     *
     * After this call, all the widgets will be unindented. It is possible to
     * call this function multiple times.
     *
     * @sa indent()
     */
    void unindent();

    /**
     * @brief Add an invisible separator
     */
    void separator();

    /**
     * @brief Add a separator line
     */
    void separatorLine();

    /**
     * @brief Add a button
     *
     * @param text The text on the button
     * @param enabled True if the button can be pressed
     * @returns True if the button has been activated
     *
     * @sa [Button - Wikipedia](https://en.wikipedia.org/wiki/Button_%28computing%29)
     */
    bool button(const std::string& text, bool enabled = true);

    /**
     * @brief Add an item
     *
     * @param text The text on the item
     * @param enabled True if the item can be pressed
     * @returns True if the item has been activated
     */
    bool item(const std::string& text, bool enabled = true);

    /**
     * @brief Add a checkbox
     *
     * @param text The text of the checkbox
     * @param checked True if the checkbox is checked
     * @param enabled True if the checkbox can be checked
     * @returns True if the checkbox has been activated
     *
     * @sa [Checkbox - Wikipedia](https://en.wikipedia.org/wiki/Checkbox)
     */
    bool check(const std::string& text, bool checked, bool enabled = true);

    /**
     * @brief Add a disclosure widget
     *
     * @param text The text of the widget
     * @param checked True if the widget is expanded
     * @param enabled True if the widget if enabled
     * @returns True if the widget has been activated
     *
     * @sa [Disclosure widget - Wikipedia](https://en.wikipedia.org/wiki/Disclosure_widget)
     */
    bool collapse(const std::string& text, bool checked, bool enabled = true);

    /**
     * @brief Add a label
     *
     * A label is a left aligned text
     *
     * @param text The text of the label
     * @sa value()
     * @sa [Label - Wikipedia](https://en.wikipedia.org/wiki/Label_%28control%29)
     */
    void label(const std::string& text);

    /**
     * @brief Add a value
     *
     * A value is a right aligned text
     *
     * @param text The text of the value
     * @sa label()
     */
    void value(const std::string& text);

    /**
     * @brief Add a slider
     *
     * @param text The text of the slider
     * @param val A pointer to the value of the slider
     * @param vmin The minimum value of the slider
     * @param vmax The maximum value of the slider
     * @param vinc The increment of the slider
     * @param enabled True if the slider is enabled
     * @returns True if the slider has been activated
     *
     * @sa [Slider - Wikipedia](https://en.wikipedia.org/wiki/Slider_%28computing%29)
     */
    bool slider(const std::string& text, float *val, float vmin, float vmax, float vinc, bool enabled = true);

    /**
     * @brief Add a cycle
     *
     * @param choices The possible values of the cycles
     * @param choice The current value of the cycle
     * @param enabled True if the cycle is enabled
     * @returns True if the cycle has been activated
     *
     * @sa [Cycle button - Wikipedia](https://en.wikipedia.org/wiki/Cycle_button)
     */
    bool cycle(const std::vector<std::string>& choices, std::size_t choice, bool enabled = true);

    virtual void draw(RenderTarget &target, RenderStates states) override;

  private:
    const UIRenderer& m_renderer;
    const UILayout& m_layout;

    enum class CommandType {
      Scissor,
      Rect,
      Text,
      Icon,
    };

    struct Command {
      CommandType type;
      UIFlags flags;
    };

    std::vector<Command> m_commands;

    enum class ScissorAction {
      Set,
      Reset,
    };

    struct ScissorCommand {
      ScissorAction action;
      RectF rect;
    };

    std::vector<ScissorCommand> m_scissorCommands;

    struct RectCommand {
      RectF rect;
      float corner;
    };

    std::vector<RectCommand> m_rectCommands;

    struct TextCommand {
      Vector2f pos;
      float width;
      std::string text;
      unsigned size;
      Alignment alignment;
    };

    std::vector<TextCommand> m_textCommands;

    struct IconCommand {
      Vector2f pos;
      UIIcon icon;
    };

    std::vector<IconCommand> m_iconCommands;

    void resetCommandQueue();
    void addScissorCommand(ScissorAction action, const RectF& rect = RectF());
    void addRectCommand(const RectF& rect, float corner, UIFlags flags);
    void addTextCommand(Vector2f pos, float width, const std::string& text, unsigned size, Alignment alignment, UIFlags flags);
    void addIconCommand(Vector2f pos, UIIcon icon, UIFlags flags);


  private:
    uint64_t m_areaId;
    uint64_t m_widgetId;

    uint64_t generateId();

  private:
    bool m_leftPressed;
    bool m_leftReleased;

    Vector2i m_mouseCursor;
    int m_scroll;

    uint64_t m_active;
    uint64_t m_hot;
    uint64_t m_nextHot;

    bool m_wentActive;

    Vector2i m_dragPos;
    float m_dragState;

    float m_widgetX;
    float m_widgetY;
    float m_widgetW;

    bool m_insideCurrentScroll;

    RectF m_scrollRect;
    float m_scrollStart;
    float *m_scrollValue;

    uint64_t m_scrollId;
    bool m_insideScrollArea;

    // helpers

    bool isAnyActive() const {
      return m_active != 0;
    }

    bool isActive(uint64_t id) const {
      return m_active == id;
    }

    bool isHot(uint64_t id) const {
      return m_hot == id;
    }

    bool inWidget(const RectF& rect, bool checkScroll = true) const;

    void clearInput() {
      m_leftPressed = false;
      m_leftReleased = false;
      m_scroll = 0;
    }

    void clearActive() {
      m_active = 0;
      clearInput();
    }

    void setActive(uint64_t id) {
      m_active = id;
      m_wentActive = true;
    }

    void setHot(uint64_t id) {
      m_nextHot = id;
    }

    bool processWidget(uint64_t id, bool over);
    RectF reserveWidget(float height, bool spacing = true);

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template<>
struct EnableBitmaskOperators<UIProperties> {
  static constexpr bool value = true;
};
#endif

}

#endif // GF_UI_H
