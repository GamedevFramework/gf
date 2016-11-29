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

#include <iostream>

#include <gf/RenderTarget.h>
#include <gf/Vertex.h>

#include "priv/String.h"

// #define NK_PRIVATE
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#define NK_MEMSET std::memset
#define NK_MEMCPY std::memcpy
#define NK_SQRT std::sqrt
#define NK_SIN std::sin
#define NK_COS std::cos
#define NK_STRTOD std::strtod

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

  static constexpr float FontHeight = 14;

  struct UI::Impl {
    nk_font_atlas atlas;
    nk_context ctx;
    nk_buffer cmds;
    AlphaTexture texture;
  };

  UI::UI()
  : m_impl(nullptr)
  , m_state(State::Start)
  {
    m_impl = new Impl;

    auto atlas = &m_impl->atlas;

    nk_font_atlas_init_default(atlas);
    nk_font_atlas_begin(atlas);

    auto font = nk_font_atlas_add_default(atlas, FontHeight, nullptr);

    int w, h;
    const void *image = nk_font_atlas_bake(atlas, &w, &h, NK_FONT_ATLAS_ALPHA8);

    auto texture = &m_impl->texture;

    texture->create({ static_cast<unsigned>(w), static_cast<unsigned>(h) });
    texture->update(static_cast<const uint8_t *>(image));

    nk_font_atlas_end(atlas, nk_handle_ptr(texture), nullptr);

    auto ctx = &m_impl->ctx;
    nk_init_default(ctx, nullptr);

    nk_style_set_font(ctx, &font->handle);

    nk_buffer_init_default(&m_impl->cmds);
  }

  UI::~UI() {
    nk_buffer_free(&m_impl->cmds);
    nk_free(&m_impl->ctx);
    nk_font_atlas_clear(&m_impl->atlas);
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

  void UI::layoutRowStatic(float height, int itemWidth, int cols) {
    setState(State::Setup);
    nk_layout_row_static(&m_impl->ctx, height, itemWidth, cols);
  }

  bool UI::buttonLabel(const std::string& title) {
    setState(State::Setup);
    return nk_button_label(&m_impl->ctx, title.c_str());
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
