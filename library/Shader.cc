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
 *
 * Part of this file comes from SFML, with the same license:
 * Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
 */
#include <gf/Shader.h>

#include <cassert>
#include <fstream>
#include <memory>

#include <glad/glad.h>

#include <gf/InputStream.h>
#include <gf/Log.h>

#include "priv/Debug.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  static std::string loadFile(const Path& filename) {
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

  static std::string loadStream(InputStream& stream) {
    std::string content;

    std::size_t size = stream.getSize();
    content.resize(size);
    stream.seek(0);

    std::size_t read = 0;

    do {
      read += stream.read(&content[read], size - read);
    } while (read < size);

    content.push_back('\0');
    return content;
  }

  static GLuint compileShader(const char *code, Shader::Type type) {
    GLuint id = 0;
    const char *typeString = nullptr;

    // create shader
    switch (type) {
      case Shader::Vertex:
        glCheck(id = glCreateShader(GL_VERTEX_SHADER));
        typeString = "vertex";
        break;

      case Shader::Fragment:
        glCheck(id = glCreateShader(GL_FRAGMENT_SHADER));
        typeString = "fragment";
        break;
    }

    if (id == 0) {
      return 0;
    }

    // compile
    const char *source[1] = { code };
    glCheck(glShaderSource(id, 1, source, nullptr));
    glCheck(glCompileShader(id));


    // and check
    GLint compileStatus = GL_FALSE;
    glCheck(glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus));

    if (compileStatus == GL_FALSE) {
      GLint infoLogLength = 0;
      glCheck(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength));

      assert(infoLogLength > 0);
      std::unique_ptr<char[]> infoLog(new char[infoLogLength]);
      glCheck(glGetShaderInfoLog(id, infoLogLength, nullptr, infoLog.get()));

      Log::error("Error while compiling %s shader:\n%s\n", typeString, infoLog.get());
      return 0;
    }

    return id;
  }

  Shader::Shader()
  : m_program(0)
  {

  }

  Shader::~Shader() {
    if (m_program != 0) {
      glCheck(glDeleteProgram(m_program));
    }
  }

  bool Shader::loadFromFile(const Path& filename, Type type) {
    std::string code = loadFile(filename);
    return loadFromMemory(code, type);
  }

  bool Shader::loadFromFile(const Path& vertexShaderFilename, const Path& fragmentShaderFilename) {
    std::string vertexShader = loadFile(vertexShaderFilename);
    std::string fragmentShader = loadFile(fragmentShaderFilename);
    return loadFromMemory(vertexShader, fragmentShader);
  }

  bool Shader::loadFromMemory(StringRef shader, Type type) {
    if (shader.isEmpty()) {
      return false;
    }

    switch (type) {
      case Vertex:
        return compile(shader.getData(), nullptr);
      case Fragment:
        return compile(nullptr, shader.getData());
    }

    return false;
  }

  bool Shader::loadFromMemory(StringRef vertexShader, StringRef fragmentShader) {
    const char *vertexShaderCode = vertexShader.isEmpty() ? nullptr : vertexShader.getData();
    const char *fragmentShaderCode = fragmentShader.isEmpty() ? nullptr : fragmentShader.getData();

    if (vertexShaderCode == nullptr && fragmentShaderCode == nullptr) {
      return false;
    }

    return compile(vertexShaderCode, fragmentShaderCode);
  }

  bool Shader::loadFromStream(InputStream& stream, Type type) {
    std::string code = loadStream(stream);
    return loadFromMemory(code, type);
  }

  bool Shader::loadFromStream(InputStream& vertexShaderStream, InputStream& fragmentShaderStream) {
    std::string vertexShader = loadStream(vertexShaderStream);
    std::string fragmentShader = loadStream(fragmentShaderStream);
    return loadFromMemory(vertexShader, fragmentShader);
  }

  bool Shader::compile(const char *vertexShaderCode, const char *fragmentShaderCode) {
    assert(vertexShaderCode || fragmentShaderCode);

    if (m_program != 0) {
      glCheck(glDeleteProgram(m_program));
    }

    glCheck(m_program = glCreateProgram());

    if (vertexShaderCode) {
      GLuint id = compileShader(vertexShaderCode, Vertex);
      glCheck(glAttachShader(m_program, id));
      glCheck(glDeleteShader(id)); // the shader is still here because it is attached to the program
    }

    if (fragmentShaderCode) {
      GLuint id = compileShader(fragmentShaderCode, Fragment);
      glCheck(glAttachShader(m_program, id));
      glCheck(glDeleteShader(id)); // the shader is still here because it is attached to the program
    }

    glCheck(glLinkProgram(m_program));

    GLint linkStatus = GL_FALSE;
    glCheck(glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus));

    if (linkStatus == GL_FALSE) {
      GLint infoLogLength;
      glCheck(glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength));

      assert(infoLogLength > 0);
      std::unique_ptr<char[]> infoLog(new char[infoLogLength]);
      glCheck(glGetProgramInfoLog(m_program, infoLogLength, nullptr, infoLog.get()));

      Log::error("Error while linking program:\n%s\n", infoLog.get());
      return false;
    }

    return true;
  }

  struct Shader::Guard {
    explicit Guard(Shader& shader)
    : m_prev(0)
    , m_curr(shader.m_program)
    {
      GLint prev = 0;
      glCheck(glGetIntegerv(GL_CURRENT_PROGRAM, &prev));
      m_prev = prev;

      if (m_prev != m_curr) {
        glCheck(glUseProgram(m_curr));
      }
    }

    ~Guard() {
      if (m_prev != m_curr) {
        glCheck(glUseProgram(m_prev));
      }
    }

    GLuint m_prev;
    GLuint m_curr;
  };

  void Shader::setUniform(StringRef name, float val) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    glCheck(glUniform1f(loc, val));
  }

  void Shader::setUniform(StringRef name, int val) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    glCheck(glUniform1i(loc, val));
  }

  void Shader::setUniform(StringRef name, const Vector2f& vec) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    glCheck(glUniform2f(loc, vec.x, vec.y));
  }

  void Shader::setUniform(StringRef name, const Vector3f& vec) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    glCheck(glUniform3f(loc, vec.x, vec.y, vec.z));
  }

  void Shader::setUniform(StringRef name, const Vector4f& vec) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    glCheck(glUniform4f(loc, vec.x, vec.y, vec.z, vec.w));
  }

  void Shader::setUniform(StringRef name, const Matrix3f& mat) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    glCheck(glUniformMatrix3fv(loc, 1, GL_FALSE, mat.data));
  }

  void Shader::setUniform(StringRef name, const Matrix4f& mat) {
    Guard guard(*this);
    int loc = getUniformLocation(name);
    glCheck(glUniformMatrix4fv(loc, 1, GL_FALSE, mat.data));
  }

  void Shader::setUniform(StringRef name, const BareTexture& tex) {
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

  int Shader::getUniformLocation(StringRef name) {
    GLint loc;
    glCheck(loc = glGetUniformLocation(static_cast<GLuint>(m_program), name.getData()));

    if (loc == -1) {
      Log::warning("Uniform not found: '%s'\n", name.getData());
    }

    return loc;
  }

  int Shader::getAttributeLocation(StringRef name) {
    GLint loc;
    glCheck(loc = glGetAttribLocation(static_cast<GLuint>(m_program), name.getData()));

    if (loc == -1) {
      Log::warning("Attribute not found: '%s'\n", name.getData());
    }

    return loc;
  }

  void Shader::bind(const Shader *shader) {
    if (shader && shader->m_program != 0) {
      glCheck(glUseProgram(static_cast<GLuint>(shader->m_program)));

      // bind textures
      GLint index = 0;
      for (auto& item : shader->m_textures) {
        glCheck(glActiveTexture(GL_TEXTURE0 + index));
        glCheck(glUniform1i(item.first, index));
        BareTexture::bind(item.second);
        index++;
      }

    } else {
      glCheck(glUseProgram(0));
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
