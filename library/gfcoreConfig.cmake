get_filename_component(GFCORE_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

find_dependency(Boost)
find_dependency(ZLIB)

if(NOT TARGET gf::gfcore0)
    include("${GFCORE_CMAKE_DIR}/gfcoreTargets.cmake")
endif()
