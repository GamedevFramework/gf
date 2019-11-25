get_filename_component(GFNET_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

find_dependency(gfcore)

if(NOT TARGET gf::gfnet0)
    include("${GFNET_CMAKE_DIR}/gfnetTargets.cmake")
endif()
