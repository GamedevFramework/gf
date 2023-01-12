add_executable(gf_pong
  gf_pong.cc
  local/Ball.cc
  local/Ground.cc
  local/Paddle.cc
  local/Points.cc
  local/Singletons.cc
)

target_include_directories(gf_pong
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(gf_pong gf0)

install(
  TARGETS gf_pong
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
