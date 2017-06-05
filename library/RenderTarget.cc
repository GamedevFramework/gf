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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#include <gf/RenderTarget.h>

#include <cassert>
#include <cstddef>

#include <glad/glad.h>

#include <gf/Drawable.h>
#include <gf/Image.h>
#include <gf/Log.h>
#include <gf/Transform.h>
#include <gf/Vertex.h>
#include <gf/VertexBuffer.h>

#include "priv/Debug.h"

#include "generated/default_alpha.frag.h"
#include "generated/default.frag.h"
#include "generated/default.vert.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  RenderTarget::~RenderTarget() {

  }

  Region RenderTarget::getCanonicalScissorBox() {
    GLint box[4];
    glCheck(glGetIntegerv(GL_SCISSOR_BOX, &box[0]));
    return { box[0], box[1], box[2], box[3] };
  }

  void RenderTarget::setCanonicalScissorBox(const Region& box) {
    glCheck(glScissor(box.left, box.bottom, box.width, box.height));
  }

  RectI RenderTarget::getScissorBox() {
    Region region = getCanonicalScissorBox();
    Vector2i size = getSize();
    return RectI(region.left, size.height - (region.bottom + region.height), region.width, region.height);
  }

  void RenderTarget::setScissorBox(const RectI& box) {
    Vector2i size = getSize();
    Region region = { box.left, size.height - (box.top + box.height), box.width, box.height };
    setCanonicalScissorBox(region);
  }

  void RenderTarget::clear(const Color4f& color) {
    glCheck(glClearColor(color.r, color.g, color.b, color.a));
    clear();
  }

  void RenderTarget::clear() {
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
  }

  RangeF RenderTarget::getAliasedLineWidthRange() const {
    float range[2];
    glCheck(glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, range));
    return { range[0], range[1] };
  }

  float RenderTarget::getLineWidth() const {
    float width = 0;
    glCheck(glGetFloatv(GL_LINE_WIDTH, &width));
    return width;
  }


  static GLenum getEnum(BlendEquation equation) {
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

  static GLenum getEnum(BlendFactor func) {
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

  void RenderTarget::draw(const Vertex *vertices, std::size_t count, PrimitiveType type, const RenderStates& states) {
    if (vertices == nullptr || count == 0) {
      return;
    }

    Locations locations;
    drawStart(vertices, states, locations);
    glCheck(glDrawArrays(getEnum(type), 0, count));
    drawFinish(locations);
  }

  void RenderTarget::draw(const Vertex *vertices, const uint16_t *indices, std::size_t count, PrimitiveType type, const RenderStates& states) {
    if (vertices == nullptr || indices == nullptr || count == 0) {
      return;
    }

    Locations locations;
    drawStart(vertices, states, locations);
    static_assert(std::is_same<uint16_t, GLushort>::value, "GLushort is not the same as uint16_t.");
    glCheck(glDrawElements(getEnum(type), count, GL_UNSIGNED_SHORT, indices));
    drawFinish(locations);
  }

  void RenderTarget::draw(const Vertex *vertices, int *first, const std::size_t *count, std::size_t primcount, PrimitiveType type, const RenderStates& states) {
    if (vertices == nullptr || first == nullptr || count == nullptr || primcount == 0) {
      return;
    }

    Locations locations;
    drawStart(vertices, states, locations);

    // simulate glMultiDrawArrays
    for (std::size_t i = 0; i < primcount; ++i) {
      if (count[i] > 0) {
        glCheck(glDrawArrays(getEnum(type), first[i], count[i]));
      }
    }

    drawFinish(locations);
  }

  void RenderTarget::draw(const Vertex *vertices, const uint16_t **indices, const std::size_t *count, std::size_t primcount, PrimitiveType type, const RenderStates& states) {
    if (vertices == nullptr || indices == nullptr || count == nullptr || primcount == 0) {
      return;
    }

    Locations locations;
    drawStart(vertices, states, locations);

    // simulate glMultiDrawElements
    for (std::size_t i = 0; i < primcount; ++i) {
      if (count[i] > 0) {
        glCheck(glDrawElements(getEnum(type), count[i], GL_UNSIGNED_SHORT, indices[i]));
      }
    }

    drawFinish(locations);
  }

  void RenderTarget::draw(const VertexBuffer& buffer, const RenderStates& states) {
    if (!buffer.hasArrayBuffer()) {
      return;
    }

    VertexBuffer::bind(&buffer);

    Locations locations;
    drawStart(nullptr, states, locations);

    if (buffer.hasElementArrayBuffer()) {
      glCheck(glDrawElements(getEnum(buffer.getPrimitiveType()), buffer.getCount(), GL_UNSIGNED_SHORT, nullptr));
    } else {
      glCheck(glDrawArrays(getEnum(buffer.getPrimitiveType()), 0, buffer.getCount()));
    }

    drawFinish(locations);

    VertexBuffer::bind(nullptr);
  }

  void RenderTarget::drawStart(const Vertex *vertices, const RenderStates& states, Locations& locations) {
    /*
     * texture
     */

    const BareTexture *texture = states.texture;

    if (texture == nullptr) {
      texture = &m_defaultTexture;
    }

    /*
     * shader
     */

    Shader *shader = states.shader;

    if (shader == nullptr) {
      switch (texture->getFormat()) {
        case BareTexture::Format::Alpha:
          shader = &m_defaultAlphaShader;
          break;
        case BareTexture::Format::Color:
          shader = &m_defaultShader;
          break;
      }
    }

    shader->setUniform("u_texture", *texture);

    /*
     * transform
     */

    Matrix3f mat = getView().getTransform() * states.transform;
    shader->setUniform("u_transform", mat);

    /*
     * blend mode
     */

    glCheck(glBlendEquationSeparate(getEnum(states.mode.colorEquation), getEnum(states.mode.alphaEquation)));
    glCheck(glBlendFuncSeparate(
      getEnum(states.mode.colorSrcFactor), getEnum(states.mode.colorDstFactor),
      getEnum(states.mode.alphaSrcFactor), getEnum(states.mode.alphaDstFactor)
    ));

    /*
     * line width
     */

    if (states.lineWidth > 0) {
      glCheck(glLineWidth(states.lineWidth));
    }

    /*
     * prepare data
     */

    Shader::bind(shader);

    int positionLoc = shader->getAttributeLocation("a_position");
    int colorLoc = shader->getAttributeLocation("a_color");
    int texCoordsLoc = shader->getAttributeLocation("a_texCoords");

    glCheck(glEnableVertexAttribArray(positionLoc));
    glCheck(glEnableVertexAttribArray(colorLoc));
    glCheck(glEnableVertexAttribArray(texCoordsLoc));

    const void *positionPointer = vertices ? &vertices[0].position : reinterpret_cast<const void *>(offsetof(Vertex, position));
    const void *colorPointer = vertices ? &vertices[0].color : reinterpret_cast<const void *>(offsetof(Vertex, color));
    const void *texCoordsPointer = vertices ? &vertices[0].texCoords : reinterpret_cast<const void *>(offsetof(Vertex, texCoords));

    glCheck(glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), positionPointer));
    glCheck(glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), colorPointer));
    glCheck(glVertexAttribPointer(texCoordsLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), texCoordsPointer));

    locations.positionLoc = positionLoc;
    locations.colorLoc = colorLoc;
    locations.texCoordsLoc = texCoordsLoc;
  }

  void RenderTarget::drawFinish(const Locations& locations) {
    glCheck(glDisableVertexAttribArray(locations.positionLoc));
    glCheck(glDisableVertexAttribArray(locations.colorLoc));
    glCheck(glDisableVertexAttribArray(locations.texCoordsLoc));
  }


  void RenderTarget::draw(Drawable& drawable, const RenderStates& states) {
    drawable.draw(*this, states);
  }

  void RenderTarget::setView(const View& view) {
    m_view = view;

    // set the GL viewport everytime a new view is defined
    Region viewport = getCanonicalViewport(getView());
    glCheck(glViewport(viewport.left, viewport.bottom, viewport.width, viewport.height));

//     Log::info("Viewport: %i %i %i %i\n", viewport.left, viewport.bottom, viewport.width, viewport.height);

    // the viewport does not scissor
    setCanonicalScissorBox(viewport);
  }

  RectI RenderTarget::getViewport(const View& view) const {
    Region region = getCanonicalViewport(view);
    Vector2i size = getSize();
    return RectI(region.left, size.height - (region.bottom + region.height), region.width, region.height);
  }

  Region RenderTarget::getCanonicalViewport(const View& view) const {
    auto size = getSize();
    const RectF& viewport = view.getViewport();

//     gf::Log::info("Normalized viewport: %fx%f %fx%f\n", viewport.left, viewport.top, viewport.width, viewport.height);

    Region region;
    region.left = static_cast<int>(viewport.left * size.width + 0.5f);
    region.bottom = static_cast<int>((1.0f - (viewport.top + viewport.height)) * size.height + 0.5f);
    region.width = static_cast<int>(viewport.width * size.width + 0.5f);
    region.height = static_cast<int>(viewport.height * size.height + 0.5f);

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
    normalized.x = 2.0f * (point.x - viewport.left) / viewport.width - 1;
    normalized.y = 1 - 2.0f * (point.y - viewport.top) / viewport.height;

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
    pixel.x = static_cast<int>((1 + normalized.x) / 2 * viewport.width + viewport.left);
    pixel.y = static_cast<int>((1 - normalized.y) / 2 * viewport.height + viewport.top);

    return pixel;
  }

  Vector2i RenderTarget::mapCoordsToPixel(Vector2f point) const {
    return mapCoordsToPixel(point, getView());
  }

  void RenderTarget::initialize() {
    initializeViews();
    initializeTexture();
    initializeShader();
  }

  Image RenderTarget::captureFramebuffer(unsigned name) const {
    auto size = getSize();
    std::vector<uint8_t> pixels(size.width * size.height * 4);

    GLint boundFrameBuffer;
    glCheck(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFrameBuffer));

    if (static_cast<unsigned>(boundFrameBuffer) != name) {
      glCheck(glBindFramebuffer(GL_FRAMEBUFFER, name));
    }

    glCheck(glReadPixels(0, 0, size.width, size.height, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));

    if (static_cast<unsigned>(boundFrameBuffer) != name) {
      glCheck(glBindFramebuffer(GL_FRAMEBUFFER, boundFrameBuffer));
    }

    Image image;
    image.create(size, pixels.data());
    image.flipHorizontally();
    return image;
  }


  void RenderTarget::initializeViews() {
    auto size = getSize();
    m_view.reset({ 0.0f, 0.0f, static_cast<float>(size.width), static_cast<float>(size.height) });
  }

  void RenderTarget::initializeShader() {
    m_defaultShader.loadFromMemory(default_vert, default_frag);
    m_defaultAlphaShader.loadFromMemory(default_vert, default_alpha_frag);
  }

  void RenderTarget::initializeTexture() {
    uint8_t pixel[] = { 0xFF, 0xFF, 0xFF, 0xFF };

    Image image;
    image.create({ 1, 1 }, pixel);

    m_defaultTexture.loadFromImage(image);
    m_defaultTexture.setRepeated(true);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
