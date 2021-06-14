# This file is used only for Android cross-compil

cmake_minimum_required(VERSION 3.10)

project(GFDEV_SOURCE_FILE
  LANGUAGES CXX C
)

add_executable(gfdev_source_file gfdev_source_file.cc)

set_target_properties(gfdev_source_file
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

install(
  TARGETS gfdev_source_file
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
)
