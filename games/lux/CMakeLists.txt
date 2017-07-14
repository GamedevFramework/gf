
set(LUX_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/lux")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

if (GF_SINGLE_COMPILTATION_UNIT)
  add_executable(lux
    code/lux.cc
    code/XUnityBuild.cc
  )
else()
  add_executable(lux
    code/lux.cc
    code/local/Bonus.cc
    code/local/Bullets.cc
    code/local/Enemy.cc
    code/local/Hero.cc
    code/local/Particles.cc
    code/local/Scenario.cc
    code/local/Ship.cc
    code/local/Shoot.cc
    code/local/Stars.cc
  )
endif()

target_include_directories(lux
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(lux gf0)

install(
  TARGETS lux
  RUNTIME DESTINATION games
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/lux"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)
