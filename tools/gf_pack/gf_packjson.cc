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
#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <fstream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/istreamwrapper.h>

#include <gf/Data.h>
#include <gf/Log.h>
#include <gf/SerializationHeaders.h>
#include <gf/SerializationOps.h>

static gf::Serializer& operator|(gf::Serializer& ar, const rapidjson::Value& value) {
  switch (value.GetType()) {
    case rapidjson::kNullType:
      ar | nullptr;
      break;

    case rapidjson::kFalseType:
      ar | false;
      break;

    case rapidjson::kTrueType:
      ar | true;
      break;

    case rapidjson::kObjectType:
      ar | gf::SerialMapHeader{value.MemberCount()};

      for (auto& item : value.GetObject()) {
        ar | item.name | item.value;
      }

      break;

    case rapidjson::kArrayType:
      ar | gf::SerialArrayHeader{value.Size()};

      for (auto& item : value.GetArray()) {
        ar | item;
      }

      break;

    case rapidjson::kStringType:
      ar | value.GetString();
      break;

    case rapidjson::kNumberType:
      if (value.IsUint64()) {
        ar | value.GetUint64();
      } else if (value.IsInt64()) {
        ar | value.GetInt64();
      } else {
        assert(value.IsDouble());
        ar | value.GetDouble();
      }
      break;
  }

  return ar;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::printf("Usage: gf_packjson <json> <pack>\n");
    return EXIT_FAILURE;
  }

  std::ifstream is(argv[1]);

  rapidjson::Document doc;
  rapidjson::IStreamWrapper wrapper(is);
  rapidjson::ParseResult ok = doc.ParseStream(wrapper);

  if (!ok) {
    gf::Log::error("Error while parsing JSON: '%s' (%zu)\n",  rapidjson::GetParseError_En(ok.Code()), ok.Offset());
    return EXIT_FAILURE;
  }

  gf::Path filename(argv[2]);
  gf::Serializer ar(filename);
  ar | doc;

  return EXIT_SUCCESS;
}
