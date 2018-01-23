/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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
#ifndef GF_DATA_H
#define GF_DATA_H

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include <algorithm>
#include <array>
#include <iterator>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "DataObject.h"
#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief Conversion functions for data objects
   *
   * @sa gf::DataObject
   */
  struct GF_API Data {
    /**
     * @brief Deleted constructor
     */
    Data() = delete;

    /**
     * @brief Convert a primitive object to a data object
     *
     * @param data The data to be converted
     * @returns A data object
     */
    template<typename T>
    static DataObject convert(T data) {
      return DataObject(data);
    }

    /**
     * @brief Convert a string to a data object
     *
     * @param data The data to be converted
     * @returns A data object
     */
    static DataObject convert(const char *data) {
      return convertString(data, data + std::strlen(data));
    }

    /**
     * @brief Convert a string to a data object
     *
     * @param data The data to be converted
     * @returns A data object
     */
    static DataObject convert(const std::string& data) {
      return convertString(data.begin(), data.end());
    }

    /**
     * @brief Convert a blob to a data object
     *
     * @param data The data to be converted
     * @returns A data object
     */
    template<std::size_t N>
    static DataObject convert(const uint8_t (&data)[N]) {
      return convertBinary(data, data + N, N);
    }

    /**
     * @brief Convert a blob to a data object
     *
     * @param data The data to be converted
     * @returns A data object
     */
    static DataObject convert(const std::vector<uint8_t>& data) {
      return convertBinary(data.begin(), data.end(), data.size());
    }

    /**
     * @brief Convert a blob to a data object
     *
     * @param data The data to be converted
     * @returns A data object
     */
    template<std::size_t N>
    static DataObject convert(const std::array<uint8_t,N>& data) {
      return convertBinary(data.begin(), data.end(), data.size());
    }

    /**
     * @brief Convert an array to a data object
     *
     * @param data The data to be converted
     * @returns A data object
     */
    template<typename T, std::size_t N>
    static DataObject convert(const T (&data)[N]) {
      return convertArray(data, data + N, N);
    }

    /**
     * @brief Convert an array to a data object
     *
     * @param data The data to be converted
     * @returns A data object
     */
    template<typename T>
    static DataObject convert(const std::vector<T>& data) {
      return convertArray(data.begin(), data.end(), data.size());
    }

    /**
     * @brief Convert an array to a data object
     *
     * @param data The data to be converted
     * @returns A data object
     */
    template<typename T, std::size_t N>
    static DataObject convert(const std::array<T,N>& data) {
      return convertArray(data.begin(), data.end(), data.size());
    }

    /**
     * @brief Convert a map to a data object
     *
     * @param data The data to be converted
     * @returns A data object
     */
    template<typename K, typename V>
    static DataObject convert(const std::map<K,V>& data) {
      return convertMap(data.begin(), data.end(), data.size());
    }

    /**
     * @brief Convert a map to a data object
     *
     * @param data The data to be converted
     * @returns A data object
     */
    template<typename K, typename V>
    static DataObject convert(const std::unordered_map<K,V>& data) {
      return convertMap(data.begin(), data.end(), data.size());
    }

  private:

    template<typename Iterator>
    static DataObject convertString(Iterator b, Iterator e) {
      static_assert(std::is_same<typename std::iterator_traits<Iterator>::value_type, char>::value, "Not a string");

      DataObject object;
      object.type = DataType::String;

      std::size_t size = std::distance(b, e);
      assert(size < UINT32_MAX);
      object.string.size = static_cast<uint32_t>(size);
      auto buffer = new char[size + 1];
      std::copy(b, e, buffer);
      buffer[size] = '\0';
      object.string.data = buffer;

      return object;
    }

    template<typename Iterator>
    static DataObject convertBinary(Iterator b, Iterator e, std::size_t size) {
      static_assert(std::is_same<typename std::iterator_traits<Iterator>::value_type, uint8_t>::value, "Not binary");

      DataObject object;
      object.type = DataType::Binary;

      assert(size < UINT32_MAX);
      object.binary.size = static_cast<uint32_t>(size);
      auto buffer = new uint8_t[size];
      std::copy(b, e, buffer);
      object.binary.data = buffer;

      return object;
    }

    template<typename Iterator>
    static DataObject convertArray(Iterator b, Iterator e, std::size_t size) {
      DataObject object;
      object.type = DataType::Array;

      using ItemType = typename std::iterator_traits<Iterator>::value_type;
      assert(size < UINT32_MAX);
      object.array.size =  static_cast<uint32_t>(size);
      auto buffer = new DataObject[size];
      std::transform(b, e, buffer, [](const ItemType& item) {
        return convert(item);
      });
      object.array.data = buffer;

      return object;
    }

    template<typename Iterator>
    static DataObject convertMap(Iterator b, Iterator e, std::size_t size) {
      DataObject object;
      object.type = DataType::Map;

      using ItemType = typename std::iterator_traits<Iterator>::value_type;
      assert(size < UINT32_MAX);
      object.map.size = size;
      auto buffer = new DataKeyValue[size];
      std::transform(b, e, buffer, [](const ItemType& item) {
        DataKeyValue kv;
        kv.key = convert(item.first);
        kv.value = convert(item.second);
        return kv;
      });
      object.map.data = buffer;

      return object;
    }

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_DATA_H
