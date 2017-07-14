
set(BRFD_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/brfd")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

if (GF_SINGLE_COMPILTATION_UNIT)
  add_executable(bank_robbery_for_dummies
    code/bank_robbery_for_dummies.cc
    code/XUnityBuild.cc
  )
else()
  add_executable(bank_robbery_for_dummies
    code/bank_robbery_for_dummies.cc
    code/local/Car.cc
    code/local/Level.cc
    code/local/Story.cc
  )
endif()

target_include_directories(bank_robbery_for_dummies
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(bank_robbery_for_dummies gf0)

install(
  TARGETS bank_robbery_for_dummies
  RUNTIME DESTINATION games
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/brfd"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)
