cmake_minimum_required(VERSION 3.0)

project(GF
  VERSION 0.1.0
  LANGUAGES CXX C
)

option(GF_DEBUG "Activate debug build" ON)
option(GF_BUILD_EXAMPLES "Build examples" ON)
option(GF_BUILD_TESTS "Build tests" ON)
option(GF_BUILD_DOCUMENTATION "Build documentation (needs Doxygen)" ON)
option(GF_BUILD_DOCUMENTATION_WITH_CLANG "Build documentation with clang" OFF)

include(GNUInstallDirs)
set(GF_DATADIR ${CMAKE_INSTALL_FULL_DATADIR})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(SDL2_BUILDING_LIBRARY TRUE)
find_package(SDL2 REQUIRED)

find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(Freetype REQUIRED)

add_definitions(-Wall -Wextra -g -O2 -std=c++11) # -pg)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

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

  if (GF_BUILD_DOCUMENTATION_WITH_CLANG)
    set(GF_DOC_CLANG "YES")
  else()
    set(GF_DOC_CLANG "NO")
  endif()

  if(DOXYGEN_FOUND)
    configure_file("${CMAKE_SOURCE_DIR}/Doxyfile.cmake" "${CMAKE_BINARY_DIR}/Doxyfile" @ONLY)
    add_custom_target(doc ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/Doxyfile")
  endif(DOXYGEN_FOUND)
endif()
