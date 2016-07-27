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
#include <gf/UI.h>

#include <glad/glad.h>

#include <gf/Color.h>
#include <gf/Math.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Text.h>

#include "priv/Debug.h"
#include "priv/Utils.h"

static std::string niceNum(float num, float precision) {
  float accpow = std::floor(std::log10(precision));

  int digits = 0;

  if (num < 0) {
    digits = static_cast<int>(std::fabs(num / std::pow(10, accpow) - 0.5f));
  } else {
    digits = static_cast<int>(std::fabs(num / std::pow(10, accpow) + 0.5f));
  }

  std::string result;

  if (digits > 0) {
    int curpow = static_cast<int>(accpow);

    for (int i = 0; i < curpow; ++i) {
      result += '0';
    }

    while (digits > 0) {
      char adigit = (digits % 10) + '0';

      if (curpow == 0 && result.length() > 0) {
        result += '.';
        result += adigit;
      } else {
        result += adigit;
      }

      digits /= 10;
      curpow += 1;
    }

    for (int i = curpow; i < 0; ++i) {
      result += '0';
    }

    if (curpow <= 0) {
      result += ".0";
    }

    if (num < 0) {
      result += '-';
    }

    std::reverse(result.begin(), result.end());
  } else {
    result = "0";
  }

  return result;
}


namespace gf {
inline namespace v1 {

  /*
   * DefaultUIRenderer
   */
  DefaultUIRenderer::DefaultUIRenderer(gf::Font& font)
  : m_font(&font)
  {

  }

  void DefaultUIRenderer::drawRect(RenderTarget& target, const RectF& rect, float corner, const Color4f& color, ui::Flags flags) const {
    GF_UNUSED(flags);

    if (corner > 0) {
      gf::RoundedRectangleShape shape(rect.size, corner);
      shape.setColor(color);
      shape.setPosition(rect.position);
      target.draw(shape);
    } else {
      gf::RectangleShape shape(rect.size);
      shape.setColor(color);
      shape.setPosition(rect.position);
      target.draw(shape);
    }

  }

  void DefaultUIRenderer::drawText(RenderTarget& target, const std::string& str, unsigned size, Vector2f pos, float width, const Color4f& color, ui::Flags flags) const {
    gf::Text text;
    text.setParagraphWidth(width);

    Text::Alignment alignment = Text::Alignment::Justify;

    if (flags & ui::AlignedLeft) {
      alignment = Text::Alignment::Left;
    }

    if (flags & ui::AlignedRight) {
      alignment = Text::Alignment::Right;
    }

    if (flags & ui::AlignedCenter) {
      alignment = Text::Alignment::Center;
    }

    text.setAlignment(alignment);

    text.setCharacterSize(size);
    text.setColor(color);
    text.setString(str);
    text.setFont(*m_font);
    text.setPosition(pos);
    text.setAnchor(Anchor::TopLeft);
    target.draw(text);
  }



  /*
   * UI
   */

  UI::UI(const UIRenderer& renderer, const UILayout& layout)
  : m_renderer(renderer)
  , m_layout(layout)
  , m_areaId(0)
  , m_widgetId(0)
  , m_active(0)
  , m_hot(0)
  , m_wentActive(false)
  , m_insideCurrentScroll(false)
  {

  }

  void UI::update(const Event& event) {
    switch (event.type) {
      case EventType::MouseMoved:
        m_mouseCursor = event.mouseCursor.coords;
        break;

      case EventType::MouseButtonPressed:
        if (event.mouseButton.button == MouseButton::Left) {
          m_leftPressed = true;
        }
        break;

      case EventType::MouseButtonReleased:
        if (event.mouseButton.button == MouseButton::Left) {
          m_leftReleased = true;
        }
        break;

      case EventType::MouseWheelScrolled:
        m_scroll = event.mouseWheel.offset.y;
        break;

      default:
        break;
    }
  }

  void UI::clear() {
    m_hot = m_nextHot;
    m_nextHot = 0;

    m_wentActive = false;

    m_widgetX = m_widgetY = m_widgetW = 0.0f;

    m_areaId = 1;
    m_widgetId = 1;

    resetCommandQueue();
  }

  bool UI::beginScrollArea(const std::string& name, const RectF& area, float *scroll) {
    ++m_areaId;
    m_widgetId = 0;
    m_scrollId = generateId();

    Vector2f headerPosition = area.getTopLeft() + m_layout.scrollAreaPadding;
    Vector2f position = headerPosition + Vector2f(0.0f, m_layout.areaHeader);

    // virtual area

    m_widgetX = position.x;
    m_widgetY = position.y - (*scroll);
    m_widgetW = area.width - m_layout.scrollAreaPadding * 4;

    // scroll bar

    m_scrollRect.left = area.left + area.width - m_layout.scrollAreaPadding * 2.5;
    m_scrollRect.top = position.y;
    m_scrollRect.width = m_layout.scrollAreaPadding * 2;
    m_scrollRect.height = area.height - (position.y - area.top) - m_layout.scrollAreaPadding;

    m_scrollValue = scroll;

    m_scrollStart = m_widgetY;

    m_insideScrollArea = inWidget(area, false);
    m_insideCurrentScroll = m_insideScrollArea;

    addRectCommand(area, m_layout.scrollAreaCorner, Color4f{ 0.0f, 0.0f, 0.0f, 0.75f });
    addTextCommand(headerPosition, area.width, name, m_layout.textHeight, Color4f{ 1.0f, 1.0f, 1.0f, 0.5f }, ui::AlignedLeft);

    addScissorCommand({ position.x, position.y, m_widgetW, m_scrollRect.height });

    return m_insideScrollArea;
  }

  void UI::endScrollArea() {
    addScissorCommand(RectF(), ui::NoScissor);

    // scroll bar

    float scrollTop = m_scrollStart;
    float scrollBottom = m_widgetY;
    float scrollHeight = scrollBottom - scrollTop;

    float barHeight = m_scrollRect.height / scrollHeight;

    if (barHeight < 1) {
      float ratio = (scrollBottom - m_scrollRect.top) / scrollHeight;
      ratio = gf::clamp(ratio, 0.0f, 1.0f);

      uint64_t thumbId = m_scrollId;

      RectF thumbRect;
      thumbRect.position = m_scrollRect.position;
      thumbRect.top += (1 - ratio) * m_scrollRect.height;
      thumbRect.width = m_scrollRect.width;
      thumbRect.height = m_scrollRect.height * barHeight;

      float range = m_scrollRect.height - thumbRect.height + 1;
      bool over = inWidget(thumbRect);

      processWidget(thumbId, over);

      if (isActive(thumbId)) {
        float u = (thumbRect.top - m_scrollRect.top) / range;

        if (m_wentActive) {
          m_dragPos.y = m_mouseCursor.y;
          m_dragState = u;
        }

        if (m_dragPos.y != m_mouseCursor.y) {
          u = gf::clamp(m_dragState + (m_mouseCursor.y - m_dragPos.y) / range, 0.0f, 1.0f);
          *m_scrollValue = u * (scrollHeight - m_scrollRect.height);
        }
      }

      // background

      addRectCommand(m_scrollRect, m_scrollRect.width / 2 - 1, Color4f{0.0f, 0.0f, 0.0f, 0.75f});

      // thumb

      if (isActive(thumbId)) {
        addRectCommand(thumbRect, thumbRect.width / 2 - 1, Color4f{ 1.0f, 0.75f, 0.0f, 0.75f });
      } else {
        if (isHot(thumbId)) {
          addRectCommand(thumbRect, thumbRect.width / 2 - 1, Color4f{ 1.0f, 0.75f, 0.0f, 0.375f });
        } else {
          addRectCommand(thumbRect, thumbRect.width / 2 - 1, Color4f{ 1.0f, 1.0f, 1.0f, 0.25f });
        }
      }

      // mouse scrolling

      if (m_insideScrollArea) {
        if (m_scroll != 0) {
          *m_scrollValue -= 20 * m_scroll;

          *m_scrollValue = gf::clamp(*m_scrollValue, 0.0f, scrollHeight - m_scrollRect.height);
        }
      }

    } else {
      *m_scrollValue = 0.0f;
    }

    m_insideCurrentScroll = false;
  }

  void UI::indent() {
    m_widgetX += m_layout.indentSize;
    m_widgetW -= m_layout.indentSize;
  }

  void UI::unindent() {
    m_widgetX -= m_layout.indentSize;
    m_widgetW += m_layout.indentSize;
  }

  void UI::separator() {
    m_widgetY += m_layout.defaultSpacing * 3;
  }

  void UI::separatorLine() {
    m_widgetY += m_layout.defaultSpacing;

    RectF line = reserveWidget(1.0f);
    addRectCommand(line, 0, Color4f{ 1.0f, 1.0f, 1.0f, 0.125f });

    m_widgetY += m_layout.defaultSpacing * 2;
  }

  bool UI::button(const std::string& text, bool enabled) {
    uint64_t id = generateId();

    RectF space = reserveWidget(m_layout.buttonHeight);

    bool over = enabled && inWidget(space);
    bool res = processWidget(id, over);

    addRectCommand(space, m_layout.buttonHeight / 2 - 1, Color4f{ 0.5f, 0.5f, 0.5f, isActive(id) ? 0.75f : 0.375f });

    Vector2f textPos = space.getTopLeft() + m_layout.buttonHeight / 2;
    textPos.y -= m_layout.textHeight / 2;

    if (enabled) {
      if (isHot(id)) {
        addTextCommand(textPos, space.width, text, m_layout.textHeight, Color4f{ 1.0f, 0.75f, 0.0f, 1.0f }, ui::AlignedLeft);
      } else {
        addTextCommand(textPos, space.width, text, m_layout.textHeight, Color4f{ 1.0f, 1.0f, 1.0f, 0.8f }, ui::AlignedLeft);
      }
    } else {
      addTextCommand(textPos, space.width, text, m_layout.textHeight, Color4f{ 0.5f, 0.5f, 0.5f, 0.8f }, ui::AlignedLeft);
    }

    return res;
  }

  bool UI::item(const std::string& text, bool enabled) {
    uint64_t id = generateId();
    RectF space = reserveWidget(m_layout.buttonHeight);

    bool over = enabled && inWidget(space);
    bool res = processWidget(id, over);

    if (isHot(id)) {
      addRectCommand(space, 2.0f, Color4f{ 1.0f, 0.75f, 0.0f, isActive(id) ? 0.75f : 0.375f });
    }

    Vector2f textPos = space.getTopLeft() + m_layout.buttonHeight / 2;
    textPos.y -= m_layout.textHeight / 2;

    if (enabled) {
      addTextCommand(textPos, space.width, text, m_layout.textHeight, Color4f{1.0f, 1.0f, 1.0f, 0.8f}, ui::AlignedLeft);
    } else {
      addTextCommand(textPos, space.width, text, m_layout.textHeight, Color4f{0.5f, 0.5f, 0.5f, 0.8f}, ui::AlignedLeft);
    }

    return res;
  }

  bool UI::check(const std::string& text, bool checked, bool enabled) {
    uint64_t id = generateId();
    RectF space = reserveWidget(m_layout.buttonHeight);

    bool over = enabled && inWidget(space);
    bool res = processWidget(id, over);

    RectF checkMark;
    checkMark.position = space.position + m_layout.buttonHeight / 2 - m_layout.checkSize / 2;
    checkMark.size = { m_layout.checkSize, m_layout.checkSize };

    RectF checkBox = checkMark.extend(3.0f);

    addRectCommand(checkBox, 4.0f, Color4f{0.5f, 0.5f, 0.5f, isActive(id) ? 0.75f : 0.375f });

    if (checked) {
      if (enabled) {
        addRectCommand(checkMark, m_layout.checkSize / 2 - 1, Color4f{ 1.0f, 1.0f, 1.0f, isActive(id) ? 1.0f : 0.8f });
      } else {
        addRectCommand(checkMark, m_layout.checkSize / 2 - 1, Color4f{ 0.5f, 0.5f, 0.5f, 0.8f });
      }
    }

    Vector2f textPos = space.getTopLeft() + m_layout.buttonHeight / 2;
    textPos.x += m_layout.buttonHeight / 2;
    textPos.y -= m_layout.textHeight / 2;

    float textWidth = space.width - m_layout.buttonHeight / 2;

    if (enabled) {
      addTextCommand(textPos, textWidth, text, m_layout.textHeight, isHot(id) ? Color4f{ 1.0f, 0.75f, 0.0f, 1.0f } : Color4f{ 1.0f, 1.0f, 1.0f, 0.8f }, ui::AlignedLeft);
    } else {
      addTextCommand(textPos, textWidth, text, m_layout.textHeight, Color4f{ 0.5f, 0.5f, 0.5f, 0.8f }, ui::AlignedLeft);
    }

    return res;
  }

  bool UI::collapse(const std::string& text, const std::string& subtext, bool checked, bool enabled) {
    GF_UNUSED(subtext);

    uint64_t id = generateId();
    RectF space = reserveWidget(m_layout.buttonHeight, false);

    bool over = enabled && inWidget(space);
    bool res = processWidget(id, over);

    RectF collapseMark;
    collapseMark.position = space.position + m_layout.buttonHeight / 2 - m_layout.checkSize / 2;
    collapseMark.size = { m_layout.checkSize, m_layout.checkSize };

    if (checked) {
      // TODO: draw an icon instead of a rectangle
      addRectCommand(collapseMark, 0, Color4f{ 1.0f, 1.0f, 1.0f, isActive(id) ? 1.0f : 0.8f });
    } else {
      addRectCommand(collapseMark, 0, Color4f{ 0.5f, 0.5f, 0.5f, isActive(id) ? 1.0f : 0.8f });
    }

    Vector2f textPos = space.getTopLeft() + m_layout.buttonHeight / 2;
    textPos.x += m_layout.buttonHeight / 2;
    textPos.y -= m_layout.textHeight / 2;

    float textWidth = space.width - m_layout.buttonHeight / 2;

    if (enabled) {
      addTextCommand(textPos, textWidth, text, m_layout.textHeight, isHot(id) ? Color4f{ 1.0f, 0.75f, 0.0f, 1.0f } : Color4f{ 1.0f, 1.0f, 1.0f, 0.8f }, ui::AlignedLeft);
    } else {
      addTextCommand(textPos, textWidth, text, m_layout.textHeight, Color4f{ 0.5f, 0.5f, 0.5f, 0.8f }, ui::AlignedLeft);
    }

    return res;
  }

  void UI::label(const std::string& text) {
    RectF space = reserveWidget(m_layout.buttonHeight, false);
    Vector2f textPos = space.getTopLeft() + m_layout.buttonHeight / 2;
    textPos.y -= m_layout.textHeight / 2;

    float textWidth = space.width - m_layout.buttonHeight;

    addTextCommand(textPos, textWidth, text, m_layout.textHeight, Color4f{1.0f, 1.0f, 1.0f, 1.0f}, ui::AlignedLeft);
  }

  void UI::value(const std::string& text) {
    RectF space = reserveWidget(m_layout.buttonHeight, false);
    Vector2f textPos = space.getTopLeft() + m_layout.buttonHeight / 2;
    textPos.y -= m_layout.textHeight / 2;

    float textWidth = space.width - m_layout.buttonHeight;

    addTextCommand(textPos, textWidth, text, m_layout.textHeight, Color4f{1.0f, 1.0f, 1.0f, 0.8f}, ui::AlignedRight);
  }

  bool UI::slider(const std::string& text, float *val, float vmin, float vmax, float vinc, bool enabled) {
    uint64_t id = generateId();
    RectF space = reserveWidget(m_layout.sliderHeight);

    addRectCommand(space, 4.0f, Color4f{ 0.0f, 0.0f, 0.0f, 0.5f });

    float range = space.width - m_layout.sliderMarkerWidth;
    float u = gf::clamp((*val - vmin) / (vmax - vmin), 0.0f, 1.0f);
    float m = u * range;

    RectF slider;
    slider.position = space.position;
    slider.left += m;
    slider.size = { m_layout.sliderMarkerWidth, m_layout.sliderHeight };

    bool over = enabled && inWidget(slider);
    bool res = processWidget(id, over);

    bool valChanged = false;

    if (isActive(id)) {
      if (m_wentActive) {
        m_dragPos.x = m_mouseCursor.x;
        m_dragState = u;
      }

      if (m_dragPos.x != m_mouseCursor.x) {
        u = gf::clamp(m_dragState + (m_mouseCursor.x - m_dragPos.x) / range, 0.0f, 1.0f);
        *val = vmin + u * (vmax - vmin);
        *val = std::floor(*val / vinc + 0.5f) * vinc; // snap to vinc
        m = u * range;
        valChanged = true;
      }
    }

    if (isActive(id)) {
      addRectCommand(slider, 4.0f, Color4f{ 1.0f, 1.0f, 1.0f, 1.0f });
    } else {
      addRectCommand(slider, 4.0f, isHot(id) ? Color4f{ 1.0f, 0.75f, 0.0f, 0.5f } : Color4f{ 1.0f, 1.0f, 1.0f, 0.25f });
    }

    std::string str = niceNum(*val, vinc);

    Vector2f textPos = space.getTopLeft() + m_layout.sliderHeight / 2;
    textPos.y -= m_layout.textHeight / 2;

    float textWidth = space.width - m_layout.sliderHeight;

    if (enabled) {
      addTextCommand(textPos, textWidth, text, m_layout.textHeight, isHot(id) ? Color4f{ 1.0f, 0.75f, 0.0f, 1.0f } : Color4f{ 1.0f, 1.0f, 1.0f, 0.8f }, ui::AlignedLeft);
      addTextCommand(textPos, textWidth, str, m_layout.textHeight, isHot(id) ? Color4f{ 1.0f, 0.75f, 0.0f, 1.0f } : Color4f{ 1.0f, 1.0f, 1.0f, 0.8f }, ui::AlignedRight);
    } else {
      addTextCommand(textPos, textWidth, text, m_layout.textHeight, Color4f{ 0.5f, 0.5f, 0.5f, 0.8f }, ui::AlignedLeft);
      addTextCommand(textPos, textWidth, str, m_layout.textHeight, Color4f{ 0.5f, 0.5f, 0.5f, 0.8f }, ui::AlignedRight);
    }

    return res || valChanged;
  }

  void UI::draw(RenderTarget &target, RenderStates states) {
    GF_UNUSED(states);

    clearInput();

    std::size_t rectIndex = 0;
    std::size_t textIndex = 0;

    // save scissor status

    GLboolean scissorTest;
    glCheck(glGetBooleanv(GL_SCISSOR_TEST, &scissorTest));

    GLint scissorBox[4];
    glCheck(glGetIntegerv(GL_SCISSOR_BOX, &scissorBox[0]));

    // apply commands

    for (const Command& cmd : m_commands) {
      switch (cmd.type) {
        case CommandType::Rect:
          m_renderer.drawRect(target, m_rectCommands[rectIndex].rect, m_rectCommands[rectIndex].corner, cmd.color, cmd.flags);
          rectIndex++;
          break;

        case CommandType::Text:
          m_renderer.drawText(target, m_textCommands[textIndex].text, m_textCommands[textIndex].size, m_textCommands[textIndex].pos, m_textCommands[textIndex].width, cmd.color, cmd.flags);
          textIndex++;
          break;

        case CommandType::Scissor:
          if (cmd.flags & ui::NoScissor) {
            if (scissorTest == GL_FALSE) {
              glCheck(glDisable(GL_SCISSOR_TEST));
            } else {
              glCheck(glScissor(scissorBox[0], scissorBox[1], scissorBox[2], scissorBox[3]));
            }
          } else {
            if (scissorTest == GL_FALSE) {
              glCheck(glEnable(GL_SCISSOR_TEST));
            }

            auto size = target.getSize();

            RectF rect = m_rectCommands[rectIndex].rect;
            glCheck(glScissor(rect.left, size.y - (rect.top + rect.height), rect.width, rect.height));
          }

          rectIndex++;
          break;
      }
    }
  }

  void UI::resetCommandQueue() {
    m_commands.clear();
    m_rectCommands.clear();
    m_textCommands.clear();
  }

  void UI::addScissorCommand(const RectF& rect, ui::Flags flags) {
    m_commands.push_back({ CommandType::Scissor, flags, Color::White });
    m_rectCommands.push_back({ rect, 0.0f });
  }

  void UI::addRectCommand(const RectF& rect, float corner, const Color4f& color, ui::Flags flags) {
    m_commands.push_back({ CommandType::Rect, flags, color });
    m_rectCommands.push_back({ rect, corner });
  }

  void UI::addTextCommand(Vector2f pos, float width, const std::string& text, unsigned size, const Color4f& color, ui::Flags flags) {
    m_commands.push_back({ CommandType::Text, flags, color });
    m_textCommands.push_back({ pos, width, text, size });
  }


  uint64_t UI::generateId() {
    ++m_widgetId;
    return (m_areaId << 32) | m_widgetId;
  }

  bool UI::inWidget(const RectF& rect, bool checkScroll) const {
    return (!checkScroll || m_insideCurrentScroll) && rect.contains(m_mouseCursor);
  }

  bool UI::processWidget(uint64_t id, bool over) {
    bool res = false;

    // process down
    if (!isAnyActive()) {
      if (over) {
        setHot(id);
      }

      if (isHot(id) && m_leftPressed) {
        setActive(id);
      }
    }

    if (isActive(id)) {
      if (over) {
        setHot(id);
      }

      if (m_leftReleased) {
        if (isHot(id)) {
          res = true;
        }

        clearActive();
      }
    }

    return res;
  }

  RectF UI::reserveWidget(float height, bool spacing) {
    RectF space{m_widgetX, m_widgetY, m_widgetW, height};
    m_widgetY += height;

    if (spacing) {
      m_widgetY += m_layout.defaultSpacing;
    }

    return space;
  }

}
}
