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
#ifndef GF_SCENE_MANAGER_H
#define GF_SCENE_MANAGER_H

#include <vector>

#include "Portability.h"
#include "Ref.h"
#include "RenderWindow.h"
#include "Scene.h"
#include "Window.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief A scene manager
   *
   * A scene manager handles several scenes, organised into a stack. It has a
   * window and a renderer and use them to handle the scenes.
   *
   * The top of the stack is the active scene. The manager runs the active
   * scene while the stack is not empty. All the scene in the stack receive
   * inputs, are updated and are rendered, beginning from the base of the
   * stack until the top.
   *
   * @sa gf::Scene
   */
  class GF_API SceneManager {
  public:
    /**
     * @brief Constructor
     *
     * @param title The title of the window
     * @param size The size of the window
     */
    SceneManager(StringRef title, Vector2i size);

    /**
     * @brief Run the scene manager until completion
     */
    void run();

    /**
     * @brief Add a scene on top of the stack
     *
     * @param scene The scene to add
     */
    void pushScene(Scene& scene);

    /**
     * @brief Remove the top scene from the non-empty stack
     */
    void popScene();

    /**
     * @brief Replace the top scene with a new scene
     *
     * It is equivalent to popping the current top scene and pushing the new scene.
     *
     * @param scene The scene to add
     */
    void replaceScene(Scene& scene) {
      popScene();
      pushScene(scene);
    }

    /**
     * @brief Get the renderer associated to the scene
     */
    const RenderTarget& getRenderer() const {
      return m_renderer;
    }

  private:
    Window m_window;
    RenderWindow m_renderer;
    std::vector<Ref<Scene>> m_scenes;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}


#endif // GF_SCENE_MANAGER_H
