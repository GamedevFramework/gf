# Very simple FindSDL2.cmake that should work in the following situations:
# - Linux
# - macos
#
# For Windows, you should use vcpkg and the provided targets.
#
# Distributed under the terms and conditions of the MIT license
# Copyright (c) 2020 Julien Bernard
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

set(SDL2_SEARCH_PATHS
  ￼	~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  ￼	/usr
  /opt/local
  /opt
)

# SDL2 library

find_library(SDL2_LIBRARY
  NAMES
    SDL2-2.0 SDL-2.0 SDL2
  HINTS
    $ENV{SDL2DIR}
  PATH_SUFFIXES
    lib
  PATHS
    ${SDL2_SEARCH_PATHS}
)

find_path(SDL2_INCLUDE_DIR SDL.h
  HINTS
    $ENV{SDL2DIR}
  PATH_SUFFIXES
    include
    include/SDL2
    SDL2
  PATHS
    ${SDL2_SEARCH_PATHS}
)

if(NOT SDL2_INCLUDE_DIR)
  message(FATAL_ERROR "Could not find SDL2 include directory.")
else()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args("SDL2" DEFAULT_MSG
  SDL2_LIBRARY
  SDL2_INCLUDE_DIR
)

message(STATUS "  SDL2 include directory: ${SDL2_INCLUDE_DIR}")

mark_as_advanced(
  SDL2_LIBRARY
  SDL2_INCLUDE_DIR
)

if(NOT TARGET SDL2::SDL2)
  add_library(SDL2::SDL2 UNKNOWN IMPORTED)
  set_target_properties(SDL2::SDL2
    PROPERTIES
      IMPORTED_LOCATION "${SDL2_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}"
  )

  if(NOT APPLE)
    find_package(Threads REQUIRED QUIET)
    set_target_properties(SDL2::SDL2
      PROPERTIES
        INTERFACE_LINK_LIBRARIES Threads::Threads
    )
  endif()
endif()
