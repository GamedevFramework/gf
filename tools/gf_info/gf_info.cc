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
#include <cstdio>
#include <algorithm>

#include <gf/GraphicsInfo.h>
#include <gf/Library.h>
#include <gf/Monitor.h>
#include <gf/Paths.h>
#include <gf/SystemInfo.h>
#include <gf/Window.h>

int main() {
  gf::Library lib;

  gf::Library::Version version = gf::Library::getVersion();
  std::printf("gf %i.%i.%i\n\n", version.major, version.minor, version.patch);

  std::printf("Dependencies:\n");

  gf::Library::Version compiled, linked;

  std::tie(compiled, linked) = gf::Library::getDependencyVersion(gf::Library::Boost);
  std::printf("- Boost: %i.%i.%i\n", compiled.major, compiled.minor, compiled.patch);

  std::tie(compiled, linked) = gf::Library::getDependencyVersion(gf::Library::SimpleDirectMediaLayer);
  std::printf("- SDL: %i.%i.%i [%i.%i.%i]\n", compiled.major, compiled.minor, compiled.patch, linked.major, linked.minor, linked.patch);

  std::tie(compiled, linked) = gf::Library::getDependencyVersion(gf::Library::FreeType);
  std::printf("- Freetype: %i.%i.%i [%i.%i.%i]\n", compiled.major, compiled.minor, compiled.patch, linked.major, linked.minor, linked.patch);

  std::printf("\n");

  std::printf("System:\n");
  std::printf("- Platform: %s\n", gf::SystemInfo::getPlatformName().c_str());
  std::printf("- CPU count: %i\n", gf::SystemInfo::getCpuCount());
  std::printf("- RAM size: %i MiB\n", gf::SystemInfo::getSystemRamSize());
  std::printf("- Cache line size: %i kiB\n", gf::SystemInfo::getCpuCacheLineSize());
  std::printf("\n");

  std::printf("Paths:\n");
  std::printf("- Current path: %s\n", gf::Paths::getCurrentPath().string().c_str());
  std::printf("- Base path: %s\n", gf::Paths::getBasePath().string().c_str());
  std::printf("- Temporary directory: %s\n", gf::Paths::getTemporaryDirectory().string().c_str());
  std::printf("\n");

  auto monitors = gf::Monitor::getAvailableMonitors();

  std::printf("Monitors (%zu):\n", monitors.size());

  for (const auto& monitor : monitors) {
    std::printf("- Monitor '%s':\n", monitor.getName().c_str());

    auto size = monitor.getPhysicalSize();
    std::printf("\tPhysical size: %ux%u\n", size.width, size.height);

    auto pos = monitor.getPosition();
    std::printf("\tPosition: %ix%i\n", pos.x, pos.y);

    auto modes = monitor.getAvailableVideoModes();
    std::printf("\tModes (%zu):\n", modes.size());

    for (const auto& mode : modes) {
      std::printf("\t\t%ux%u | %u bpp | %i Hz\n", mode.size.width, mode.size.height, mode.bitsPerPixel, mode.refreshRate);
    }

    auto mode = monitor.getCurrentVideoMode();
    std::printf("\tCurrent mode: %ux%u | %u bpp | %i Hz\n", mode.size.width, mode.size.height, mode.bitsPerPixel, mode.refreshRate);
  }

  std::printf("\n");

  gf::Window window("gf_info", { 0u, 0u }, ~gf::WindowHints::Visible); // to create an OpenGL context

  std::printf("GL:\n");
  std::printf("- Vendor: %s\n", gf::GraphicsInfo::getVendor().c_str());
  std::printf("- Renderer: %s\n", gf::GraphicsInfo::getRenderer().c_str());
  std::printf("- Version: %s\n", gf::GraphicsInfo::getVersion().c_str());
  std::printf("- Shading language version: %s\n", gf::GraphicsInfo::getShadingLanguageVersion().c_str());

  auto extensions = gf::GraphicsInfo::getExtensions();

  std::printf("- Extensions (%zu):", extensions.size());

  std::sort(extensions.begin(), extensions.end());
  unsigned i = 0;

  for (const auto& extension : extensions) {
    if (i % 3 == 0) {
      std::printf("\n\t");
    }

    std::printf("%s ", extension.c_str());
    ++i;
  }

  printf("\n");

  auto textures = gf::GraphicsInfo::getCompressedTextureFormats();
  std::printf("- Compressed texture formats (%zu):\n", textures.size());

  for (const auto& texture : textures) {
    std::printf("\t%s (0x%X)\n", texture.name.c_str(), texture.symbol);
  }

  auto shaders = gf::GraphicsInfo::getShaderBinaryFormats();
  std::printf("- Shader binary formats (%zu):\n", shaders.size());

  for (const auto& shader : shaders) {
    std::printf("\t%s (0x%X)\n", shader.name.c_str(), shader.symbol);
  }

  std::printf("- Parameters:\n");
  std::printf("\tGL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: %i\n", gf::GraphicsInfo::getMaxCombinedTextureImageUnits());
  std::printf("\tGL_MAX_CUBE_MAP_TEXTURE_SIZE: %i\n", gf::GraphicsInfo::getMaxCubeMapTextureSize());
  std::printf("\tGL_MAX_FRAGMENT_UNIFORM_VECTORS: %i\n", gf::GraphicsInfo::getMaxFragmentUniformVectors());
  std::printf("\tGL_MAX_RENDERBUFFER_SIZE: %i\n", gf::GraphicsInfo::getMaxRenderbufferSize());
  std::printf("\tGL_MAX_TEXTURE_IMAGE_UNITS: %i\n", gf::GraphicsInfo::getMaxTextureImageUnits());
  std::printf("\tGL_MAX_TEXTURE_SIZE: %i\n", gf::GraphicsInfo::getMaxTextureSize());
  std::printf("\tGL_MAX_VARYING_VECTORS: %i\n", gf::GraphicsInfo::getMaxVaryingVectors());
  std::printf("\tGL_MAX_VERTEX_ATTRIBS: %i\n", gf::GraphicsInfo::getMaxVertexAttribs());
  std::printf("\tGL_MAX_VERTEX_TEXTURE_IMAGE_UNITS: %i\n", gf::GraphicsInfo::getMaxVertexTextureImageUnits());
  std::printf("\tGL_MAX_VERTEX_UNIFORM_VECTORS: %i\n", gf::GraphicsInfo::getMaxVertexUniformVectors());

  auto dims = gf::GraphicsInfo::getMaxViewportDims();
  std::printf("\tGL_MAX_VIEWPORT_DIMS: %ix%i\n", dims.width, dims.height);

  std::printf("\tGL_SUBPIXEL_BITS: %i\n", gf::GraphicsInfo::getSubpixelBits());

  return 0;
}
