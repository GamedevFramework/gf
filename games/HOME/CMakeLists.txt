find_package(SFML 2 COMPONENTS audio)
find_package(Box2D)

if(NOT SFML_FOUND OR NOT BOX2D_FOUND)
  return()
endif()

set(HOME_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/HOME")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(HOME
  code/main.cc
  code/local/BackgroundMusic.cc
  code/local/Backpack.cc
  code/local/DayNight.cc
  code/local/Hero.cc
  code/local/Map.cc
  code/local/Physics.cc
  code/local/ResourceManager.cc
  code/local/Singletons.cc
  code/local/Spaceship.cc
  code/local/SupplyManager.cc
  code/local/Timekeeper.cc
  code/local/Timer.cc
  code/local/Victory.cc
)

target_include_directories(HOME
  PRIVATE
    ${BOX2D_INCLUDE_DIRS}
    ${SFML_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(HOME
  PUBLIC
    gf0
    ${BOX2D_LIBRARIES}
    ${SFML_LIBRARIES}
)

install(
  TARGETS HOME
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/HOME"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)
