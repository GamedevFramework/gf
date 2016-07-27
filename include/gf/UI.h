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
#include <vector>

#include "Drawable.h"
#include "Event.h"
#include "Portability.h"
#include "Rect.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Font;

  namespace ui {
    typedef uint64_t Flags;

    //
    static constexpr Flags Selected = 0x0001;
    static constexpr Flags Disabled = 0x0002;

    // text alignment
    static constexpr Flags AlignedLeft   = 0x1000;
    static constexpr Flags AlignedRight  = 0x2000;
    static constexpr Flags AlignedCenter = 0x4000;

    // special commands
    static constexpr Flags NoScissor = 0x10000;
  }

  struct UILayout {
    float buttonHeight = 20.0f;
    float sliderHeight = 20.0f;
    float sliderMarkerWidth = 10.0f;
    float checkSize = 8.0f;
    float defaultSpacing = 4.0f;
    float textHeight = 12.0f;
    float scrollAreaPadding = 6.0f;
    float scrollAreaCorner = 6.0f;
    float indentSize = 16.0f;
    float areaHeader = 20.0f;
  };


  class UIRenderer {
  public:
    virtual void drawRect(RenderTarget& target, const RectF& rect, float corner, const Color4f& color, ui::Flags flags) const = 0;
    virtual void drawText(RenderTarget& target, const std::string& text, unsigned size, Vector2f pos, float width, const Color4f& color, ui::Flags flags) const = 0;
  };

  class DefaultUIRenderer : public UIRenderer {
  public:
    DefaultUIRenderer(gf::Font& font);

    virtual void drawRect(RenderTarget& target, const RectF& rect, float corner, const Color4f& color, ui::Flags flags) const override;
    virtual void drawText(RenderTarget& target, const std::string& text, unsigned size, Vector2f pos, float width, const Color4f& color, ui::Flags flags) const override;

  private:
    gf::Font *m_font;
  };

  class UI : public Drawable {
  public:
    UI(const UIRenderer& renderer, const UILayout& layout);

    void update(const Event& event);

    void clear();

    bool beginScrollArea(const std::string& name, const RectF& area, float *scroll);
    void endScrollArea();

    void indent();
    void unindent();

    void separator();
    void separatorLine();

    bool button(const std::string& text, bool enabled = true);
    bool item(const std::string& text, bool enabled = true);
    bool check(const std::string& text, bool checked, bool enabled = true);
    bool collapse(const std::string& text, const std::string& subtext, bool checked, bool enabled = true);

    void label(const std::string& text);
    void value(const std::string& text);

    bool slider(const std::string& text, float *val, float vmin, float vmax, float vinc, bool enabled = true);

    virtual void draw(RenderTarget &target, RenderStates states) override;

  private:
    const UIRenderer& m_renderer;
    UILayout m_layout;

    enum class CommandType {
      Rect,
      Text,
      Scissor,
    };

    struct Command {
      CommandType type;
      ui::Flags flags;
      Color4f color;
    };

    std::vector<Command> m_commands;

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
    };

    std::vector<TextCommand> m_textCommands;

    void resetCommandQueue();
    void addScissorCommand(const RectF& rect, ui::Flags flags = 0);
    void addRectCommand(const RectF& rect, float corner, const Color4f& color, ui::Flags flags = 0);
    void addTextCommand(Vector2f pos, float width, const std::string& text, unsigned size, const Color4f& color, ui::Flags flags = 0);

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
}

#endif // GF_UI_H
