cmake_minimum_required(VERSION 3.12)

project(GF
  VERSION 1.2.0
  LANGUAGES CXX C
)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build the library as a shared library" ON)
option(BUILD_TESTING "Build tests" ON)

option(GF_BUILD_WITH_OPENGL3 "Build with OpenGL 3.3 instead of OpenGL ES 2.0" OFF)
option(GF_BUILD_EXAMPLES "Build examples" ON)
option(GF_BUILD_DOCUMENTATION "Build documentation (needs Doxygen)" ON)
option(GF_BUILD_GAMES "Build games" OFF)

if(UNIX)
  option(GF_DEV_ENABLE_ASAN "Enable Address Sanitizer" OFF)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)
set(GF_DATADIR ${CMAKE_INSTALL_FULL_DATADIR})

set(SDL2_BUILDING_LIBRARY TRUE)
find_package(SDL2 REQUIRED)

find_package(Boost REQUIRED)
find_package(Freetype REQUIRED)
find_package(pugixml REQUIRED)
find_package(Stb REQUIRED)
find_package(ZLIB REQUIRED)

find_package(Threads REQUIRED)

if(APPLE)
  # on macos, always use OpenGL 3.3
  set(GF_BUILD_WITH_OPENGL3 ON)
  find_package(OpenGL REQUIRED)
endif()

if(UNIX)
  if(GF_DEV_ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  endif()
endif()

if(NOT APPLE)
  set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()

add_subdirectory(library)

if(GF_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/gf"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/examples"
  DESTINATION "${CMAKE_INSTALL_DOCDIR}"
  PATTERN "CMakeLists.txt" EXCLUDE
)

if (GF_BUILD_DOCUMENTATION)
  find_package(Doxygen)

  if(DOXYGEN_FOUND)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cmake" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" @ONLY)
    add_custom_target(doc Doxygen::doxygen "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
  endif(DOXYGEN_FOUND)

  add_subdirectory(docs/snippets)
endif()

if(GF_BUILD_GAMES)
  add_subdirectory(games/gf_2048)
  add_subdirectory(games/gf_pong)
endif()

add_subdirectory(tools/gf_info)
