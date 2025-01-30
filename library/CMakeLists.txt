configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

if(NOT APPLE)
  if (GF_BUILD_WITH_OPENGL3)
    set(GF_GLAD_SRC "vendor/glad/gl33/src/glad.cc")
    set(GF_GLAD_INCLUDE_DIR "vendor/glad/gl33/include")
  else()
    set(GF_GLAD_SRC "vendor/glad/gles20/src/glad.cc")
    set(GF_GLAD_INCLUDE_DIR "vendor/glad/gles20/include")
  endif()
endif()

function(add_gf_library GF_LIBRARY)
  set(OPTIONS )
  set(ONE_VALUE_ARGS NAME)
  set(MULTI_VALUE_ARGS SOURCES)
  cmake_parse_arguments(PARSE_ARGV 0 GF "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}")

  add_library(${GF_LIBRARY}
    ${GF_SOURCES}
  )

  if(BUILD_SHARED_LIBS)
    target_compile_definitions(${GF_LIBRARY} PRIVATE GF_BUILD)
  else()
    target_compile_definitions(${GF_LIBRARY} PUBLIC GF_STATIC)
    set_property(TARGET ${GF_LIBRARY} PROPERTY POSITION_INDEPENDENT_CODE ON)
  endif()

  target_include_directories(${GF_LIBRARY}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/../include-priv"
      "${CMAKE_CURRENT_BINARY_DIR}"
  )

  target_compile_features(${GF_LIBRARY}
    PUBLIC
      cxx_std_17
  )

  set_target_properties(${GF_LIBRARY}
    PROPERTIES
      EXPORT_NAME ${GF_NAME}
      CXX_STANDARD 17
      CXX_EXTENSIONS OFF
      VERSION ${PROJECT_VERSION}
      SOVERSION ${PROJECT_VERSION_MAJOR}
  )

endfunction()

#
# Library: Core
#

add_gf_library(gfcore0
  NAME
    core
  SOURCES
    core/Activities.cc
    core/Activity.cc
    core/Array2D.cc
    core/AssetManager.cc
    core/BasicGeometry.cc
    core/Cells.cc
    core/Cells_Hexagonal.cc
    core/Cells_Orthogonal.cc
    core/Cells_Staggered.cc
    core/Circ.cc
    core/Clock.cc
    core/Collision.cc
    core/Color.cc
    core/ColorRamp.cc
    core/Dice.cc
    core/Direction.cc
    core/Easings.cc
    core/Flags.cc
    core/Geometry.cc
    core/Heightmap.cc
    core/Image.cc
    core/Log.cc
    core/Map.cc
    core/Math.cc
    core/Matrix.cc
    core/MessageManager.cc
    core/Model.cc
    core/ModelContainer.cc
    core/Models.cc
    core/Move.cc
    core/Noise.cc
    core/Noises.cc
    core/Noises_OpenSimplex.cc
    core/Orientation.cc
    core/Path.cc
    core/PointSequence.cc
    core/Polygon.cc
    core/Polyline.cc
    core/RandomBinaryTree.cc
    core/Random.cc
    core/Range.cc
    core/Rect.cc
    core/Serialization.cc
    core/SerializationOps.cc
    core/Sleep.cc
    core/Spatial_DynamicTree.cc
    core/Spatial_QuadTree.cc
    core/Spatial_RStarTree.cc
    core/Spatial_SimpleSpatialIndex.cc
    core/Stream.cc
    core/Streams.cc
    core/StringUtils.cc
    core/Time.cc
    core/Tmx.cc
    core/Transform.cc
    core/Triangulation.cc
    core/Vector.cc
)

target_compile_definitions(gfcore0
  PRIVATE
    ZLIB_CONST
)

target_include_directories(gfcore0
  SYSTEM
  PRIVATE
    "${Stb_INCLUDE_DIR}"
)

target_link_libraries(gfcore0
  PUBLIC
    Threads::Threads
    Boost::boost
    ZLIB::ZLIB
  PRIVATE
    pugixml
)

#
# Library: Graphics
#

add_gf_library(gf0
  NAME
    graphics
  SOURCES
    graphics/Action.cc
    graphics/AnimatedSprite.cc
    graphics/Animation.cc
    graphics/BasicSprite.cc
    graphics/BasicText.cc
    graphics/Blend.cc
    graphics/BufferedGeometry.cc
    graphics/Clipboard.cc
    graphics/Console.cc
    graphics/ConsoleFont.cc
    graphics/Control.cc
    graphics/Controls.cc
    graphics/Coordinates.cc
    graphics/Cursor.cc
    graphics/Curve.cc
    graphics/Curves.cc
    graphics/Drawable.cc
    graphics/Effects.cc
    graphics/Entity.cc
    graphics/EntityContainer.cc
    graphics/Font.cc
    graphics/GameManager.cc
    graphics/Gamepad.cc
    graphics/GamepadValues.cc
    graphics/GlDebug.cc
    graphics/GraphicsHandle.cc
    graphics/GraphicsInfo.cc
    graphics/Grid.cc
    graphics/Keyboard.cc
    graphics/Library.cc
    graphics/Logo.cc
    graphics/Monitor.cc
    graphics/MouseValues.cc
    graphics/NinePatch.cc
    graphics/Particles.cc
    graphics/Paths.cc
    graphics/PostProcessing.cc
    graphics/RenderPipeline.cc
    graphics/RenderTarget.cc
    graphics/RenderTexture.cc
    graphics/RenderWindow.cc
    graphics/ResourceManager.cc
    graphics/Scene.cc
    graphics/SceneManager.cc
    graphics/SdlDebug.cc
    graphics/Segue.cc
    graphics/SegueEffect.cc
    graphics/SegueEffects.cc
    graphics/Shader.cc
    graphics/Shape.cc
    graphics/Shapes.cc
    graphics/SharedGraphics.cc
    graphics/SpriteBatch.cc
    graphics/Sprite.cc
    graphics/SystemInfo.cc
    graphics/Text.cc
    graphics/TextureAtlas.cc
    graphics/Texture.cc
    graphics/TileLayer.cc
    graphics/Tileset.cc
    graphics/TmxOps.cc
    graphics/Transformable.cc
    graphics/VertexArray.cc
    graphics/VertexBuffer.cc
    graphics/Vertex.cc
    graphics/View.cc
    graphics/ViewContainer.cc
    graphics/Views.cc
    graphics/Widget.cc
    graphics/WidgetContainer.cc
    graphics/Widgets.cc
    graphics/Window.cc
    # generated
    graphics/data/generated.cc
    # vendor
    ${GF_GLAD_SRC}
)

target_compile_definitions(gf0
  PRIVATE
    ZLIB_CONST
)

if (GF_BUILD_WITH_OPENGL3)
  target_compile_definitions(gf0
    PRIVATE GF_OPENGL3
  )
endif()

if (APPLE)
  target_include_directories(gf0
    SYSTEM
    PRIVATE
      ${OPENGL_INCLUDE_DIR}
  )
else()
  target_include_directories(gf0
    SYSTEM
    PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/${GF_GLAD_INCLUDE_DIR}"
  )
endif()

target_link_libraries(gf0
  PUBLIC
    gfcore0
    Threads::Threads
  PRIVATE
    SDL2::SDL2
    Freetype::Freetype
    pugixml
)

if (APPLE)
  target_link_libraries(gf0
    PRIVATE
      ${OPENGL_gl_LIBRARY}
  )
endif()

#
# Library: Net
#

add_gf_library(gfnet0
  NAME
    net
  SOURCES
    net/Socket.cc
    net/SocketAddress.cc
    net/SocketGuard.cc
    net/SocketPrivate.cc
    net/SocketSelector.cc
    net/TcpListener.cc
    net/TcpSocket.cc
    net/UdpSocket.cc
)

if (UNIX AND NOT APPLE)
  target_compile_definitions(gfnet0
    PUBLIC
      _POSIX_C_SOURCE=200809L
  )
endif()

target_link_libraries(gfnet0
  PUBLIC
    gfcore0
)

if (WIN32)
  target_link_libraries(gfnet0
    PUBLIC
      ws2_32
  )
endif()


#
# Install
#

install(
  TARGETS gfcore0 gf0 gfnet0
  EXPORT gfTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          COMPONENT GF_Runtime
          NAMELINK_COMPONENT GF_Development
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          COMPONENT GF_Development
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
          COMPONENT GF_Runtime
)

if(WIN32 AND BUILD_SHARED_LIBS)
  install(
    TARGETS gfcore0 gf0 gfnet0
    RUNTIME DESTINATION "${CMAKE_INSTALL_DOCDIR}/examples"
  )
endif()

install(
  EXPORT gfTargets
  NAMESPACE gf::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gf"
)

install(
  FILES gfConfig.cmake
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gf"
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/gfConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/gfConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gf"
)
