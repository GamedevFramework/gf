
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(gf_automata gf_automata.cc)

target_include_directories(gf_automata
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(gf_automata gf0)

install(
  TARGETS gf_automata
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
