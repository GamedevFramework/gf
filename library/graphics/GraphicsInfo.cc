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
#include <gf/GraphicsInfo.h>

#include <cassert>

#include <algorithm>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <gf/Log.h>

#include <gfpriv/GlDebug.h>
#include <gfpriv/GlFwd.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    std::string getString(GLenum name) {
      const GLubyte *str = glGetString(name);

      GLenum code = glGetError();

      if (code != GL_NO_ERROR) {
        assert(code == GL_INVALID_ENUM);
        Log::error("Error in OpenGL call to glGetString: GL_INVALID_ENUM\n");
        return "";
      }

      return std::string(reinterpret_cast<const char *>(str));
    }

    int getInteger(GLenum name) {
      GLint value;
      GL_CHECK(glGetIntegerv(name, &value));
      return value;
    }

  } // anonymous namespace

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
    std::vector<std::string> ret;
#ifdef GF_OPENGL3
    int count = getInteger(GL_NUM_EXTENSIONS);

    for (int i = 0; i < count; ++i) {
      const GLubyte *str = glGetStringi(GL_EXTENSIONS, i);
      std::string extension = reinterpret_cast<const char *>(str);

      GLenum code = glGetError();

      if (code != GL_NO_ERROR) {
        switch (code) {
          case GL_INVALID_ENUM:
            Log::error("Error in OpenGL call to glGetStringi: GL_INVALID_ENUM\n");
            break;

          case GL_INVALID_VALUE:
            Log::error("Error in OpenGL call to glGetStringi: GL_INVALID_VALUE\n");
            break;

          default:
            assert(false);
            break;
        }

        continue;
      }

      ret.push_back(std::move(extension));
    }
#else
    std::string extensions = getString(GL_EXTENSIONS);
    boost::algorithm::split(ret, extensions, boost::is_any_of(" \n"), boost::algorithm::token_compress_on);
    ret.erase(std::remove_if(ret.begin(), ret.end(), [](const std::string& ext) { return ext.empty(); }), ret.end());
#endif
    return ret;
  }


  namespace {

    struct TextureFormatString {
      GLint format;
      const char *string;
    };

    TextureFormatString TextureFormats[] = {
      // EXT_texture_compression_s3tc
      { 0x83F0, "GL_COMPRESSED_RGB_S3TC_DXT1_EXT" },
      { 0x83F1, "GL_COMPRESSED_RGBA_S3TC_DXT1_EXT" },
      { 0x83F2, "GL_COMPRESSED_RGBA_S3TC_DXT3_EXT" },
      { 0x83F3, "GL_COMPRESSED_RGBA_S3TC_DXT5_EXT" },

      // 3DFX_texture_compression_FXT1
      { 0x86B0, "COMPRESSED_RGB_FXT1_3DFX" },
      { 0x86B1, "COMPRESSED_RGBA_FXT1_3DFX" },

      // NV_sRGB_formats
      { 0x88EE, "ETC1_SRGB8_NV" },

      // OES_compressed_paletted_texture
      { 0x8B90, "PALETTE4_RGB8_OES" },
      { 0x8B91, "PALETTE4_RGBA8_OES" },
      { 0x8B92, "PALETTE4_R5_G6_B5_OES" },
      { 0x8B93, "PALETTE4_RGBA4_OES" },
      { 0x8B94, "PALETTE4_RGB5_A1_OES" },
      { 0x8B95, "PALETTE8_RGB8_OES" },
      { 0x8B96, "PALETTE8_RGBA8_OES" },
      { 0x8B97, "PALETTE8_R5_G6_B5_OES" },
      { 0x8B98, "PALETTE8_RGBA4_OES" },
      { 0x8B99, "PALETTE8_RGB5_A1_OES" },

      // EXT_texture_sRGB
      { 0x8C40, "SRGB_EXT" },
      { 0x8C41, "SRGB8_EXT" },
      { 0x8C42, "SRGB_ALPHA_EXT" },
      { 0x8C43, "SRGB8_ALPHA8_EXT" },
      { 0x8C44, "SLUMINANCE_ALPHA_EXT" },
      { 0x8C45, "SLUMINANCE8_ALPHA8_EXT" },
      { 0x8C46, "SLUMINANCE_EXT" },
      { 0x8C47, "SLUMINANCE8_EXT" },
      { 0x8C48, "COMPRESSED_SRGB_EXT" },
      { 0x8C49, "COMPRESSED_SRGB_ALPHA_EXT" },
      { 0x8C4A, "COMPRESSED_SLUMINANCE_EXT" },
      { 0x8C4B, "COMPRESSED_SLUMINANCE_ALPHA_EXT" },
      { 0x8C4C, "COMPRESSED_SRGB_S3TC_DXT1_EXT" },
      { 0x8C4D, "COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT" },
      { 0x8C4E, "COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT" },
      { 0x8C4F, "COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT" },

      // OES_compressed_ETC1_RGB8_texture
      { 0x8D64, "ETC1_RGB8_OES" },

      // EXT_texture_compression_rgtc
      { 0x8DBB, "COMPRESSED_RED_RGTC1_EXT" },
      { 0x8DBC, "COMPRESSED_SIGNED_RED_RGTC1_EXT" },
      { 0x8DBD, "COMPRESSED_RED_GREEN_RGTC2_EXT" },
      { 0x8DBE, "COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT" },

      // ARB_texture_compression_bptc
      { 0x8E8C, "COMPRESSED_RGBA_BPTC_UNORM_ARB" },
      { 0x8E8D, "COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB" },
      { 0x8E8E, "COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB" },
      { 0x8E8F, "COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB" },

      // ARB_ES3_compatibility
      { 0x9270, "COMPRESSED_R11_EAC" },
      { 0x9271, "COMPRESSED_SIGNED_R11_EAC" },
      { 0x9272, "COMPRESSED_RG11_EAC" },
      { 0x9273, "COMPRESSED_SIGNED_RG11_EAC" },
      { 0x9274, "COMPRESSED_RGB8_ETC2" },
      { 0x9275, "COMPRESSED_SRGB8_ETC2" },
      { 0x9276, "COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2" },
      { 0x9277, "COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2" },
      { 0x9278, "COMPRESSED_RGBA8_ETC2_EAC" },
      { 0x9279, "COMPRESSED_SRGB8_ALPHA8_ETC2_EAC" },

      // KHR_texture_compression_astc_hdr
      { 0x93B0, "COMPRESSED_RGBA_ASTC_4x4_KHR" },
      { 0x93B1, "COMPRESSED_RGBA_ASTC_5x4_KHR" },
      { 0x93B2, "COMPRESSED_RGBA_ASTC_5x5_KHR" },
      { 0x93B3, "COMPRESSED_RGBA_ASTC_6x5_KHR" },
      { 0x93B4, "COMPRESSED_RGBA_ASTC_6x6_KHR" },
      { 0x93B5, "COMPRESSED_RGBA_ASTC_8x5_KHR" },
      { 0x93B6, "COMPRESSED_RGBA_ASTC_8x6_KHR" },
      { 0x93B7, "COMPRESSED_RGBA_ASTC_8x8_KHR" },
      { 0x93B8, "COMPRESSED_RGBA_ASTC_10x5_KHR" },
      { 0x93B9, "COMPRESSED_RGBA_ASTC_10x6_KHR" },
      { 0x93BA, "COMPRESSED_RGBA_ASTC_10x8_KHR" },
      { 0x93BB, "COMPRESSED_RGBA_ASTC_10x10_KHR" },
      { 0x93BC, "COMPRESSED_RGBA_ASTC_12x10_KHR" },
      { 0x93BD, "COMPRESSED_RGBA_ASTC_12x12_KHR" },
      { 0x93D0, "COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR" },
      { 0x93D1, "COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR" },
      { 0x93D2, "COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR" },
      { 0x93D3, "COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR" },
      { 0x93D4, "COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR" },
      { 0x93D5, "COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR" },
      { 0x93D6, "COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR" },
      { 0x93D7, "COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR" },
      { 0x93D8, "COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR" },
      { 0x93D9, "COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR" },
      { 0x93DA, "COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR" },
      { 0x93DB, "COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR" },
      { 0x93DC, "COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR" },
      { 0x93DD, "COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR" },
    };

    bool operator<(const TextureFormatString& lhs, GLint format) {
      return lhs.format < format;
    }

    bool operator<(GLint format, const TextureFormatString& rhs) {
      return format < rhs.format;
    }

    const char *getTextureFormatName(GLint format) {
      auto it = std::lower_bound(std::begin(TextureFormats), std::end(TextureFormats), format);

      if (it != std::end(TextureFormats) && !(format < *it)) {
        return it->string;
      }

      return "UNKNOWN";
    }

    const char *getShaderFormatName([[maybe_unused]] GLint format) {
      return "UNKNOWN";
    }

  } // anonymous namespace

  std::vector<GraphicsInfo::Format> GraphicsInfo::getCompressedTextureFormats() {
    GLint count;
    GL_CHECK(glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &count));

    std::vector<GLint> formats(count);

    if (count > 0) {
      GL_CHECK(glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, &formats[0]));
      std::sort(formats.begin(), formats.end());
    }

    std::vector<Format> ret;

    for (auto format : formats) {
      ret.push_back({ format, std::string(getTextureFormatName(format)) });
    }

    return ret;
  }

  std::vector<GraphicsInfo::Format> GraphicsInfo::getShaderBinaryFormats() {
    GLint count;
    GL_CHECK(glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &count));

    std::vector<GLint> formats(count);

    if (count > 0) {
      GL_CHECK(glGetIntegerv(GL_SHADER_BINARY_FORMATS, &formats[0]));
      std::sort(formats.begin(), formats.end());
    }

    std::vector<Format> ret;

    for (auto format : formats) {
      ret.push_back({ format, std::string(getShaderFormatName(format)) });
    }

    return ret;
  }


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
    GL_CHECK(glGetIntegerv(GL_MAX_VIEWPORT_DIMS, value));
    return { value[0], value[1] };
  }

  int GraphicsInfo::getSubpixelBits() {
    return getInteger(GL_SUBPIXEL_BITS);
  }




#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

