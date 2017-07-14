
set(HUACA_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/huaca")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

if (GF_SINGLE_COMPILTATION_UNIT)
  add_executable(huaca
    code/huaca.cc
    code/XUnityBuild.cc
  )
else()
  add_executable(huaca
    code/huaca.cc
    code/local/Display.cc
    code/local/Hero.cc
    code/local/Level.cc
    code/local/Singletons.cc
    code/local/Timer.cc
  )
endif()

target_include_directories(huaca
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(huaca gf0)

install(
  TARGETS huaca
  RUNTIME DESTINATION games
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/huaca"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)
