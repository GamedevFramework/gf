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
#include <cstdio>
#include <cstdlib>
#include <cinttypes>

#include <iomanip>
#include <sstream>

#include <gf/Data.h>
#include <gf/SerializationOps.h>

struct DataPrinter {

  void print(gf::DataObject& object) {
    switch (object.type) {
      case gf::DataType::Array:
        printIndent();
        printf("[\n");
        indent++;
        for (uint32_t i = 0; i < std::min(UINT32_C(10), object.array.size); ++i) {
          print(object.array.data[i]);
          printf("\n");
        }
        indent--;
        printIndent();
        printf("]\n");
        break;
      case gf::DataType::Map:
        printf("{\n");
        indent++;
        for (uint32_t i = 0; i < std::min(UINT32_C(10), object.map.size); ++i) {
          printIndent();
          printSimple(object.map.data[i].key);
          printf(": ");
          print(object.map.data[i].value);
          printf("\n");
        }
        indent--;
        printIndent();
        printf("}\n");
        break;
      default:
        printSimple(object);
        break;
    }
  }

  void printSimple(gf::DataObject& object) {
    switch (object.type) {
      case gf::DataType::Nil:
        std::printf("nil");
        break;
      case gf::DataType::Boolean:
        std::printf("<boolean> %s", object.boolean ? "true" : "false");
        break;
      case gf::DataType::Signed:
        std::printf("<signed> %" PRIi64 " (0x%" PRIX64 ")", object.i64, object.i64);
        break;
      case gf::DataType::Unsigned:
        std::printf("<unsigned> %" PRIu64 " (0x%" PRIX64 ")", object.u64, object.u64);
        break;
      case gf::DataType::Float:
        std::printf("<float> %f", object.f32);
        break;
      case gf::DataType::Double:
        std::printf("<double> %f", object.f64);
        break;
      case gf::DataType::String: {
        std::ostringstream ss;
        ss << std::quoted(object.string.data);
        std::printf("<string> %s", ss.str().c_str());
        break;
      }
      case gf::DataType::Binary:
        std::printf("<binary>");

        for (uint32_t i = 0; i < std::min(UINT32_C(20), object.binary.size); ++i) {
          std::printf(" %.2" PRIX8, object.binary.data[i]);
        }

        break;
      case gf::DataType::Extension:
        std::printf("<extension> <%" PRIi8 ">", object.extension.type);

        for (uint32_t i = 0; i < std::min(UINT32_C(20), object.extension.size); ++i) {
          std::printf(" %.2" PRIX8, object.binary.data[i]);
        }

        break;
      default:
        break;
    }
  }

  void printIndent() {
    for (int i = 0; i < indent; ++i) {
      printf("\t");
    }
  }

  int indent = 0;
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::printf("Usage: gf_packview <file>\n");
    return EXIT_FAILURE;
  }

  gf::Path filename(argv[1]);
  gf::Deserializer ar(filename);
  gf::DataObject object;

  ar | object;

  DataPrinter printer;
  printer.print(object);

  return 0;
}
