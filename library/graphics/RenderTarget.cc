/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#include <gf/RenderTarget.h>

#include <cassert>
#include <cstddef>

#include <gf/Drawable.h>
#include <gf/Image.h>
#include <gf/Log.h>
#include <gf/Transform.h>
#include <gf/Vertex.h>
#include <gf/VertexBuffer.h>

#include <gfpriv/GlDebug.h>
#include <gfpriv/GlFwd.h>

#include "generated/default_alpha.frag.h"
#include "generated/default.frag.h"
#include "generated/default.vert.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

#define ENUM_CHECK(GF_VAL, GL_VAL) static_assert(static_cast<int>(GF_VAL) == (GL_VAL), "Problem with " #GL_VAL)

  ENUM_CHECK(RenderAttributeType::Byte, GL_BYTE);
  ENUM_CHECK(RenderAttributeType::UByte, GL_UNSIGNED_BYTE);
  ENUM_CHECK(RenderAttributeType::Short, GL_SHORT);
  ENUM_CHECK(RenderAttributeType::UShort, GL_UNSIGNED_SHORT);
  ENUM_CHECK(RenderAttributeType::Float, GL_FLOAT);

#undef ENUM_CHECK

  void GraphicsTrait<GraphicsTag::Framebuffer>::gen(int n, unsigned* resources) {
    GL_CHECK(glGenFramebuffers(n, resources));
  }

  void GraphicsTrait<GraphicsTag::Framebuffer>::del(int n, const unsigned* resources) {
    GL_CHECK(glDeleteFramebuffers(n, resources));
  }

  namespace {

    constexpr RenderAttributeInfo PredefinedAttributes[] = {
      { "a_position",   2,  RenderAttributeType::Float, false,  offsetof(Vertex, position)  },
      { "a_color",      4,  RenderAttributeType::Float, false,  offsetof(Vertex, color)     },
      { "a_texCoords",  2,  RenderAttributeType::Float, false,  offsetof(Vertex, texCoords) },
    };

    Image createWhitePixel() {
      uint8_t pixel[] = { 0xFF, 0xFF, 0xFF, 0xFF };
      return Image({ 1, 1 }, pixel);
    }
  }

  RenderTarget::RenderTarget(Vector2i size)
  : m_view(RectF::fromSize(size))
  , m_defaultShader(default_vert, default_frag)
  , m_defaultAlphaShader(default_vert, default_alpha_frag)
  , m_defaultTexture(createWhitePixel())
  {
    m_defaultTexture.setRepeated(true);
  }

  RenderTarget::~RenderTarget() = default;

  void RenderTarget::setActive() {
  }

  Region RenderTarget::getCanonicalScissorBox() {
    GLint box[4];
    GL_CHECK(glGetIntegerv(GL_SCISSOR_BOX, &box[0]));
    return { box[0], box[1], box[2], box[3] };
  }

  void RenderTarget::setCanonicalScissorBox(const Region& box) {
    GL_CHECK(glScissor(box.left, box.bottom, box.width, box.height));
  }

  RectI RenderTarget::getScissorBox() {
    Region region = getCanonicalScissorBox();
    Vector2i size = getSize();
    return RectI::fromPositionSize({ region.left, size.height - (region.bottom + region.height) }, { region.width, region.height });
  }

  void RenderTarget::setScissorBox(const RectI& box) {
    Vector2i size = getSize();
    Region region = { box.min.x, size.height - box.max.y, box.getWidth(), box.getHeight() };
    setCanonicalScissorBox(region);
  }

  void RenderTarget::clear(const Color4f& color) {
    GL_CHECK(glClearColor(color.r, color.g, color.b, color.a));
    clear();
  }

  void RenderTarget::clear() {
    Region saved = getCanonicalScissorBox();
    Vector2i size = getSize();
    setCanonicalScissorBox({ 0, 0, size.width, size.height });
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
    setCanonicalScissorBox(saved);
  }

  RangeF RenderTarget::getAliasedLineWidthRange() const {
    float range[2];
    GL_CHECK(glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, range));
    return { range[0], range[1] };
  }

  float RenderTarget::getLineWidth() const {
    float width = 0;
    GL_CHECK(glGetFloatv(GL_LINE_WIDTH, &width));
    return width;
  }


  namespace {

    GLenum getEnum(BlendEquation equation) {
      switch (equation) {
        case BlendEquation::Add:
          return GL_FUNC_ADD;
        case BlendEquation::Substract:
          return GL_FUNC_SUBTRACT;
        case BlendEquation::ReverseSubstract:
          return GL_FUNC_REVERSE_SUBTRACT;
      }

      assert(false);
      return GL_FUNC_ADD;
    }

    GLenum getEnum(BlendFactor func) {
      switch (func) {
        case BlendFactor::Zero:
          return GL_ZERO;
        case BlendFactor::One:
          return GL_ONE;
        case BlendFactor::SrcColor:
          return GL_SRC_COLOR;
        case BlendFactor::OneMinusSrcColor:
          return GL_ONE_MINUS_SRC_COLOR;
        case BlendFactor::DstColor:
          return GL_DST_COLOR;
        case BlendFactor::OneMinusDstColor:
          return GL_ONE_MINUS_DST_COLOR;
        case BlendFactor::SrcAlpha:
          return GL_SRC_ALPHA;
        case BlendFactor::OneMinusSrcAlpha:
          return GL_ONE_MINUS_SRC_ALPHA;
        case BlendFactor::DstAlpha:
          return GL_DST_ALPHA;
        case BlendFactor::OneMinusDstAlpha:
          return GL_ONE_MINUS_DST_ALPHA;
      }

      assert(false);
      return GL_ZERO;
    }

    GLenum getEnum(PrimitiveType type) {
      switch (type) {
        case PrimitiveType::Points:
          return GL_POINTS;
        case PrimitiveType::LineStrip:
          return GL_LINE_STRIP;
        case PrimitiveType::LineLoop:
          return GL_LINE_LOOP;
        case PrimitiveType::Lines:
          return GL_LINES;
        case PrimitiveType::TriangleStrip:
          return GL_TRIANGLE_STRIP;
        case PrimitiveType::TriangleFan:
          return GL_TRIANGLE_FAN;
        case PrimitiveType::Triangles:
          return GL_TRIANGLES;
      }

      assert(false);
      return GL_POINTS;
    }

  } // anonymous namespace

  void RenderTarget::draw(const Vertex *vertices, std::size_t count, PrimitiveType type, const RenderStates& states) {
    if (vertices == nullptr || count == 0) {
      return;
    }

    VertexBuffer buffer(vertices, count, type);
    draw(buffer, states);
  }

  void RenderTarget::draw(const Vertex *vertices, const uint16_t *indices, std::size_t count, PrimitiveType type, const RenderStates& states) {
    if (vertices == nullptr || indices == nullptr || count == 0) {
      return;
    }

    VertexBuffer buffer(vertices, indices, count, type);
    draw(buffer, states);
  }

  void RenderTarget::draw(const VertexBuffer& buffer, const RenderStates& states) {
    customDraw(buffer, PredefinedAttributes, states);
  }

  void RenderTarget::customDraw(const void *vertices, std::size_t size, std::size_t count, PrimitiveType type, Span<const RenderAttributeInfo> attributes, const RenderStates& states) {
    if (vertices == nullptr || count == 0) {
      return;
    }

    VertexBuffer buffer(vertices, size, count, type);
    customDraw(buffer, attributes, states);
  }

  void RenderTarget::customDraw(const void *vertices, std::size_t size, const uint16_t *indices, std::size_t count, PrimitiveType type, Span<const RenderAttributeInfo> attributes, const RenderStates& states) {
    if (vertices == nullptr || indices == nullptr || count == 0) {
      return;
    }

    VertexBuffer buffer(vertices, size, indices, count, type);
    customDraw(buffer, attributes, states);
  }

  void RenderTarget::customDraw(const VertexBuffer& buffer, Span<const RenderAttributeInfo> attributes, const RenderStates& states) {
    if (!buffer.hasArrayBuffer()) {
      return;
    }

    VertexBuffer::bind(&buffer);

    Locations locations;
    drawStart(states, locations, buffer.getVertexSize(), attributes);

    if (buffer.hasElementArrayBuffer()) {
      GL_CHECK(glDrawElements(getEnum(buffer.getPrimitiveType()), static_cast<GLsizei>(buffer.getCount()), GL_UNSIGNED_SHORT, nullptr));
    } else {
      GL_CHECK(glDrawArrays(getEnum(buffer.getPrimitiveType()), 0, static_cast<GLsizei>(buffer.getCount())));
    }

    drawFinish(locations);

    VertexBuffer::bind(nullptr);
  }

  void RenderTarget::drawStart(const RenderStates& states, Locations& locations, std::size_t size, Span<const RenderAttributeInfo> attributes) {
    assert(attributes.getSize() <= Locations::CountMax);

    /*
     * texture
     */

    const BareTexture *texture0 = states.texture[0];
    const BareTexture *texture1 = states.texture[1];

    if (texture0 == nullptr) {
      texture0 = &m_defaultTexture;
    }

    if (texture1 == nullptr) {
      texture1 = &m_defaultTexture;
    }

    /*
     * shader
     */

    Shader *shader = states.shader;

    if (shader == nullptr) {
      switch (texture0->getFormat()) {
        case BareTexture::Format::Alpha:
          shader = &m_defaultAlphaShader;
          break;
        case BareTexture::Format::Color:
          shader = &m_defaultShader;
          break;
        default:
          assert(false);
          break;
      }
    }

    shader->setUniform("u_texture0", *texture0);
    shader->setUniform("u_texture1", *texture1);

    /*
     * transform
     */

    Matrix3f mat = getView().getTransform() * states.transform;
    shader->setUniform("u_transform", mat);

    /*
     * blend mode
     */

    GL_CHECK(glBlendEquationSeparate(getEnum(states.mode.colorEquation), getEnum(states.mode.alphaEquation)));
    GL_CHECK(glBlendFuncSeparate(
      getEnum(states.mode.colorSrcFactor), getEnum(states.mode.colorDstFactor),
      getEnum(states.mode.alphaSrcFactor), getEnum(states.mode.alphaDstFactor)
    ));

    /*
     * line width
     */

    if (states.lineWidth > 0) {
      GL_CHECK(glLineWidth(states.lineWidth));
    }

    /*
     * prepare data
     */

    Shader::bind(shader);

    for (auto info : attributes) {
      int loc = shader->getAttributeLocation(info.name);
      locations.data[locations.count++] = loc;

      if (loc == -1) {
        continue;
      }

      GL_CHECK(glEnableVertexAttribArray(loc));
      const void *pointer = reinterpret_cast<const void *>(info.offset);
      GL_CHECK(glVertexAttribPointer(loc, static_cast<GLsizei>(info.size), static_cast<GLenum>(info.type), info.normalized ? GL_TRUE : GL_FALSE, static_cast<GLsizei>(size), pointer));
    }
  }

  void RenderTarget::drawFinish(const Locations& locations) {
    for (std::size_t i = 0; i < locations.count; ++i) {
      auto loc = locations.data[i];

      if (loc != -1) {
        GL_CHECK(glDisableVertexAttribArray(loc));
      }
    }
  }

  void RenderTarget::draw(Drawable& drawable, const RenderStates& states) {
    drawable.draw(*this, states);
  }


  void RenderTarget::setView(const View& view) {
    m_view = view;

    // set the GL viewport everytime a new view is defined
    Region viewport = getCanonicalViewport(getView());
    GL_CHECK(glViewport(viewport.left, viewport.bottom, viewport.width, viewport.height));

//     Log::info("Viewport: %i %i %i %i\n", viewport.left, viewport.bottom, viewport.width, viewport.height);

    // the viewport does not scissor
    setCanonicalScissorBox(viewport);
  }

  RectI RenderTarget::getViewport(const View& view) const {
    Region region = getCanonicalViewport(view);
    Vector2i size = getSize();
    return RectI::fromPositionSize({ region.left, size.height - (region.bottom + region.height) }, { region.width, region.height });
  }

  Region RenderTarget::getCanonicalViewport(const View& view) const {
    auto size = getSize();
    const RectF& viewport = view.getViewport();

//     gf::Log::info("Normalized viewport: %fx%f %fx%f\n", viewport.left, viewport.top, viewport.width, viewport.height);

    Region region;
    region.left = static_cast<int>(viewport.min.x * size.width + 0.5f);
    region.bottom = static_cast<int>((1.0f - viewport.max.y) * size.height + 0.5f);
    region.width = static_cast<int>(viewport.getWidth() * size.width + 0.5f);
    region.height = static_cast<int>(viewport.getHeight() * size.height + 0.5f);

//     gf::Log::info("Computed viewport: %ix%i %ix%i\n", region.left, region.bottom, region.width, region.height);

    return region;
  }

  Vector2f RenderTarget::mapPixelToCoords(Vector2i point, const View& view) const {
    RectI viewport = getViewport(view);

    /* simulate inverse projection transform
     * i.e. compute normalized device coordinates from screen coordinates
     *
     * 0 +---------+      1 +---------+
     *   |         |        |         |
     *   |         | ===>   |         |
     *   |         |        |         |
     * h +---------+     -1 +---------+
     *   0         w       -1         1
     */
    Vector2f normalized;
    normalized.x = 2.0f * (point.x - viewport.min.x) / viewport.getWidth() - 1;
    normalized.y = 1 - 2.0f * (point.y - viewport.min.y) / viewport.getHeight();

    /* apply inverse view transform
     * i.e. compute world coordinates from normalized device coordinates
     */
    return transform(view.getInverseTransform(), normalized);
  }

  Vector2f RenderTarget::mapPixelToCoords(Vector2i point) const {
    return mapPixelToCoords(point, getView());
  }

  Vector2i RenderTarget::mapCoordsToPixel(Vector2f point, const View& view) const {
    RectI viewport = getViewport(view);

    /* apply view transform
     * i.e. compute normalized device coordinates from world coordinates
     */
    Vector2f normalized = transform(view.getTransform(), point);

    /* simulate projection transform
     * i.e. compute screen coordinates from normalized device coordinates
     *
     *  1 +---------+     0 +---------+
     *    |         |       |         |
     *    |         | ===>  |         |
     *    |         |       |         |
     * -1 +---------+     h +---------+
     *   -1         1       0         w
     */
    Vector2i pixel;
    pixel.x = static_cast<int>((1 + normalized.x) / 2 * viewport.getWidth() + viewport.min.x);
    pixel.y = static_cast<int>((1 - normalized.y) / 2 * viewport.getHeight() + viewport.min.y);

    return pixel;
  }

  Vector2i RenderTarget::mapCoordsToPixel(Vector2f point) const {
    return mapCoordsToPixel(point, getView());
  }

  Image RenderTarget::captureFramebuffer(unsigned name) const {
    auto size = getSize();
    std::vector<uint8_t> pixels(static_cast<std::size_t>(size.width) * static_cast<std::size_t>(size.height) * 4);

    GLint boundFrameBuffer;
    GL_CHECK(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFrameBuffer));

    if (static_cast<unsigned>(boundFrameBuffer) != name) {
      GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, name));
    }

    GL_CHECK(glReadPixels(0, 0, size.width, size.height, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));

    if (static_cast<unsigned>(boundFrameBuffer) != name) {
      GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, boundFrameBuffer));
    }

    Image image(size, pixels.data());
    image.flipHorizontally();
    return image;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
