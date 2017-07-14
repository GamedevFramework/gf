add_executable(gf_info gf_info.cc)

target_link_libraries(gf_info gf0)

install(
  TARGETS gf_info
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
