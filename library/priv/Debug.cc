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
#include "Debug.h"

#include <glad/glad.h>

#include <gf/Log.h>

namespace gf {
  namespace priv {

    void glCheckError(const char *file, unsigned int line, const char *expr) {
      GLenum code = glGetError();

      if (code == GL_NO_ERROR) {
        return;
      }

      const char *name = "Unknown";
      const char *desc = "-";

      switch (code) {
        case GL_INVALID_ENUM:
          name = "GL_INVALID_ENUM";
          desc = "An unacceptable value is specified for an enumerated argument.";
          break;
        case GL_INVALID_VALUE:
          name = "GL_INVALID_VALUE";
          desc = "A numeric argument is out of range.";
          break;
        case GL_INVALID_OPERATION:
          name = "GL_INVALID_OPERATION";
          desc = "The specified operation is not allowed in the current state.";
          break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
          name = "GL_INVALID_FRAMEBUFFER_OPERATION";
          desc = "The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete.";
          break;
        case GL_OUT_OF_MEMORY:
          name = "GL_OUT_OF_MEMORY";
          desc = "There is not enough memory left to execute the command.";
          break;
        default:
          break;
      }

      Log::error("Error in OpenGL call:\n\tLocation: %s:%u\n\tExpression: %s\n\tError: %s\n\tDescription: %s\n",
        file, line, expr, name, desc
      );
    }

  }
}
