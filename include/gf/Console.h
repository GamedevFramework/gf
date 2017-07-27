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
#ifndef GF_CONSOLE_H
#define GF_CONSOLE_H

#include <cstdint>

#include "Alignment.h"
#include "Array2D.h"
#include "Blend.h"
#include "Flags.h"
#include "Path.h"
#include "Portability.h"
#include "StringRef.h"
#include "Texture.h"
#include "Transformable.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief Special characters to use in a virtual console
   *
   * The values of these characters use the values of non-printable characters
   * in ASCII and the @f$ [128, 255] @f$ range.
   *
   * @sa gf::Console, gf::ConsoleFont
   */
  enum class SpecialChar : uint8_t {
    // single walls
    WallHorizontalLine        = 196,  ///< A horizontal line
    WallVerticalLine          = 179,  ///< A vertical line
    WallNorthEast             = 191,  ///< A corner in north-east
    WallNorthWest             = 218,  ///< A corner in north-west
    WallSouthEast             = 217,  ///< A corner in south-east
    WallSouthWest             = 192,  ///< A corner in south-west
    WallTeeWest               = 180,  ///< A tee in the west
    WallTeeEast               = 195,  ///< A tee in the east
    WallTeeNorth              = 193,  ///< A tee in the north
    WallTeeSouth              = 194,  ///< A tee in the south
    WallCross                 = 197,  ///< A cross
    // double walls
    DoubleWallHorizontalLine  = 205,  ///< A double horizontal line
    DoubleWallVerticalLine    = 186,  ///< A double vertical line
    DoubleWallNorthEast       = 187,  ///< A double corner in north-east
    DoubleWallNorthWest       = 201,  ///< A double corner in north-west
    DoubleWallSouthEast       = 188,  ///< A double corner in south-east
    DoubleWallSouthWest       = 200,  ///< A double corner in south-west
    DoubleWallTeeWest         = 185,  ///< A double tee in the west
    DoubleWallTeeEast         = 204,  ///< A double tee in the east
    DoubleWallTeeNorth        = 202,  ///< A double tee in the north
    DoubleWallTeeSouth        = 203,  ///< A double tee in the south
    DoubleWallCross           = 206,  ///< A double cross
    // blocks
    Block1                    = 176,  ///< A block of type 1
    Block2                    = 177,  ///< A block of type 2
    Block3                    = 178,  ///< A block of type 3
    // arrows
    ArrowNorth                = 24,   ///< An arrow pointing north
    ArrowSouth                = 25,   ///< An arrow pointing south
    ArrowEast                 = 26,   ///< An arrow pointing east
    ArrowWest                 = 27,   ///< An arrow pointing west
    // arrows without tail
    ArrowNoTailNorth          = 30,   ///< An arrow pointing north with no tail
    ArrowNoTailSouth          = 31,   ///< An arrow pointing south with no tail
    ArrowNoTailEast           = 16,   ///< An arrow pointing east with no tail
    ArrowNoTailWest           = 17,   ///< An arrow pointing west with no tail
    // double arrows
    DoubleArrowHorizontal     = 29,   ///< A horizontal double arrow
    DoubleArrowVertical       = 18,   ///< A vertical double arrow
    // gui
    CheckboxUnset             = 224,  ///< A checkbox unset
    CheckboxSet               = 225,  ///< A checkbox set
    RadioUnset                = 9,    ///< A radio button unset
    RadioSet                  = 10,   ///< A radio button set
    // sub-pixel resolution kit
    SubPixelNorthWest         = 226,  ///< Sub-pixel in the north-west
    SubPixelNorthEast         = 227,  ///< Sub-pixel in the north-east
    SubPixelNorth             = 228,  ///< Sub-pixel in the north
    SubPixelSouthEast         = 229,  ///< Sub-pixel in the south-east
    SubPixelDiagonal          = 230,  ///< Sub-pixel in the diagonal
    SubPixelEast              = 231,  ///< Sub-pixel in the east
    SubPixelSouthWest         = 232,  ///< Sub-pixel in the south-west
    // miscellaneous
    Smilie                    = 1,    ///< A smilie
    SmilieInverted            = 2,    ///< An inverted smilie
    Heart                     = 3,    ///< The heart suit
    Diamond                   = 4,    ///< The diamond suit
    Club                      = 5,    ///< The club suit
    Spade                     = 6,    ///< The spade suit
    Bullet                    = 7,    ///< A bullet
    BulletInverted            = 8,    ///< An inverted bullet
    Male                      = 11,   ///< The male symbol
    Female                    = 12,   ///< The female symbol
    Note                      = 13,   ///< A note
    NoteDouble                = 14,   ///< A double note
    Light                     = 15,   ///< A light
    ExclamationDouble         = 19,   ///< A double exclamation mark
    Pilcrow                   = 20,   ///< A pilcrow (¶)
    Section                   = 21,   ///< A section sign (§)
    Pound                     = 156,  ///< The pound sign (£)
    Multiplication            = 158,  ///< The multiplication sign
    Function                  = 159,  ///< The function sign
    Reserved                  = 169,  ///< The registered trademark sign (®)
    Half                      = 171,  ///< One over two
    OneQuarter                = 172,  ///< One over four
    Copyright                 = 184,  ///< The copyright sign (©)
    Cent                      = 189,  ///< The cent sign (¢)
    Yen                       = 190,  ///< The yen sign (¥)
    Currency                  = 207,  ///< The currency sign (¤)
    ThreeQuarters             = 243,  ///< Three over four
    Division                  = 246,  ///< The division sign
    Grade                     = 248,
    Umlaut                    = 249,  ///< The umlaut diacritics
    Pow1                      = 251,  ///< Power one
    Pow2                      = 252,  ///< Power two
    Pow3                      = 253,  ///< Power three
    BulletSquare              = 254,  ///< A square bullet
  };

  /**
   * @ingroup game
   * @brief The alignment of the text in the console
   *
   * @sa gf::Console
   */
  enum class ConsoleAlignment {
    Left,   ///< Left alignment
    Center, ///< Centered alignment
    Right,  ///< Right alignment
  };

  /**
   * @ingroup game
   * @brief A type for color controls in a console
   *
   * @sa gf::Console
   */
  using ConsoleColorControl = char;

  /**
   * @ingroup game
   * @brief The constant for color control #1
   *
   * @sa gf::Console
   */
  constexpr ConsoleColorControl ConsoleColorControl1 = '\x01';

  /**
   * @ingroup game
   * @brief The constant for color control #2
   *
   * @sa gf::Console
   */
  constexpr ConsoleColorControl ConsoleColorControl2 = '\x02';

  /**
   * @ingroup game
   * @brief The constant for color control #3
   *
   * @sa gf::Console
   */
  constexpr ConsoleColorControl ConsoleColorControl3 = '\x03';

  /**
   * @ingroup game
   * @brief The constant for color control #4
   *
   * @sa gf::Console
   */
  constexpr ConsoleColorControl ConsoleColorControl4 = '\x04';

  /**
   * @ingroup game
   * @brief The constant for color control #5
   *
   * @sa gf::Console
   */
  constexpr ConsoleColorControl ConsoleColorControl5 = '\x05';

  /**
   * @ingroup game
   * @brief The constant for color control stop
   *
   * @sa gf::Console
   */
  constexpr ConsoleColorControl ConsoleColorControlStop = '\x06';


  /**
   * @ingroup game
   * @brief A bitmap console font
   *
   * This class is based on [libtcod](http://roguecentral.org/doryen/libtcod/)
   * console font handling. It supports bitmap fonts provided by libtcod but
   * can also be used to define a custom mapping.
   *
   * @sa gf::Console
   */
  class GF_API ConsoleFont {
  public:

    /**
     * @brief Console font layout
     *
     * A layout specifies how the characters are organized in the bitmap. The
     * predefined layouts are: in column (the default), in row, special
     * (libtcod layout).
     *
     * The transparency information can be given in three ways:
     *
     * - standard: transparency is given by a key color (assumed white)
     * - alpha: transparency is given by the alpha channel of the image
     * - grayscale: transparency is given by the value of gray
     */
    enum class Layout {
      InColumn,           ///< In column
      InRow,              ///< In row
      Special,            ///< Special libtcod mapping
      Custom,             ///< Custom mapping, i.e. not predefined
      GrayscaleInColumn,  ///< In column, grayscale
      GrayscaleInRow,     ///< In row, grayscale
      GrayscaleSpecial,   ///< Special libtcod mapping, grayscale
      GrayscaleCustom,    ///< Custom mapping, grayscale
    };

    /**
     * @brief Constructor
     *
     * No font is loaded.
     */
    ConsoleFont();

    /**
     * @brief Load a console font from a file
     *
     * @param filename The name of the file
     * @param layout The layout of the console font
     * @param size The size, in characters, of the font
     */
    bool loadFromFile(const Path& filename, Layout layout = Layout::InColumn, Vector2u size = { 0u, 0u });

    /**
     * @brief Get the texture rectangle for a character
     *
     * @param c A character
     * @returns The corresponding texture rectangle
     *
     * @sa getTexture()
     */
    RectF getTextureRect(uint8_t c) const;

    /**
     * @brief Get the texture rectangle for a special character
     *
     * @param c A special character
     * @returns The corresponding texture rectangle
     *
     * @sa getTexture()
     */
    RectF getTextureRect(SpecialChar c) const;

    /**
     * @brief Get the texture of the console font
     *
     * @returns The texture of the font
     */
    const AlphaTexture *getTexture() const;

    /**
     * @brief Get the character size
     *
     * @returns The size of the characters in the font
     */
    Vector2u getCharacterSize() const {
      return m_characterSize;
    }

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
    void mapAsciiCode(uint8_t c, Vector2u position);

    /**
     * @brief Map a special character to a position in the font
     *
     * @param c A special character
     * @param position The position, in characters, of the special character
     */
    void mapAsciiCode(SpecialChar c, Vector2u position);

    /**
     * @brief Map a range of characters to consecutive positions in the font
     *
     * @param c The first character in the range
     * @param count The number of character in the range
     * @param position The position, in characters, of the first character
     */
    void mapAsciiCodeRange(uint8_t c, std::size_t count, Vector2u position);

    /**
     * @brief Map characters from a string to consecutive positions in the font
     *
     * @param str The string with the characters
     * @param position The position, in characters, of the first character of the string
     */
    void mapString(StringRef str, Vector2u position);

    /**
     * @}
     */

  private:
    AlphaTexture m_texture;
    std::vector<Vector2u> m_mapping;
    Vector2u m_characterSize;
  };

  /**
   * @brief A console effect on the background color
   *
   * A console effect is used to modify the background color of the console.
   *
   * If @f$ \textbf{b} @f$ is the current background color, and @f$ \textbf{c} @f$
   * is the wanted color, then the result color @f$ \textbf{r} @f$ is given by
   * the following description. Note that ConsoleEffect::AddAlpha and
   * ConsoleEffect::Alpha need an @f$ \alpha @f$ value.
   *
   * @sa gf::Console
   */
  class GF_API ConsoleEffect {
  public:

    /**
     * @brief The kind of console effect
     */
    enum Kind : uint32_t {
      None,       ///< Do not change the background color
      Set,        ///< @f[ \textbf{r} = \textbf{c} @f]
      Multiply,   ///< @f[ \textbf{r} = \textbf{b} \cdot \textbf{c} @f]
      Lighten,    ///< @f[ \textbf{r} = \max(\textbf{b}, \textbf{c}) @f]
      Darken,     ///< @f[ \textbf{r} = \min(\textbf{b}, \textbf{c}) @f]
      Screen,     ///< @f[ \textbf{r} = 1 - (1 - \textbf{b}) \cdot (1 - \textbf{c}) @f]
      ColorDodge, ///< @f[ \textbf{r} = \frac{\textbf{c}}{1 - \textbf{b}} @f]
      ColorBurn,  ///< @f[ \textbf{r} = 1 - \frac{1 - \textbf{b}}{\textbf{c}} @f]
      Add,        ///< @f[ \textbf{r} = \textbf{b} + \textbf{c} @f]
      AddAlpha,   ///< @f[ \textbf{r} = \textbf{b} + \alpha \cdot \textbf{c} @f]
      Burn,       ///< @f[ \textbf{r} = \textbf{b} + \textbf{c} - 1 @f]
      Overlay,    ///< @f[ \textbf{r} = \begin{cases} 2 \cdot \textbf{b} \cdot \textbf{c} & \text{if } \textbf{b} \leq 0.5 \\ 1 - 2 \cdot (1 - \textbf{b}) \cdot (1 - \textbf{c}) & \text{otherwise}  \end{cases} @f]
      Alpha,      ///< @f[ \textbf{r} = (1 - \alpha) \cdot \textbf{b} + \alpha \cdot \textbf{c} @f]
      Default,    ///< Use the default console effect
    };

    /**
     * @brief Constructor with a kind only
     *
     * This constructor sould be used with all the effects, except
     * ConsoleEffect::AddAlpha and ConsoleEffect::Alpha.
     *
     * @param kind The kind of effect
     */
    constexpr ConsoleEffect(Kind kind)
    : m_kind(kind)
    , m_alpha(0.0f)
    {

    }

    /**
     * @brief Constructor with a kind and @f$ \alpha @f$
     *
     * This constructor should be used with ConsoleEffect::AddAlpha and
     * ConsoleEffect::Alpha.
     *
     * @param kind The kind of effect
     * @param alpha The @f$ \alpha @f$ value
     */
    constexpr ConsoleEffect(Kind kind, float alpha)
    : m_kind(kind)
    , m_alpha(alpha)
    {

    }

    /**
     * @brief Get the kind of effect
     */
    constexpr Kind getKind() const noexcept {
      return m_kind;
    }

    /**
     * @brief Check if the effect if Console::Default
     */
    constexpr bool isDefault() const noexcept {
      return m_kind == Default;
    }

    /**
     * @brief Get the alpha value
     */
    constexpr float getAlpha() const noexcept {
      return m_alpha;
    }

  private:
    Kind m_kind;
    float m_alpha;
  };

  /**
   * @ingroup game
   * @brief A virtual console
   *
   * A console is a virtual terminal where you can print the characters from a
   * console font. Each cell of the console has a background color, a
   * foreground color and a 8-bit character.
   *
   * A console has a state with default values for different aspects:
   * - a default background color (initiallly gf::Color::Black)
   * - a default foreground color (initiallly gf::Color::White)
   * - a default console effect (initiallly gf::ConsoleEffect::None)
   * - a default alignment (initiallly gf::ConsoleAlignment::Left)
   *
   * Several functions use this state to determine actual values.
   *
   * @sa gf::ConsoleFont
   */
  class GF_API Console : public Transformable {
  public:
    /**
     * @brief An action when printing
     */
    enum class PrintAction {
      None,   ///< No action
      Clear,  ///< Fill the area with space characters
    };

    /**
     * @brief Constructor
     *
     * @param font A console font
     * @param size The size of the console, in characters
     */
    Console(const ConsoleFont& font, Vector2i size);

    /**
     * @brief Get the width of the console
     */
    int getWidth() const {
      return m_data.getSize().width;
    }

    /**
     * @brief Get the height of the console
     */
    int getHeight() const {
      return m_data.getSize().height;
    }

    /**
     * @name Basic printing
     * @{
     */

    /**
     * @brief Set the default background color
     *
     * @param color The new background color
     * @sa getDefaultBackground()
     */
    void setDefaultBackground(const Color4f& color) {
      m_background = color;
    }

    /**
     * @brief Get the default background color
     *
     * @returns The current default background color
     * @sa setDefaultBackground()
     */
    const Color4f& getDefaultBackground() const {
      return m_background;
    }

    /**
     * @brief Set the default foreground color
     *
     * @param color The new foreground color
     * @sa getDefaultForeground()
     */
    void setDefaultForeground(const Color4f& color) {
      m_foreground = color;
    }

    /**
     * @brief Get the default foreground color
     *
     * @returns The current default foreground color
     * @sa setDefaultForeground()
     */
    const Color4f& getDefaultForeground() const {
      return m_foreground;
    }

    /**
     * @brief Clear the console
     *
     * For each cell of the console, this function:
     * - sets the background color to the default background color
     * - sets the foreground color to the default foreground color
     * - sets the character to space (ASCII 32)
     */
    void clear();

    /**
     * @brief Set the character background color
     *
     * @param position The position of the cell
     * @param color The new background color
     * @param effect The effect to apply to background
     *
     * @sa getCharBackground()
     */
    void setCharBackground(Vector2i position, const Color4f& color, ConsoleEffect effect = ConsoleEffect::Set);

    /**
     * @brief Get the character background color
     *
     * @param position The position of the cell
     * @returns The current character background color
     *
     * @sa setCharBackground()
     */
    const Color4f& getCharBackground(Vector2i position) const;

    /**
     * @brief Set the character foreground color
     *
     * @param position The position of the cell
     * @param color The new foreground color
     *
     * @sa getCharForeground()
     */
    void setCharForeground(Vector2i position, const Color4f& color);

    /**
     * @brief Get the character foreground color
     *
     * @param position The position of the cell
     * @returns The current character foreground color
     *
     * @sa setCharForeground()
     */
    const Color4f& getCharForeground(Vector2i position) const;

    /**
     * @brief Set a character
     *
     * @param position The position of the cell
     * @param c The character
     *
     * @sa putChar(), getChar()
     */
    void setChar(Vector2i position, uint8_t c);

    /**
     * @brief Set a special character
     *
     * @param position The position of the cell
     * @param c The special character
     *
     * @sa putChar(), getChar()
     */
    void setChar(Vector2i position, SpecialChar c);

    /**
     * @brief Get a character
     *
     * @param position The position of the cell
     * @returns The current character at the position
     *
     * @sa setChar()
     */
    uint8_t getChar(Vector2i position) const;

    /**
     * @brief Modify a cell in the console
     *
     * At the specified position, this function:
     * - sets the background color thanks to the effect and the default background color
     * - sets the foreground color thanks to the default foreground color
     * - sets the character to a new value
     *
     * @param position The position of the cell
     * @param c The character to set
     * @param effect The effect to apply to the background
     */
    void putChar(Vector2i position, uint8_t c, ConsoleEffect effect = ConsoleEffect::Default);

    /**
     * @brief Modify a cell in the console
     *
     * At the specified position, this function:
     * - sets the background color thanks to the effect and the default background color
     * - sets the foreground color thanks to the default foreground color
     * - sets the character to a new value
     *
     * @param position The position of the cell
     * @param c The special character to set
     * @param effect The effect to apply to the background
     */
    void putChar(Vector2i position, SpecialChar c, ConsoleEffect effect = ConsoleEffect::Default);

    /**
     * @brief Modify a cell in the console
     *
     * At the specified position, this function:
     * - sets the background color to the specified color
     * - sets the foreground color to the specified color
     * - sets the character to a new value
     *
     * @param position The position of the cell
     * @param c The character to set
     * @param foreground The foreground color to set
     * @param background The background color to set
     */
    void putChar(Vector2i position, uint8_t c, const Color4f& foreground, const Color4f& background);

    /**
     * @brief Modify a cell in the console
     *
     * At the specified position, this function:
     * - sets the background color to the specified color
     * - sets the foreground color to the specified color
     * - sets the character to a new value
     *
     * @param position The position of the cell
     * @param c The special character to set
     * @param foreground The foreground color to set
     * @param background The background color to set
     */
    void putChar(Vector2i position, SpecialChar c, const Color4f& foreground, const Color4f& background);

    /**
     * @}
     */

    /**
     * @name String printing
     * @{
     */

    /**
     * @brief Set the default console effect
     *
     * @param effect The new effect
     *
     * @sa getDefaultConsoleEffect()
     */
    void setDefaultConsoleEffect(ConsoleEffect effect) {
      if (!effect.isDefault()) {
        m_effect = effect;
      }
    }

    /**
     * @brief Get the default console effect
     *
     * @returns The current default console effect
     *
     * @sa setDefaultConsoleEffect()
     */
    ConsoleEffect getDefaultConsoleEffect() const {
      return m_effect;
    }

    /**
     * @brief Set the default alignment
     *
     * @param alignment The new alignment
     *
     * @sa getDefaultAlignment()
     */
    void setDefaultAlignment(ConsoleAlignment alignment) {
      m_alignment = alignment;
    }

    /**
     * @brief Get the default alignment
     *
     * @returns The current default alignment
     *
     * @sa setDefaultAlignment()
     */
    ConsoleAlignment getDefaultAlignment() const {
      return m_alignment;
    }

    /**
     * @brief Print a formatted string
     *
     * This function uses the default values for background color, foreground
     * color, console effect and alignment.
     *
     * The specified position indicates:
     * - The first character of the string if the alignment is gf::ConsoleAlignment::Left
     * - The center character of the string if the alignment is gf::ConsoleAlignment::Center
     * - The last character of the string if the alignment is gf::ConsoleAlignment::Right
     *
     * @param position The position of a cell
     * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
     */
    void print(Vector2i position, const char *fmt, ...);

    /**
     * @brief Print a formatted string
     *
     * This function uses the default values for background color, foreground
     * color but uses the specified console effect and alignment.
     *
     * The specified position indicates:
     * - The first character of the string if the alignment is gf::ConsoleAlignment::Left
     * - The center character of the string if the alignment is gf::ConsoleAlignment::Center
     * - The last character of the string if the alignment is gf::ConsoleAlignment::Right
     *
     * @param position The position of a cell
     * @param effect The effect to apply to the background
     * @param alignment The alignment of the text
     * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
     */
    void print(Vector2i position, ConsoleEffect effect, ConsoleAlignment alignment, const char *fmt, ...);

    /**
     * @brief Print a multi-line formatted string
     *
     * The string is split in paragraphs according to new lines and then split
     * in lines to fit the width of the specified rectangle. If the specified
     * height is reached, the string is truncated. I the height is 0, then the
     * string is truncated at the bottom of the console.
     *
     * @param rect The rectangle to print the string
     * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
     * @returns The height in console lines of the printed string
     */
    int printRect(const RectI& rect, const char *fmt, ...);

    /**
     * @brief Print a multi-line formatted string
     *
     * The string is split in paragraphs according to new lines and then split
     * in lines to fit the width of the specified rectangle. If the specified
     * height is reached, the string is truncated. I the height is 0, then the
     * string is truncated at the bottom of the console.
     *
     * @param rect The rectangle to print the string
     * @param effect The effect to apply to the background
     * @param alignment The alignment of the text
     * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
     * @returns The height in console lines of the printed string
     */
    int printRect(const RectI& rect, ConsoleEffect effect, ConsoleAlignment alignment, const char *fmt, ...);

    /**
     * @brief Get the expected number of console lines of a multi-line formatted string
     *
     * This function does not print anything, it only computes the number of
     * lines that a formatted string would take if it was printed with
     * printRect()
     *
     * @param rect The rectangle to print the string
     * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
     *
     * @sa printRect()
     */
    int getHeight(const RectI& rect, const char *fmt, ...);

    /**
     * @brief Set a color for color control
     *
     * Color control codes allow to change color in a string on the fly. There
     * are five color control codes: gf::ConsoleColorControl1,
     * gf::ConsoleColorControl2, gf::ConsoleColorControl3,
     * gf::ConsoleColorControl4 and  gf::ConsoleColorControl5. Each one is
     * associated with a foreground and a background color that you can set
     * with this function. The color control code gf::ConsoleColorControlStop
     * changes the colors back to their default.
     *
     * ~~~{.cc}
     * gf::Console console(font, size);
     * // A string with a red over black word, using predefined color control codes
     * console.setColorControl(gf::ConsoleColorControl1, gf::Color::Red, gf::Color::Black);
     * console.print({ 1, 1 }, "String with a %cred%c word.", gf::ConsoleColorControl1, gf::ConsoleColorControlStop);
     * ~~~
     */
    void setColorControl(ConsoleColorControl ctrl, const Color4f& foreground, const Color4f& background);

    /**
     * @}
     */

    /**
     * @name Advanced printing
     * @{
     */

    /**
     * @brief Draw a filled rectangle
     *
     * This function fills the specified rectangle with the default background
     * color. If the print action is gf::PrintAction::Clear, then the
     * characters inside the rectangle are set to space (ASCII 32).
     *
     * @param rect The rectangle to print the rectangle
     * @param action The action when drawing
     * @param effect The effect to apply to the background
     */
    void drawRectangle(const RectI& rect, PrintAction action = PrintAction::None, ConsoleEffect effect = ConsoleEffect::Default);

    /**
     * @brief Draw a horizontal line
     *
     * This function draws a horizontal line using gf::SpecialChar::WallHorizontalLine.
     *
     * @param left The left end point of the line
     * @param width The width of the line
     * @param effect The effect to apply to the background
     */
    void drawHorizontalLine(Vector2i left, int width, ConsoleEffect effect = ConsoleEffect::Default);

    /**
     * @brief Draw a vertical line
     *
     * This function draws a vertical line using gf::SpecialChar::WallVerticalLine.
     *
     * @param top The top end point of the line
     * @param height The height of the line
     * @param effect The effect to apply to the background
     */
    void drawVerticalLine(Vector2i top, int height, ConsoleEffect effect = ConsoleEffect::Default);

    /**
     * @brief Draw a frame
     *
     * This funtion first draws a rectangle using the specified action and
     * effect. Then it draws a frame. If a title is given, it is printed on the
     * top of the frame with inverted colors.
     *
     * @param rect The rectangle to print the rectangle
     * @param action The action when drawing
     * @param effect The effect to apply to the background
     * @param title The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf) of the title
     */
    void drawFrame(const RectI& rect, PrintAction action = PrintAction::None, ConsoleEffect effect = ConsoleEffect::Default, const char *title = nullptr, ...);

    /**
     * @}
     */

    /**
     * @name Screen fading
     * @{
     */

    /**
     * @brief Set the fading parameters
     *
     * @param amount The fading amount, with 0 meaning the fading color and 1 meaning no fading
     * @param color The fading color
     */
    void setFade(float amount, const Color4f& color) {
      m_fadingAmount = amount;
      m_fadingColor = color;
    }

    /**
     * @brief Get the fading amount
     *
     * @returns The current fading amount
     */
    float getFadingAmount() const {
      return m_fadingAmount;
    }

    /**
     * @brief Get the fading color
     *
     * @returns The current fading color
     */
    const Color4f& getFadingColor() const {
      return m_fadingColor;
    }

    /**
     * @}
     */

    /**
     * @brief Blit a console on another console
     *
     * @param src The source rectangle on the source console
     * @param con The destination console
     * @param dst The destination position on the destination console
     * @param foregroundAlpha The alpha of the blitted console foreground color, 0 is a completely transparent source console and 1 is a completely opaque source console
     * @param backgroundAlpha The alpha of the blitted console background color, 0 is a completely transparent source console and 1 is a completely opaque source console
     */
    void blit(const RectI& src, Console& con, Vector2i dst, float foregroundAlpha = 1.0f, float backgroundAlpha = 1.0f) const;

    virtual void draw(RenderTarget& target, RenderStates states) override;

  private:
    Color4f computeColor(ConsoleEffect effect, const Color4f& existing, const Color4f& current);

    int putWord(Vector2i position, ConsoleEffect effect, const std::string& message, const Color4f& foreground, const Color4f& background);

    enum class PrintOption {
      Split     = 0x01,
      CountOnly = 0x02,
    };

    using PrintOptionFlags = Flags<PrintOption>;

    int printInternal(const RectI& rect, ConsoleEffect effect, ConsoleAlignment alignment, const std::string& message, PrintOptionFlags flags = None);

  private:
    struct Cell {
      Color4f fg;
      Color4f bg;
      uint8_t c;
    };

    const ConsoleFont *m_font;
    Array2D<Cell, int> m_data;
    Color4f m_background;
    Color4f m_foreground;

    ConsoleEffect m_effect;
    ConsoleAlignment m_alignment;

    struct ColorControl {
      Color4f fg;
      Color4f bg;
    };

    static constexpr char ColorControlCount = 5;
    ColorControl m_controls[ColorControlCount];

    float m_fadingAmount;
    Color4f m_fadingColor;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_CONSOLE_H
