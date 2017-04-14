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
#ifndef GF_BLEND_H
#define GF_BLEND_H

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup graphics
   * @brief Enumeration of the blending equations
   *
   * The equations are mapped directly to their OpenGL equivalents,
   * specified by [glBlendEquation()](http://docs.gl/es2/glBlendEquation) or
   * [glBlendEquationSeparate()](http://docs.gl/es2/glBlendEquationSeparate).
   *
   * @sa gf::BlendMode
   */
  enum class BlendEquation {
    Add,              ///< Pixel = Src * SrcFactor + Dst * DstFactor
    Substract,        ///< Pixel = Src * SrcFactor - Dst * DstFactor
    ReverseSubstract, ///< Pixel = Dst * DstFactor - Src * SrcFactor
  };

  /**
   * @ingroup graphics
   * @brief Enumeration of the blending factors
   *
   * The factors are mapped directly to their OpenGL equivalents,
   * specified by [glBlendFunc()](http://docs.gl/es2/glBlendFunc) or
   * [glBlendFuncSeparate()](http://docs.gl/es2/glBlendFuncSeparate).
   *
   * @sa gf::BlendMode
   */
  enum class BlendFactor {
    Zero,             ///< (0, 0, 0, 0)
    One,              ///< (1, 1, 1, 1)
    SrcColor,         ///< (src.r, src.g, src.b, src.a)
    OneMinusSrcColor, ///< (1, 1, 1, 1) - (src.r, src.g, src.b, src.a)
    DstColor,         ///< (dst.r, dst.g, dst.b, dst.a)
    OneMinusDstColor, ///< (1, 1, 1, 1) - (dst.r, dst.g, dst.b, dst.a)
    SrcAlpha,         ///< (src.a, src.a, src.a, src.a)
    OneMinusSrcAlpha, ///< (1, 1, 1, 1) - (src.a, src.a, src.a, src.a)
    DstAlpha,         ///< (dst.a, dst.a, dst.a, dst.a)
    OneMinusDstAlpha  ///< (1, 1, 1, 1) - (dst.a, dst.a, dst.a, dst.a)
  };

  /**
   * @ingroup graphics
   * @brief Blending modes for drawing.
   *
   * gf::BlendMode is a class that represents a
   * [blend mode](https://www.opengl.org/wiki/Blending). A blend
   * mode determines how the colors of an object you draw are
   * mixed with the colors that are already in the buffer.
   *
   * The class is composed of 6 components, each of which has its
   * own public member variable:
   *
   * - color source factor (@ref colorSrcFactor)
   * - color destination Factor (@ref colorDstFactor)
   * - color blend equation (@ref colorEquation)
   * - alpha source factor (@ref alphaSrcFactor)
   * - alpha destination factor (@ref alphaDstFactor)
   * - alpha blend equation (@ref alphaEquation)
   *
   * The source factor specifies how the pixel you are drawing contributes
   * to the final color. The destination factor specifies how the pixel
   * already drawn in the buffer contributes to the final color.
   *
   * The color channels RGB (red, green, blue; simply referred to as
   * color) and A (alpha; the transparency) can be treated separately. This
   * separation can be useful for specific blend modes, but most often you
   * won't need it and will simply treat the color as a single unit.
   *
   * The blend factors and equations correspond to their OpenGL equivalents.
   * In general, the color of the resulting pixel is calculated according
   * to the following formula (`src` is the color of the source pixel, `dst`
   * the color of the destination pixel, the other variables correspond to the
   * public members, with the equations being + or - operators):
   *
   * ~~~
   * dst.rgb = colorSrcFactor * src.rgb (colorEquation) colorDstFactor * dst.rgb
   * dst.a   = alphaSrcFactor * src.a   (alphaEquation) alphaDstFactor * dst.a
   * ~~~
   *
   * All factors and colors are represented as floating point numbers between
   * 0 and 1. Where necessary, the result is clamped to fit in that range.
   *
   * The most common blending modes are defined as constants
   * in the `gf::PredefinedBlendMonde` namespace:
   *
   * ~~~{.cc}
   * gf::BlendMode alphaBlending          = gf::BlendAlpha;
   * gf::BlendMode additiveBlending       = gf::BlendAdd;
   * gf::BlendMode multiplicativeBlending = gf::BlendMultiply;
   * gf::BlendMode noBlending             = gf::BlendNone;
   * ~~~
   *
   * A blend mode can be specified every time you draw a gf::Drawable
   * object to a gf::RenderWindow. It is part of the gf::RenderStates compound
   * that is passed to the member function gf::RenderTarget::draw().
   *
   * @sa gf::BlendFactor, gf::BlendEquation, gf::RenderStates, gf::RenderWindow
   *
   */
  struct GF_API BlendMode {

    /**
     * @brief Default constructor
     *
     * Constructs a blending mode that does alpha blending.
     */
    constexpr BlendMode()
    : colorSrcFactor(BlendFactor::One)
    , colorDstFactor(BlendFactor::Zero)
    , colorEquation(BlendEquation::Add)
    , alphaSrcFactor(BlendFactor::One)
    , alphaDstFactor(BlendFactor::Zero)
    , alphaEquation(BlendEquation::Add)
    {

    }

    /**
     * @brief Construct the blend mode given the factors and equation.
     *
     * This constructor uses the same factors and equation for both
     * color and alpha components. It also defaults to the Add equation.
     *
     * @param sourceFactor      Specifies how to compute the source factor for the color and alpha channels.
     * @param destinationFactor Specifies how to compute the destination factor for the color and alpha channels.
     * @param equation          Specifies how to combine the source and destination colors and alpha.
     */
    constexpr BlendMode(BlendFactor sourceFactor, BlendFactor destinationFactor, BlendEquation equation = BlendEquation::Add)
    : colorSrcFactor(sourceFactor)
    , colorDstFactor(destinationFactor)
    , colorEquation(equation)
    , alphaSrcFactor(sourceFactor)
    , alphaDstFactor(destinationFactor)
    , alphaEquation(equation)
    {

    }

    /**
     * @brief Construct the blend mode given the factors and equation.
     *
     * @param colorSourceFactor       Specifies how to compute the source function for the color channels.
     * @param colorDestinationFactor  Specifies how to compute the destination factor for the color channels.
     * @param colorBlendEquation      Specifies how to combine the source and destination colors.
     * @param alphaSourceFactor       Specifies how to compute the source factor.
     * @param alphaDestinationFactor  Specifies how to compute the destination factor.
     * @param alphaBlendEquation      Specifies how to combine the source and destination alphas.
     */
    constexpr BlendMode(BlendFactor colorSourceFactor, BlendFactor colorDestinationFactor, BlendEquation colorBlendEquation,
                BlendFactor alphaSourceFactor, BlendFactor alphaDestinationFactor, BlendEquation alphaBlendEquation)
    : colorSrcFactor(colorSourceFactor)
    , colorDstFactor(colorDestinationFactor)
    , colorEquation(colorBlendEquation)
    , alphaSrcFactor(alphaSourceFactor)
    , alphaDstFactor(alphaDestinationFactor)
    , alphaEquation(alphaBlendEquation)
    {

    }

    BlendFactor colorSrcFactor;   ///< Source blending factor for the color channels
    BlendFactor colorDstFactor;   ///< Destination blending factor for the color channels
    BlendEquation colorEquation;  ///< Blending equation for the color channels
    BlendFactor alphaSrcFactor;   ///< Source blending factor for the alpha channel
    BlendFactor alphaDstFactor;   ///< Destination blending factor for the alpha channel
    BlendEquation alphaEquation;  ///< Blending equation for the alpha channel
  };

  /**
   * @ingroup graphics
   * @brief Alpha blend mode
   *
   * Blend source and dest according to dest alpha. Also known as
   * [alpha blending](https://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending).
   *
   * @sa gf::BlendMode
   */
  constexpr BlendMode BlendAlpha
  #ifndef DOXYGEN_SHOULD_SKIP_THIS
  = BlendMode(
    BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha, BlendEquation::Add,
    BlendFactor::One, BlendFactor::OneMinusSrcAlpha, BlendEquation::Add
  )
  #endif
  ;

  /**
   * @ingroup graphics
   * @brief Additive blend mode
   *
   * Add source to dest.
   *
   * @sa gf::BlendMode
   */
  constexpr BlendMode BlendAdd
  #ifndef DOXYGEN_SHOULD_SKIP_THIS
  = BlendMode(
    BlendFactor::SrcAlpha, BlendFactor::One, BlendEquation::Add,
    BlendFactor::One, BlendFactor::One, BlendEquation::Add
  )
  #endif
  ;

  /**
   * @ingroup graphics
   * @brief Multiplicative blend mode
   *
   * Multiply source and dest.
   *
   * @sa gf::BlendMode
   */
  constexpr BlendMode BlendMultiply
  #ifndef DOXYGEN_SHOULD_SKIP_THIS
  = BlendMode (BlendFactor::DstColor, BlendFactor::Zero)
  #endif
  ;

  /**
   * @ingroup graphics
   * @brief No blend mode
   *
   * Overwrite dest with source.
   *
   * @sa gf::BlendMode
   */
  constexpr BlendMode BlendNone
  #ifndef DOXYGEN_SHOULD_SKIP_THIS
  = BlendMode(BlendFactor::One, BlendFactor::Zero)
  #endif
  ;


  /**
   * @relates BlendMode
   * @brief Equality operator
   *
   * @param lhs First blend mode
   * @param rhs Second blend mode
   * @return True if the blend modes are the same
   */
  constexpr bool operator==(const BlendMode& lhs, const BlendMode& rhs) {
    return lhs.colorSrcFactor == rhs.colorSrcFactor
        && lhs.colorDstFactor == rhs.colorDstFactor
        && lhs.colorEquation  == rhs.colorEquation
        && lhs.alphaSrcFactor == rhs.alphaSrcFactor
        && lhs.alphaDstFactor == rhs.alphaDstFactor
        && lhs.alphaEquation  == rhs.alphaEquation;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_BLEND_H
