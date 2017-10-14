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
#include <gf/UI.h>

#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdlib>

#include <string>
#include <type_traits>

#include <iostream>

#include <SDL.h>

#include <gf/Clipboard.h>
#include <gf/Log.h>
#include <gf/Paths.h>
#include <gf/RenderTarget.h>
#include <gf/StringUtils.h>
#include <gf/Transform.h>
#include <gf/Texture.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>
#include <gf/Vertex.h>

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

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif
  /*
   * gf pass some values directly to nuklear without any checks. These static
   * asserts verify at compile time that the values in gf and nuklear are
   * consistent to avoid errors.
   */

#define FLAG_CHECK(GF_VAL, NK_VAL) static_assert(static_cast<uint32_t>(GF_VAL) == NK_VAL, "Problem with " #NK_VAL)
#define ENUM_CHECK(GF_VAL, NK_VAL) static_assert(static_cast<int>(GF_VAL) == NK_VAL, "Problem with " #NK_VAL)

  // checks for UIWindow / nk_panel_flags

  FLAG_CHECK(UIWindow::Border, NK_WINDOW_BORDER);
  FLAG_CHECK(UIWindow::Movable, NK_WINDOW_MOVABLE);
  FLAG_CHECK(UIWindow::Scalable, NK_WINDOW_SCALABLE);
  FLAG_CHECK(UIWindow::Closable, NK_WINDOW_CLOSABLE);
  FLAG_CHECK(UIWindow::Minimizable, NK_WINDOW_MINIMIZABLE);
  FLAG_CHECK(UIWindow::NoScrollbar, NK_WINDOW_NO_SCROLLBAR);
  FLAG_CHECK(UIWindow::Title, NK_WINDOW_TITLE);
  FLAG_CHECK(UIWindow::ScrollAutoHide, NK_WINDOW_SCROLL_AUTO_HIDE);
  FLAG_CHECK(UIWindow::Background, NK_WINDOW_BACKGROUND);
  FLAG_CHECK(UIWindow::ScaleLeft, NK_WINDOW_SCALE_LEFT);
  FLAG_CHECK(UIWindow::NoInput, NK_WINDOW_NO_INPUT);

  // checks for UICollapse / nk_collapse_states

  ENUM_CHECK(UICollapse::Minimized, NK_MINIMIZED);
  ENUM_CHECK(UICollapse::Maximized, NK_MAXIMIZED);

  // checks for UILayout / enum nk_layout_format

  ENUM_CHECK(UILayout::Dynamic, NK_DYNAMIC);
  ENUM_CHECK(UILayout::Static, NK_STATIC);

  // checks for UITree / nk_tree_type

  ENUM_CHECK(UITree::Node, NK_TREE_NODE);
  ENUM_CHECK(UITree::Tab, NK_TREE_TAB);

  // checks for UIAlignment / nk_text_alignment

  ENUM_CHECK(UIAlignment::Left, NK_TEXT_LEFT);
  ENUM_CHECK(UIAlignment::Center, NK_TEXT_CENTERED);
  ENUM_CHECK(UIAlignment::Right, NK_TEXT_RIGHT);

  // check for UIScroll / nk_scroll

  static_assert(sizeof(nk_scroll) == sizeof(UIScroll), "Problem with size of gf::UIScroll");

  // checks for UIButtonBehavior / nk_button_behavior

  ENUM_CHECK(UIButtonBehavior::Default, NK_BUTTON_DEFAULT);
  ENUM_CHECK(UIButtonBehavior::Repeater, NK_BUTTON_REPEATER);

  // checks for UISymbol / nk_symbol_type

  ENUM_CHECK(UISymbol::None, NK_SYMBOL_NONE);
  ENUM_CHECK(UISymbol::X, NK_SYMBOL_X);
  ENUM_CHECK(UISymbol::Underscore, NK_SYMBOL_UNDERSCORE);
  ENUM_CHECK(UISymbol::CircleSolid, NK_SYMBOL_CIRCLE_SOLID);
  ENUM_CHECK(UISymbol::CircleOutline, NK_SYMBOL_CIRCLE_OUTLINE);
  ENUM_CHECK(UISymbol::RectSolid, NK_SYMBOL_RECT_SOLID);
  ENUM_CHECK(UISymbol::RectOutline, NK_SYMBOL_RECT_OUTLINE);
  ENUM_CHECK(UISymbol::TriangleUp, NK_SYMBOL_TRIANGLE_UP);
  ENUM_CHECK(UISymbol::TriangleDown, NK_SYMBOL_TRIANGLE_DOWN);
  ENUM_CHECK(UISymbol::TriangleLeft, NK_SYMBOL_TRIANGLE_LEFT);
  ENUM_CHECK(UISymbol::TriangleRight, NK_SYMBOL_TRIANGLE_RIGHT);
  ENUM_CHECK(UISymbol::Plus, NK_SYMBOL_PLUS);
  ENUM_CHECK(UISymbol::Minus, NK_SYMBOL_MINUS);

  // checks for UIEdit / nk_edit_flags

  FLAG_CHECK(UIEdit::Default, NK_EDIT_DEFAULT);
  FLAG_CHECK(UIEdit::ReadOnly, NK_EDIT_READ_ONLY);
  FLAG_CHECK(UIEdit::AutoSelect, NK_EDIT_AUTO_SELECT);
  FLAG_CHECK(UIEdit::SigEnter, NK_EDIT_SIG_ENTER);
  FLAG_CHECK(UIEdit::AllowTab, NK_EDIT_ALLOW_TAB);
  FLAG_CHECK(UIEdit::NoCursor, NK_EDIT_NO_CURSOR);
  FLAG_CHECK(UIEdit::Selectable, NK_EDIT_SELECTABLE);
  FLAG_CHECK(UIEdit::Clipboard, NK_EDIT_CLIPBOARD);
  FLAG_CHECK(UIEdit::CtrlEnterNewline, NK_EDIT_CTRL_ENTER_NEWLINE);
  FLAG_CHECK(UIEdit::NoHorizontalScroll, NK_EDIT_NO_HORIZONTAL_SCROLL);
  FLAG_CHECK(UIEdit::AlwaysInsertMode, NK_EDIT_ALWAYS_INSERT_MODE);
  FLAG_CHECK(UIEdit::Multiline, NK_EDIT_MULTILINE);
  FLAG_CHECK(UIEdit::GotoEndOnActivate, NK_EDIT_GOTO_END_ON_ACTIVATE);

  // checks for UIEditEvent / nk_edit_events

  FLAG_CHECK(UIEditEvent::Active, NK_EDIT_ACTIVE);
  FLAG_CHECK(UIEditEvent::Inactive, NK_EDIT_INACTIVE);
  FLAG_CHECK(UIEditEvent::Activated, NK_EDIT_ACTIVATED);
  FLAG_CHECK(UIEditEvent::Deactivated, NK_EDIT_DEACTIVATED);
  FLAG_CHECK(UIEditEvent::Commited, NK_EDIT_COMMITED);

  // checks for UIPopup / nk_popup_type

  ENUM_CHECK(UIPopup::Static, NK_POPUP_STATIC);
  ENUM_CHECK(UIPopup::Dynamic, NK_POPUP_DYNAMIC);

#undef ENUM_CHECK
#undef FLAG_CHECK

  static float getTextWidth(nk_handle handle, float characterSize, const char *text, int len) {
    auto font = static_cast<Font *>(handle.ptr);

    std::string originalText(text, len);
    std::u32string unicodeText = computeUnicodeString(originalText);

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

  static void clipboardPaste(nk_handle usr, struct nk_text_edit *edit) {
    gf::unused(usr);

    auto text = Clipboard::getString();

    if (!text.empty()) {
      nk_textedit_paste(edit, text.c_str(), text.size());
    }
  }

  static void clipboardCopy(nk_handle usr, const char *text, int len) {
    gf::unused(usr);

    if (len == 0) {
      return;
    }

    std::string str(text, len);
    Clipboard::setString(str);
  }

  struct UI::UIImpl {
    State state;
    Font *font;
    nk_user_font user;
    nk_context ctx;
    nk_buffer cmds;
  };

  UI::UI(Font& font, unsigned characterSize)
  : m_impl(new UIImpl)
  {
    m_impl->state = State::Start;

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
    ctx->clip.copy = clipboardCopy;
    ctx->clip.paste = clipboardPaste;
    ctx->clip.userdata = nk_handle_ptr(nullptr);

    nk_buffer_init_default(&m_impl->cmds);
  }

  UI::~UI() {
    nk_buffer_free(&m_impl->cmds);
    nk_free(&m_impl->ctx);
  }

  UI::UI(UI&&) = default;

  UI& UI::operator=(UI&&) = default;

  void UI::processEvent(const Event& event) {
    setState(State::Input);

    switch (event.type) {
      case EventType::MouseMoved:
        nk_input_motion(&m_impl->ctx, event.mouseCursor.coords.x, event.mouseCursor.coords.y);
        break;

      case EventType::MouseWheelScrolled:
        nk_input_scroll(&m_impl->ctx, nk_vec2(event.mouseWheel.offset.x, event.mouseWheel.offset.y));
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

        break;
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

          case Keycode::A:
            if (event.key.modifiers.test(Mod::Control)) {
              nk_input_key(&m_impl->ctx, NK_KEY_TEXT_SELECT_ALL, down);
            }
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

        break;
      }

      case EventType::TextEntered: {
        nk_glyph glyph;
        std::memcpy(glyph, event.text.rune.data, NK_UTF_SIZE);
        nk_input_glyph(&m_impl->ctx, glyph);
        break;
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

  RectF UI::windowGetBounds() {
    setState(State::Setup);
    auto bounds = nk_window_get_bounds(&m_impl->ctx);
    return { bounds.x, bounds.y, bounds.w, bounds.y };
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

  void UI::image(const Texture& texture, const RectF& textureRect) {
    setState(State::Setup);
    auto size = texture.getSize();

    struct nk_image image;
    image.handle.ptr = const_cast<Texture *>(&texture);
    image.w = size.width;
    image.h = size.height;

    Vector2u topLeft = size * textureRect.getTopLeft();
    Vector2u bottomRight = size * textureRect.getBottomRight();
    image.region[0] = topLeft.x;
    image.region[1] = topLeft.y;
    image.region[2] = bottomRight.x;
    image.region[3] = bottomRight.y;

    nk_image(&m_impl->ctx, image);
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

  static nk_plugin_filter getPluginFilter(UIEditFilter filter) {
    switch (filter) {
      case UIEditFilter::Default:
        return nk_filter_default;
      case UIEditFilter::Ascii:
        return nk_filter_ascii;
      case UIEditFilter::Float:
        return nk_filter_float;
      case UIEditFilter::Decimal:
        return nk_filter_decimal;
      case UIEditFilter::Hex:
        return nk_filter_hex;
      case UIEditFilter::Oct:
        return nk_filter_oct;
      case UIEditFilter::Binary:
        return nk_filter_binary;
    }

    assert(false);
    return nk_filter_default;
  }

  const UIEditFlags UIEditType::Simple =
      UIEdit::AlwaysInsertMode;

  const UIEditFlags UIEditType::Field = combineFlags(
      UIEdit::AlwaysInsertMode,
      UIEdit::Selectable,
      UIEdit::Clipboard
  );

  const UIEditFlags UIEditType::Box = combineFlags(
      UIEdit::AlwaysInsertMode,
      UIEdit::Selectable,
      UIEdit::Multiline,
      UIEdit::AllowTab,
      UIEdit::Clipboard
  );

  const UIEditFlags UIEditType::Editor = combineFlags(
      UIEdit::Selectable,
      UIEdit::Multiline,
      UIEdit::AllowTab,
      UIEdit::Clipboard
  );

  UIEditEventFlags UI::edit(UIEditFlags flags, BufferRef<char> buffer, std::size_t& length, UIEditFilter filter) {
    setState(State::Setup);
    int len = length;
    nk_flags ret = nk_edit_string(&m_impl->ctx, flags.getValue(), buffer.getData(), &len, buffer.getSize(), getPluginFilter(filter));
    length = len;
    return static_cast<UIEditEvent>(ret);
  }

  namespace {

    struct DirectoryRange {
      DirectoryRange(const Path& directory)
      : path(directory)
      {

      }

      boost::filesystem::directory_iterator begin() const {
        return boost::filesystem::directory_iterator(path);
      }

      boost::filesystem::directory_iterator end() const {
        return boost::filesystem::directory_iterator();
      }

      const Path& path;
    };

  }

  bool UI::fileSelector(UIBrowser& browser, const std::string& title, const RectF& bounds) {
    if (browser.currentPath.empty()) {
      browser.currentPath = Paths::getCurrentPath();
    }

    assert(boost::filesystem::is_directory(browser.currentPath));

    if (!popupBegin(UIPopup::Dynamic, title, combineFlags(UIWindow::Border, UIWindow::Title, UIWindow::Closable), bounds)) {
      return false;
    }

    layoutRowDynamic(25, 1);
    bool dummy = false;

    if (selectableLabel("../", UIAlignment::Left, dummy)) {
      browser.currentPath = browser.currentPath.parent_path();
    }

    std::vector<Path> paths;

    for (boost::filesystem::directory_entry& x : DirectoryRange(browser.currentPath)) {
      paths.push_back(x.path());
    }

    std::sort(paths.begin(), paths.end());

    for (const Path& x : paths) {
      bool selected = x == browser.selectedPath;

      std::string name = x.filename().string();

      if (boost::filesystem::is_directory(x)) {
        name += '/';
      }

      if (selectableLabel(name, UIAlignment::Left, selected)) {
        if (boost::filesystem::is_directory(x)) {
          browser.currentPath = x;
        } else {
          browser.selectedPath = x;
        }
      }
    }

    layoutRowDynamic(25, 1);

    if (buttonLabel("OK")) {
      popupClose();
      popupEnd();
      return false;
    }

    popupEnd();
    return true;
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

  void UI::spacing(int cols) {
    setState(State::Setup);
    nk_spacing(&m_impl->ctx, cols);
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

    auto ctx = &m_impl->ctx;
    auto cmds = &m_impl->cmds;

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
    nk_convert(ctx, cmds, &vertexBuffer, &elementBuffer, &config);

    auto vertices = static_cast<const Vertex *>(nk_buffer_memory_const(&vertexBuffer));
    auto indices = static_cast<const uint16_t *>(nk_buffer_memory_const(&elementBuffer));

    Region box = target.getCanonicalScissorBox();

    for (auto command = nk__draw_begin(ctx, cmds); command != nullptr; command = nk__draw_next(command, cmds, ctx)) {
      if (!command->elem_count) {
        continue;
      }

      states.texture = static_cast<const BareTexture*>(command->texture.ptr);
      target.setScissorBox(RectI(command->clip_rect.x, command->clip_rect.y, command->clip_rect.w, command->clip_rect.h));
      target.draw(vertices, indices, command->elem_count, PrimitiveType::Triangles, states);

      indices += command->elem_count;
    }

    target.setCanonicalScissorBox(box);

    nk_buffer_free(&elementBuffer);
    nk_buffer_free(&vertexBuffer);
  }

  void UI::setState(State state) {
    if (m_impl->state == state) {
      return;
    }

    switch (m_impl->state) {
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

    m_impl->state = state;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
