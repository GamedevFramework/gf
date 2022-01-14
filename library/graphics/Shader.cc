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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#include <gf/Shader.h>

#include <cassert>
#include <fstream>
#include <memory>

#include <gf/Stream.h>
#include <gf/Log.h>

#include <gfpriv/GlDebug.h>
#include <gfpriv/GlFwd.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    std::string loadFile(const Path& filename) {
      std::ifstream file(filename.string());

      if (!file) {
        Log::error("File for shader does not exist: '%s'\n", filename.string().c_str());
        return "";
      }

      std::string content;

      for (std::string line; std::getline(file, line); ) {
        content.append(line);
        content.append(1, '\n');
      }

      Log::debug("Shader loaded from file: '%s'\n", filename.string().c_str());

      return content;
    }

    std::string loadStream(InputStream& stream) {
      std::string content;

      static constexpr std::size_t BufferSize = 1024;
      char buffer[BufferSize];

      while (!stream.isFinished()) {
        std::size_t size = stream.read(Span<uint8_t>(reinterpret_cast<uint8_t*>(buffer), BufferSize));
        content.insert(content.end(), buffer, buffer + size);
      }

      return content;
    }

    GLuint compileShader(const char *code, Shader::Type type) {
      GLuint id = 0;
      const char *typeString = nullptr;

      // create shader
      switch (type) {
        case Shader::Vertex:
          GL_CHECK(id = glCreateShader(GL_VERTEX_SHADER));
          typeString = "vertex";
          break;

        case Shader::Fragment:
          GL_CHECK(id = glCreateShader(GL_FRAGMENT_SHADER));
          typeString = "fragment";
          break;
      }

      if (id == 0) {
        return 0;
      }

      // compile
      const char *source[1] = { code };
      GL_CHECK(glShaderSource(id, 1, source, nullptr));
      GL_CHECK(glCompileShader(id));


      // and check
      GLint compileStatus = GL_FALSE;
      GL_CHECK(glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus));

      if (compileStatus == GL_FALSE) {
        GLint infoLogLength = 0;
        GL_CHECK(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength));

        assert(infoLogLength > 0);
        std::unique_ptr<char[]> infoLog(new char[infoLogLength]);
        GL_CHECK(glGetShaderInfoLog(id, infoLogLength, nullptr, infoLog.get()));

        Log::error("Error while compiling %s shader:\n%s\n", typeString, infoLog.get());
        return 0;
      }

      return id;
    }

    GLuint compile(const char *vertexShaderCode, const char *fragmentShaderCode) {
      assert(vertexShaderCode != nullptr || fragmentShaderCode != nullptr);

      GLuint program = 0;
      GL_CHECK(program = glCreateProgram());

      if (vertexShaderCode != nullptr) {
        GLuint id = compileShader(vertexShaderCode, Shader::Vertex);
        GL_CHECK(glAttachShader(program, id));
        GL_CHECK(glDeleteShader(id)); // the shader is still here because it is attached to the program
      }

      if (fragmentShaderCode != nullptr) {
        GLuint id = compileShader(fragmentShaderCode, Shader::Fragment);
        GL_CHECK(glAttachShader(program, id));
        GL_CHECK(glDeleteShader(id)); // the shader is still here because it is attached to the program
      }

      GL_CHECK(glLinkProgram(program));

      GLint linkStatus = GL_FALSE;
      GL_CHECK(glGetProgramiv(program, GL_LINK_STATUS, &linkStatus));

      if (linkStatus == GL_FALSE) {
        GLint infoLogLength;
        GL_CHECK(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength));

        assert(infoLogLength > 0);
        std::unique_ptr<char[]> infoLog(new char[infoLogLength]);
        GL_CHECK(glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog.get()));

        Log::error("Error while linking program:\n%s\n", infoLog.get());
        throw std::runtime_error("Error while linking program");
      }

      return program;
    }


  } // anonymous namespace

  Shader::Shader()
  : m_program(0)
  {

  }

  Shader::Shader(const Path& filename, Type type)
  : Shader(loadFile(filename).c_str(), type)
  {
  }

  Shader::Shader(const Path& vertexShaderFilename, const Path& fragmentShaderFilename)
  : Shader(loadFile(vertexShaderFilename).c_str(), loadFile(fragmentShaderFilename).c_str())
  {
  }

  Shader::Shader(const char *shader, Type type)
  : m_program(0)
  {
    if (shader == nullptr) {
      return;
    }

    switch (type) {
      case Vertex:
        m_program = compile(shader, nullptr);
        break;
      case Fragment:
        m_program = compile(nullptr, shader);
        break;
    }
  }

  Shader::Shader(const char *vertexShader, const char *fragmentShader)
  : m_program(0)
  {
    if (vertexShader == nullptr && fragmentShader == nullptr) {
      return;
    }

    m_program = compile(vertexShader, fragmentShader);
  }

  Shader::Shader(InputStream& stream, Type type)
  : Shader(loadStream(stream).c_str(), type)
  {
  }

  Shader::Shader(InputStream& vertexShaderStream, InputStream& fragmentShaderStream)
  : Shader(loadStream(vertexShaderStream).c_str(), loadStream(fragmentShaderStream).c_str())
  {
  }

  Shader::~Shader() {
    if (m_program != 0) {
      GL_CHECK(glDeleteProgram(m_program));
    }
  }


  struct Shader::Guard {
    explicit Guard(Shader& shader)
    : m_prev(0)
    , m_curr(shader.m_program)
    {
      GLint prev = 0;
      GL_CHECK(glGetIntegerv(GL_CURRENT_PROGRAM, &prev));
      m_prev = prev;

      if (m_prev != m_curr) {
        GL_CHECK(glUseProgram(m_curr));
      }
    }

    ~Guard() {
      if (m_prev != m_curr) {
        GL_CHECK(glUseProgram(m_prev));
      }
    }

    GLuint m_prev;
    GLuint m_curr;
  };

  void Shader::setUniform(const std::string& name, float val) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    GL_CHECK(glUniform1f(loc, val));
  }

  void Shader::setUniform(const std::string& name, int val) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    GL_CHECK(glUniform1i(loc, val));
  }

  void Shader::setUniform(const std::string& name, const Vector2f& vec) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    GL_CHECK(glUniform2f(loc, vec.x, vec.y));
  }

  void Shader::setUniform(const std::string& name, const Vector3f& vec) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    GL_CHECK(glUniform3f(loc, vec.x, vec.y, vec.z));
  }

  void Shader::setUniform(const std::string& name, const Vector4f& vec) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    GL_CHECK(glUniform4f(loc, vec.x, vec.y, vec.z, vec.w));
  }

  void Shader::setUniform(const std::string& name, const Vector2i& vec) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    GL_CHECK(glUniform2i(loc, vec.x, vec.y));
  }

  void Shader::setUniform(const std::string& name, const Vector3i& vec) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    GL_CHECK(glUniform3i(loc, vec.x, vec.y, vec.z));
  }

  void Shader::setUniform(const std::string& name, const Vector4i& vec) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    GL_CHECK(glUniform4i(loc, vec.x, vec.y, vec.z, vec.w));
  }

  void Shader::setUniform(const std::string& name, const Matrix3f& mat) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    GL_CHECK(glUniformMatrix3fv(loc, 1, GL_FALSE, mat.getData()));
  }

  void Shader::setUniform(const std::string& name, const Matrix4f& mat) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    GL_CHECK(glUniformMatrix4fv(loc, 1, GL_FALSE, mat.getData()));
  }

  void Shader::setUniform(const std::string& name, const BareTexture& tex) {
    int loc = getUniformLocation(name);

    if (loc == -1) {
      return;
    }

    auto it = m_textures.find(loc);

    if (it == m_textures.end()) {
      m_textures.insert(std::make_pair(loc, &tex));
    } else {
      it->second = &tex;
    }

  }

  int Shader::getUniformLocation(const std::string& name) {
    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(static_cast<GLuint>(m_program), name.c_str()));

//     if (loc == -1) {
//       Log::warning("Uniform not found: '%s'\n", name.c_str());
//     }

    return loc;
  }

  int Shader::getAttributeLocation(const std::string& name) {
    GLint loc;
    GL_CHECK(loc = glGetAttribLocation(static_cast<GLuint>(m_program), name.c_str()));

//     if (loc == -1) {
//       Log::warning("Attribute not found: '%s'\n", name.c_str());
//     }

    return loc;
  }

  void Shader::bind(const Shader *shader) {
    if (shader != nullptr && shader->m_program != 0) {
      GL_CHECK(glUseProgram(static_cast<GLuint>(shader->m_program)));

      // bind textures
      GLint index = 0;
      for (auto& item : shader->m_textures) {
        GL_CHECK(glActiveTexture(GL_TEXTURE0 + index));
        GL_CHECK(glUniform1i(item.first, index));
        BareTexture::bind(item.second);
        index++;
      }

    } else {
      GL_CHECK(glUseProgram(0));
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
