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
#include <cinttypes>

#include <iomanip>
#include <sstream>

#include <gf/Data.h>
#include <gf/SerializationOps.h>

#define COLOR_RED     "\x1B[31m"
#define COLOR_GREEN   "\x1B[32m"
#define COLOR_YELLOW  "\x1B[33m"
#define COLOR_BLUE    "\x1B[34m"
#define COLOR_MAGENTA "\x1B[35m"
#define COLOR_CYAN    "\x1B[36m"
#define COLOR_STOP    "\x1B[0m"

struct DataPrinter {

  void print(gf::DataObject& object) {
    switch (object.type) {
      case gf::DataType::Array:
        printf(COLOR_RED "<array>" COLOR_STOP " (%" PRIu32 ") [\n", object.array.size);
        indent++;
        for (uint32_t i = 0; i < object.array.size; ++i) {
          printIndent();
          print(object.array.data[i]);
          printf("\n");
        }
        indent--;
        printIndent();
        printf("]");
        break;
      case gf::DataType::Map:
        printf(COLOR_RED "<map>" COLOR_STOP " (%" PRIu32 ") {\n", object.map.size);
        indent++;
        for (uint32_t i = 0; i < object.map.size; ++i) {
          printIndent();
          printSimple(object.map.data[i].key);
          printf(": ");
          print(object.map.data[i].value);
          printf("\n");
        }
        indent--;
        printIndent();
        printf("}");
        break;
      default:
        printSimple(object);
        break;
    }
  }

  void printSimple(gf::DataObject& object) {
    switch (object.type) {
      case gf::DataType::Nil:
        std::printf(COLOR_RED "<nil>" COLOR_STOP " " COLOR_GREEN "nil" COLOR_STOP);
        break;
      case gf::DataType::Boolean:
        std::printf(COLOR_RED "<boolean>" COLOR_STOP " " COLOR_GREEN "%s" COLOR_STOP, object.boolean ? "true" : "false");
        break;
      case gf::DataType::Signed:
        std::printf(COLOR_RED "<signed>" COLOR_STOP " %" PRIi64 " (0x%" PRIX64 ")", object.i64, object.i64);
        break;
      case gf::DataType::Unsigned:
        std::printf(COLOR_RED "<unsigned>" COLOR_STOP " %" PRIu64 " (0x%" PRIX64 ")", object.u64, object.u64);
        break;
      case gf::DataType::Float:
        std::printf(COLOR_RED "<float>" COLOR_STOP " %f", object.f32);
        break;
      case gf::DataType::Double:
        std::printf(COLOR_RED "<double>" COLOR_STOP " %f", object.f64);
        break;
      case gf::DataType::String: {
        std::ostringstream ss;
        ss << std::quoted(object.string.data);
        std::printf(COLOR_RED "<string>" COLOR_STOP " " COLOR_CYAN "%s" COLOR_STOP, ss.str().c_str());
        break;
      }
      case gf::DataType::Binary:
        std::printf(COLOR_RED "<binary>" COLOR_STOP);

        for (uint32_t i = 0; i < std::min(UINT32_C(20), object.binary.size); ++i) {
          std::printf(" %.2" PRIX8, object.binary.data[i]);
        }

        break;
      case gf::DataType::Extension:
        std::printf(COLOR_RED "<extension>" COLOR_STOP " <%" PRIi8 ">", object.extension.type);

        for (uint32_t i = 0; i < std::min(UINT32_C(20), object.extension.size); ++i) {
          std::printf(" %.2" PRIX8, object.binary.data[i]);
        }

        break;
      default:
        assert(false);
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
  printf("\n");

  return 0;
}
