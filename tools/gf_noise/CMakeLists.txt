
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(gf_noise gf_noise.cc)

target_include_directories(gf_noise
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(gf_noise gf0)
