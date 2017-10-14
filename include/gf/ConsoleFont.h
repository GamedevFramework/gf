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
#ifndef GF_CONSOLE_FONT_H
#define GF_CONSOLE_FONT_H

#include "ArrayRef.h"
#include "Path.h"
#include "Portability.h"
#include "StringRef.h"
#include "Texture.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief A console font format
   *
   * @sa gf::ConsoleFont, gf::PredefinedConsoleFontFormat
   */
  struct GF_API ConsoleFontFormat {
    /**
     * @brief A transparency method
     */
    enum Transparency {
      Alpha,      ///< Given by the alpha channel of the image
      Grayscale,  ///< Given by the level of gray (bitmap font only)
      ColorKey,   ///< Given by a color key
    };

    Transparency transparency; ///< The transparency method of the font

    /**
     * @brief A layout
     */
    enum Layout {
      InColumn, ///< In column
      InRow,    ///< In row
    };

    Layout layout; ///< The layout of the font

    /**
     * @brief A mapping
     */
    enum Mapping {
      CodePage437,          ///< Code page 437
      ModifiedCodePage437,  ///< Code page 437 modified by libtcod
      Special,              ///< The special libtcod mapping
      Custom,               ///< A user-defined mapping
    };

    Mapping mapping; ///< The mapping of the font
  };

  /**
   * @ingroup game
   * @brief A console font element
   *
   * @sa gf::ConsoleFont
   */
  struct GF_API ConsoleFontElement {
    char16_t character; ///< The represented character
    uint8_t index;      ///< The index in the font
  };

  /**
   * @ingroup game
   * @brief Predefined console font formats
   */
  struct GF_API PredefinedConsoleFontFormat {
    /**
     * @brief The libtcod format
     */
    static constexpr ConsoleFontFormat Libtcod = { ConsoleFontFormat::ColorKey, ConsoleFontFormat::InRow, ConsoleFontFormat::Special };

    /**
     * @brief The Dwarf Fortress format
     */
    static constexpr ConsoleFontFormat DwarfFortress = { ConsoleFontFormat::ColorKey, ConsoleFontFormat::InRow, ConsoleFontFormat::CodePage437 };
  };

  /**
   * @ingroup game
   * @brief A console font
   *
   * @sa gf::Console, gf::ConsoleFontFormat, gf::ConsoleFontElement
   */
  class GF_API ConsoleFont {
  public:
    /**
     * @brief Constructor
     *
     * No font is loaded.
     */
    ConsoleFont();

    /**
     * @brief Destructor
     */
    virtual ~ConsoleFont();

    /**
     * @name Properties
     * @{
     */

    /**
     * @brief Get the format of the font
     */
    ConsoleFontFormat getFormat() const {
      return m_format;
    }

    /**
     * @brief Get the size of the fonts
     *
     * @returns The size in characters of the font
     */
    Vector2u getSize() const {
      return m_size;
    }

    /**
     * @brief Get the size of the characters
     *
     * @returns The size in pixels of the characters
     */
    Vector2u getCharacterSize() const {
      return m_characterSize;
    }

    /**
     * @}
     */

    /**
     * @name Mapping
     * @{
     */

    /**
     * @brief Map a character to a position in the font
     *
     * @param c A character
     * @param position The position, in characters, of the character
     */
    void mapCode(char16_t c, Vector2u position);

    /**
     * @brief Map a range of characters to consecutive positions in the font
     *
     * @param c The first character in the range
     * @param count The number of character in the range
     * @param position The position, in characters, of the first character
     */
    void mapCodeRange(char16_t c, unsigned count, Vector2u position);

    /**
     * @brief Map characters from a string to consecutive positions in the font
     *
     * @param str The string with the characters
     * @param position The position, in characters, of the first character of the string
     */
    void mapString(StringRef str, Vector2u position);

    /**
     * @brief Map an element in the font
     *
     * @param element A font element
     */
    void mapElement(ConsoleFontElement element);

    /**
     * @brief Map some elements in the font
     *
     * @param elements The font elements
     */
    void mapElements(ArrayRef<ConsoleFontElement> elements);

    /**
     * @brief Clear the mapping
     *
     * All characters are mapped to the first position in the font.
     */
    void clearMapping();

    /**
     * @}
     */

    /**
     * @name Texture
     * @{
     */

    /**
     * @brief Get the sub-texture rectangle
     *
     * @param c A character
     * @returns The corresponding sub-texture rectangle
     *
     * @sa getTexture(), getTextureRect()
     */
    RectU getSubTexture(char16_t c) const;

    /**
     * @brief Get the texture rectangle for a character
     *
     * @param c A character
     * @returns The corresponding texture rectangle
     *
     * @sa getTexture(), getSubTexture()
     */
    RectF getTextureRect(char16_t c) const;

    /**
     * @brief Get the texture of the console font
     *
     * @returns The texture of the font
     */
    virtual const BareTexture *getTexture() const = 0;

    /**
     * @}
     */

  protected:
    /**
     * @brief Set the format of the font
     *
     * This function sets the format of the font and computes the actual size
     * in characters of the font and the size of the characters.
     *
     * @param format The format of the font
     * @param size The size given by the user, may be @f$ (0, 0) @f$
     * @param imageSize The size of the source image
     */
    bool setFormatAndComputeSizes(ConsoleFontFormat format, Vector2u size, Vector2u imageSize);

    /**
     * @brief Print the format of the font
     *
     * This function logs the format of the font
     *
     * @param filename The filename of the source image
     */
    void logFormat(const Path& filename) const;

    /**
     * @brief Get the color key position
     *
     * This function gives the position of a pixel of the space character in
     * the font.
     *
     * @returns A pixel position where to find the color key
     */
    Vector2u getColorKeyPosition() const;

  private:
    std::vector<uint8_t> m_mapping;
    ConsoleFontFormat m_format;
    Vector2u m_size;
    Vector2u m_characterSize;
  };

  /**
   * @ingroup game
   * @brief A bitmap console font
   *
   * This class is based on [libtcod](http://roguecentral.org/doryen/libtcod/)
   * console font handling. It supports bitmap fonts provided by libtcod.
   *
   * @sa gf::ConsoleFont
   */
  class GF_API BitmapConsoleFont : public ConsoleFont {
  public:

    virtual const BareTexture *getTexture() const override;

    /**
     * @brief Load a console font from a file
     *
     * @param filename The name of the file
     * @param format The format of the font
     * @param size The size, in characters, of the font
     * @returns True if the font was correctly loaded
     */
    bool loadFromFile(const Path& filename, ConsoleFontFormat format, Vector2u size = { 0u, 0u });

  private:
    AlphaTexture m_texture;
  };

  /**
   * @ingroup game
   * @brief A colored console font
   *
   * This class handles colored fonts such as the fonts created for
   * [Dwarf Fortress](http://dwarffortresswiki.org/Tileset_repository).
   */
  class GF_API ColoredConsoleFont : public ConsoleFont {
  public:

    virtual const BareTexture *getTexture() const override;

    /**
     * @brief Load a console font from a file
     *
     * @param filename The name of the file
     * @param format The format of the font
     * @param size The size, in characters, of the font
     * @returns True if the font was correctly loaded
     */
    bool loadFromFile(const Path& filename, ConsoleFontFormat format, Vector2u size = { 0u, 0u });

  private:
    Texture m_texture;
  };


#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_CONSOLE_FONT_H
