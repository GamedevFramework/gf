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
#ifndef GF_FONT_H
#define GF_FONT_H

#include <cstdint>
#include <map>

#include "Path.h"
#include "Portability.h"
#include "Texture.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class InputStream;

  /**
   * @ingroup graphics
   * @brief A glyph
   *
   * A glyph is the visual representation of a character.
   *
   * Glyphs are loaded from fonts and put in a texture. In order to draw a
   * glyph, you need to known the bounding rectangle of the glyph and the
   * texture coordinates where the glyph is. Then, you can compute the
   * position of the next glyph with the `advance` field.
   *
   * Generally, you do not have to manipulate glyphs directly. gf::Text
   * can display text and make all the necessary computations for you.
   *
   * @sa gf::Font, gf::Text
   */
  struct GF_API Glyph {
    RectF bounds;         ///< Bouding rectangle of the glyph, in coordinates relative to the baseline
    RectF textureRect;    ///< Texture coordinates of the glyph inside the font's texture
    float advance = 0.0f; ///< Offset to move horizontally to the next character
  };

  /**
   * @ingroup graphics
   * @brief A character font
   *
   * Fonts can be loaded from a file, from memory or from a custom
   * stream, and supports the most common types of fonts. See
   * the `loadFromFile()` function for the complete list of supported formats.
   *
   * Once it is loaded, a gf::Font instance provides three
   * types of information about the font:
   *
   * - Global metrics, such as the line spacing
   * - Per-glyph metrics, such as bounding box or kerning
   * - Pixel representation of glyphs
   *
   * Fonts alone are not very useful: they hold the font data
   * but cannot make anything useful of it. To do so you need to
   * use the gf::Text class, which is able to properly output text
   * with several options such as character size, style, color,
   * position, rotation, etc.
   *
   * This separation allows more flexibility and better performances:
   * indeed a gf::Font is a heavy resource, and any operation on it
   * is slow (often too slow for real-time applications). On the other
   * side, a gf::Text is a lightweight object which can combine the
   * glyphs data and metrics of a gf::Font to display any text on a
   * render target.
   *
   * Note that it is also possible to bind several gf::Text instances
   * to the same gf::Font.
   *
   * It is important to note that the gf::Text instance doesn't
   * copy the font that it uses, it only keeps a reference to it.
   * Thus, a gf::Font must not be destructed while it is
   * used by a gf::Text (i.e. never write a function that
   * uses a local gf::Font instance for creating a text).
   *
   * Usage example:
   *
   * ~~~{.cc}
   * // Declare a new font
   * gf::Font font;
   *
   * // Load it from a file
   * if (!font.loadFromFile("arial.ttf"))
   * {
   *     // error...
   * }
   *
   * // Create a text which uses our font
   * gf::Text text1;
   * text1.setString("Hello World!")
   * text1.setFont(font);
   * text1.setCharacterSize(30);
   *
   * // Create another text using the same font, but with different parameters
   * gf::Text text2;
   * text2.setString("Goodbye world!");
   * text2.setFont(font);
   * text2.setCharacterSize(50);
   * ~~~
   *
   * Apart from loading font files, and passing them to instances
   * of gf::Text, you should normally not have to deal directly
   * with this class. However, it may be useful to access the
   * font metrics or rasterized glyphs for advanced usage.
   */
  class GF_API Font {
  public:
    /**
     * @brief Default constructor
     *
     * This constructor defines an empty font.
     */
    Font();

    /**
     * @brief Destructor
     */
    ~Font();

    /**
     * @brief Deleted copy constructor
     */
    Font(const Font&) = delete;

    /**
     * @brief Deleted copy assignement
     */
    Font& operator=(const Font&) = delete;

    /**
     * @brief Move constructor
     */
    Font(Font&& other);

    /**
     * @brief Move assignement
	   */
    Font& operator=(Font&& other);

    /**
     * @brief Load the font from a file
     *
     * The supported font formats are: TrueType, Type 1, CFF,
     * OpenType, SFNT, X11 PCF, Windows FNT, BDF, PFR and Type 42.
     * Note that this function know nothing about the standard
     * fonts installed on the user's system, thus you can't
     * load them directly.
     *
     * @warning gf cannot preload all the font data in this
     * function, so the file has to remain accessible until
     * the gf::Font object loads a new font or is destroyed.
     *
     * @param filename Path of the font file to load
     * @return True if loading succeeded, false if it failed
     *
     * @sa loadFromMemory(), loadFromStream()
     */
    bool loadFromFile(const Path& filename);

    /**
     * @brief Load the font from a custom stream
     *
     * The supported font formats are: TrueType, Type 1, CFF,
     * OpenType, SFNT, X11 PCF, Windows FNT, BDF, PFR and Type 42.
     *
     * @warning gf cannot preload all the font data in this
     * function, so the stream has to remain accessible until
     * the gf::Font object loads a new font or is destroyed.
     *
     * @param stream Source stream to read from
     * @return True if loading succeeded, false if it failed
     *
     * @sa loadFromFile(), loadFromMemory()
     */
    bool loadFromStream(InputStream& stream);


    /**
     * @brief Load the font from a file in memory
     *
     * The supported font formats are: TrueType, Type 1, CFF,
     * OpenType, SFNT, X11 PCF, Windows FNT, BDF, PFR and Type 42.
     *
     * @warning gf cannot preload all the font data in this
     * function, so the stream has to remain accessible until
     * the gf::Font object loads a new font or is destroyed.
     *
     * @param data Pointer to the file data in memory
     * @param length Length of the data to load, in bytes
     * @return True if loading succeeded, false if it failed
     *
     * @sa loadFromFile(), loadFromStream()
     */
    bool loadFromMemory(const uint8_t *data, std::size_t length);


    /**
     * @brief Retrieve a glyph of the font
     *
     * If the font is a bitmap font, not all character sizes
     * might be available. If the glyph is not available at the
     * requested size, an empty glyph is returned.
     *
     * @param codepoint Unicode code point of the character to get
     * @param characterSize Reference character size
     * @param outlineThickness Thickness of outline (when != 0 the glyph will not be filled)
     *
     * @return The corresponding glyph
     */
    const Glyph& getGlyph(char32_t codepoint, unsigned characterSize, float outlineThickness = 0.0f);

    /**
     * @brief Get the kerning offset of two glyphs
     *
     * The kerning is an extra offset (negative) to apply between two
     * glyphs when rendering them, to make the pair look more "natural".
     * For example, the pair "AV" have a special kerning to make them
     * closer than other characters. Most of the glyphs pairs have a
     * kerning offset of zero, though.
     *
     * @param left Unicode code point of the left character
     * @param right Unicode code point of the right character
     * @param characterSize Reference character size
     *
     * @return Kerning value, in pixels
     */
    float getKerning(char32_t left, char32_t right, unsigned characterSize);

    /**
     * @brief Get the line spacing
     *
     * Line spacing is the vertical offset to apply between two
     * consecutive lines of text.
     *
     * @param characterSize Reference character size
     *
     * @return Line spacing, in pixels
     */
    float getLineSpacing(unsigned characterSize);

    /**
     * @brief Retrieve the texture containing the loaded glyphs of a certain size
     *
     * The contents of the returned texture changes as more glyphs
     * are requested, thus it is not very relevant. It is mainly
     * used internally by gf::Text.
     *
     * @param characterSize Reference character size
     *
     * @return Texture containing the glyphs of the requested size
     */
    const AlphaTexture *getTexture(unsigned characterSize);

    /**
     * @brief Generate the texture for a given character size
     *
     * After this call, the texture corresponding to the size is
     * available through `getTexture`.
     *
     * @param characterSize Reference character size
     */
    void generateTexture(unsigned characterSize);

  private:
    struct Packing {
      unsigned top = 0;
      unsigned bottom = 0;
      unsigned right = 0;
    };

    struct GlyphCache {
      AlphaTexture texture;
      std::map<uint64_t, Glyph> glyphs;
      Packing packing;
    };

  private:
    GlyphCache createCache(unsigned characterSize);
    Glyph createGlyph(char32_t codepoint, unsigned characterSize, float outlineThickness, GlyphCache& cache);

    bool setCurrentCharacterSize(unsigned characterSize);

  private:
    void *m_library;
    void *m_stroker;
    void *m_face;
    unsigned m_currentCharacterSize;
    std::map<unsigned, GlyphCache> m_cache;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_FONT_H
