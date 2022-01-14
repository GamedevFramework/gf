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
#include <gf/Text.h>

#include <gf/Color.h>
#include <gf/Font.h>
#include <gf/RenderTarget.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  Text::Text()
  : m_color(Color::Black)
  , m_vertices(PrimitiveType::Triangles)
  , m_outlineColor(Color::Black)
  , m_outlineVertices(PrimitiveType::Triangles)
  {

  }

  Text::Text(std::string string, Font& font, unsigned characterSize)
  : m_basic(std::move(string), font, characterSize)
  , m_color(Color::Black)
  , m_vertices(PrimitiveType::Triangles)
  , m_outlineColor(Color::Black)
  , m_outlineVertices(PrimitiveType::Triangles)
  {
    updateGeometry();
  }

  void Text::setString(std::string string) {
    m_basic.setString(std::move(string));
    updateGeometry();
  }

  void Text::setCharacterSize(unsigned characterSize) {
    m_basic.setCharacterSize(characterSize);
    updateGeometry();
  }

  void Text::setFont(Font& font) {
    m_basic.setFont(font);
    updateGeometry();
  }

  void Text::setColor(const Color4f& color) {
    m_color = color;

    for (auto& vertex : m_vertices) {
      vertex.color = color;
    }
  }

  void Text::setOutlineColor(const Color4f& color) {
    m_outlineColor = color;

    for (auto& vertex : m_outlineVertices) {
      vertex.color = color;
    }
  }

  void Text::setOutlineThickness(float thickness) {
    m_basic.setOutlineThickness(thickness);
    updateGeometry();
  }

  void Text::setLineSpacing(float spacingFactor) {
    m_basic.setLineSpacing(spacingFactor);
    updateGeometry();
  }

  void Text::setLetterSpacing(float spacingFactor) {
    m_basic.setLetterSpacing(spacingFactor);
    updateGeometry();
  }

  void Text::setParagraphWidth(float paragraphWidth) {
    m_basic.setParagraphWidth(paragraphWidth);
    updateGeometry();
  }

  void Text::setAlignment(Alignment align) {
    m_basic.setAlignment(align);
    updateGeometry();
  }

  void Text::setAnchor(Anchor anchor) {
    setOriginFromAnchorAndBounds(anchor, getLocalBounds());
  }

  VertexBuffer Text::commitGeometry() const {
    return VertexBuffer(m_vertices.getVertexData(), m_vertices.getVertexCount(), m_vertices.getPrimitiveType());
  }

  VertexBuffer Text::commitOutlineGeometry() const {
    return VertexBuffer(m_outlineVertices.getVertexData(), m_outlineVertices.getVertexCount(), m_outlineVertices.getPrimitiveType());
  }

  void Text::draw(RenderTarget& target, const RenderStates& states) {
    if (m_basic.getFont() == nullptr || m_basic.getCharacterSize() == 0) {
      return;
    }

    RenderStates localStates = states;

    localStates.transform *= getTransform();
    localStates.texture[0] = m_basic.getFontTexture();

    if (m_basic.getOutlineThickness() > 0) {
      target.draw(m_outlineVertices, localStates);
    }

    target.draw(m_vertices, localStates);
  }

  void Text::updateGeometry() {
    m_basic.updateGeometry(m_vertices, m_outlineVertices);
    setColor(m_color);
    setOutlineColor(m_outlineColor);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
