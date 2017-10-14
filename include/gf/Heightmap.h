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
#ifndef GF_HEIGHTMAP_H
#define GF_HEIGHTMAP_H

#include <tuple>

#include "Array2D.h"
#include "ColorRamp.h"
#include "Image.h"
#include "Noise.h"
#include "Portability.h"
#include "Rect.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A heightmap
   *
   * @sa gf::midpointDisplacement2D(), gf::diamondSquare2D()
   */
  class GF_API Heightmap {
  public:
    /**
     * @brief Constructor
     *
     * @param size The size of the heightmap
     */
    Heightmap(Vector2i size);

    /**
     * @brief Get the size of the heightmap
     *
     * @returns The current size of the heightmap
     */
    Vector2i getSize() const {
      return m_data.getSize();
    }

    /**
     * @brief Reset the heightmap
     *
     * The values are all set to @f$ 0.0 @f$
     */
    void reset();

    /**
     * @brief Get the value at the specified position
     *
     * @param position A position
     * @returns The value at the given position
     */
    double getValue(Vector2i position) const {
      return m_data(position);
    }

    /**
     * @brief Set the value at the specified position
     *
     * @param position A position
     * @param value The new value
     */
    void setValue(Vector2i position, double value) {
      m_data(position) = value;
    }

    /**
     * @brief Get the minimum and maximum of the heightmap
     *
     * @returns A pair with the first element being the minimum and the second element the maximum
     */
    std::tuple<double, double> getMinMax() const;

    /**
     * @name Shaping
     * @{
     */

    /**
     * @brief Normalize the heightmap
     *
     * @param min The new minimum value (defaults to @f$ 0.0 @f$)
     * @param max The new maximum value (defaults to @f$ 1.0 @f$)
     */
    void normalize(double min = 0.0, double max = 1.0);

    /**
     * @brief Add a hill to the heightmap
     *
     * @param center The center of the hill
     * @param radius The radius of the hill
     * @param height The height of the hill
     */
    void addHill(Vector2d center, double radius, double height);

    /**
     * @brief Dig a hill in the heightmap
     *
     * @param center The center of the hill
     * @param radius The radius of the hill
     * @param height The height of the hill
     */
    void digHill(Vector2d center, double radius, double height);

    /**
     * @brief Add a noise to the heightmap
     *
     * @param noise The noise to use
     * @param scale The scale of the noise
     */
    void addNoise(Noise2D& noise, double scale = 1.0);

    /**
     * @brief Add a constant to the heightmap
     *
     * @param value The value of the constant
     */
    void addValue(double value);

    /**
     * @brief Scale the values of the heightmap
     *
     * @param value The factor of scaling
     */
    void scale(double value);

    /**
     * @brief Clamp the values of the heightmap
     *
     * @param min The minimum value (defaults to @f$ 0.0 @f$)
     * @param max The maximum value (defaults to @f$ 1.0 @f$)
     * @sa normalize()
     */
    void clamp(double min = 0.0, double max = 1.0);

    /**
     * @}
     */

    /**
     * @name Erosion
     * @{
     */

    /**
     * @brief Compute the slope at a position
     *
     * The slope is defined as "the greatest of the height differences between
     * the cell and its four neighbours in a Von Neumann neighbourhood".
     *
     * @f[ \text{slope}(x, y) = \max(|h(x,y) - h(x - 1, y)|, |h(x,y) - h(x + 1, y)|, |h(x,y) - h(x, y - 1)|, |h(x,y) - h(x, y + 1)|) @f]
     *
     * @param position The given position
     */
    double getSlope(Vector2i position) const;

    /**
     * @brief Apply thermal erosion to the heightmap
     *
     * @param iterations The number of iterations
     * @param talus The minimum difference for a move of material (typically @f$ \frac{4}{N} @f$)
     * @param fraction The fraction of material that moves (typically @f$ 0.5 @f$)
     */
    void thermalErosion(unsigned iterations, double talus, double fraction);

    /**
     * @brief Apply hydraulic erosision to the heightmap
     *
     * @param iterations The number of iterations
     * @param rainAmount The amount of rain each cell receives at each iteration (typically @f$ 0.01 @f$)
     * @param solubility The solubility of the terrain (typically @f$ 0.01 @f$)
     * @param evaporation The proportion of evaporated water at each iteration (typically @f$ 0.5 @f$)
     * @param capacity The maximum proportion of material that can be carried by water (typically @f$ 0.01 @f$)
     */
    void hydraulicErosion(unsigned iterations, double rainAmount, double solubility, double evaporation, double capacity);

    /**
     * @brief Apply fast erosion to the heightmap
     *
     * @param iterations The number of iterations
     * @param talus The maximum difference for a move of material (typically @f$ \frac{8}{N} @f$ or more)
     * @param fraction The fraction of material that moves (typically @f$ 0.5 @f$)
     */
    void fastErosion(unsigned iterations, double talus, double fraction);

    /**
     * @brief Compute the erosion score for the heightmap
     *
     * The erosion score is the average slope divided by the standard
     * deviation of slopes. A high erosion score means that the terrain has
     * large flat areas and has some steep areas.
     */
    double getErosionScore() const;

    /**
     * @}
     */

    /**
     * @brief Rendering mode
     */
    enum class Render {
      Colored, ///< Export to a simply colored image
      Shaded,  ///< Export to a shaded image
    };

    /**
     * @name Export
     * @{
     */

    /**
     * @brief Get a sub-map of the heightmap
     *
     * @param area The area of the sub-map in the heightmap
     */
    Heightmap subMap(RectI area) const;

    /**
     * @brief Export to a grayscale image
     *
     * The heightmap is assumed to be normalized.
     *
     * @returns A grayscale image representing the heightmap
     *
     * @sa copyToColoredImage()
     */
    Image copyToGrayscaleImage() const;

    /**
     * @brief Export to a colored image
     *
     * The heightmap is assumed to be normalized.
     *
     * The color ramp assumes that the water level is at @f$ 0.5 @f$. The
     * actual water level can be specified and the ramp is automatically
     * adapted on the fly.
     *
     * @param ramp A color ramp
     * @param waterLevel The actual water level (defaults to @f$ 0.5 @f$)
     * @param render The rendering mode
     * @returns A colored image representing the heightmap
     *
     * @sa copyToGrayscaleImage()
     */
    Image copyToColoredImage(const ColorRamp& ramp, double waterLevel = 0.5, Render render = Render::Colored) const;

    /**
     * @}
     */

  private:
    Array2D<double, int> m_data;
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_HEIGHTMAP_H
