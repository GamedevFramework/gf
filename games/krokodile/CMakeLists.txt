
set(KROKODILE_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/krokodile")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(krokodile
  code/krokodile.cc
  code/local/FinalScreen.cc
  code/local/Hud.cc
  code/local/Kreature.cc
  code/local/KreatureContainer.cc
  code/local/Map.cc
  code/local/Singletons.cc
)

target_include_directories(krokodile
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(krokodile gf0)

install(
  TARGETS krokodile
  RUNTIME DESTINATION games
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/krokodile"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)
