
add_executable(gf_tests
  main.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/src/gtest-all.cc
  testArrayRef.cc
  testBox.cc
  testBufferRef.cc
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
  testStringRef.cc
  testVector.cc
  testVector1.cc
  testVector2.cc
  testVector3.cc
  testVector4.cc
)

target_include_directories(gf_tests
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include
    ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest
)

target_link_libraries(gf_tests
  PRIVATE
    gf0
    Threads::Threads
)
