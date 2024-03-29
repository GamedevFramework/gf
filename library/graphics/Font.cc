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
#include <gf/Font.h>

#include <cstring>

#include <stdexcept>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#include <gf/Stream.h>
#include <gf/Log.h>
#include <gf/VectorOps.h>

#include <gfpriv/GlDebug.h>
#include <gfpriv/GlFwd.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  static constexpr float Scale = (1 << 6);

  namespace {

    float convert(FT_Pos value) {
      return static_cast<float>(value) / Scale;
    }

    uint64_t makeKey(char32_t codepoint, float thickness) {
      static_assert(sizeof(float) == sizeof(uint32_t), "Float is not 32 bits.");
      uint32_t hex;
      std::memcpy(&hex, &thickness, sizeof(float));
      uint64_t key = codepoint | static_cast<uint64_t>(hex) << 32;
      return key;
    }

    const char *FT_ErrorMessage(FT_Error error) {
      switch (error) {

        #undef FTERRORS_H_
        #define FT_ERRORDEF(e, v, s)  \
        case v:                       \
          return s;
        #include FT_ERRORS_H

        default:
          break;
      };

      return "unknown error";
    }

    unsigned long callbackRead(FT_Stream rec, unsigned long offset, unsigned char* buffer, unsigned long count) {
      InputStream *stream = static_cast<InputStream*>(rec->descriptor.pointer);
      stream->seek(offset);

      if (count == 0) {
        return 0;
      }

      return static_cast<unsigned long>(stream->read(Span<uint8_t>(buffer, count)));
    }

    void callbackClose([[maybe_unused]] FT_Stream rec) {
      // nothing to do
    }

  } // anonymous namespace

  Font::Font()
  : m_library(nullptr)
  , m_stroker(nullptr)
  , m_face(nullptr)
  , m_currentCharacterSize(0)
  {
    FT_Library library;

    if (auto err = FT_Init_FreeType(&library)) {
      Log::error("Could not init Freetype library: %s\n", FT_ErrorMessage(err));
      throw std::runtime_error("Could not init Freetype library");
    }

    m_library = library;

    FT_Stroker stroker;

    if (auto err = FT_Stroker_New(library, &stroker)) {
      Log::error("Could not create the stroker: %s\n", FT_ErrorMessage(err));
      throw std::runtime_error("Could not create the stroker");
    }

    m_stroker = stroker;
  }

  Font::Font(const Path& filename)
  : Font()
  {
    FT_Library library = static_cast<FT_Library>(m_library);

    // load face

    FT_Face face = nullptr;

    if (auto err = FT_New_Face(library, filename.string().c_str(), 0, &face)) {
      Log::error("Could not create the font face '%s': %s\n", filename.string().c_str(), FT_ErrorMessage(err));
      throw std::runtime_error("Could not create the font face");
    }

    m_face = face;
  }

  Font::Font(InputStream& stream)
  : Font()
  {
    FT_Library library = static_cast<FT_Library>(m_library);

    // load face

    FT_StreamRec rec;
    std::memset(&rec, 0, sizeof(FT_StreamRec));
    rec.base = nullptr;
    rec.size = 0x7FFFFFFF; // unknown size
    rec.pos = 0;
    rec.descriptor.pointer = &stream;
    rec.read = callbackRead;
    rec.close = callbackClose;

    FT_Open_Args args;
    std::memset(&args, 0, sizeof(FT_Open_Args));
    args.flags = FT_OPEN_STREAM;
    args.stream = &rec;
    args.driver = nullptr;

    FT_Face face = nullptr;

    if (auto err = FT_Open_Face(library, &args, 0, &face)) {
      Log::error("Could not create the font face from stream: %s\n", FT_ErrorMessage(err));
      throw std::runtime_error("Could not create the font face from stream");
    }

    m_face = face;
  }

  Font::Font(Span<const uint8_t> content)
  : Font()
  {
    FT_Library library = static_cast<FT_Library>(m_library);

    // load face

    FT_Face face = nullptr;

    if (auto err = FT_New_Memory_Face(library, static_cast<const FT_Byte*>(content.getData()), static_cast<FT_Long>(content.getSize()), 0, &face)) {
      Log::error("Could not create the font face: %s\n", FT_ErrorMessage(err));
      throw std::runtime_error("Could not create the font face");
    }

    m_face = face;
  }

  Font::~Font() {
    if (m_face != nullptr) {
      if (auto err = FT_Done_Face(static_cast<FT_Face>(m_face))) {
        Log::error("Could not destroy the font face: %s\n", FT_ErrorMessage(err));
      }
    }

    if (m_stroker != nullptr) {
      // no possible error for this call
      FT_Stroker_Done(static_cast<FT_Stroker>(m_stroker));
    }

    if (m_library != nullptr) {
      if (auto err = FT_Done_FreeType(static_cast<FT_Library>(m_library))) {
        Log::error("Could not destroy Freetype library: %s\n", FT_ErrorMessage(err));
      }
    }
  }

  Font::Font(Font&& other) noexcept
  : m_library(std::exchange(other.m_library, nullptr))
  , m_stroker(std::exchange(other.m_stroker, nullptr))
  , m_face(std::exchange(other.m_face, nullptr))
  , m_currentCharacterSize(other.m_currentCharacterSize)
  , m_cache(std::move(other.m_cache))
  {

  }

  Font& Font::operator=(Font&& other) noexcept {
    std::swap(m_library, other.m_library);
    std::swap(m_stroker, other.m_stroker);
    std::swap(m_face, other.m_face);
    std::swap(m_cache, other.m_cache);
    return *this;
  }

  const Glyph& Font::getGlyph(char32_t codepoint, unsigned characterSize, float outlineThickness) {
    auto cacheIt = m_cache.find(characterSize);

    if (cacheIt == m_cache.end()) {
      GlyphCache cache = createCache();
      std::tie(cacheIt, std::ignore) = m_cache.insert(std::make_pair(characterSize, std::move(cache)));
    }

    GlyphCache& cache = cacheIt->second;

    uint64_t key = makeKey(codepoint, outlineThickness);
    auto glyphIt = cache.glyphs.find(key);

    if (glyphIt == cache.glyphs.end()) {
      Glyph glyph = createGlyph(codepoint, characterSize, outlineThickness, cache);
      std::tie(glyphIt, std::ignore) = cache.glyphs.insert(std::make_pair(key, glyph));
    }

    return glyphIt->second;
  }

  float Font::getKerning(char32_t left, char32_t right, unsigned characterSize) {
    if (left == 0 || right == 0) {
      return 0.0f;
    }

    if (m_face == nullptr) {
      return 0.0f;
    }

    if (!setCurrentCharacterSize(characterSize)) {
      return 0.0f;
    }

    FT_Face face = static_cast<FT_Face>(m_face);

    if (!FT_HAS_KERNING(face)) {
      return 0.0f;
    }

    auto indexLeft = FT_Get_Char_Index(face, left);
    auto indexRight = FT_Get_Char_Index(face, right);

    FT_Vector kerning;
    if (auto err = FT_Get_Kerning(face, indexLeft, indexRight, FT_KERNING_UNFITTED, &kerning)) {
      Log::warning("Could not get kerning: %s\n", FT_ErrorMessage(err));
    }

    return convert(kerning.x);
  }

  float Font::getLineSpacing(unsigned characterSize) {
    if (m_face == nullptr) {
      return 0.0f;
    }

    if (!setCurrentCharacterSize(characterSize)) {
      return 0.0f;
    }

    FT_Face face = static_cast<FT_Face>(m_face);
    return convert(face->size->metrics.height);
  }

  const AlphaTexture *Font::getTexture(unsigned characterSize) {
    auto it = m_cache.find(characterSize);

    if (it == m_cache.end()) {
      return nullptr;
    }

    GlyphCache& cache = it->second;
    return &cache.texture;
  }

  void Font::generateTexture(unsigned characterSize) {
    getGlyph(' ', characterSize, 0);
  }

  static constexpr int DefaultSize = 4096;

  Font::GlyphCache Font::createCache() {
    GlyphCache cache;

    AlphaTexture texture({ DefaultSize, DefaultSize });
    cache.texture = std::move(texture);

    return cache;
  }

  static constexpr int Padding = 1;

  Glyph Font::createGlyph(char32_t codepoint, unsigned characterSize, float outlineThickness, GlyphCache& cache) {
    Glyph out;

    if (m_face == nullptr) {
      return out;
    }

    if (!setCurrentCharacterSize(characterSize)) {
      return out;
    }

    FT_Face face = static_cast<FT_Face>(m_face);

    FT_Int32 flags = FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT;

    if (outlineThickness > 0) {
      flags |= FT_LOAD_NO_BITMAP;
    }

    if (auto err = FT_Load_Char(face, codepoint, flags)) {
      Log::error("Could not load the glyph: %s\n", FT_ErrorMessage(err));
      return out;
    }

    FT_GlyphSlot slot = face->glyph;

    FT_Glyph glyph;

    if (auto err = FT_Get_Glyph(slot, &glyph)) {
      Log::error("Could not extract the glyph: %s\n", FT_ErrorMessage(err));
      return out;
    }

    if (outlineThickness > 0) {
      assert(glyph->format == FT_GLYPH_FORMAT_OUTLINE);

      FT_Stroker stroker = static_cast<FT_Stroker>(m_stroker);

      FT_Stroker_Set(stroker, static_cast<FT_Fixed>(outlineThickness * Scale), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
      FT_Glyph_Stroke(&glyph, stroker, 0);
    }

    if (auto err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, 1)) {
      Log::error("Could create a bitmap from the glyph: %s\n", FT_ErrorMessage(err));
      FT_Done_Glyph(glyph);
      return out;
    }

    assert(glyph->format == FT_GLYPH_FORMAT_BITMAP);
    FT_BitmapGlyph bglyph = reinterpret_cast<FT_BitmapGlyph>(glyph);

    // advance

    out.advance = convert(slot->metrics.horiAdvance);

    // size

    Vector2i glyphSize(bglyph->bitmap.width, bglyph->bitmap.rows);

    if (glyphSize.height == 0 || glyphSize.width == 0) {
      FT_Done_Glyph(glyph);
      return out;
    }

    // textureRect (dumb online 2D bin packing)

    glyphSize += gf::vec(2 * Padding, 2 * Padding);

    auto textureSize = cache.texture.getSize();

    if (cache.packing.right + glyphSize.width > textureSize.width) {
      cache.packing.right = 0;
      cache.packing.top = cache.packing.bottom;
    }

    if (cache.packing.top + glyphSize.height > textureSize.height) {
      // TODO: Houston, we have a problem!

      Log::error("Could not add a new glyph to the cache\n");
      FT_Done_Glyph(glyph);
      return out;
    }

    RectI rect = RectI::fromPositionSize({ cache.packing.right, cache.packing.top }, glyphSize);

    if (rect.max.y > cache.packing.bottom) {
      cache.packing.bottom = rect.max.y;
    }

    cache.packing.right += (rect.max.x - rect.min.x);

    out.textureRect = cache.texture.computeTextureCoords(rect.shrink(Padding));

    // bounds

    if (outlineThickness == 0.0f) {
      out.bounds = RectF::fromPositionSize({ convert(slot->metrics.horiBearingX), - convert(slot->metrics.horiBearingY) }, { convert(slot->metrics.width), convert(slot->metrics.height) });
    } else {
      out.bounds = RectF::fromPositionSize( { static_cast<float>(bglyph->left), - static_cast<float>(bglyph->top) }, { static_cast<float>(bglyph->bitmap.width), static_cast<float>(bglyph->bitmap.rows) });
    }

    // bitmap

    auto size = rect.getSize();

    std::vector<uint8_t> paddedBuffer(static_cast<std::size_t>(size.width) * static_cast<std::size_t>(size.height), 0);
    const uint8_t *sourceBuffer = bglyph->bitmap.buffer;

    for (int y = Padding; y < size.height - Padding; ++y) {
      for (int x = Padding; x < size.width - Padding; ++x) {
        int index = y * size.width + x;
        paddedBuffer[index] = sourceBuffer[x - Padding];
      }

      sourceBuffer += bglyph->bitmap.pitch;
    }

    cache.texture.update(paddedBuffer.data(), rect);

    GL_CHECK(glFlush());

    FT_Done_Glyph(glyph);
    return out;
  }

  bool Font::setCurrentCharacterSize(unsigned characterSize) {
    if (m_currentCharacterSize == characterSize) {
      return true;
    }

    FT_Face face = static_cast<FT_Face>(m_face);

    if (auto err = FT_Set_Pixel_Sizes(face, 0, characterSize)) {
      Log::error("Could not change the font size: %s\n", FT_ErrorMessage(err));
      return false;
    }

    m_currentCharacterSize = characterSize;
    return true;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
