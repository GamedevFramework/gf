# gf_packview

add_executable(gf_packview gf_packview.cc)

target_link_libraries(gf_packview gf0)

install(
  TARGETS gf_packview
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# gf_packjson

add_executable(gf_packjson gf_packjson.cc)

target_include_directories(gf_packjson
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/rapidjson/include"
)

target_link_libraries(gf_packjson gf0)

install(
  TARGETS gf_packjson
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
