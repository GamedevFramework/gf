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
#ifndef GF_RENDER_PIPELINE_H
#define GF_RENDER_PIPELINE_H

#include <vector>

#include "Effects.h"
#include "Portability.h"
#include "RenderTarget.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Effect;
  class Window;

  /**
   * @ingroup graphics
   * @brief A render pipeline
   *
   * A render pipeline automates the application of post-processing effects.
   * A good way to use this class is to make a subclass with all the effects
   * you want to add.
   *
   * ~~~{.cc}
   * class MyPipeline : public gf::RenderPipeline {
   * public:
   *   MyPipeline(gf::Window& window)
   *   : gf::RenderPipeline(window)
   *   {
   *     addEffect(m_effect);
   *     addEffect(m_other);
   *   }
   *
   * protected:
   *   virtual void onFramebufferResize(Vector2u size) override {
   *     // update effects with size
   *   }
   *
   * private:
   *   MyEffect m_effect;
   *   MyOtherEffect m_other;
   * };
   * ~~~
   *
   * @sa gf::Effect
   */
  class GF_API RenderPipeline : public RenderTarget {
  public:
    /**
     * @brief Constructor
     *
     * @param window The window to render to
     */
    RenderPipeline(Window& window);

    /**
     * @brief Destructor
     */
    ~RenderPipeline();

    /**
     * @brief Add an effect to the pipeline
     *
     * @param effect The effect
     */
    void addEffect(Effect& effect);

    /**
     * @brief Clear the pipeline
     */
    void clearEffects();

    /**
     * @brief Update the size of the target
     *
     * This function must be called when the window change its size, before
     * anything is drawn on the target. You can do it in the event processing.
     *
     * ~~~{.cc}
     * gf::Event event;
     *
     * // ...
     *
     * switch (event.type) {
     *   case gf::EventType::Resized:
     *     renderer.resized();
     *     break;
     *
     *   // ...
     * }
     * ~~~
     */
    void resized();

    virtual Vector2u getSize() const override;

    /**
     * @brief Apply the effects and display what has been rendered
     */
    void display();

  protected:
    /**
     * @brief Callback when the screen has just been resized
     *
     * This function is called by resized() with the correct size.
     *
     * @param size The new framebuffer size
     */
    virtual void onFramebufferResize(Vector2u size);

  private:
    Window& m_window;

    struct TextureBuffer {
      Texture texture;
      unsigned name;
    };

    TextureBuffer m_buffers[2];
    int m_current;

    DefaultEffect m_defaultEffect;
    std::vector<Effect*> m_effects;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_RENDER_PIPELINE_H
