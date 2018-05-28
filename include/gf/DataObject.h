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
#ifndef GF_DATA_OBJECT_H
#define GF_DATA_OBJECT_H

#include <cstdint>
#include <cstddef>

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief Generic data type for data objects
   *
   * @sa gf::DataObject
   */
  enum class DataType {
    Nil,        ///< A null object
    Boolean,    ///< A boolean object
    Signed,     ///< A signed integer object
    Unsigned,   ///< An unsigned integer object
    Float,      ///< A single precision float object
    Double,     ///< A double precision float object
    String,     ///< A string object
    Binary,     ///< A binary object
    Array,      ///< An array object
    Map,        ///< A map object
    Extension,  ///< An extension object
  };

  struct DataObject;

  /**
   * @ingroup game
   * @brief A string object
   *
   * @sa gf::DataObject
   */
  struct GF_API DataString {
    uint32_t size;    ///< The size of the string
    const char *data; ///< The data of the string
  };

  /**
   * @ingroup game
   * @brief A binary object
   *
   * A binary object is represented with a byte array (`uint8_t`).
   *
   * @sa gf::DataObject
   */
  struct GF_API DataBinary {
    uint32_t size;        ///< The size of the binary
    const uint8_t *data;  ///< The data of the binary
  };

  /**
   * @ingroup game
   * @brief An array object
   *
   * An array object contains data objects.
   *
   * @sa gf::DataObject
   */
  struct GF_API DataArray {
    uint32_t size;    ///< The size of the array
    DataObject *data; ///< The data of the array

    /**
     * @brief Get an iterator to the first element
     *
     * @returns A pointer to the first element
     *
     * @sa end()
     */
    DataObject *begin() noexcept;

    /**
     * @brief Get an iterator past the last element
     *
     * @returns A pointer past the last element
     *
     * @sa begin()
     */
    DataObject *end() noexcept;

    /**
     * @brief Get an iterator to the first element
     *
     * @returns A pointer to the first element
     *
     * @sa end()
     */
    const DataObject *begin() const noexcept;

    /**
     * @brief Get an iterator past the last element
     *
     * @returns A pointer past the last element
     *
     * @sa begin()
     */
    const DataObject *end() const noexcept;
  };

  struct GF_API DataKeyValue;

  /**
   * @ingroup game
   * @brief A map object
   *
   * A map object contains an array of key-value pairs of data objects.
   *
   * @sa gf::DataObject, gf::DataKeyValue
   */
  struct GF_API DataMap {
    uint32_t size;      ///< The size of the map
    DataKeyValue *data; ///< The data of the map

    /**
     * @brief Get an iterator to the first element
     *
     * @returns A pointer to the first element
     * @sa end()
     */
    DataKeyValue *begin() noexcept;

    /**
     * @brief Get an iterator past the last element
     *
     * @returns A pointer past the last element
     *
     * @sa begin()
     */
    DataKeyValue *end() noexcept;

    /**
     * @brief Get an iterator to the first element
     *
     * @returns A pointer to the first element
     * @sa end()
     */
    const DataKeyValue *begin() const noexcept;

    /**
     * @brief Get an iterator past the last element
     *
     * @returns A pointer past the last element
     *
     * @sa begin()
     */
    const DataKeyValue *end() const noexcept;
  };

  /**
   * @ingroup game
   * @brief An extension object
   *
   * An extension can code a special type not available directly.
   *
   * @sa gf::DataObject
   */
  struct GF_API DataExtension {
    uint32_t size;        ///< The size of the extension
    int8_t type;          ///< The type of the extension
    const uint8_t *data;  ///< The data of the extension
  };

  /**
   * @ingroup game
   * @brief A data object
   *
   * A data object is a generic representation of any data. Its main purpose is
   * to represent serialized data (e.g. a JSON document).
   *
   * @sa gf::DataType, gf::Data
   */
  struct GF_API DataObject {
    DataType type;  ///< The type of the object

    union {
      bool boolean;             ///< A boolean object
      uint64_t u64;             ///< An unsigned integer object
      int64_t i64;              ///< A signed integer object
      float f32;                ///< A single precision float object
      double f64;               ///< A double precision float object
      DataString string;        ///< A string object
      DataBinary binary;        ///< A binary object
      DataArray array;          ///< An array object
      DataMap map;              ///< A map object
      DataExtension extension;  ///< An extension object
    };

    /**
     * @brief Default constructor
     *
     * By default, the data object is a null object
     */
    DataObject()
    : type(DataType::Nil)
    {
    }

    /**
     * @brief Constructor for a null object
     */
    explicit DataObject(std::nullptr_t)
    : type(DataType::Nil)
    {
    }

    /**
     * @brief Constructor for a boolean object
     */
    explicit DataObject(bool data)
    : type(DataType::Boolean)
    , boolean(data)
    {

    }

    /**
     * @brief Constructor for an unsigned object
     */
    explicit DataObject(uint8_t data)
    : type(DataType::Unsigned)
    , u64(data)
    {

    }

    /**
     * @brief Constructor for a signed object
     */
    explicit DataObject(int8_t data)
    : type(DataType::Signed)
    , i64(data)
    {

    }

    /**
     * @brief Constructor for an unsigned object
     */
    explicit DataObject(uint16_t data)
    : type(DataType::Unsigned)
    , u64(data)
    {

    }

    /**
     * @brief Constructor for a signed object
     */
    explicit DataObject(int16_t data)
    : type(DataType::Signed)
    , i64(data)
    {

    }

    /**
     * @brief Constructor for an unsigned object
     */
    explicit DataObject(uint32_t data)
    : type(DataType::Unsigned)
    , u64(data)
    {

    }

    /**
     * @brief Constructor for a signed object
     */
    explicit DataObject(int32_t data)
    : type(DataType::Signed)
    , i64(data)
    {

    }

    /**
     * @brief Constructor for an unsigned object
     */
    explicit DataObject(uint64_t data)
    : type(DataType::Unsigned)
    , u64(data)
    {

    }

    /**
     * @brief Constructor for a signed object
     */
    explicit DataObject(int64_t data)
    : type(DataType::Signed)
    , i64(data)
    {

    }

    /**
     * @brief Constructor for a single precision float object
     */
    explicit DataObject(float data)
    : type(DataType::Float)
    , f32(data)
    {

    }

    /**
     * @brief Constructor for a double precision float object
     */
    explicit DataObject(double data)
    : type(DataType::Double)
    , f64(data)
    {

    }

    /**
     * @brief Deleted copy constructor
     */
    DataObject(const DataObject&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    DataObject& operator=(const DataObject&) = delete;

    /**
     * @brief Move constructor
     */
    DataObject(DataObject&& other);

    /**
     * @brief Move assignment
     */
    DataObject& operator=(DataObject&& other);

    /**
     * @brief Destructor
     */
    ~DataObject();

    /**
     * @brief Set a data object to a null object
     *
     * If memory was allocated, it is freed.
     */
    void clear();

  private:
    void move(DataObject&& other);
  };

  /**
   * @ingroup game
   * @brief A key-value pair of data objects
   *
   * @sa gf::DataMap
   */
  struct DataKeyValue {
    DataObject key;   ///< The key of the pair
    DataObject value; ///< The value of the pair
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_DATA_OBJECT_H
