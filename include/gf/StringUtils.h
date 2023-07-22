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
#ifndef GF_STRING_UTILS_H
#define GF_STRING_UTILS_H

#include <cassert>
#include <cstdarg>
#include <cstdint>

#include <vector>
#include <string>
#include <string_view>

#include "CoreApi.h"
#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core_strings
   * @brief Create a string representation of a floating point number
   *
   * It is based on [Python niceNum](https://mail.python.org/pipermail/python-list/1999-October/005748.html).
   *
   * @param num The number to display
   * @param precision The precision to use for display
   * @returns A string representing the number
   */
  GF_CORE_API std::string niceNum(float num, float precision);

  /**
   * @ingroup core_strings
   * @brief Format a string like printf
   *
   * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
   */
  GF_CORE_API std::string formatString(const char *fmt, ...) GF_FORMAT(1, 2);

  /**
   * @ingroup core_strings
   * @brief Format a string like vprintf
   *
   * @param fmt The [format string](http://en.cppreference.com/w/cpp/io/c/fprintf)
   * @param ap The arguments of the format string
   */
  GF_CORE_API std::string formatString(const char *fmt, va_list ap);

  /**
   * @ingroup core_strings
   * @brief Escape a string
   *
   * This function replaces characters with their escaped equivalent. For
   * example, a '\\n' character is replaced with "\\\\n". This function can be
   * used to display strings.
   *
   * @param str The input string
   * @returns An escaped string
   */
  GF_CORE_API std::string escapeString(std::string_view str);

  /**
   * @ingroup core_strings
   * @brief Split a string in multiples paragraphs
   *
   * The paragraphs are separated by '\\n'.
   *
   * @param str The input string
   * @returns A vector of strings containing the paragraphs
   */
  GF_CORE_API std::vector<std::string_view> splitInParagraphs(std::string_view str);

  /**
   * @ingroup core_strings
   * @brief Split a string in multiples words
   *
   * The words are separated by ' ' (space) or '\\t' (tabulation).
   *
   * @param str The input string
   * @returns A vector of strings containing the words
   */
  GF_CORE_API std::vector<std::string_view> splitInWords(std::string_view str);


  /**
   * @ingroup core_strings
   * @brief A range over a sequence of codepoints in UTF-8
   *
   * @sa gf::codepoints
   */
  struct GF_CORE_API CodepointRange {
    std::string_view ref;

    /**
     * @ingroup core_strings
     * @brief Iterator for a range of codepoints
     */
    struct Iterator {
      using difference_type = std::ptrdiff_t;
      using value_type = char32_t;
      using pointer = value_type;
      using reference = value_type;
      using iterator_category = std::forward_iterator_tag;

      /**
       * @brief Swap the iterator with another iterator
       */
      void swap(Iterator& other) noexcept {
        std::swap(current, other.current);
      }

      /**
       * @brief Dereference operator
       *
       * @returns The position
       */
      constexpr reference operator*() const noexcept {
        return decode();
      }

      /**
       * @brief Pointer operator
       *
       * @returns The position
       */
      constexpr pointer operator->() const noexcept {
        return decode();
      }

      /**
       * @brief Increment operator (prefix)
       *
       * @returns The iterator
       */
      constexpr Iterator& operator++() noexcept {
        step();
        return *this;
      }

      /**
       * @brief Increment operator (postfix)
       *
       * @returns The iterator
       */
      constexpr Iterator operator++(int) noexcept {
        Iterator copy = *this;
        step();
        return copy;
      }

      /**
       * @brief Inequality operator
       *
       * @param other Another iterator
       * @return True if the iterator are different
       */
      constexpr bool operator!=(const Iterator& other) const noexcept {
        return current != other.current;
      }

      /**
       * @brief Equality operator
       *
       * @param other Another iterator
       * @return True if the iterator are the same
       */
      constexpr bool operator==(const Iterator& other) const noexcept {
        return current == other.current;
      }

      const char *current;

    private:
      constexpr char32_t decode() const noexcept {
        char32_t codepoint = 0;
        uint8_t c = current[0];

        if ((c & 0b10000000) == 0b00000000) {
          codepoint = c & 0b01111111;
        } else if ((c & 0b11100000) == 0b11000000) {
          codepoint = c & 0b00011111;
          codepoint = (codepoint << 6) + (current[1] & 0b00111111);
        } else if ((c & 0b11110000) == 0b11100000) {
          codepoint = c & 0b00001111;
          codepoint = (codepoint << 6) + (current[1] & 0b00111111);
          codepoint = (codepoint << 6) + (current[2] & 0b00111111);
        } else {
          assert((c & 0b11111000) == 0b11110000);
          codepoint = c & 0b00000111;
          codepoint = (codepoint << 6) + (current[1] & 0b00111111);
          codepoint = (codepoint << 6) + (current[2] & 0b00111111);
          codepoint = (codepoint << 6) + (current[3] & 0b00111111);
        }

        return codepoint;
      }

      constexpr void step() noexcept {
        uint8_t c = current[0];

        if ((c & 0b10000000) == 0b00000000) {
          current += 1;
        } else if ((c & 0b11100000) == 0b11000000) {
          current += 2;
        } else if ((c & 0b11110000) == 0b11100000) {
          current += 3;
        } else {
          assert((c & 0b11111000) == 0b11110000);
          current += 4;
        }
      }
    };

    Iterator begin() const noexcept {
      return Iterator{ ref.data() };
    }

    Iterator end() const noexcept {
      return Iterator{ ref.data() + ref.size() };
    }

  };

  /**
   * @relates CodepointRange
   * @brief Create a range over codepoints from a string
   */
  inline
  constexpr CodepointRange codepoints(std::string_view ref) {
    return CodepointRange{ ref };
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_STRING_UTILS_H
