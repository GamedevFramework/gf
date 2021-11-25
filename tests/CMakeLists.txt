find_package(Threads REQUIRED)

include(CTest)
include(GoogleTest)

# gf::core tests

add_executable(gf_core_tests
  main.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/src/gtest-all.cc
  testCirc.cc
  testDice.cc
  testFlags.cc
  testId.cc
  testMatrix.cc
  testMatrix2.cc
  testRange.cc
  testRect.cc
  testSerialization.cc
  testSingleton.cc
  testSpatial.cc
  testSpan.cc
  testVector.cc
  testVector1.cc
  testVector2.cc
  testVector3.cc
  testVector4.cc
)

target_include_directories(gf_core_tests
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest
)

target_link_libraries(gf_core_tests
  PRIVATE
    gfcore0
    Threads::Threads
)

gtest_discover_tests(gf_core_tests)

# gf::net tests

add_executable(gf_net_tests
  main.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/src/gtest-all.cc
  testSocket.cc
)

target_include_directories(gf_net_tests
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest
)

target_link_libraries(gf_net_tests
  PRIVATE
    gfnet0
    Threads::Threads
)

gtest_discover_tests(gf_net_tests)
