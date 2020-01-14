get_filename_component(GF_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

find_dependency(gfcore)

if (NOT BUILD_SHARED_LIBS)
  find_dependency(SDL2)
  find_dependency(Freetype)
  find_dependency(ZLIB)
endif()

if(NOT TARGET gf::gf0)
    include("${GF_CMAKE_DIR}/gfTargets.cmake")
endif()
