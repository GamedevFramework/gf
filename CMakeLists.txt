cmake_minimum_required(VERSION 3.10)

project(GF
  VERSION 1.2.0
  LANGUAGES CXX C
)

include(GenerateExportHeader)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT MSVC AND (NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL ""))
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

option(BUILD_SHARED_LIBS "Build the library as a shared library" ON)
option(BUILD_TESTING "Build tests" ON)

option(GF_DEBUG "Activate debug build" ON)
option(GF_BUILD_WITH_OPENGL3 "Build with OpenGL 3.3 instead of OpenGL ES 2.0" OFF)
option(GF_BUILD_EXAMPLES "Build examples" ON)
option(GF_BUILD_DOCUMENTATION "Build documentation (needs Doxygen)" ON)
option(GF_BUILD_GAMES "Build games" OFF)
option(GF_USE_EMBEDDED_LIBS "Build the library with the submodule libraries" ON)

set(GF_PACKAGE_SUFFIX "~${CMAKE_SYSTEM_NAME}" CACHE STRING "CPack package suffix")

if(UNIX)
  option(GF_DEV_ENABLE_ASAN "Enable Address Sanitizer" OFF)
endif()

set(GF_SHARED ${BUILD_SHARED_LIBS})

include(GNUInstallDirs)
set(GF_DATADIR ${CMAKE_INSTALL_FULL_DATADIR})

set(SDL2_BUILDING_LIBRARY TRUE)
find_package(SDL2 REQUIRED)

find_package(Boost REQUIRED)
find_package(Freetype REQUIRED)
find_package(pugixml REQUIRED)
find_package(ZLIB REQUIRED)

find_package(Threads REQUIRED)

if(GF_USE_EMBEDDED_LIBS)
  message(STATUS "Build with embedded libraries")
  set(Stb_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/library/vendor/stb")
else()
  # works on vcpkg
  find_package(Stb REQUIRED)
endif()

if(APPLE)
  # on macos, always use OpenGL 3.3
  set(GF_BUILD_WITH_OPENGL3 ON)
  find_package(OpenGL REQUIRED)
endif()

if(MSVC)
  message(STATUS "Using MSVC compiler")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNOMINMAX /W4 /utf-8")
  link_directories(${Boost_LIBRARY_DIRS})
  # for more information on the last two switches:
  # - https://blogs.msdn.microsoft.com/vcblog/2016/06/07/standards-version-switches-in-the-compiler/
  # - https://blogs.msdn.microsoft.com/vcblog/2016/11/16/permissive-switch/
else()
  message(STATUS "Using GCC or Clang compiler")
  # assume it's a GNU-like compiler (GCC, clang)
  set(CMAKE_CXX_FLAGS                 "-Wall -Wextra -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG           "-O1 -g")
  set(CMAKE_CXX_FLAGS_MINSIZEREL      "-Os -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE         "-O3 -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g")
endif()

if(UNIX)
  if(GF_DEV_ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  endif()
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

if (UNIX AND NOT APPLE)
  set(CPACK_DEBIAN_PACKAGE_NAME "gf-dev")
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_DEBIAN_PACKAGE_NAME}-${CMAKE_PROJECT_VERSION}-${GF_PACKAGE_SUFFIX}")
  string(TOLOWER "${CPACK_PACKAGE_FILE_NAME}" CPACK_PACKAGE_FILE_NAME)
  set(CPACK_GENERATOR "DEB")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-dev,libboost-dev,libfreetype6-dev,zlib1g-dev,libpugixml-dev")
  set(CPACK_PACKAGE_DESCRIPTION "Gamedev Framework (gf) is a framework to build 2D games in C++17. It is based on SDL and OpenGL ES 2.0, and presents an API that is very similar to the graphics module of SFML with additional features.")
  set(CPACK_PACKAGE_CONTACT "Julien Bernard julien.bernard@univ-fcomte.fr")

  include(CPack)
endif()

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
