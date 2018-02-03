add_executable(gf_packview gf_packview.cc)

target_link_libraries(gf_packview gf0)

install(
  TARGETS gf_packview
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
