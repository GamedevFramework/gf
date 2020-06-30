/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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
#include <gf/ResourceManager.h>

#include <thread>

#include <SDL.h>

#include <gf/Log.h>
#include <gf/Sleep.h>

#include "priv/OpenGLFwd.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  namespace {

    template<typename T>
    class ResourceLoader {
    public:
      std::unique_ptr<T> operator()(const Path& filename) {
        return std::make_unique<T>(filename);
      }
    };

  }

  ResourceManager::ResourceManager()
  : m_images(ResourceLoader<Image>())
  , m_textures(ResourceLoader<Texture>())
  , m_fonts(ResourceLoader<Font>())
  {

  }

  ResourceManager::ResourceManager(std::initializer_list<Path> paths)
  : ResourceManager()
  {
    for (auto path : paths) {
      addSearchDir(path);
    }
  }

  void ResourceManager::asynchronousLoading(Window& window, std::function<void()> function) {
    m_sharedContext = window.getSharedGLContext();

    std::hash<std::thread::id> hasher;
    Log::debug("main thread id %lu\n", hasher(std::this_thread::get_id()));

    m_loadingThread = std::thread([this, function, hasher]{
      // Attach the context to the new thread
      SDL_GL_MakeCurrent(m_sharedContext.sdlWindow, m_sharedContext.sdlContext);
      Log::debug("loading thread id %lu\n", hasher(std::this_thread::get_id()));

      // Call the code wich loading something
      function();

      glFlush();
      // // Syncronize OpenGL contexts -- Unable to link to opengl
      // GLsync fenceId = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
      // for (;;) {
      //   // 5 Second timeout
      //   GLenum result = glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(5000000000));

      //   // Ignore timeouts and wait until all OpenGL commands are processed
      //   if(result != GL_TIMEOUT_EXPIRED) {
      //     break;
      //   }
      // }

      // Unbind the context from the thread
      SDL_GL_MakeCurrent(m_sharedContext.sdlWindow, nullptr);
    });
  }

  void ResourceManager::waitLoading() {
    m_loadingThread.join();

    // SDL_GL_DeleteContext(m_sharedContext.sdlContext);
    // m_sharedContext.sdlContext = nullptr;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
