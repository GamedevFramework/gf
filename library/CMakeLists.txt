file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated")

add_executable(gfdev_source_file tools/gfdev_source_file.cc)

# process gamecontrollerdb

set(GCDB_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/gamecontrollerdb/gamecontrollerdb.txt")
set(GCDB_HEADER "${CMAKE_CURRENT_BINARY_DIR}/generated/gamecontrollerdb.txt.h")

add_custom_command(
  OUTPUT "${GCDB_HEADER}"
  COMMAND gfdev_source_file "${GCDB_SOURCE}" "${GCDB_HEADER}" "gamecontrollerdb"
  COMMENT "Compiling gamecontrollerdb to header"
  DEPENDS gfdev_source_file "${GCDB_SOURCE}"
)

add_custom_target(gf0_gamecontrollerdb ALL DEPENDS ${GCDB_HEADER})

# process shader files

set(SHADER_SOURCES
  data/shaders/color_matrix.frag
  data/shaders/default_alpha.frag
  data/shaders/default.frag
  data/shaders/default.vert
  data/shaders/edge.frag
  data/shaders/fxaa.frag
#   data/shaders/simple_fxaa.frag
)

set(SHADER_HEADERS "")

foreach(SHADER_RELATIVE_SOURCE ${SHADER_SOURCES})
  set(SHADER_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_RELATIVE_SOURCE}")
  get_filename_component(SHADER_NAME ${SHADER_RELATIVE_SOURCE} NAME)
  string(REPLACE "." "_" SHADER_VARIABLE ${SHADER_NAME})
  set(SHADER_HEADER "${CMAKE_CURRENT_BINARY_DIR}/generated/${SHADER_NAME}.h")
  add_custom_command(
    OUTPUT "${SHADER_HEADER}"
    COMMAND gfdev_source_file "${SHADER_SOURCE}" "${SHADER_HEADER}" "${SHADER_VARIABLE}"
    COMMENT "Compiling ${SHADER_NAME} to header"
    DEPENDS gfdev_source_file "${SHADER_SOURCE}"
  )
  list(APPEND SHADER_HEADERS ${SHADER_HEADER})
endforeach()

add_custom_target(gf0_shader_headers ALL DEPENDS ${SHADER_HEADERS})

# build library

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

if (GF_SINGLE_COMPILTATION_UNIT)
  add_library(gf0 SHARED
    XUnityBuild.cc
  )
else()
  add_library(gf0 SHARED
    Action.cc
    Activities.cc
    Activity.cc
    AnimatedSprite.cc
    Animation.cc
    AssetManager.cc
    Blend.cc
    BufferedGeometry.cc
    Circ.cc
    Clipboard.cc
    Clock.cc
    Collision.cc
    Color.cc
    ColorRamp.cc
    Console.cc
    ConsoleFont.cc
    Control.cc
    Controls.cc
    Coordinates.cc
    Cursor.cc
    Curve.cc
    Curves.cc
    Direction.cc
    Drawable.cc
    Easings.cc
    Effects.cc
    Entity.cc
    EntityContainer.cc
    Flags.cc
    Font.cc
    Gamepad.cc
    Geometry.cc
    GraphicsInfo.cc
    Grid.cc
    Heightmap.cc
    Image.cc
    InputStream.cc
    InputStreams.cc
    Keyboard.cc
    Library.cc
    Log.cc
    Logo.cc
    Map.cc
    Math.cc
    Matrix.cc
    MessageManager.cc
    Model.cc
    ModelContainer.cc
    Models.cc
    Monitor.cc
    NinePatch.cc
    Noise.cc
    Noises.cc
    Orientation.cc
    Particles.cc
    Paths.cc
    PhysicsBody.cc
    PhysicsGeometry.cc
    PhysicsModel.cc
    Polygon.cc
    Polyline.cc
    PostProcessing.cc
    Random.cc
    Range.cc
    Rect.cc
    RenderPipeline.cc
    RenderTarget.cc
    RenderTexture.cc
    RenderWindow.cc
    ResourceManager.cc
    Shader.cc
    Shape.cc
    Shapes.cc
    Sleep.cc
    SpaceTree.cc
    SpriteBatch.cc
    Sprite.cc
    StringUtils.cc
    SystemInfo.cc
    Text.cc
    TextureAtlas.cc
    Texture.cc
    TileLayer.cc
    Time.cc
    Tmx.cc
    Transformable.cc
    Transform.cc
    UI.cc
    Vector.cc
    VertexArray.cc
    VertexBuffer.cc
    View.cc
    ViewContainer.cc
    Views.cc
    Window.cc
    # generated
    data/generated.cc
    # priv
    priv/Debug.cc
    # vendor
    vendor/tinyxml2/tinyxml2.cpp
    vendor/glad/src/glad.cc
  )
endif()

add_dependencies(gf0
  gf0_shader_headers
  gf0_gamecontrollerdb
)

if (GF_DEBUG)
  target_compile_definitions(gf0
    PRIVATE GF_DEBUG
  )
endif()

target_compile_definitions(gf0
  PRIVATE
    GF_EXPORT_SYMBOLS
    ZLIB_CONST
)

target_include_directories(gf0
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    ${Boost_INCLUDE_DIRS}
  PRIVATE
    ${SDL2_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
)

target_link_libraries(gf0
  ${SDL2_LIBRARY}
  ${Boost_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${ZLIB_LIBRARIES}
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
  EXPORT gfConfig
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
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
  EXPORT gfConfig
  NAMESPACE gf::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gf"
)
