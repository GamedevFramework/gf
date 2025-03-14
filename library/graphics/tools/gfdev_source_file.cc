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
#include <cstdarg>
#include <cstdlib>
#include <cstdio>

namespace {

  class File {
  public:
    File(const char *filename, const char *mode)
    : m_stream(std::fopen(filename, mode))
    {

    }

    ~File() {
      if (m_stream) {
        std::fclose(m_stream);
      }
    }

    operator bool() const {
      return m_stream != nullptr;
    }

    int getc() const { // Flawfinder: ignore
      return std::fgetc(m_stream); // Flawfinder: ignore
    }

    int printf(const char *fmt, ...) {
      va_list args;
      va_start(args, fmt);
      int ret = std::vfprintf(m_stream, fmt, args);
      va_end(args);
      return ret;
    }

  private:
    std::FILE *m_stream;
  };

}


static void usage() {
  std::printf("Usage: gfdev_source_file <source> <destination> <name>\n");
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    usage();
    return EXIT_FAILURE;
  }

  File inFile(argv[1], "rb");

  if (!inFile) {
    usage();
    return EXIT_FAILURE;
  }

  File outFile(argv[2], "wb");

  if (!outFile) {
    usage();
    return EXIT_FAILURE;
  }

  outFile.printf("// DO NOT MODIFY!\n");
  outFile.printf("// This file has been generated\n\n");
  outFile.printf("#ifndef GF_GENERARED_%s\n#define GF_GENERARED_%s\n\n", argv[3], argv[3]);

  outFile.printf("namespace gf {\ninline namespace generated {\n\n");

  outFile.printf("extern const char %s[];\n\n", argv[3]);

  outFile.printf("#ifdef GF_IMPLEMENTATION\n");
  outFile.printf("const char %s[] = {", argv[3]);

  std::size_t count = 0;
  int c;

  while ((c = inFile.getc()) != EOF) { // Flawfinder: ignore
    if (count % 12 == 0) {
      outFile.printf("\n   ");
    }

    outFile.printf("0x%.2X, ", c);

    count++;
  }

  if (count % 12 == 0) {
    outFile.printf("\n   ");
  }

  outFile.printf("0x00\n};\n\n");
  outFile.printf("// size = %zu;\n", count);
  outFile.printf("#endif // GF_IMPLEMENTATION\n\n");

  outFile.printf("} // namespace generated\n");
  outFile.printf("} // namespace gf\n\n");

  outFile.printf("#endif\n");

  return EXIT_SUCCESS;
}
