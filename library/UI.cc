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

  static void getFontGlyph(nk_handle handle, float characterSize, nk_user_font_glyph *g, nk_rune currCodepoint, nk_rune nextCodepoint) {
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
        int down = (event.type == EventType::MouseButtonPressed) ? nk_true : nk_false;
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
    return nk_begin(&m_impl->ctx, title.c_str(), { bounds.left, bounds.top, bounds.width, bounds.height }, flags.getValue()) != 0;
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

  void UI::layoutRowBegin(UILayout format, float height, int cols) {
    setState(State::Setup);
    nk_layout_row_begin(&m_impl->ctx, static_cast<nk_layout_format>(format), height, cols);
  }

  void UI::layoutRowPush(float width) {
    setState(State::Setup);
    nk_layout_row_push(&m_impl->ctx, width);
  }

  void UI::layoutRowEnd() {
    setState(State::Setup);
    nk_layout_row_end(&m_impl->ctx);
  }

  void UI::layoutRow(UILayout format, float height, ArrayRef<float> ratio) {
    setState(State::Setup);
    nk_layout_row(&m_impl->ctx, static_cast<nk_layout_format>(format), height, ratio.getSize(), ratio.getData());
  }

  void UI::separator(float height) {
    setState(State::Setup);
    nk_layout_row_dynamic(&m_impl->ctx, height, 0);
  }

  bool UI::groupBegin(const std::string& title, UIWindowFlags flags) {
    setState(State::Setup);
    return nk_group_begin(&m_impl->ctx, title.c_str(), flags.getValue()) != 0;
  }

  void UI::groupEnd() {
    setState(State::Setup);
    nk_group_end(&m_impl->ctx);
  }

  bool UI::groupScrolledBegin(UIScroll& scroll, const std::string& title, UIWindowFlags flags) {
    setState(State::Setup);
    return nk_group_scrolled_begin(&m_impl->ctx, reinterpret_cast<nk_scroll*>(&scroll), title.c_str(), flags.getValue()) != 0;
  }

  void UI::groupScrolledEnd() {
    setState(State::Setup);
    nk_group_scrolled_end(&m_impl->ctx);
  }

  bool UI::treePush(UITree type, const std::string& title, UICollapse& state) {
    setState(State::Setup);
    nk_collapse_states localState = static_cast<nk_collapse_states>(state);
    int ret = nk_tree_state_push(&m_impl->ctx, static_cast<nk_tree_type>(type), title.c_str(), &localState);
    state = static_cast<UICollapse>(localState);
    return ret != 0;
  }

  void UI::treePop() {
    setState(State::Setup);
    nk_tree_state_pop(&m_impl->ctx);
  }

  void UI::label(StringRef title, UIAlignment align) {
    setState(State::Setup);
    nk_text(&m_impl->ctx, title.getData(), title.getSize(), static_cast<nk_flags>(align));
  }

  void UI::labelColored(const Color4f& color, StringRef title, UIAlignment align) {
    setState(State::Setup);
    nk_text_colored(&m_impl->ctx, title.getData(), title.getSize(), static_cast<nk_flags>(align), nk_rgba_f(color.r, color.g, color.b, color.a));
  }

  void UI::labelWrap(StringRef title) {
    setState(State::Setup);
    nk_text_wrap(&m_impl->ctx, title.getData(), title.getSize());
  }

  void UI::labelWrapColored(const Color4f& color, StringRef title) {
    setState(State::Setup);
    nk_text_wrap_colored(&m_impl->ctx, title.getData(), title.getSize(), nk_rgba_f(color.r, color.g, color.b, color.a));
  }

  void UI::buttonSetBehavior(UIButtonBehavior behavior) {
    setState(State::Setup);
    nk_button_set_behavior(&m_impl->ctx, static_cast<enum nk_button_behavior>(behavior));
  }

  bool UI::buttonPushBehavior(UIButtonBehavior behavior) {
    setState(State::Setup);
    return nk_button_push_behavior(&m_impl->ctx, static_cast<enum nk_button_behavior>(behavior)) != 0;
  }

  bool UI::buttonPopBehavior() {
    setState(State::Setup);
    return nk_button_pop_behavior(&m_impl->ctx) != 0;
  }

  bool UI::buttonLabel(StringRef title) {
    setState(State::Setup);
    return nk_button_text(&m_impl->ctx, title.getData(), title.getSize()) != 0;
  }

  bool UI::buttonColor(const Color4f& color) {
    setState(State::Setup);
    return nk_button_color(&m_impl->ctx, nk_rgba_f(color.r, color.g, color.b, color.a)) != 0;
  }

  bool UI::buttonSymbol(UISymbol symbol) {
    setState(State::Setup);
    return nk_button_symbol(&m_impl->ctx, static_cast<nk_symbol_type>(symbol)) != 0;
  }

  bool UI::buttonSymbolLabel(UISymbol symbol, StringRef title, UIAlignment align) {
    setState(State::Setup);
    return nk_button_symbol_text(&m_impl->ctx, static_cast<nk_symbol_type>(symbol), title.getData(), title.getSize(), static_cast<nk_flags>(align)) != 0;
  }

  bool UI::checkbox(StringRef title, bool& active) {
    setState(State::Setup);
    int localActive = active ? nk_true : nk_false;
    int ret = nk_checkbox_text(&m_impl->ctx, title.getData(), title.getSize(), &localActive);
    active = (localActive != 0);
    return ret != 0;
  }

  bool UI::checkboxFlags(StringRef title, unsigned& flags, unsigned value) {
    setState(State::Setup);
    return nk_checkbox_flags_text(&m_impl->ctx, title.getData(), title.getSize(), &flags, value) != 0;
  }

  bool UI::option(StringRef title, bool active) {
    setState(State::Setup);
    return nk_option_text(&m_impl->ctx, title.getData(), title.getSize(), active) != 0;
  }

  bool UI::radio(StringRef title, bool& active) {
    setState(State::Setup);
    int localActive = active ? nk_true : nk_false;
    int ret = nk_radio_text(&m_impl->ctx, title.getData(), title.getSize(), &localActive);
    active = (localActive != 0);
    return ret != 0;
  }

  bool UI::selectableLabel(StringRef title, UIAlignment align, bool& value) {
    setState(State::Setup);
    int localValue = value ? nk_true : nk_false;
    int ret = nk_selectable_text(&m_impl->ctx, title.getData(), title.getSize(), static_cast<nk_flags>(align), &localValue);
    value = (localValue != 0);
    return ret != 0;
  }

  bool UI::sliderFloat(float min, float& val, float max, float step) {
    setState(State::Setup);
    return nk_slider_float(&m_impl->ctx, min, &val, max, step) != 0;
  }

  bool UI::sliderInt(int min, int& val, int max, int step) {
    setState(State::Setup);
    return nk_slider_int(&m_impl->ctx, min, &val, max, step) != 0;
  }

  bool UI::progress(std::size_t& current, std::size_t max, UIProgress modifyable) {
    static_assert(std::is_same<std::size_t, nk_size>::value, "nk_size is not std::size_t");
    setState(State::Setup);
    return nk_progress(&m_impl->ctx, &current, max, static_cast<bool>(modifyable)) != 0;
  }

  bool UI::colorPicker(Color4f& color) {
    setState(State::Setup);
    nk_color localColor = nk_rgba_f(color.r, color.g, color.b, color.a);
    int ret = nk_color_pick(&m_impl->ctx, &localColor, NK_RGBA);
    nk_color_f(&color.r, &color.g, &color.b, &color.a, localColor);
    return ret != 0;
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

  bool UI::popupBegin(UIPopup type, const std::string& title, UIWindowFlags flags, const RectF& bounds) {
    setState(State::Setup);
    return nk_popup_begin(&m_impl->ctx, static_cast<nk_popup_type>(type), title.c_str(), flags.getValue(), { bounds.left, bounds.top, bounds.width, bounds.height }) != 0;
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
    setState(State::Setup);

    std::vector<const char *> itemsCString;

    for (auto& item : items) {
      itemsCString.push_back(item.c_str());
    }

    nk_combobox(&m_impl->ctx, &itemsCString[0], itemsCString.size(), &selected, itemHeight, { size.width, size.height });
  }

  void UI::comboboxSeparator(const std::string& itemsSeparatedBySeparator, char separator, int& selected, int itemHeight, Vector2f size) {
    setState(State::Setup);

    auto count = std::count(itemsSeparatedBySeparator.begin(), itemsSeparatedBySeparator.end(), separator);

    nk_combobox_separator(&m_impl->ctx, itemsSeparatedBySeparator.c_str(), separator, &selected, count + 1, itemHeight, { size.width, size.height });
  }

  bool UI::comboBeginLabel(StringRef selected, Vector2f size) {
    setState(State::Setup);
    return nk_combo_begin_text(&m_impl->ctx, selected.getData(), selected.getSize(), { size.width, size.height }) != 0;
  }

  bool UI::comboBeginColor(const Color4f& color, Vector2f size) {
    setState(State::Setup);
    nk_color localColor = nk_rgba_f(color.r, color.g, color.b, color.a);
    return nk_combo_begin_color(&m_impl->ctx, localColor, { size.width, size.height }) != 0;
  }

  bool UI::comboBeginSymbol(UISymbol symbol, Vector2f size) {
    setState(State::Setup);
    return nk_combo_begin_symbol(&m_impl->ctx, static_cast<nk_symbol_type>(symbol), { size.width, size.height }) != 0;
  }

  bool UI::comboBeginSymbolLabel(UISymbol symbol, StringRef selected, Vector2f size) {
    setState(State::Setup);
    return nk_combo_begin_symbol_text(&m_impl->ctx, selected.getData(), selected.getSize(), static_cast<nk_symbol_type>(symbol), { size.width, size.height }) != 0;
  }

  bool UI::comboItemLabel(StringRef title, UIAlignment align) {
    setState(State::Setup);
    return nk_combo_item_text(&m_impl->ctx, title.getData(), title.getSize(), static_cast<nk_flags>(align)) != 0;
  }

  bool UI::comboItemSymbolLabel(UISymbol symbol, StringRef title, UIAlignment align) {
    setState(State::Setup);
    return nk_combo_item_symbol_text(&m_impl->ctx, static_cast<nk_symbol_type>(symbol), title.getData(), title.getSize(), static_cast<nk_flags>(align)) != 0;
  }

  void UI::comboClose() {
    setState(State::Setup);
    nk_combo_close(&m_impl->ctx);
  }

  void UI::comboEnd() {
    setState(State::Setup);
    nk_combo_end(&m_impl->ctx);
  }

  bool UI::contextualBegin(UIWindowFlags flags, Vector2f size, const RectF& triggerBounds) {
    setState(State::Setup);
    return nk_contextual_begin(&m_impl->ctx, flags.getValue(), { size.width, size.height }, { triggerBounds.left, triggerBounds.top, triggerBounds.width, triggerBounds.height }) != 0;
  }

  bool UI::contextualItemLabel(StringRef title, UIAlignment align) {
    setState(State::Setup);
    return nk_contextual_item_text(&m_impl->ctx, title.getData(), title.getSize(), static_cast<nk_flags>(align)) != 0;
  }

  bool UI::contextualItemSymbolLabel(UISymbol symbol, StringRef title, UIAlignment align) {
    setState(State::Setup);
    return nk_contextual_item_symbol_text(&m_impl->ctx, static_cast<nk_symbol_type>(symbol), title.getData(), title.getSize(), static_cast<nk_flags>(align)) != 0;
  }

  void UI::contextualClose() {
    setState(State::Setup);
    nk_contextual_close(&m_impl->ctx);
  }

  void UI::contextualEnd() {
    setState(State::Setup);
    nk_contextual_end(&m_impl->ctx);
  }

  void UI::tooltip(const std::string& text) {
    setState(State::Setup);
    nk_tooltip(&m_impl->ctx, text.c_str());
  }

  bool UI::tooltipBegin(float width) {
    setState(State::Setup);
    return nk_tooltip_begin(&m_impl->ctx, width) != 0;
  }

  void UI::tooltipEnd() {
    setState(State::Setup);
    nk_tooltip_end(&m_impl->ctx);
  }

  void UI::menubarBegin() {
    setState(State::Setup);
    nk_menubar_begin(&m_impl->ctx);
  }

  void UI::menubarEnd() {
    setState(State::Setup);
    nk_menubar_end(&m_impl->ctx);
  }

  bool UI::menuBeginLabel(StringRef title, UIAlignment align, Vector2f size) {
    setState(State::Setup);
    return nk_menu_begin_text(&m_impl->ctx, title.getData(), title.getSize(), static_cast<nk_flags>(align), { size.width, size.height }) != 0;
  }

  bool UI::menuBeginSymbol(const std::string& id, UISymbol symbol, Vector2f size) {
    setState(State::Setup);
    return nk_menu_begin_symbol(&m_impl->ctx, id.c_str(), static_cast<nk_symbol_type>(symbol), { size.width, size.height }) != 0;
  }

  bool UI::menuBeginSymbolLabel(UISymbol symbol, StringRef title, UIAlignment align, Vector2f size) {
    setState(State::Setup);
    return nk_menu_begin_symbol_text(&m_impl->ctx, title.getData(), title.getSize(), static_cast<nk_flags>(align), static_cast<nk_symbol_type>(symbol), { size.width, size.height }) != 0;
  }

  bool UI::menuItemLabel(StringRef title, UIAlignment align) {
    setState(State::Setup);
    return nk_menu_item_text(&m_impl->ctx, title.getData(), title.getSize(), static_cast<nk_flags>(align)) != 0;
  }

  bool UI::menuItemSymbolLabel(UISymbol symbol, StringRef title, UIAlignment align) {
    setState(State::Setup);
    return nk_menu_item_symbol_text(&m_impl->ctx, static_cast<nk_symbol_type>(symbol), title.getData(), title.getSize(), static_cast<nk_flags>(align)) != 0;
  }

  void UI::menuClose() {
    setState(State::Setup);
    nk_menu_close(&m_impl->ctx);
  }

  void UI::menuEnd() {
    setState(State::Setup);
    nk_menu_end(&m_impl->ctx);
  }

  RectF UI::getWidgetBounds() {
    setState(State::Setup);
    auto bounds = nk_widget_bounds(&m_impl->ctx);
    return RectF(bounds.x, bounds.y, bounds.w, bounds.h);
  }

  bool UI::isWidgetHovered() {
    setState(State::Setup);
    return nk_widget_is_hovered(&m_impl->ctx);
  }

  /*
   * Predefined styles from nuklear;
   * nuklear/demo/style.c
   */
  void UI::setPredefinedStyle(UIPredefinedStyle style) {
    nk_color table[NK_COLOR_COUNT];

    switch (style) {
      case UIPredefinedStyle::White:
        table[NK_COLOR_TEXT] = nk_rgba(70, 70, 70, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_HEADER] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_BORDER] = nk_rgba(0, 0, 0, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(185, 185, 185, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(170, 170, 170, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(160, 160, 160, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(120, 120, 120, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_SELECT] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(80, 80, 80, 255);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(70, 70, 70, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(60, 60, 60, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_EDIT] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(0, 0, 0, 255);
        table[NK_COLOR_COMBO] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_CHART] = nk_rgba(160, 160, 160, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(45, 45, 45, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(180, 180, 180, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(140, 140, 140, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(160, 160, 160, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(180, 180, 180, 255);
        nk_style_from_table(&m_impl->ctx, table);
        break;

      case UIPredefinedStyle::Red:
        table[NK_COLOR_TEXT] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(30, 33, 40, 215);
        table[NK_COLOR_HEADER] = nk_rgba(181, 45, 69, 220);
        table[NK_COLOR_BORDER] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(190, 50, 70, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(195, 55, 75, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 60, 60, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_SELECT] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(186, 50, 74, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(191, 55, 79, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_EDIT] = nk_rgba(51, 55, 67, 225);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_COMBO] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_CHART] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(170, 40, 60, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(30, 33, 40, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(64, 84, 95, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(70, 90, 100, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(75, 95, 105, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(181, 45, 69, 220);
        nk_style_from_table(&m_impl->ctx, table);
        break;

      case UIPredefinedStyle::Blue:
        table[NK_COLOR_TEXT] = nk_rgba(20, 20, 20, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(202, 212, 214, 215);
        table[NK_COLOR_HEADER] = nk_rgba(137, 182, 224, 220);
        table[NK_COLOR_BORDER] = nk_rgba(140, 159, 173, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(142, 187, 229, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(147, 192, 234, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(177, 210, 210, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(182, 215, 215, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_SELECT] = nk_rgba(177, 210, 210, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(177, 210, 210, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(137, 182, 224, 245);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(142, 188, 229, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(147, 193, 234, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_EDIT] = nk_rgba(210, 210, 210, 225);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(20, 20, 20, 255);
        table[NK_COLOR_COMBO] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_CHART] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(190, 200, 200, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(64, 84, 95, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(70, 90, 100, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(75, 95, 105, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(156, 193, 220, 255);
        nk_style_from_table(&m_impl->ctx, table);
        break;

      case UIPredefinedStyle::Dark:
        table[NK_COLOR_TEXT] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 215);
        table[NK_COLOR_HEADER] = nk_rgba(51, 51, 56, 220);
        table[NK_COLOR_BORDER] = nk_rgba(46, 46, 46, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(58, 93, 121, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(63, 98, 126, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 53, 56, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_SELECT] = nk_rgba(57, 67, 61, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(48, 83, 111, 245);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_EDIT] = nk_rgba(50, 58, 61, 225);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_COMBO] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_CHART] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(48, 83, 111, 255);
        nk_style_from_table(&m_impl->ctx, table);
        break;

      case UIPredefinedStyle::Default:
        nk_style_default(&m_impl->ctx);
        break;
    }
  }

  void UI::draw(RenderTarget &target, RenderStates states) {
    setState(State::Draw);

    nk_convert_config config;

    static const nk_draw_vertex_layout_element vertexLayout[] = {
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

    nk_buffer vertexBuffer;
    nk_buffer elementBuffer;

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
