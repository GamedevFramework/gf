cmake_minimum_required(VERSION 3.0)

project(GF
  VERSION 0.1.0
  LANGUAGES CXX C
)

option(GF_DEBUG "Activate debug build" ON)
option(GF_BUILD_EXAMPLES "Build examples" ON)
option(GF_BUILD_TESTS "Build tests" ON)
option(GF_BUILD_DOCUMENTATION "Build documentation (needs Doxygen)" ON)

include(GNUInstallDirs)
set(GF_DATADIR ${CMAKE_INSTALL_FULL_DATADIR})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(SDL2_BUILDING_LIBRARY TRUE)
find_package(SDL2 REQUIRED)

find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(Freetype REQUIRED)

if(MSVC)
  message(STATUS "Using MSVC compiler")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNOMINMAX /W4")
else()
  message(STATUS "Using GCC or Clang compiler")
  # assume it's a GNU-like compiler (GCC, clang)
  set(CMAKE_CXX_FLAGS                 "-Wall -Wextra -std=c++11")
  set(CMAKE_CXX_FLAGS_DEBUG           "-O0 -g")
  set(CMAKE_CXX_FLAGS_MINSIZEREL      "-Os -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE         "-O3 -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g")
endif()

add_subdirectory(library)

if(GF_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(GF_BUILD_TESTS)
  add_subdirectory(tests)
endif()

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/include/gf"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/data/gf"
  DESTINATION ${CMAKE_INSTALL_DATADIR}
)

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/examples"
  DESTINATION "${CMAKE_INSTALL_DOCDIR}"
  FILES_MATCHING
  PATTERN "*.cc"
  PATTERN "*.png"
  PATTERN "*.ttf"
  PATTERN "*.xml"
  PATTERN "CREDITS"
)

if (GF_BUILD_DOCUMENTATION)
  find_package(Doxygen)

  if(DOXYGEN_FOUND)
    configure_file("${CMAKE_SOURCE_DIR}/Doxyfile.cmake" "${CMAKE_BINARY_DIR}/Doxyfile" @ONLY)
    add_custom_target(doc ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/Doxyfile")
  endif(DOXYGEN_FOUND)
endif()
