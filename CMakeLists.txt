cmake_minimum_required(VERSION 3.0)

project(GF
  VERSION 0.1.0
  LANGUAGES CXX C
)

if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

option(GF_DEBUG "Activate debug build" ON)
option(GF_BUILD_EXAMPLES "Build examples" ON)
option(GF_BUILD_TESTS "Build tests" ON)
option(GF_BUILD_DOCUMENTATION "Build documentation (needs Doxygen)" ON)
option(GF_BUILD_GAMES "Build games" OFF)
option(GF_BUILD_TOOLS "Build tools" OFF)

include(GNUInstallDirs)
set(GF_DATADIR ${CMAKE_INSTALL_FULL_DATADIR})

if(MSVC)
  message(STATUS "Environment variables:")
  message(STATUS "- BOOST_INCLUDEDIR: $ENV{BOOST_INCLUDEDIR}")
  message(STATUS "- BOOST_LIBRARYDIR: $ENV{BOOST_LIBRARYDIR}")
  message(STATUS "- SDL2DIR: $ENV{SDL2DIR}")
  message(STATUS "- SDL2ARCH: $ENV{SDL2ARCH}")
  message(STATUS "- FREETYPE_DIR: $ENV{FREETYPE_DIR}")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(SDL2_BUILDING_LIBRARY TRUE)
find_package(SDL2 REQUIRED)

find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(Freetype REQUIRED)

if(MSVC)
  message(STATUS "Using MSVC compiler")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNOMINMAX /W4")
  link_directories(${Boost_LIBRARY_DIRS})
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
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/gf"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/gf"
  DESTINATION ${CMAKE_INSTALL_DATADIR}
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/examples"
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
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cmake" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" @ONLY)
    add_custom_target(doc ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
  endif(DOXYGEN_FOUND)

  add_subdirectory(docs/snippets)
endif()

if(GF_BUILD_GAMES)
  add_subdirectory(games/gf_pong)
  add_subdirectory(games/gf_2048)
  add_subdirectory(games/huaca)
endif()

if(GF_BUILD_TOOLS)
  add_subdirectory(tools/gf_noise)

  install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/gf_tools"
    DESTINATION ${CMAKE_INSTALL_DATADIR}
  )
endif()
