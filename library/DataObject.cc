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
#include <gf/DataObject.h>

#include <algorithm>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /*
   * DataArray
   */

  DataObject *DataArray::begin() noexcept {
    return data;
  }

  DataObject *DataArray::end() noexcept {
    return data + size;
  }

  const DataObject *DataArray::begin() const noexcept {
    return data;
  }

  const DataObject *DataArray::end() const noexcept {
    return data + size;
  }

  /*
   * DataMap
   */

  DataKeyValue *DataMap::begin() noexcept {
    return data;
  }

  DataKeyValue *DataMap::end() noexcept {
    return data + size;
  }

  const DataKeyValue *DataMap::begin() const noexcept {
    return data;
  }

  const DataKeyValue *DataMap::end() const noexcept {
    return data + size;
  }

  /*
   * DataObject
   */

  DataObject::DataObject(DataObject&& other)
  : type(other.type)
  {
    move(std::move(other));
  }

  DataObject& DataObject::operator=(DataObject&& other) {
    clear();
    type = other.type;
    move(std::move(other));
    return *this;
  }

  DataObject::~DataObject() {
    clear();
  }

  void DataObject::clear() {
    switch (type) {
      case DataType::String:
        delete [] string.data;
        break;
      case DataType::Binary:
        delete [] binary.data;
        break;
      case DataType::Array:
        delete [] array.data;
        break;
      case DataType::Map:
        delete [] map.data;
        break;
      case DataType::Extension:
        delete [] extension.data;
        break;
      default:
        break;
    }

    type = DataType::Nil;
  }

  void DataObject::move(DataObject&& other) {
    switch (type) {
      case DataType::Nil:
        break;
      case DataType::Boolean:
        boolean = other.boolean;
        break;
      case DataType::Signed:
        i64 = other.i64;
        break;
      case DataType::Unsigned:
        u64 = other.u64;
        break;
      case DataType::Float:
        f32 = other.f32;
        break;
      case DataType::Double:
        f64 = other.f64;
        break;
      case DataType::String:
        string.size = std::exchange(other.string.size, 0);
        string.data = std::exchange(other.string.data, nullptr);
        break;
      case DataType::Binary:
        binary.size = std::exchange(other.binary.size, 0);
        binary.data = std::exchange(other.binary.data, nullptr);
        break;
      case DataType::Array:
        array.size = std::exchange(other.array.size, 0);
        array.data = std::exchange(other.array.data, nullptr);
        break;
      case DataType::Map:
        map.size = std::exchange(other.map.size, 0);
        map.data = std::exchange(other.map.data, nullptr);
        break;
      case DataType::Extension:
        extension.size = std::exchange(other.extension.size, 0);
        extension.type = std::exchange(other.extension.type, 0);
        extension.data = std::exchange(other.extension.data, nullptr);
        break;
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
