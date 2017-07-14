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
#ifndef GF_LIBRARY_H
#define GF_LIBRARY_H

#include "Portability.h"

#include <tuple>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup core
   * @brief A class to represent the library
   *
   * This class is used to initialize the library (especially SDL). Normally,
   * you do not have to care about it if you use a gf::Window or a
   * gf::Monitor as the library is automatically initialized with these classes.
   *
   * Internally, the class uses reference couting to avoid multiple
   * initializations.
   */
  class GF_API Library {
  public:
    /**
     * @brief Default constructor
     *
     * This function initialize the library
     */
    Library();

    /**
     * @brief Deleted copy constructor
     */
    Library(const Library&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    Library& operator=(const Library&) = delete;

    /**
     * @brief Move constructor
     */
    Library(Library&&);

    /**
     * @brief Move assignment
     */
    Library& operator=(Library&&);

    /**
     * @brief Destructor
     *
     * This function quits the library
     */
    ~Library();

    /**
     * @brief Information about version
     */
    struct Version {
      int major; ///< Major version number
      int minor; ///< Minor version number
      int patch; ///< Patch version number
    };

    /**
     * @brief Get the version of the gf library
     *
     * @return The version of the gf library
     */
    static Version getVersion();

    /**
     * @brief Depedencies of gf
     */
    enum Dependency {
      Boost,                  ///< Boost
      SimpleDirectMediaLayer, ///< SDL
      FreeType,               ///< FreeType
    };

    /**
     * @brief Get the version information about the library dependencies
     *
     * @return A tuple with the compiled version and the linked version
     */
    static std::tuple<Version, Version> getDependencyVersion(Dependency dep);

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_LIBRARY_H
