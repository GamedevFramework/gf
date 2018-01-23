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

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  enum class DataType {
    Nil,
    Boolean,
    Signed,
    Unsigned,
    Float,
    Double,
    String,
    Binary,
    Array,
    Map,
    Extension,
  };

  struct DataObject;

  struct GF_API DataString {
    uint32_t size;
    const char *data;
  };

  struct GF_API DataBinary {
    uint32_t size;
    const uint8_t *data;
  };

  struct GF_API DataArray {
    uint32_t size;
    DataObject *data;

    DataObject *begin() noexcept;

    DataObject *end() noexcept;

    const DataObject *begin() const noexcept;

    const DataObject *end() const noexcept;
  };

  struct GF_API DataKeyValue;

  struct GF_API DataMap {
    uint32_t size;
    DataKeyValue *data;

    DataKeyValue *begin() noexcept;

    DataKeyValue *end() noexcept;

    const DataKeyValue *begin() const noexcept;

    const DataKeyValue *end() const noexcept;
  };

  struct GF_API DataExtension {
    uint32_t size;
    int8_t type;
    const uint8_t *data;
  };

  struct GF_API DataObject {
    DataType type;

    union {
      bool boolean;
      uint64_t u64;
      int64_t i64;
      float f32;
      double f64;
      DataString string;
      DataBinary binary;
      DataArray array;
      DataMap map;
      DataExtension extension;
    };

    DataObject()
    : type(DataType::Nil)
    {
    }

    explicit DataObject(std::nullptr_t)
    : type(DataType::Nil)
    {
    }

    explicit DataObject(bool data)
    : type(DataType::Boolean)
    , boolean(data)
    {

    }

    explicit DataObject(uint8_t data)
    : type(DataType::Unsigned)
    , u64(data)
    {

    }

    explicit DataObject(int8_t data)
    : type(DataType::Signed)
    , i64(data)
    {

    }

    explicit DataObject(uint16_t data)
    : type(DataType::Unsigned)
    , u64(data)
    {

    }

    explicit DataObject(int16_t data)
    : type(DataType::Signed)
    , i64(data)
    {

    }

    explicit DataObject(uint32_t data)
    : type(DataType::Unsigned)
    , u64(data)
    {

    }

    explicit DataObject(int32_t data)
    : type(DataType::Signed)
    , i64(data)
    {

    }

    explicit DataObject(uint64_t data)
    : type(DataType::Unsigned)
    , u64(data)
    {

    }

    explicit DataObject(int64_t data)
    : type(DataType::Signed)
    , i64(data)
    {

    }

    explicit DataObject(float data)
    : type(DataType::Float)
    , f32(data)
    {

    }

    explicit DataObject(double data)
    : type(DataType::Double)
    , f64(data)
    {

    }

    DataObject(const DataObject&) = delete;
    DataObject& operator=(const DataObject&) = delete;

    DataObject(DataObject&& other);
    DataObject& operator=(DataObject&& other);

    ~DataObject();

    void clear();

  private:
    void move(DataObject&& other);
  };

  struct DataKeyValue {
    DataObject key;
    DataObject value;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_DATA_OBJECT_H
