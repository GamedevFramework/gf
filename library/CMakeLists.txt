file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Create gfdev_source_file tool

add_subdirectory(graphics/tools)
set(GFDEV_SOURCE_FILE_EXEC "gfdev_source_file")

# process gamecontrollerdb

set(GCDB_SOURCES
  gamecontrollerdb.txt
)

set(GCDB_HEADERS "")

foreach(GCDB_RELATIVE_SOURCE ${GCDB_SOURCES})
  set(GCDB_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL_GameControllerDB/${GCDB_RELATIVE_SOURCE}")
  get_filename_component(GCDB_NAME ${GCDB_RELATIVE_SOURCE} NAME)
  string(REPLACE "." "_" GCDB_VARIABLE ${GCDB_NAME})
  set(GCDB_HEADER "${CMAKE_CURRENT_BINARY_DIR}/generated/${GCDB_NAME}.h")
  add_custom_command(
    OUTPUT "${GCDB_HEADER}"
    COMMAND ${GFDEV_SOURCE_FILE_EXEC} "${GCDB_SOURCE}" "${GCDB_HEADER}" "gamecontrollerdb"
    COMMENT "Compiling ${GCDB_NAME} to header"
    DEPENDS gfdev_source_file "${GCDB_SOURCE}"
  )
  list(APPEND GCDB_HEADERS ${GCDB_HEADER})
endforeach()

add_custom_target(gf0_gamecontrollerdb ALL DEPENDS ${GCDB_HEADERS})

# process shader files

set(SHADER_SOURCES
  graphics/data/shaders/blackout.frag
  graphics/data/shaders/color_matrix.frag
  graphics/data/shaders/default_alpha.frag
  graphics/data/shaders/default.frag
  graphics/data/shaders/default.vert
  graphics/data/shaders/edge.frag
  graphics/data/shaders/fxaa.frag
#   data/shaders/simple_fxaa.frag
  graphics/data/shaders/fade.frag
  graphics/data/shaders/slide.frag
  graphics/data/shaders/glitch.frag
  graphics/data/shaders/checkerboard.frag
  graphics/data/shaders/circle.frag
  graphics/data/shaders/pixelate.frag
  graphics/data/shaders/radial.frag
  graphics/data/shaders/zoomblur.frag
  graphics/data/shaders/light_shape.frag
  graphics/data/shaders/light_unshadow.frag
)

set(SHADER_HEADERS "")

foreach(SHADER_RELATIVE_SOURCE ${SHADER_SOURCES})
  set(SHADER_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_RELATIVE_SOURCE}")
  get_filename_component(SHADER_NAME ${SHADER_RELATIVE_SOURCE} NAME)
  string(REPLACE "." "_" SHADER_VARIABLE ${SHADER_NAME})
  set(SHADER_HEADER "${CMAKE_CURRENT_BINARY_DIR}/generated/${SHADER_NAME}.h")
  add_custom_command(
    OUTPUT "${SHADER_HEADER}"
    COMMAND ${GFDEV_SOURCE_FILE_EXEC} "${SHADER_SOURCE}" "${SHADER_HEADER}" "${SHADER_VARIABLE}"
    COMMENT "Compiling ${SHADER_NAME} to header"
    DEPENDS gfdev_source_file "${SHADER_SOURCE}"
  )
  list(APPEND SHADER_HEADERS ${SHADER_HEADER})
endforeach()

add_custom_target(gf0_shader_headers ALL DEPENDS ${SHADER_HEADERS})

# build library

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

if (NOT APPLE)
  if (GF_BUILD_WITH_OPENGL3)
    set(GF_GLAD_SRC "vendor/glad/gl33/src/glad.cc")
    set(GF_GLAD_INCLUDE_DIR "vendor/glad/gl33/include")
  else()
    set(GF_GLAD_SRC "vendor/glad/gles20/src/glad.cc")
    set(GF_GLAD_INCLUDE_DIR "vendor/glad/gles20/include")
  endif()
endif()

add_library(gfcore0
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

add_library(gf0
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

add_library(gfnet0
  net/Socket.cc
  net/SocketAddress.cc
  net/SocketGuard.cc
  net/SocketPrivate.cc
  net/SocketSelector.cc
  net/TcpListener.cc
  net/TcpSocket.cc
  net/UdpSocket.cc
)

generate_export_header(gfcore0
  BASE_NAME core
  EXPORT_MACRO_NAME CORE_API
  EXPORT_FILE_NAME CoreApi.h
  STATIC_DEFINE STATIC
  PREFIX_NAME GF_
)

generate_export_header(gf0
  BASE_NAME graphics
  EXPORT_MACRO_NAME GRAPHICS_API
  EXPORT_FILE_NAME GraphicsApi.h
  STATIC_DEFINE STATIC
  PREFIX_NAME GF_
)

generate_export_header(gfnet0
  BASE_NAME net
  EXPORT_MACRO_NAME NET_API
  EXPORT_FILE_NAME NetApi.h
  STATIC_DEFINE STATIC
  PREFIX_NAME GF_
)

add_dependencies(gf0
  gf0_shader_headers
  gf0_gamecontrollerdb
)

if (GF_BUILD_WITH_OPENGL3)
  target_compile_definitions(gf0
    PRIVATE GF_OPENGL3
  )
endif()

if (GF_DEBUG)
  target_compile_definitions(gfcore0
    PRIVATE GF_DEBUG
  )

  target_compile_definitions(gf0
    PRIVATE GF_DEBUG
  )

  target_compile_definitions(gfnet0
    PRIVATE GF_DEBUG
  )
endif()

if (NOT BUILD_SHARED_LIBS)
  target_compile_definitions(gfcore0
    PUBLIC GF_STATIC
  )

  target_compile_definitions(gf0
    PUBLIC GF_STATIC
  )

  target_compile_definitions(gfnet0
    PUBLIC GF_STATIC
  )
endif()

target_compile_definitions(gfcore0
  PRIVATE
    ZLIB_CONST
)

target_compile_definitions(gf0
  PRIVATE
    ZLIB_CONST
)

if (UNIX AND NOT APPLE)
  target_compile_definitions(gfnet0
    PUBLIC
      _POSIX_C_SOURCE=200809L
  )
endif()


target_include_directories(gfcore0
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../include-priv"
)

target_include_directories(gfcore0
  SYSTEM
  PRIVATE
    "${Stb_INCLUDE_DIR}"
)

target_include_directories(gf0
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include-priv"
)

target_include_directories(gfnet0
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../include-priv"
)

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

target_link_libraries(gfcore0
  PUBLIC
    Threads::Threads
    Boost::boost
    ZLIB::ZLIB
  PRIVATE
    pugixml
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1")
    message(STATUS "Using explicit C++ std::filesystem library (GNU)")
    target_link_libraries(gfcore0
      PUBLIC
        stdc++fs
    )
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    message(STATUS "Using explicit C++ std::filesystem library (clang)")
    target_link_libraries(gfcore0
      PUBLIC
        stdc++fs
    )
  endif()
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

target_link_libraries(gfnet0
  PUBLIC
    gfcore0
)

if (APPLE)
  target_link_libraries(gf0
    PRIVATE
      ${OPENGL_gl_LIBRARY}
  )
endif()

if (WIN32)
  target_link_libraries(gfnet0
    PUBLIC
      ws2_32
  )
endif()

target_compile_features(gfcore0
  PUBLIC
    cxx_std_17
)

set_target_properties(gfcore0
  PROPERTIES
    EXPORT_NAME "core"
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_compile_features(gf0
  PUBLIC
    cxx_std_17
)

set_target_properties(gf0
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    EXPORT_NAME "graphics"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_compile_features(gfnet0
  PUBLIC
    cxx_std_17
)

set_target_properties(gfnet0
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    EXPORT_NAME "net"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

if(NOT BUILD_SHARED_LIBS)
  set_property(TARGET gfcore0 PROPERTY POSITION_INDEPENDENT_CODE ON)
  set_property(TARGET gf0 PROPERTY POSITION_INDEPENDENT_CODE ON)
  set_property(TARGET gfnet0 PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/CoreApi.h"
    "${CMAKE_CURRENT_BINARY_DIR}/GraphicsApi.h"
    "${CMAKE_CURRENT_BINARY_DIR}/NetApi.h"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/gf"
)

install(
  TARGETS gfcore0 gf0 gfnet0
  EXPORT gfTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
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
