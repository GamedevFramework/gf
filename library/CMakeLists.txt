
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_library(gf0 SHARED
  # core
  Circ.cc
  Clock.cc
  Direction.cc
  Image.cc
  InputStream.cc
  InputStreams.cc
  Library.cc
  Log.cc
  Math.cc
  Matrix.cc
  Noise.cc
  Orientation.cc
  Random.cc
  Range.cc
  Rect.cc
  SystemInfo.cc
  Time.cc
  Transform.cc
  Vector.cc
  # window
  Action.cc
  Control.cc
  Keyboard.cc
  Gamepad.cc
  Monitor.cc
  Window.cc
  WindowGeometryTracker.cc
  # graphics
  AnimatedSprite.cc
  Animation.cc
  Blend.cc
  BufferedGeometry.cc
  Color.cc
  ColorRamp.cc
  Curve.cc
  Curves.cc
  Drawable.cc
  Effects.cc
  Font.cc
  NinePatch.cc
  PostProcessing.cc
  RenderPipeline.cc
  RenderTarget.cc
  RenderTexture.cc
  RenderWindow.cc
  Shader.cc
  Shape.cc
  Shapes.cc
  Sprite.cc
  SpriteBatch.cc
  Text.cc
  Texture.cc
  TextureAtlas.cc
  TileLayer.cc
  Transformable.cc
  VertexArray.cc
  VertexBuffer.cc
  View.cc
  Views.cc
  ViewContainer.cc
  # game
  AssetManager.cc
  Collision.cc
  Controls.cc
  Entity.cc
  EntityContainer.cc
  MessageManager.cc
  Model.cc
  ModelContainer.cc
  ResourceManager.cc
  # priv
  priv/Debug.cc
  # vendor
  vendor/tinyxml2/tinyxml2.cpp
  vendor/glad/src/glad.cc
)

if (GF_DEBUG)
  target_compile_definitions(gf0
    PRIVATE GF_DEBUG
  )
endif()

target_compile_definitions(gf0
  PRIVATE GF_EXPORT_SYMBOLS
)

target_include_directories(gf0
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    ${Boost_INCLUDE_DIRS}
  PRIVATE
    ${SDL2_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
)

target_link_libraries(gf0
  ${SDL2_LIBRARY}
  ${Boost_LIBRARIES}
  ${FREETYPE_LIBRARIES}
)

# workaround for Travis-CI
# see: https://bugs.launchpad.net/ubuntu/+source/gcc-5/+bug/1568899
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(gf0
    gcc_s
    gcc
  )
endif()

set_target_properties(gf0
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(
  TARGETS gf0
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  EXPORT gfConfig
)

if(WIN32)
  install(
    TARGETS gf0 
    RUNTIME DESTINATION "${CMAKE_INSTALL_DOCDIR}/examples"
  )
  install(
    TARGETS gf0 
    RUNTIME DESTINATION games
  )
endif()

install(
  FILES "${CMAKE_CURRENT_SOURCE_DIR}/vendor/gamecontrollerdb/gamecontrollerdb.txt"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/gf"
)

install(
  EXPORT gfConfig
  NAMESPACE gf::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gf"
)
