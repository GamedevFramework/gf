
set(GF_2048_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/gf_2048")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(gf_2048
  code/gf_2048.cc
  code/local/Board.cc
  code/local/Grid.cc
)

target_include_directories(gf_2048
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(gf_2048 gf0)

install(
  TARGETS gf_2048
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/gf_2048"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)
