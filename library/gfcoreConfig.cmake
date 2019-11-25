get_filename_component(GFCORE_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

find_dependency(Threads)
find_dependency(Boost REQUIRED COMPONENTS filesystem)

if(NOT TARGET gf::gfcore0)
    include("${GFCORE_CMAKE_DIR}/gfcoreTargets.cmake")
endif()
