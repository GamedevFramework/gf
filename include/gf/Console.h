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
 */
#ifndef GF_CONSOLE_H
#define GF_CONSOLE_H

#include <cstdint>
#include <string_view>

#include "Alignment.h"
#include "Array2D.h"
#include "Blend.h"
#include "Color.h"
#include "ConsoleFont.h"
#include "Flags.h"
#include "GraphicsApi.h"
#include "Path.h"
#include "Portability.h"
#include "Texture.h"
#include "Transformable.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif


  /**
   * @ingroup graphics_console
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
   * @ingroup graphics_console
   * @brief A type for color controls in a console
   *
   * @sa gf::Console
   */
  using ConsoleColorControl = char;

  /**
   * @ingroup graphics_console
   * @brief The constant for color control #1
   *
   * @sa gf::Console
   */
  constexpr ConsoleColorControl ConsoleColorControl1 = '\x01';

  /**
   * @ingroup graphics_console
   * @brief The constant for color control #2
   *
   * @sa gf::Console
   */
  constexpr ConsoleColorControl ConsoleColorControl2 = '\x02';

  /**
   * @ingroup graphics_console
   * @brief The constant for color control #3
   *
   * @sa gf::Console
   */
  constexpr ConsoleColorControl ConsoleColorControl3 = '\x03';

  /**
   * @ingroup graphics_console
   * @brief The constant for color control #4
   *
   * @sa gf::Console
   */
  constexpr ConsoleColorControl ConsoleColorControl4 = '\x04';

  /**
   * @ingroup graphics_console
   * @brief The constant for color control #5
   *
   * @sa gf::Console
   */
  constexpr ConsoleColorControl ConsoleColorControl5 = '\x05';

  /**
   * @ingroup graphics_console
   * @brief The constant for color control stop
   *
   * @sa gf::Console
   */
  constexpr ConsoleColorControl ConsoleColorControlStop = '\x06';

  /**
   * @ingroup graphics_console
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
  class GF_GRAPHICS_API ConsoleEffect {
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
   * @ingroup graphics_console
   * @brief A console style
   *
   */
  struct GF_GRAPHICS_API ConsoleStyle {
    Color4f foreground = Color::White;
    Color4f background = Color::Black;
    ConsoleEffect effect = ConsoleEffect(ConsoleEffect::Set);
    ConsoleAlignment alignment = ConsoleAlignment::Left;
  };

  /**
   * @ingroup graphics_console
   * @brief A virtual console
   *
   * A console is a virtual terminal where you can print the characters from a
   * console font. Each cell of the console has a background color, a
   * foreground color and a 8-bit character.
   *
   * @sa gf::ConsoleFont
   */
  class GF_GRAPHICS_API Console : public Transformable {
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
     * @brief Clear the console
     *
     * For each cell of the console, this function:
     * - sets the background color to the style background color
     * - sets the foreground color to the style foreground color
     * - sets the character to space (ASCII 32)
     */
    void clear(const ConsoleStyle& style = ConsoleStyle());

    /**
     * @brief Set the character background color
     *
     * @param position The position of the cell
     * @param color The new background color
     * @param effect The effect to apply to background
     *
     * @sa getCharBackground()
     */
    void setCharBackground(Vector2i position, Color4f color, ConsoleEffect effect = ConsoleEffect::Set);

    /**
     * @brief Get the character background color
     *
     * @param position The position of the cell
     * @returns The current character background color
     *
     * @sa setCharBackground()
     */
    Color4f getCharBackground(Vector2i position) const;

    /**
     * @brief Set the character foreground color
     *
     * @param position The position of the cell
     * @param color The new foreground color
     *
     * @sa getCharForeground()
     */
    void setCharForeground(Vector2i position, Color4f color);

    /**
     * @brief Get the character foreground color
     *
     * @param position The position of the cell
     * @returns The current character foreground color
     *
     * @sa setCharForeground()
     */
    Color4f getCharForeground(Vector2i position) const;

    /**
     * @brief Set a character
     *
     * @param position The position of the cell
     * @param c The character
     *
     * @sa putChar(), getChar()
     */
    void setChar(Vector2i position, char16_t c);

    /**
     * @brief Get a character
     *
     * @param position The position of the cell
     * @returns The current character at the position
     *
     * @sa setChar()
     */
    char16_t getChar(Vector2i position) const;

    /**
     * @brief Modify a cell in the console
     *
     * At the specified position, this function:
     * - sets the background color thanks to the style effect and the style background color
     * - sets the foreground color thanks to the style foreground color
     * - sets the character to a new value
     *
     * @param position The position of the cell
     * @param c The character to set
     * @param style The style to apply
     */
    void putChar(Vector2i position, char16_t c, const ConsoleStyle& style = ConsoleStyle());

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
    void putChar(Vector2i position, char16_t c, Color4f foreground, Color4f background);

    /**
     * @}
     */

    /**
     * @name String printing
     * @{
     */

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
     * @param style The style to apply to the text
     * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
     */
    void print(Vector2i position, const ConsoleStyle& style, const char *fmt, ...) GF_FORMAT(4, 5);

    /**
     * @brief Print a multi-line formatted string
     *
     * The string is split in paragraphs according to new lines and then split
     * in lines to fit the width of the specified rectangle. If the specified
     * height is reached, the string is truncated. I the height is 0, then the
     * string is truncated at the bottom of the console.
     *
     * @param rect The rectangle to print the string
     * @param style The style to apply to the text
     * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
     * @returns The height in console lines of the printed string
     */
    int printRect(const RectI& rect, const ConsoleStyle& style, const char *fmt, ...) GF_FORMAT(4, 5);

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
    int getHeight(const RectI& rect, const char *fmt, ...) GF_FORMAT(3, 4);

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
     * @snippet snippets/doc_class_console.cc console
     */
    void setColorControl(ConsoleColorControl ctrl, Color4f foreground, Color4f background);

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
     * @param style The style to apply
     * @param action The action when drawing
     */
    void drawRectangle(const RectI& rect, const ConsoleStyle& style, PrintAction action = PrintAction::None);

    /**
     * @brief Draw a horizontal line
     *
     * This function draws a horizontal line using gf::SpecialChar::WallHorizontalLine.
     *
     * @param left The left end point of the line
     * @param width The width of the line
     * @param style The style to apply
     */
    void drawHorizontalLine(Vector2i left, int width, const ConsoleStyle& style);

    /**
     * @brief Draw a vertical line
     *
     * This function draws a vertical line using gf::SpecialChar::WallVerticalLine.
     *
     * @param top The top end point of the line
     * @param height The height of the line
     * @param style The style to apply
     */
    void drawVerticalLine(Vector2i top, int height, const ConsoleStyle& style);

    /**
     * @brief Draw a frame
     *
     * This funtion first draws a rectangle using the specified action and
     * effect. Then it draws a frame. If a title is given, it is printed on the
     * top of the frame with inverted colors.
     *
     * @param rect The rectangle to print the rectangle
     * @param style The style to apply
     * @param action The action when drawing
     * @param title The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf) of the title
     */
    void drawFrame(const RectI& rect, const ConsoleStyle& style, PrintAction action = PrintAction::None, const char *title = nullptr, ...) GF_FORMAT(5, 6);

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
    void setFade(float amount, Color4f color) {
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
    Color4f getFadingColor() const {
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

    virtual void draw(RenderTarget& target, const RenderStates& states) override;

  private:
    Color4f computeColor(ConsoleEffect effect, Color4f existing, Color4f current);

    int putWord(Vector2i position, std::string_view message, const ConsoleStyle& style);

    enum class PrintOption {
      Split     = 0x01,
      CountOnly = 0x02,
    };

    int printInternal(const RectI& rect, const std::string& message, const ConsoleStyle& style, Flags<PrintOption> flags = None);

  private:
    struct Cell {
      Color4f fg;
      Color4f bg;
      char16_t c;
    };

    const ConsoleFont *m_font;
    Array2D<Cell, int> m_data;

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
