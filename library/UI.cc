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

#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdlib>

#include <algorithm>
#include <string>
#include <type_traits>

#include <iostream>

#include <gf/RenderTarget.h>
#include <gf/Transform.h>
#include <gf/Vertex.h>

#include "priv/String.h"

// #define NK_PRIVATE
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT

// #define NK_MEMSET std::memset
// #define NK_MEMCPY std::memcpy
// #define NK_SQRT std::sqrt
// #define NK_SIN std::sin
// #define NK_COS std::cos
// #define NK_STRTOD std::strtod

#define NK_IMPLEMENTATION
#include "vendor/nuklear/nuklear.h"


#if 0
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
#endif

namespace gf {
inline namespace v1 {

  static float getTextWidth(nk_handle handle, float characterSize, const char *text, int len) {
    auto font = static_cast<Font *>(handle.ptr);

    std::string originalText(text, len);
    std::u32string unicodeText = getUnicodeString(originalText);

    float textWidth = 0;
    char32_t prevCodepoint = '\0';

    for (char32_t currCodepoint : unicodeText) {
      textWidth += font->getKerning(prevCodepoint, currCodepoint, characterSize);
      prevCodepoint = currCodepoint;

      const Glyph& glyph = font->getGlyph(currCodepoint, characterSize);
      textWidth += glyph.advance;
    }

    return textWidth;
  }

  static void getFontGlyph(nk_handle handle, float characterSize, struct nk_user_font_glyph *g, nk_rune currCodepoint, nk_rune nextCodepoint) {
    auto font = static_cast<Font *>(handle.ptr);
    assert(font);

    float kerning = font->getKerning(currCodepoint, nextCodepoint, characterSize);
    const Glyph& glyph = font->getGlyph(currCodepoint, characterSize);

    g->width = glyph.bounds.width;
    g->height = glyph.bounds.height;
    g->xadvance = glyph.advance + kerning; // is it good?

    g->uv[0].x = glyph.textureRect.left;
    g->uv[0].y = glyph.textureRect.top;
    g->uv[1].x = glyph.textureRect.left + glyph.textureRect.width;
    g->uv[1].y = glyph.textureRect.top + glyph.textureRect.height;

    g->offset.x = glyph.bounds.left;
    g->offset.y = glyph.bounds.top + characterSize; // hacky but works
  }

  struct UI::Impl {
    Font *font;
    nk_user_font user;
    nk_context ctx;
    nk_buffer cmds;
  };

  UI::UI(Font& font, unsigned characterSize)
  : m_impl(nullptr)
  , m_state(State::Start)
  {
    m_impl = new Impl;

    m_impl->font = &font;
    font.generateTexture(characterSize);
    auto texture = static_cast<const void *>(font.getTexture(characterSize));
    assert(texture);

    auto user = &m_impl->user;

    user->userdata.ptr = m_impl->font;
    user->height = characterSize;
    user->width = getTextWidth;
    user->query = getFontGlyph;
    user->texture.ptr = const_cast<void *>(texture);

    auto ctx = &m_impl->ctx;
    nk_init_default(ctx, user);

    nk_buffer_init_default(&m_impl->cmds);
  }

  UI::~UI() {
    nk_buffer_free(&m_impl->cmds);
    nk_free(&m_impl->ctx);
    delete m_impl;
  }

  UI::UI(UI&& other)
  : m_impl(other.m_impl)
  , m_state(other.m_state)
  {
    other.m_impl = nullptr;
  }

  UI& UI::operator=(UI&& other) {
    std::swap(m_impl, other.m_impl);
    std::swap(m_state, other.m_state);
    return *this;
  }

  void UI::update(const Event& event) {
    setState(State::Input);

    switch (event.type) {
      case EventType::MouseMoved:
        nk_input_motion(&m_impl->ctx, event.mouseCursor.coords.x, event.mouseCursor.coords.y);
        break;

      case EventType::MouseWheelScrolled:
        nk_input_scroll(&m_impl->ctx, event.mouseWheel.offset.y);
        break;

      case EventType::MouseButtonPressed:
      case EventType::MouseButtonReleased:
      {
        int down = event.type == EventType::MouseButtonPressed;
        int x = event.mouseButton.coords.x;
        int y = event.mouseButton.coords.y;

        switch (event.mouseButton.button) {
          case MouseButton::Left:
            nk_input_button(&m_impl->ctx, NK_BUTTON_LEFT, x, y, down);
            break;
          case MouseButton::Middle:
            nk_input_button(&m_impl->ctx, NK_BUTTON_MIDDLE, x, y, down);
            break;
          case MouseButton::Right:
            nk_input_button(&m_impl->ctx, NK_BUTTON_RIGHT, x, y, down);
            break;
          default:
            break;
        }
      }

      case EventType::KeyPressed:
      case EventType::KeyReleased:
      {
        int down = event.type == EventType::KeyPressed;

        switch (event.key.keycode) {
          case Keycode::LeftShift:
          case Keycode::RightShift:
            nk_input_key(&m_impl->ctx, NK_KEY_SHIFT, down);
            break;

          case Keycode::LeftCtrl:
          case Keycode::RightCtrl:
            nk_input_key(&m_impl->ctx, NK_KEY_CTRL, down);
            break;

          case Keycode::Delete:
            nk_input_key(&m_impl->ctx, NK_KEY_DEL, down);
            break;

          case Keycode::Return:
            nk_input_key(&m_impl->ctx, NK_KEY_ENTER, down);
            break;

          case Keycode::Tab:
            nk_input_key(&m_impl->ctx, NK_KEY_TAB, down);
            break;

          case Keycode::Backspace:
            nk_input_key(&m_impl->ctx, NK_KEY_BACKSPACE, down);
            break;

          case Keycode::Up:
            nk_input_key(&m_impl->ctx, NK_KEY_UP, down);
            break;

          case Keycode::Down:
            nk_input_key(&m_impl->ctx, NK_KEY_DOWN, down);
            break;

          case Keycode::Left:
            if (event.key.modifiers.test(Mod::Control)) {
              nk_input_key(&m_impl->ctx, NK_KEY_TEXT_WORD_LEFT, down);
            } else {
              nk_input_key(&m_impl->ctx, NK_KEY_LEFT, down);
            }
            break;

          case Keycode::Right:
            if (event.key.modifiers.test(Mod::Control)) {
              nk_input_key(&m_impl->ctx, NK_KEY_TEXT_WORD_RIGHT, down);
            } else {
              nk_input_key(&m_impl->ctx, NK_KEY_RIGHT, down);
            }
            break;

          case Keycode::Home:
            nk_input_key(&m_impl->ctx, NK_KEY_TEXT_START, down);
            nk_input_key(&m_impl->ctx, NK_KEY_SCROLL_START, down);
            break;

          case Keycode::End:
            nk_input_key(&m_impl->ctx, NK_KEY_TEXT_END, down);
            nk_input_key(&m_impl->ctx, NK_KEY_SCROLL_END, down);
            break;

          case Keycode::PageUp:
            nk_input_key(&m_impl->ctx, NK_KEY_SCROLL_UP, down);
            break;

          case Keycode::PageDown:
            nk_input_key(&m_impl->ctx, NK_KEY_SCROLL_DOWN, down);
            break;

          case Keycode::C:
            if (event.key.modifiers.test(Mod::Control)) {
              nk_input_key(&m_impl->ctx, NK_KEY_COPY, down);
            }
            break;

          case Keycode::X:
            if (event.key.modifiers.test(Mod::Control)) {
              nk_input_key(&m_impl->ctx, NK_KEY_CUT, down);
            }
            break;

          case Keycode::V:
            if (event.key.modifiers.test(Mod::Control)) {
              nk_input_key(&m_impl->ctx, NK_KEY_PASTE, down);
            }
            break;

          case Keycode::B:
            if (event.key.modifiers.test(Mod::Control)) {
              nk_input_key(&m_impl->ctx, NK_KEY_TEXT_LINE_START, down);
            }
            break;

          case Keycode::E:
            if (event.key.modifiers.test(Mod::Control)) {
              nk_input_key(&m_impl->ctx, NK_KEY_TEXT_LINE_END, down);
            }
            break;

          case Keycode::Z:
            if (event.key.modifiers.test(Mod::Control)) {
              nk_input_key(&m_impl->ctx, NK_KEY_TEXT_UNDO, down);
            }
            break;

          case Keycode::R:
            if (event.key.modifiers.test(Mod::Control)) {
              nk_input_key(&m_impl->ctx, NK_KEY_TEXT_REDO, down);
            }
            break;

          default:
            // nothing to do
            break;
        }
      }

      default:
        break;
    }


  }

  bool UI::begin(const std::string& title, const RectF& bounds, UIWindowFlags flags) {
    setState(State::Setup);
    return nk_begin(&m_impl->ctx, title.c_str(), { bounds.left, bounds.top, bounds.width, bounds.height }, flags.getValue());
  }

  void UI::end() {
    setState(State::Setup);
    nk_end(&m_impl->ctx);
  }

  void UI::layoutRowDynamic(float height, int cols) {
    setState(State::Setup);
    nk_layout_row_dynamic(&m_impl->ctx, height, cols);
  }

  void UI::layoutRowStatic(float height, int itemWidth, int cols) {
    setState(State::Setup);
    nk_layout_row_static(&m_impl->ctx, height, itemWidth, cols);
  }

  void UI::layoutRowBegin(UILayoutFormat format, float rowHeight, int cols) {
    setState(State::Setup);
    nk_layout_row_begin(&m_impl->ctx, static_cast<nk_layout_format>(format), rowHeight, cols);
  }

  void UI::layoutRowPush(float value) {
    setState(State::Setup);
    nk_layout_row_push(&m_impl->ctx, value);
  }

  void UI::layoutRowEnd() {
    setState(State::Setup);
    nk_layout_row_end(&m_impl->ctx);
  }

  void UI::layoutRow(UILayoutFormat format, float height, ArrayRef<float> ratio) {
    setState(State::Setup);
    nk_layout_row(&m_impl->ctx, static_cast<nk_layout_format>(format), height, ratio.getSize(), ratio.getData());
  }

  bool UI::groupBegin(const std::string& title, UIWindowFlags flags) {
    setState(State::Setup);
    return nk_group_begin(&m_impl->ctx, title.c_str(), flags.getValue());
  }

  void UI::groupEnd() {
    setState(State::Setup);
    nk_group_end(&m_impl->ctx);
  }

  bool UI::groupScrolledBegin(UIScroll& scroll, const std::string& title, UIWindowFlags flags) {
    setState(State::Setup);
    return nk_group_scrolled_begin(&m_impl->ctx, reinterpret_cast<nk_scroll*>(&scroll), title.c_str(), flags.getValue());
  }

  void UI::groupScrolledEnd() {
    setState(State::Setup);
    nk_group_scrolled_end(&m_impl->ctx);
  }

  bool UI::treePush(UITreeType type, const std::string& title, UICollapseStates& state) {
    setState(State::Setup);
    enum nk_collapse_states localState = static_cast<enum nk_collapse_states>(state);
    auto ret = nk_tree_state_push(&m_impl->ctx, static_cast<enum nk_tree_type>(type), title.c_str(), &localState);
    state = static_cast<UICollapseStates>(localState);
    return ret;
  }

  void UI::treePop() {
    setState(State::Setup);
    nk_tree_state_pop(&m_impl->ctx);
  }

  void UI::label(const std::string& title, UITextAlignment align) {
    setState(State::Setup);
    nk_label(&m_impl->ctx, title.c_str(), static_cast<nk_flags>(align));
  }

  void UI::labelColored(const std::string& title, UITextAlignment align, const Color4f& color) {
    setState(State::Setup);
    nk_label_colored(&m_impl->ctx, title.c_str(), static_cast<nk_flags>(align), nk_rgba_f(color.r, color.g, color.b, color.a));
  }

  void UI::labelWrap(const std::string& title) {
    setState(State::Setup);
    nk_label_wrap(&m_impl->ctx, title.c_str());
  }

  void UI::labelColoredWrap(const std::string& title, const Color4f& color) {
    setState(State::Setup);
    nk_label_colored_wrap(&m_impl->ctx, title.c_str(), nk_rgba_f(color.r, color.g, color.b, color.a));
  }

  void UI::buttonSetBehavior(UIButtonBehavior behavior) {
    setState(State::Setup);
    nk_button_set_behavior(&m_impl->ctx, static_cast<enum nk_button_behavior>(behavior));
  }

  bool UI::buttonPushBehavior(UIButtonBehavior behavior) {
    setState(State::Setup);
    return nk_button_push_behavior(&m_impl->ctx, static_cast<enum nk_button_behavior>(behavior));
  }

  bool UI::buttonPopBehavior() {
    setState(State::Setup);
    return nk_button_pop_behavior(&m_impl->ctx);
  }

  bool UI::buttonLabel(const std::string& title) {
    setState(State::Setup);
    return nk_button_label(&m_impl->ctx, title.c_str());
  }

  bool UI::buttonColor(const Color4f& color) {
    setState(State::Setup);
    return nk_button_color(&m_impl->ctx, nk_rgba_f(color.r, color.g, color.b, color.a));
  }

  bool UI::buttonSymbol(UISymbolType symbol) {
    setState(State::Setup);
    return nk_button_symbol(&m_impl->ctx, static_cast<nk_symbol_type>(symbol));
  }

  bool UI::buttonSymbolLabel(UISymbolType symbol, const std::string& title, UITextAlignment align) {
    setState(State::Setup);
    return nk_button_symbol_label(&m_impl->ctx, static_cast<nk_symbol_type>(symbol), title.c_str(), static_cast<nk_flags>(align));
  }

  bool UI::checkboxLabel(const std::string& title, bool& active) {
    setState(State::Setup);
    int localActive = active;
    int ret = nk_checkbox_label(&m_impl->ctx, title.c_str(), &localActive);
    active = localActive;
    return ret;
  }

  bool UI::checkboxFlagsLabel(const std::string& title, unsigned& flags, unsigned value) {
    setState(State::Setup);
    return nk_checkbox_flags_label(&m_impl->ctx, title.c_str(), &flags, value);
  }

  bool UI::optionLabel(const std::string& title, bool active) {
    setState(State::Setup);
    return nk_option_label(&m_impl->ctx, title.c_str(), active);
  }

  bool UI::radioLabel(const std::string& title, bool& active) {
    setState(State::Setup);
    int localActive = active;
    int ret = nk_radio_label(&m_impl->ctx, title.c_str(), &localActive);
    active = localActive;
    return ret;
  }

  bool UI::selectableLabel(const std::string& title, UITextAlignment align, bool& value) {
    setState(State::Setup);
    int localValue = value;
    int ret = nk_selectable_label(&m_impl->ctx, title.c_str(), static_cast<nk_flags>(align), &localValue);
    value = localValue;
    return ret;
  }

  bool UI::sliderFloat(float min, float& val, float max, float step) {
    setState(State::Setup);
    return nk_slider_float(&m_impl->ctx, min, &val, max, step);
  }

  bool UI::sliderInt(int min, int& val, int max, int step) {
    setState(State::Setup);
    return nk_slider_int(&m_impl->ctx, min, &val, max, step);
  }

  bool UI::progress(std::size_t& current, std::size_t max, bool modifyable) {
    static_assert(std::is_same<std::size_t, nk_size>::value, "nk_size is not std::size_t");
    setState(State::Setup);
    return nk_progress(&m_impl->ctx, &current, max, modifyable);
  }

  bool UI::colorPicker(Color4f& color) {
    setState(State::Setup);
    nk_color localColor = nk_rgba_f(color.r, color.g, color.b, color.a);
    bool ret = nk_color_pick(&m_impl->ctx, &localColor, NK_RGBA);
    nk_color_f(&color.r, &color.g, &color.b, &color.a, localColor);
    return ret;
  }

  void UI::propertyInt(const std::string& name, int min, int& val, int max, int step, float incPerPixel) {
    setState(State::Setup);
    nk_property_int(&m_impl->ctx, name.c_str(), min, &val, max, step, incPerPixel);
  }

  void UI::propertyFloat(const std::string& name, float min, float& val, float max, float step, float incPerPixel) {
    setState(State::Setup);
    nk_property_float(&m_impl->ctx, name.c_str(), min, &val, max, step, incPerPixel);
  }

  void UI::propertyDouble(const std::string& name, double min, double& val, double max, double step, float incPerPixel) {
    setState(State::Setup);
    nk_property_double(&m_impl->ctx, name.c_str(), min, &val, max, step, incPerPixel);
  }

  bool UI::popupBegin(UIPopupType type, const std::string& title, UIWindowFlags flags, const RectF& bounds) {
    setState(State::Setup);
    return nk_popup_begin(&m_impl->ctx, static_cast<enum nk_popup_type>(type), title.c_str(), flags.getValue(), { bounds.left, bounds.top, bounds.width, bounds.height });
  }

  void UI::popupClose() {
    setState(State::Setup);
    nk_popup_close(&m_impl->ctx);
  }

  void UI::popupEnd() {
    setState(State::Setup);
    nk_popup_end(&m_impl->ctx);
  }

  void UI::combobox(const std::vector<std::string>& items, int& selected, int itemHeight, Vector2f size) {
    std::vector<const char *> itemsCString;

    for (auto& item : items) {
      itemsCString.push_back(item.c_str());
    }

    nk_combobox(&m_impl->ctx, &itemsCString[0], itemsCString.size(), &selected, itemHeight, { size.width, size.height });
  }

  void UI::comboboxSeparator(const std::string& itemsSeparatedBySeparator, char separator, int& selected, int itemHeight, Vector2f size) {
    auto count = std::count(itemsSeparatedBySeparator.begin(), itemsSeparatedBySeparator.end(), separator);

    nk_combobox_separator(&m_impl->ctx, itemsSeparatedBySeparator.c_str(), separator, &selected, count + 1, itemHeight, { size.width, size.height });
  }

  bool UI::comboBeginLabel(const std::string& selected, Vector2f size) {
    return nk_combo_begin_label(&m_impl->ctx, selected.c_str(), { size.width, size.height });
  }

  bool UI::comboBeginColor(const Color4f& color, Vector2f size) {
    nk_color localColor = nk_rgba_f(color.r, color.g, color.b, color.a);
    return nk_combo_begin_color(&m_impl->ctx, localColor, { size.width, size.height });
  }

  bool UI::comboBeginSymbol(UISymbolType symbol, Vector2f size) {
    return nk_combo_begin_symbol(&m_impl->ctx, static_cast<nk_symbol_type>(symbol), { size.width, size.height });
  }

  bool UI::comboBeginSymbolLabel(UISymbolType symbol, const std::string& selected, Vector2f size) {
    return nk_combo_begin_symbol_label(&m_impl->ctx, selected.c_str(), static_cast<nk_symbol_type>(symbol), { size.width, size.height });
  }

  bool UI::comboItemLabel(const std::string& title, UITextAlignment align) {
    return nk_combo_item_label(&m_impl->ctx, title.c_str(), static_cast<nk_flags>(align));
  }

  bool UI::comboItemSymbolLabel(UISymbolType symbol, const std::string& title, UITextAlignment align) {
    return nk_combo_item_symbol_label(&m_impl->ctx, static_cast<nk_symbol_type>(symbol), title.c_str(), static_cast<nk_flags>(align));
  }

  void UI::comboClose() {
    nk_combo_close(&m_impl->ctx);
  }

  void UI::comboEnd() {
    nk_combo_end(&m_impl->ctx);
  }

  bool UI::contextualBegin(UIWindowFlags flags, Vector2f size, const RectF& triggerBounds) {
    return nk_contextual_begin(&m_impl->ctx, flags.getValue(), { size.width, size.height }, { triggerBounds.left, triggerBounds.top, triggerBounds.width, triggerBounds.height });
  }

  bool UI::contextualItemLabel(const std::string& title, UITextAlignment align) {
    return nk_contextual_item_label(&m_impl->ctx, title.c_str(), static_cast<nk_flags>(align));
  }

  bool UI::contextualItemSymbolLabel(UISymbolType symbol, const std::string& title, UITextAlignment align) {
    return nk_contextual_item_symbol_label(&m_impl->ctx, static_cast<nk_symbol_type>(symbol), title.c_str(), static_cast<nk_flags>(align));
  }

  void UI::contextualClose() {
    nk_contextual_close(&m_impl->ctx);
  }

  void UI::contextualEnd() {
    nk_contextual_end(&m_impl->ctx);
  }

  void UI::tooltip(const std::string& text) {
    nk_tooltip(&m_impl->ctx, text.c_str());
  }

  bool UI::tooltipBegin(float width) {
    return nk_tooltip_begin(&m_impl->ctx, width);
  }

  void UI::tooltipEnd() {
    nk_tooltip_end(&m_impl->ctx);
  }

  void UI::menubarBegin() {
    nk_menubar_begin(&m_impl->ctx);
  }

  void UI::menubarEnd() {
    nk_menubar_end(&m_impl->ctx);
  }

  bool UI::menuBeginLabel(const std::string& title, UITextAlignment align, Vector2f size) {
    return nk_menu_begin_label(&m_impl->ctx, title.c_str(), static_cast<nk_flags>(align), { size.width, size.height });
  }

  bool UI::menuBeginSymbol(const std::string& id, UISymbolType symbol, Vector2f size) {
    return nk_menu_begin_symbol(&m_impl->ctx, id.c_str(), static_cast<nk_symbol_type>(symbol), { size.width, size.height });
  }

  bool UI::menuBeginSymbolLabel(UISymbolType symbol, const std::string& title, UITextAlignment align, Vector2f size) {
    return nk_menu_begin_symbol_label(&m_impl->ctx, title.c_str(), static_cast<nk_flags>(align), static_cast<nk_symbol_type>(symbol), { size.width, size.height });
  }

  bool UI::menuItemLabel(const std::string& title, UITextAlignment align) {
    return nk_menu_item_label(&m_impl->ctx, title.c_str(), static_cast<nk_flags>(align));
  }

  bool UI::menuItemSymbolLabel(UISymbolType symbol, const std::string& title, UITextAlignment align) {
    return nk_menu_item_symbol_label(&m_impl->ctx, static_cast<nk_symbol_type>(symbol), title.c_str(), static_cast<nk_flags>(align));
  }

  void UI::menuClose() {
    nk_menu_close(&m_impl->ctx);
  }

  void UI::menuEnd() {
    nk_menu_end(&m_impl->ctx);
  }

  RectF UI::getWidgetBounds() {
    auto bounds = nk_widget_bounds(&m_impl->ctx);
    return RectF(bounds.x, bounds.y, bounds.w, bounds.h);
  }

  bool UI::isWidgetHovered() {
    return nk_widget_is_hovered(&m_impl->ctx);
  }

  bool UI::isMouseHoveringRect(const RectF& bounds) {
    return nk_input_is_mouse_hovering_rect(&m_impl->ctx.input, { bounds.left, bounds.top, bounds.width, bounds.height });
  }


  void UI::draw(RenderTarget &target, RenderStates states) {
    setState(State::Draw);

    struct nk_convert_config config;

    static const struct nk_draw_vertex_layout_element vertexLayout[] = {
        { NK_VERTEX_POSITION, NK_FORMAT_FLOAT, offsetof(Vertex, position) },
        { NK_VERTEX_COLOR, NK_FORMAT_R32G32B32A32_FLOAT, offsetof(Vertex, color) },
        { NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, offsetof(Vertex, texCoords) },
        { NK_VERTEX_LAYOUT_END }
    };

    NK_MEMSET(&config, 0, sizeof(config));

    config.global_alpha = 1.0f;

    config.shape_AA = NK_ANTI_ALIASING_ON;
    config.line_AA = NK_ANTI_ALIASING_ON;

    config.circle_segment_count = 22;
    config.arc_segment_count = 22;
    config.curve_segment_count = 22;

    config.null.texture.ptr = nullptr;

    config.vertex_layout = vertexLayout;
    config.vertex_size = sizeof(Vertex);
    config.vertex_alignment = alignof(Vertex);

    struct nk_buffer vertexBuffer;
    struct nk_buffer elementBuffer;

    nk_buffer_init_default(&vertexBuffer);
    nk_buffer_init_default(&elementBuffer);
    nk_convert(&m_impl->ctx, &m_impl->cmds, &vertexBuffer, &elementBuffer, &config);

    auto vertices = static_cast<const Vertex *>(nk_buffer_memory_const(&vertexBuffer));
    auto indices = static_cast<const uint16_t *>(nk_buffer_memory_const(&elementBuffer));

    target.setScissorTest(true);

    for (auto cmd = nk__draw_begin(&m_impl->ctx, &m_impl->cmds); cmd != nullptr; cmd = nk__draw_next(cmd, &m_impl->cmds, &m_impl->ctx)) {
      if (!cmd->elem_count) {
        continue;
      }

      states.texture = static_cast<const BareTexture*>(cmd->texture.ptr);
      target.setScissorBox(RectI(cmd->clip_rect.x, cmd->clip_rect.y, cmd->clip_rect.w, cmd->clip_rect.h));
      target.draw(vertices, indices, cmd->elem_count, PrimitiveType::Triangles, states);

      indices += cmd->elem_count;
    }

    target.setScissorTest(false);

    nk_buffer_free(&elementBuffer);
    nk_buffer_free(&vertexBuffer);
  }

  void UI::setState(State state) {
//     std::cerr << "setState(): " << static_cast<int>(m_state) << " -> " << static_cast<int>(state) << "\n";

    if (m_state == state) {
      return;
    }

    switch (m_state) {
      case State::Start:
        switch (state) {
          case State::Input:
            nk_input_begin(&m_impl->ctx);
            break;

          case State::Setup:
          case State::Draw:
            nk_input_begin(&m_impl->ctx);
            nk_input_end(&m_impl->ctx);
            break;

          case State::Start:
            assert(false);
            break;
        }
        break;

      case State::Input:
        switch (state) {
          case State::Setup:
            nk_input_end(&m_impl->ctx);
            break;

          default:
            assert(false && "TODO");
            break;
        }

      case State::Setup:
        break;

      case State::Draw:
        switch (state) {
          case State::Input:
            nk_clear(&m_impl->ctx);
            nk_input_begin(&m_impl->ctx);
            break;

          case State::Setup:
            nk_clear(&m_impl->ctx);
            nk_input_begin(&m_impl->ctx);
            nk_input_end(&m_impl->ctx);
            break;

          default:
            assert(false && "TODO");
            break;
        }
        break;


    }

    m_state = state;
  }

}
}
