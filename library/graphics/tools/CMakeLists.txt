cmake_minimum_required(VERSION 3.10)

project(GFDEV_SOURCE_FILE
  LANGUAGES CXX C
)

option(GFDEV_ANDROID_TOOL "Build gfdev tools for Android cross-compilation" OFF)

add_executable(gfdev_source_file gfdev_source_file.cc)

set_target_properties(gfdev_source_file
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

if (GFDEV_ANDROID_TOOL)
  install(
    TARGETS gfdev_source_file
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
  )
endif ()
