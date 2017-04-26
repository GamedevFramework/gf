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
#ifndef GF_RENDER_TARGET_H
#define GF_RENDER_TARGET_H

#include <cstdint>

#include "Image.h"
#include "Matrix.h"
#include "Portability.h"
#include "PrimitiveType.h"
#include "Range.h"
#include "Region.h"
#include "RenderStates.h"
#include "Shader.h"
#include "View.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  class Drawable;
  class VertexBuffer;
  struct Vertex;

  /**
   * @ingroup graphics
   * @brief Base class for all render targets (window, texture, ...)
   *
   * gf::RenderTarget defines the common behavior of all the
   * 2D render targets usable in the graphics module. It makes
   * it possible to draw 2D entities like sprites, shapes, text
   * without using any OpenGL command directly.
   *
   * A gf::RenderTarget is also able to use views (gf::View),
   * which are a kind of 2D cameras. With views you can globally
   * scroll, rotate or zoom everything that is drawn,
   * without having to transform every single entity. See the
   * documentation of gf::View for more details and sample pieces of
   * code about this class.
   *
   * @sa gf::RenderWindow, gf::RenderTexture, gf::View
   */
  class GF_API RenderTarget {
  public:

    /**
     * @brief Default constructor
     */
    RenderTarget() = default;

    /**
     * @brief Destructor
     */
    virtual ~RenderTarget();

    /**
     * @brief Deleted copy constructor
     */
    RenderTarget(const RenderTarget&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    RenderTarget& operator=(const RenderTarget&) = delete;

    /**
     * @brief Return the size of the rendering region of the target
     *
     * @return Size in pixels
     */
    virtual Vector2u getSize() const = 0;

    /**
     * @name Drawing commands
     * @{
     */

    /**
     * @brief Get the current canonical scissor box
     *
     * @return The current canonical scissor box
     */
    Region getCanonicalScissorBox();

    /**
     * @brief Define the canonical scissor box
     *
     * @param box The new canonical scissor box
     */
    void setCanonicalScissorBox(const Region& box);

    /**
     * @brief Get the current scissor box
     *
     * @return The current scissor box
     */
    RectI getScissorBox();

    /**
     * @brief Define the scissor box
     *
     * @param box The new scissor box
     */
    void setScissorBox(const RectI& box);

    /**
     * @brief Clear the entire target with a single color
     *
     * This function is usually called once every frame,
     * to clear the previous contents of the target.
     *
     * @param color Fill color to use to clear the render target
     */
    void clear(const Color4f& color);

    /**
     * @brief Clear the entire target
     *
     * This function is usually called once every frame,
     * to clear the previous contents of the target.
     *
     * The color used to clear the target is the last color passed
     * to the other version of clear().
     */
    void clear();

    /**
     * @brief Get the range for aliased line width
     *
     * @return A range for the line width
     * @sa setLineWidth(), getLineWidth()
     */
    RangeF getAliasedLineWidthRange() const;

    /**
     * @brief Get the line width
     *
     * @return The current line width
     * @sa setLineWidth(), getAliasedLineWidthRange()
     */
    float getLineWidth() const;

    /**
     * @brief Draw primitives defined by an array of vertices
     *
     * @param vertices Pointer to the vertices
     * @param count Number of vertices in the array
     * @param type Type of primitives to draw
     * @param states Render states to use for drawing
     */
    void draw(const Vertex *vertices, std::size_t count, PrimitiveType type, const RenderStates& states = RenderStates());

    /**
     * @brief Draw primitives defined by an array of vertices and their indices
     *
     * @param vertices Pointer to the vertices
     * @param indices Pointer to the indices
     * @param count Number of indices in the array
     * @param type Type of primitives to draw
     * @param states Render states to use for drawing
     */
    void draw(const Vertex *vertices, const uint16_t *indices, std::size_t count, PrimitiveType type, const RenderStates& states = RenderStates());

    /**
     * @brief Draw primitives defined by an array of vertices
     *
     * @param vertices Pointer to the vertices
     * @param first Array of starting indices
     * @param count Array of number of vertices
     * @param primcount Number of elements in `first` and `count`
     * @param type Type of primitives to draw
     * @param states Render states to use for drawing
     */
    void draw(const Vertex *vertices, int *first, const std::size_t *count, std::size_t primcount, PrimitiveType type, const RenderStates& states = RenderStates());

    /**
     * @brief Draw primitives defined by an array of vertices and their indices
     *
     * @param vertices Pointer to the vertices
     * @param indices Array of pointers to the indices
     * @param count Array of number of indices
     * @param primcount Number of elements in `indices` and `count`
     * @param type Type of primitives to draw
     * @param states Render states to use for drawing
     */
    void draw(const Vertex *vertices, const uint16_t **indices, const std::size_t *count, std::size_t primcount, PrimitiveType type, const RenderStates& states = RenderStates());

    /**
     * @brief Draw a vertex buffer to the render target
     *
     * @param buffer A vertex buffer containing a geometry
     * @param states Render states to use for drawing
     */
    void draw(const VertexBuffer& buffer, const RenderStates& states = RenderStates());

    /**
     * @brief Draw a drawable object to the render target
     *
     * @param drawable Object to draw
     * @param states Render states to use for drawing
     */
    void draw(Drawable& drawable, const RenderStates& states = RenderStates());

    /** @} */

    /**
     * @name View management
     * @{
     */

    /**
     * @brief Change the current active view
     *
     * The view is like a 2D camera, it controls which part of
     * the 2D scene is visible, and how it is viewed in the
     * render target.
     *
     * The new view will affect everything that is drawn, until
     * another view is set.
     *
     * The render target keeps its own copy of the view object,
     * so it is not necessary to keep the original one alive
     * after calling this function.
     *
     * @param view New view to use
     *
     * @sa getView()
     */
    void setView(const View& view);

    /**
     * @brief Get the view currently in use in the render target
     *
     * @return The view object that is currently used
     *
     * @sa setView()
     */
    const View& getView() const {
      return m_view;
    }

    /**
     * @brief Get the canonical viewport of a view, applied to this render target
     *
     * The viewport is defined in the view as a ratio, this function
     * simply applies this ratio to the current dimensions of the
     * render target to calculate the pixels rectangle that the viewport
     * actually covers in the target.
     *
     * @param view The view for which we want to compute the viewport
     *
     * @return Canonical viewport rectangle, expressed in pixels
     */
    Region getCanonicalViewport(const View& view) const;

    /**
     * @brief Get the viewport of a view, applied to this render target
     *
     * The viewport is defined in the view as a ratio, this function
     * simply applies this ratio to the current dimensions of the
     * render target to calculate the pixels rectangle that the viewport
     * actually covers in the target.
     *
     * @param view The view for which we want to compute the viewport
     *
     * @return Viewport rectangle, expressed in pixels
     */
    RectI getViewport(const View& view) const;

    /**
     * @brief Convert a point from target coordinates to world coordinates
     *
     * This function finds the 2D position that matches the
     * given pixel of the render target. In other words, it does
     * the inverse of what the graphics card does, to find the
     * initial position of a rendered pixel.
     *
     * Initially, both coordinate systems (world units and target pixels)
     * match perfectly. But if you define a custom view or resize your
     * render target, this assertion is not true anymore, i.e. a point
     * located at @f$(10, 50)@f$ in your render target may map to the point
     * @f$(150, 75)@f$ in your 2D world -- if the view is translated by
     * @f$(140, 25)@f$.
     *
     * For windows, this function is typically used to find
     * which point (or object) is located below the mouse cursor.
     *
     * This version uses a custom view for calculations, see the other
     * overload of the function if you want to use the current view of the
     * render target.
     *
     * @param point Pixel to convert
     * @param view The view to use for converting the point
     *
     * @return The converted point, in world coordinates
     *
     * @sa mapCoordsToPixel()
     */
    Vector2f mapPixelToCoords(Vector2i point, const View& view) const;

    /**
     * @brief Convert a point from target coordinates to world
     * coordinates, using the current view
     *
     * This function is an overload of the mapPixelToCoords()
     * function that implicitly uses the current view.
     *
     * It is equivalent to:
     *
     * ~~~{.cc}
     * target.mapPixelToCoords(point, target.getView());
     * ~~~
     *
     * @param point Pixel to convert
     *
     * @return The converted point, in world coordinates
     *
     * @sa mapCoordsToPixel()
     */
    Vector2f mapPixelToCoords(Vector2i point) const;

    /**
     * @brief Convert a point from world coordinates to target coordinates
     *
     * This function finds the pixel of the render target that matches
     * the given 2D point. In other words, it goes through the same process
     * as the graphics card, to compute the final position of a rendered point.
     *
     * Initially, both coordinate systems (world units and target pixels)
     * match perfectly. But if you define a custom view or resize your
     * render target, this assertion is not true anymore, i.e. a point
     * located at @f$(150, 75)@f$ in your 2D world may map to the pixel
     * @f$(10, 50)@f$ of your render target -- if the view is translated by
     * @f$(140, 25)@f$.
     *
     * This version uses a custom view for calculations, see the other
     * overload of the function if you want to use the current view of the
     * render target.
     *
     * @param point Point to convert
     * @param view The view to use for converting the point
     *
     * @return The converted point, in target coordinates (pixels)
     *
     * @sa mapPixelToCoords()
     */
    Vector2i mapCoordsToPixel(Vector2f point, const View& view) const;

    /**
     * @brief Convert a point from world coordinates to target
     * coordinates, using the current view
     *
     * This function is an overload of the mapCoordsToPixel
     * function that implicitly uses the current view.
     *
     * It is equivalent to:
     *
     * ~~~{.cc}
     * target.mapCoordsToPixel(point, target.getView());
     * ~~~
     *
     * @param point Point to convert
     *
     * @return The converted point, in target coordinates (pixels)
     *
     * @sa mapPixelToCoords()
     */
    Vector2i mapCoordsToPixel(Vector2f point) const;

    /** @} */

  protected:
    /**
     * @brief Performs the common initialization step after creation
     *
     * The derived classes must call this function after the
     * target is created and ready for drawing.
     */
    void initialize();


    /**
     * @brief Capture the given framebuffer
     *
     * @param name The name of the framebuffer
     */
    Image captureFramebuffer(unsigned name) const;

  private:
    void initializeViews();
    void initializeShader();
    void initializeTexture();

    struct Locations {
      int positionLoc;
      int colorLoc;
      int texCoordsLoc;
    };

    void drawStart(const Vertex *vertices, const RenderStates& states, Locations& locations);
    void drawFinish(const Locations& locations);

  private:
    View m_view;
    Shader m_defaultShader;
    Shader m_defaultAlphaShader;
    Texture m_defaultTexture;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_RENDER_TARGET_H
