
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(gf_dungeons gf_dungeons.cc)

target_include_directories(gf_dungeons
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(gf_dungeons gf0)

install(
  TARGETS gf_dungeons
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
