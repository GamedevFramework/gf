/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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
#ifndef GF_SERIALIZATION_OPS_H
#define GF_SERIALIZATION_OPS_H

#include <cstdint>
#include <cstring>

#include <array>
#include <map>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "BufferRef.h"
#include "Portability.h"
#include "Serialization.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @relates Serializer
   * @brief Serialize a boolean object
   */
  GF_API Serializer& operator|(Serializer& ar, bool data);

  /**
   * @relates Serializer
   * @brief Serialize a char object
   */
  GF_API Serializer& operator|(Serializer& ar, char data);

  /**
   * @relates Serializer
   * @brief Serialize an unsigned integer object
   */
  GF_API Serializer& operator|(Serializer& ar, uint8_t data);

  /**
   * @relates Serializer
   * @brief Serialize an unsigned integer object
   */
  GF_API Serializer& operator|(Serializer& ar, uint16_t data);

  /**
   * @relates Serializer
   * @brief Serialize an unsigned integer object
   */
  GF_API Serializer& operator|(Serializer& ar, uint32_t data);

  /**
   * @relates Serializer
   * @brief Serialize an unsigned integer object
   */
  GF_API Serializer& operator|(Serializer& ar, uint64_t data);

  /**
   * @relates Serializer
   * @brief Serialize a signed integer object
   */
  GF_API Serializer& operator|(Serializer& ar, int8_t data);

  /**
   * @relates Serializer
   * @brief Serialize a signed integer object
   */
  GF_API Serializer& operator|(Serializer& ar, int16_t data);

  /**
   * @relates Serializer
   * @brief Serialize a signed integer object
   */
  GF_API Serializer& operator|(Serializer& ar, int32_t data);

  /**
   * @relates Serializer
   * @brief Serialize a signed integer object
   */
  GF_API Serializer& operator|(Serializer& ar, int64_t data);

  /**
   * @relates Serializer
   * @brief Serialize an enum object
   */
  template<typename T, typename E = typename std::enable_if<std::is_enum<T>::value, T>::type>
  inline
  Serializer& operator|(Serializer& ar, T data) {
    using U = typename std::underlying_type<T>::type;
    return ar | static_cast<U>(data);
  }

  /**
   * @relates Serializer
   * @brief Serialize a single precision float object
   */
  GF_API Serializer& operator|(Serializer& ar, float data);

  /**
   * @relates Serializer
   * @brief Serialize a double precision float object
   */
  GF_API Serializer& operator|(Serializer& ar, double data);

  /**
   * @relates Serializer
   * @brief Serialize a string object
   */
  GF_API Serializer& operator|(Serializer& ar, const char *str);

  /**
   * @relates Serializer
   * @brief Serialize a string object
   */
  GF_API Serializer& operator|(Serializer& ar, const std::string& str);

  /**
   * @relates Serializer
   * @brief Serialize an array object
   */
  template<typename T, std::size_t N>
  inline
  Serializer& operator|(Serializer& ar, const T (&array)[N]) {
    ar.writeSizeHeader(N);

    for (auto& item : array) {
      ar | const_cast<T&>(item);
    }

    return ar;
  }

  /**
   * @relates Serializer
   * @brief Serialize an array object
   */
  template<typename T>
  inline
  Serializer& operator|(Serializer& ar, const std::vector<T>& array) {
    ar.writeSizeHeader(array.size());

    for (auto& item : array) {
      ar | const_cast<T&>(item);
    }

    return ar;
  }

  /**
   * @relates Serializer
   * @brief Serialize an array object
   */
  template<typename T, std::size_t N>
  inline
  Serializer& operator|(Serializer& ar, const std::array<T, N>& array) {
    ar.writeSizeHeader(array.size());

    for (auto& item : array) {
      ar | const_cast<T&>(item);
    }

    return ar;
  }

  /**
   * @relates Serializer
   * @brief Serialize a set object
   */
  template<typename T>
  inline
  Serializer& operator|(Serializer& ar, const std::set<T>& set) {
    ar.writeSizeHeader(set.size());

    for (auto& item : set) {
      ar | const_cast<T&>(item);
    }

    return ar;
  }

  /**
   * @relates Serializer
   * @brief Serialize a set object
   */
  template<typename T>
  inline
  Serializer& operator|(Serializer& ar, const std::unordered_set<T>& set) {
    ar.writeSizeHeader(set.size());

    for (auto& item : set) {
      ar | const_cast<T&>(item);
    }

    return ar;
  }

  /**
   * @relates Serializer
   * @brief Serialize a map object
   */
  template<typename K, typename V>
  inline
  Serializer& operator|(Serializer& ar, const std::map<K, V>& map) {
    ar.writeSizeHeader(map.size());

    for (auto& item : map) {
      ar | const_cast<K&>(item.first) | const_cast<V&>(item.second);
    }

    return ar;
  }

  /**
   * @relates Serializer
   * @brief Serialize a map object
   */
  template<typename K, typename V>
  inline
  Serializer& operator|(Serializer& ar, const std::unordered_map<K, V>& map) {
    ar.writeSizeHeader(map.size());

    for (auto& item : map) {
      ar | const_cast<K&>(item.first) | const_cast<V&>(item.second);
    }

    return ar;
  }



  /**
   * @relates Deserializer
   * @brief Deserialize a bool object
   */
  GF_API Deserializer& operator|(Deserializer& ar, bool& data);

  /**
   * @relates Deserializer
   * @brief Deserialize a char object
   */
  GF_API Deserializer& operator|(Deserializer& ar, char& data);

  /**
   * @relates Deserializer
   * @brief Deserialize an unsigned integer object
   */
  GF_API Deserializer& operator|(Deserializer& ar, uint8_t& data);

  /**
   * @relates Deserializer
   * @brief Deserialize an unsigned integer object
   */
  GF_API Deserializer& operator|(Deserializer& ar, uint16_t& data);

  /**
   * @relates Deserializer
   * @brief Deserialize an unsigned integer object
   */
  GF_API Deserializer& operator|(Deserializer& ar, uint32_t& data);

  /**
   * @relates Deserializer
   * @brief Deserialize an unsigned integer object
   */
  GF_API Deserializer& operator|(Deserializer& ar, uint64_t& data);

  /**
   * @relates Deserializer
   * @brief Deserialize a signed integer object
   */
  GF_API Deserializer& operator|(Deserializer& ar, int8_t& data);

  /**
   * @relates Deserializer
   * @brief Deserialize a signed integer object
   */
  GF_API Deserializer& operator|(Deserializer& ar, int16_t& data);

  /**
   * @relates Deserializer
   * @brief Deserialize a signed integer object
   */
  GF_API Deserializer& operator|(Deserializer& ar, int32_t& data);

  /**
   * @relates Deserializer
   * @brief Deserialize a signed integer object
   */
  GF_API Deserializer& operator|(Deserializer& ar, int64_t& data);

  /**
   * @relates Deserializer
   * @brief Deserialize an enum object
   */
  template<typename T, typename E = typename std::enable_if<std::is_enum<T>::value, T>::type>
  inline
  Deserializer& operator|(Deserializer& ar, T& data) {
    using U = typename std::underlying_type<T>::type;
    U underlying;
    ar | underlying;
    data = static_cast<T>(underlying);
    return ar;
  }

  /**
   * @relates Deserializer
   * @brief Deserialize a single precision float object
   */
  GF_API Deserializer& operator|(Deserializer& ar, float& data);

  /**
   * @relates Deserializer
   * @brief Deserialize a double precision float object
   */
  GF_API Deserializer& operator|(Deserializer& ar, double& data);

  /**
   * @relates Deserializer
   * @brief Deserialize a string object
   */
  GF_API Deserializer& operator|(Deserializer& ar, BufferRef<char> str);

  /**
   * @relates Deserializer
   * @brief Deserialize a string object
   */
  GF_API Deserializer& operator|(Deserializer& ar, std::string& str);

  /**
   * @relates Deserializer
   * @brief Deserialize an array object
   */
  template<typename T>
  inline
  Deserializer& operator|(Deserializer& ar, BufferRef<T> array) {
    std::size_t size;

    if (!ar.readSizeHeader(size)) {
      return ar;
    }

    if (size != array.getSize()) {
      return ar;
    }

    for (auto& item : array) {
      ar | item;
    }

    return ar;
  }

  /**
   * @relates Deserializer
   * @brief Deserialize an array object
   */
  template<typename T, std::size_t N>
  inline
  Deserializer& operator|(Deserializer& ar, T (&array)[N]) {
    return ar | BufferRef<T>(array);
  }

  /**
   * @relates Deserializer
   * @brief Deserialize an array object
   */
  template<typename T, std::size_t N>
  inline
  Deserializer& operator|(Deserializer& ar, std::array<T, N>& array) {
    return ar | BufferRef<T>(array);
  }

  /**
   * @relates Deserializer
   * @brief Deserialize an array object
   */
  template<typename T>
  inline
  Deserializer& operator|(Deserializer& ar, std::vector<T>& array) {
    std::size_t size;

    if (!ar.readSizeHeader(size)) {
      return ar;
    }

    array.clear();

    for (std::size_t i = 0; i < size; ++i) {
      T item;
      ar | item;
      array.emplace_back(std::move(item));
    }

    return ar;
  }

  /**
   * @relates Deserializer
   * @brief Deserialize a set object
   */
  template<typename T>
  inline
  Deserializer& operator|(Deserializer& ar, std::set<T>& set) {
    std::size_t size;

    if (!ar.readSizeHeader(size)) {
      return ar;
    }

    set.clear();

    for (std::size_t i = 0; i < size; ++i) {
      T item;
      ar | item;
      set.emplace(std::move(item));
    }

    return ar;
  }

  /**
   * @relates Deserializer
   * @brief Deserialize a set object
   */
  template<typename T>
  inline
  Deserializer& operator|(Deserializer& ar, std::unordered_set<T>& set) {
    std::size_t size;

    if (!ar.readSizeHeader(size)) {
      return ar;
    }

    set.clear();

    for (std::size_t i = 0; i < size; ++i) {
      T item;
      ar | item;
      set.emplace(std::move(item));
    }

    return ar;
  }

  /**
   * @relates Deserializer
   * @brief Deserialize a map object
   */
  template<typename K, typename V>
  inline
  Deserializer& operator|(Deserializer& ar, std::map<K,V>& map) {
    std::size_t size;

    if (!ar.readSizeHeader(size)) {
      return ar;
    }

    map.clear();

    for (std::size_t i = 0; i < size; ++i) {
      K key;
      ar | key;
      V value;
      ar | value;

      map.emplace(std::move(key), std::move(value));
    }

    return ar;
  }

  /**
   * @relates Deserializer
   * @brief Deserialize a map object
   */
  template<typename K, typename V>
  inline
  Deserializer& operator|(Deserializer& ar, std::unordered_map<K,V>& map) {
    std::size_t size;

    if (!ar.readSizeHeader(size)) {
      return ar;
    }

    map.clear();

    for (std::size_t i = 0; i < size; ++i) {
      K key;
      ar | key;
      V value;
      ar | value;

      map.emplace(std::move(key), std::move(value));
    }

    return ar;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_SERIALIZATION_OPS_H
