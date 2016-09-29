add_subdirectory(gf_noise)

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/data/gf_tools"
  DESTINATION ${CMAKE_INSTALL_DATADIR}
)
