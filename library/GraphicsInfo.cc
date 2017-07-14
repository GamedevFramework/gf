/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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
#include <gf/GraphicsInfo.h>

#include <cassert>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <glad/glad.h>

#include <gf/Log.h>

#include "priv/Debug.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  static std::string getString(GLenum name) {
    const GLubyte* str = glGetString(name);

    GLenum code = glGetError();

    if (code != GL_NO_ERROR) {
      assert(code == GL_INVALID_ENUM);
      Log::error("Error in OpenGL call to glGetString: GL_INVALID_ENUM\n");
      return "";
    }

    return std::string(reinterpret_cast<const char *>(str));
  }

  std::string GraphicsInfo::getVendor() {
    return getString(GL_VENDOR);
  }

  std::string GraphicsInfo::getRenderer() {
    return getString(GL_RENDERER);
  }

  std::string GraphicsInfo::getVersion() {
    return getString(GL_VERSION);
  }

  std::string GraphicsInfo::getShadingLanguageVersion() {
    return getString(GL_SHADING_LANGUAGE_VERSION);
  }

  std::vector<std::string> GraphicsInfo::getExtensions() {
    std::string extensions = getString(GL_EXTENSIONS);
    std::vector<std::string> ret;
    boost::algorithm::split(ret, extensions, boost::is_any_of(" \n"), boost::algorithm::token_compress_on);
    ret.erase(std::remove_if(ret.begin(), ret.end(), [](const std::string& ext) { return ext.empty(); }), ret.end());
    return ret;
  }

  static const char *getTextureFormatName(GLint format) {
    switch (format) {
      // EXT_texture_compression_s3tc
      case 0x83F0:
        return "GL_COMPRESSED_RGB_S3TC_DXT1_EXT";
      case 0x83F1:
        return "GL_COMPRESSED_RGBA_S3TC_DXT1_EXT";
      case 0x83F2:
        return "GL_COMPRESSED_RGBA_S3TC_DXT3_EXT";
      case 0x83F3:
        return "GL_COMPRESSED_RGBA_S3TC_DXT5_EXT";

      // 3DFX_texture_compression_FXT1
      case 0x86B0:
        return "COMPRESSED_RGB_FXT1_3DFX";
      case 0x86B1:
        return "COMPRESSED_RGBA_FXT1_3DFX";

      // OES_compressed_paletted_texture
      case 0x8B90:
        return "PALETTE4_RGB8_OES";
      case 0x8B91:
        return "PALETTE4_RGBA8_OES";
      case 0x8B92:
        return "PALETTE4_R5_G6_B5_OES";
      case 0x8B93:
        return "PALETTE4_RGBA4_OES";
      case 0x8B94:
        return "PALETTE4_RGB5_A1_OES";
      case 0x8B95:
        return "PALETTE8_RGB8_OES";
      case 0x8B96:
        return "PALETTE8_RGBA8_OES";
      case 0x8B97:
        return "PALETTE8_R5_G6_B5_OES";
      case 0x8B98:
        return "PALETTE8_RGBA4_OES";
      case 0x8B99:
        return "PALETTE8_RGB5_A1_OES";

      // EXT_texture_sRGB
      case 0x8C40:
        return "SRGB_EXT";
      case 0x8C41:
        return "SRGB8_EXT";
      case 0x8C42:
        return "SRGB_ALPHA_EXT";
      case 0x8C43:
        return "SRGB8_ALPHA8_EXT";
      case 0x8C44:
        return "SLUMINANCE_ALPHA_EXT";
      case 0x8C45:
        return "SLUMINANCE8_ALPHA8_EXT";
      case 0x8C46:
        return "SLUMINANCE_EXT";
      case 0x8C47:
        return "SLUMINANCE8_EXT";
      case 0x8C48:
        return "COMPRESSED_SRGB_EXT";
      case 0x8C49:
        return "COMPRESSED_SRGB_ALPHA_EXT";
      case 0x8C4A:
        return "COMPRESSED_SLUMINANCE_EXT";
      case 0x8C4B:
        return "COMPRESSED_SLUMINANCE_ALPHA_EXT";
      case 0x8C4C:
        return "COMPRESSED_SRGB_S3TC_DXT1_EXT";
      case 0x8C4D:
        return "COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT";
      case 0x8C4E:
        return "COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT";
      case 0x8C4F:
        return "COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT";

      // OES_compressed_ETC1_RGB8_texture
      case 0x8D64:
        return "ETC1_RGB8_OES";

      // ARB_ES3_compatibility
      case 0x9274:
        return "COMPRESSED_RGB8_ETC2";
      case 0x9275:
        return "COMPRESSED_SRGB8_ETC2";
      case 0x9276:
        return "COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2";
      case 0x9277:
        return "COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2";
      case 0x9278:
        return "COMPRESSED_RGBA8_ETC2_EAC";
      case 0x9279:
        return "COMPRESSED_SRGB8_ALPHA8_ETC2_EAC";
      case 0x9270:
        return "COMPRESSED_R11_EAC";
      case 0x9271:
        return "COMPRESSED_SIGNED_R11_EAC";
      case 0x9272:
        return "COMPRESSED_RG11_EAC";
      case 0x9273:
        return "COMPRESSED_SIGNED_RG11_EAC";

      // KHR_texture_compression_astc_hdr
      case 0x93B0:
        return "COMPRESSED_RGBA_ASTC_4x4_KHR";
      case 0x93B1:
        return "COMPRESSED_RGBA_ASTC_5x4_KHR";
      case 0x93B2:
        return "COMPRESSED_RGBA_ASTC_5x5_KHR";
      case 0x93B3:
        return "COMPRESSED_RGBA_ASTC_6x5_KHR";
      case 0x93B4:
        return "COMPRESSED_RGBA_ASTC_6x6_KHR";
      case 0x93B5:
        return "COMPRESSED_RGBA_ASTC_8x5_KHR";
      case 0x93B6:
        return "COMPRESSED_RGBA_ASTC_8x6_KHR";
      case 0x93B7:
        return "COMPRESSED_RGBA_ASTC_8x8_KHR";
      case 0x93B8:
        return "COMPRESSED_RGBA_ASTC_10x5_KHR";
      case 0x93B9:
        return "COMPRESSED_RGBA_ASTC_10x6_KHR";
      case 0x93BA:
        return "COMPRESSED_RGBA_ASTC_10x8_KHR";
      case 0x93BB:
        return "COMPRESSED_RGBA_ASTC_10x10_KHR";
      case 0x93BC:
        return "COMPRESSED_RGBA_ASTC_12x10_KHR";
      case 0x93BD:
        return "COMPRESSED_RGBA_ASTC_12x12_KHR";
      case 0x93D0:
        return "COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR";
      case 0x93D1:
        return "COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR";
      case 0x93D2:
        return "COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR";
      case 0x93D3:
        return "COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR";
      case 0x93D4:
        return "COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR";
      case 0x93D5:
        return "COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR";
      case 0x93D6:
        return "COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR";
      case 0x93D7:
        return "COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR";
      case 0x93D8:
        return "COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR";
      case 0x93D9:
        return "COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR";
      case 0x93DA:
        return "COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR";
      case 0x93DB:
        return "COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR";
      case 0x93DC:
        return "COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR";
      case 0x93DD:
        return "COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR";

      default:
        break;
    }

    return "UNKNOWN";
  }

  std::vector<GraphicsInfo::Format> GraphicsInfo::getCompressedTextureFormats() {
    GLint count;
    glCheck(glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &count));

    std::vector<GLint> formats(count);

    if (count > 0) {
      glCheck(glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, &formats[0]));
      std::sort(formats.begin(), formats.end());
    }

    std::vector<Format> ret;

    for (auto format : formats) {
      ret.push_back({ format, std::string(getTextureFormatName(format)) });
    }

    return ret;
  }

  static const char *getShaderFormatName(GLint format) {
    (void) format;
    return "UNKNOWN";
  }

  std::vector<GraphicsInfo::Format> GraphicsInfo::getShaderBinaryFormats() {
    GLint count;
    glCheck(glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &count));

    std::vector<GLint> formats(count);

    if (count > 0) {
      glCheck(glGetIntegerv(GL_SHADER_BINARY_FORMATS, &formats[0]));
      std::sort(formats.begin(), formats.end());
    }

    std::vector<Format> ret;

    for (auto format : formats) {
      ret.push_back({ format, std::string(getShaderFormatName(format)) });
    }

    return ret;
  }


  static int getInteger(GLenum name) {
    GLint value;
    glCheck(glGetIntegerv(name, &value));
    return value;
  };

  int GraphicsInfo::getMaxCombinedTextureImageUnits() {
    return getInteger(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
  }

  int GraphicsInfo::getMaxCubeMapTextureSize() {
    return getInteger(GL_MAX_CUBE_MAP_TEXTURE_SIZE);
  }

  int GraphicsInfo::getMaxFragmentUniformVectors() {
    return getInteger(GL_MAX_FRAGMENT_UNIFORM_VECTORS);
  }

  int GraphicsInfo::getMaxRenderbufferSize() {
    return getInteger(GL_MAX_RENDERBUFFER_SIZE);
  }

  int GraphicsInfo::getMaxTextureImageUnits() {
    return getInteger(GL_MAX_TEXTURE_IMAGE_UNITS);
  }

  int GraphicsInfo::getMaxTextureSize() {
    return getInteger(GL_MAX_TEXTURE_SIZE);
  }

  int GraphicsInfo::getMaxVaryingVectors() {
    return getInteger(GL_MAX_VARYING_VECTORS);
  }

  int GraphicsInfo::getMaxVertexAttribs() {
    return getInteger(GL_MAX_VERTEX_ATTRIBS);
  }

  int GraphicsInfo::getMaxVertexTextureImageUnits() {
    return getInteger(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);
  }

  int GraphicsInfo::getMaxVertexUniformVectors() {
    return getInteger(GL_MAX_VERTEX_UNIFORM_VECTORS);
  }

  Vector2i GraphicsInfo::getMaxViewportDims() {
    GLint value[2];
    glCheck(glGetIntegerv(GL_MAX_VIEWPORT_DIMS, value));
    return { value[0], value[1] };
  }

  int GraphicsInfo::getSubpixelBits() {
    return getInteger(GL_SUBPIXEL_BITS);
  }




#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

