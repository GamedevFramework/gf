# ChangeLog for gf

## gf 1.2.0 (14 Jul 2023)

- Core (gf::core)
- Graphics (gf::graphics)
  - Add Animation::isFinished()
  - Fix a bug in TileLayer
- Network (gf::net)
- Misc

## gf 1.1.0 (14 Jan 2023)

- Core (gf::core)
- Graphics (gf::graphics)
  - Add Grid::setSelectedColor() implementation!
  - Add Scene::getWorldEntities() and Scene::getHudEntities()
- Network (gf::net)
  - Add MSG_NOSIGNAL to send flags if available
- Misc
  - Update CPack configuration

## gf 1.0.0 (14 Jul 2022)

- Core (gf::core)
  - Add gf::Flags serialization
  - Add gf::angleTo and gf::projectOnto
  - Remove gf::unused in favor of `[[maybe_unused]]`
- Graphics (gf::graphics)
  - Add VertexArray::appendAll()
  - Add ShapeParticles::addPolygon()
  - Add a flag of EventFilter in Window::pollEvent() and Window::waitEvent()
  - Add gf::ConsoleStyle and update related classes
  - Add TileLayer::setTilesetSmooth()
  - Remove gf::LightSystem and related classes (not ready)
- Network (gf::net)
- Misc
  - 6th anniversary!

## gf 0.22.0 (14 Jan 2022)

- Core (gf::core)
  - Replace Mersenne Twister with xoshiro256++
  - Update OpenSimplex implementation
  - Fix a bug in TMX CSV parsing
- Graphics (gf::graphics)
- Network (gf::net)
- Misc
  - Remove Buddy.works as CI
  - Add Github Actions for building on Linux and Windows

## gf 0.21.0 (14 Jul 2021)

- Core (gf::core)
  - Remove gf::HexagonHelper, gf::StaggerHelper and gf::OrthogonalHelper
  - Add gf::Cells and its subclasses gf::HexagonalCells, gf::StaggeredCells, gf::OrthogonalCells
  - Remove gf::SquareGrid, gf::HexagonalGrid
  - Add gf::Grid (with gf::Cells)
  - Rework gf::Tileset (with gf::Cells)
  - Rename and refactor cell related classes and enums
- Graphics (gf::graphics)
  - Add some missing texture format constants
  - Rework Animation::addTileset and add tileOffset (thanks @mquinson)
- Network (gf::net)
- Misc
  - Add .editorconfig
  - Changed CI from Travis CI to Buddy.works
  - Preliminary work for Android compilation
  - Delete IRC channel on freenode (prefer Discord)

## gf 0.20.0 (14 Apr 2021)

- Core (gf::core)
  - Add gf::Polygon::getPrevPoint() and gf::Polygon::getNextPoint()
  - Add gf::Rect::fromSize
  - Add gf::Polyline::getWinding() and gf::Polyline::contains()
  - Add gf::PointSequence::reverse()
- Graphics (gf::graphics)
  - Add gf::Coordinates::getWindowSize()
  - Images are now uploaded correctly in textures
  - Make gf::RenderTarget::setActive() virtual
  - Fix SquareGrid rendering (thanks @NiiRoZz)
  - setTexture now takes a gf::RectF instead of a boolean (thanks @NiiRoZz)
  - Add area in texture loading functions (thanks @NiiRoZz)
  - Add gf::LightSystem and more light classes (early stage)
- Network (gf::net)
- Misc

## gf 0.19.0 (14 Jan 2021)

- Core (gf::core)
  - Remove gf::StringRef in favor of std::string_view
  - gf::Path is now std::filesystem::path
  - Improve activities
- Graphics (gf::graphics)
  - Change the default texture size for fonts
  - Add support for multiple tilesets in gf::TileLayer
  - Add gf::GamepadValues and gf::MouseValues
- Network (gf::net)
  - Set address reuse in gf::TcpListener
- Misc
  - Use C++17!

## gf 0.18.0 (14 Oct 2020)

- Core (gf::core)
  - Add gf::PointSequence
  - Add gf::Span
  - Remove gf::BufferRef and gf::ArrayRef (in favor of gf::Span)
  - Remove physics classes
- Graphics (gf::graphics)
  - Remove gf::UI (and nuklear dependency)
  - Handle multiple windows (events and rendering)
  - Add gf::GameManager
- Tools and games
  - Remove games from repository
- Misc
  - Use GenerateExportHeader in cmake
  - Now integrated in vcpkg!
  - Add submodules in the documentation
  - Put debug include files in their own directory

## gf 0.17.0 (14 Jul 2020)

- Core (gf::core)
  - Add default constructor for gf::Heightmap
  - Fix a bug in gf::Random::computePosition(const CircF&)
- Graphics (gf::graphics)
  - Add a constructor for gf::Cursor with a path
  - Add gf::Scene::onActivityChange()
  - Add gf::ResourceManager::getImage()
  - Add gf::TouchEvent
  - Add gf::SplineCurve
  - Add gf::Tileset
  - Add gf::Animation::setLoop()
  - Add gf::BlackoutSegueEffect
  - Add gf::SharedGraphics
- Network (gf::net)
  - Fix a bug in gf::SocketSelector
- Tools and games
  - Remove tools from repository and convert to Imgui (see gf-tools)
  - Games may be removed soon too
- Misc
  - SDL_GameControllerDB is now copied manually, not as a submodule
  - GF_USE_EMBEDDED_LIBS cmake options to use submodules or not
  - CMake targets were changed in components

## gf 0.16.0 (14 Apr 2020)

- Core (gfcore)
  - Add gf::HexagonHelper
  - Add Random::computeId()
  - Add gf::Polygon::contains()
  - Add gf::Rect<T>::empty()
  - Add gf::Handle
  - Add gf::DynamicTree (spatial index)
  - Rework all spatial indices
  - Remove gf::Ball and gf::Box
  - Add gf::TileOrientation
  - Improve gf::Ref
- Graphics (gf)
  - Add gf::HexagonalGrid
  - Add missing get/set functions to gf::TextWidget
  - Add gf::Scene::{get,set}ClearColor()
  - Add multi-texture support
  - Add WidgetContainer::clear()
  - Add gf::Segue and gf::SequeEffect
  - Add gf::RenderTarget::customDraw() and gf::RenderAttributeInfo
  - Add gf::Animation::addTileset()
  - Add segue effects:
    - gf::CheckerboardSegueEffect
    - gf::CircleSegueEffect
    - gf::FadeSegueEffect
    - gf::GlitchSegueEffect
    - gf::PixelateSegueEffect
    - gf::RadialSegueEffect
    - gf::SlideSegueEffect
    - gf::ZoomBlurSegueEffect
  - Add gf::buildLines()
  - Add add gf::makeTileLayer()
  - Add a default constructor to gf::TileLayer
  - Unselect the widget when an action is triggered
  - Add gf::SceneManager::computeWindowToGameCoordinates() and gf::SceneManager::computeGameToWindowCoordinates()
  - Add gf::SceneManager::pushScenes()
  - Add gf::Pie
- Network (gfnet)
  - Add gf::SocketSelector
- Tools and games
  - Add gf-imgui repository (gf target for imgui)
  - Add gf-box2d repository (Box2D helpers for gf)
- Misc
  - Remove GF.h
  - Make flags explicit in the API
  - Thanks @ahugeat, @NiiRoZz, @magestik, @TheCubeOfFire for the contributions!

## gf 0.15.0 (14 Jan 2020)

- Core (gfcore)
  - Add a boolean parameter in SquareMap::setWalkable() and SquareMap::setTransparent()
  - SquareMap::computeRoute() now returns an empty vector if no route exists
  - Remove std::initializer constructor in gf::ArrayRef
  - Templatize gf::ColorRamp
  - Add gf::BufferInputStream and gf::BufferOutputStream
- Graphics (gf)
  - Add some useful methods to gf::UI
- Network (gfnet)
  - Creation of gfnet library
  - Add gf::TcpSocket, gf::UdpSocket, gf::TcpListener
- Tools and games
- Misc

## gf 0.14.0 (14 Oct 2019)

- Core (gfcore)
  - Add gf::array()
  - Add gf::Random::computeRadius(), gf::Random::computeAngle()
  - Add gf::Random::computePosition() (uniformly in a circle)
  - gf::Rect now inherits from gf::Box
  - Remove gf::Rect constructor and add static functions for creating rectangles
  - gf::Circ now inherits from gf::Ball
  - Improve math functions regarding type conversions
- Graphics (gf)
  - Add gf::GraphicsHandle and make several class use it
- Tools and games
- Misc
  - Make examples compilable in documentation
  - Remove all loadFrom* methods and replace them by constructors

## gf 0.13.0 (14 Jul 2019)

- Core (gfcore)
  - Add gf::ColorBase as a templatized version of Color
  - Add gf::cube() and gf::pi<T>()
- Graphics (gf)
  - Add gf::UICharBuffer to handle edition in gf::UI
  - Add gf::Scene and gf::SceneManager to handle scenes
  - Fix a bug in gf::TileLayer regarding staggered map
- Tools and games
- Misc
  - Remove all uses of `unsigned`, use `int` instead
  - Reduce the complexity of some functions (e.g. UI::processEvent())

## gf 0.12.0 (14 Apr 2019)

- Core (gfcore)
  - Add gf::StaggerHelper
  - Split Array2D in Array2D and Index2D
  - Add iterators in Index2D to replace visitors of Array2D
  - Change splitIn* to use StringRef instead of std::u32string
  - Add gf::orientation()
  - Add gf::escapeString()
  - Add gf::projx() and gf::projy()
  - Add gf::dirx() and gf::diry()
  - Add gf::vec()
  - Add CircD
  - Fix bugs in TMX parsing
- Graphics (gf)
  - Support staggered map in gf::TileLayer
  - Fix behaviour in GamepadAxisControl
- Tools and games
  - Add H.O.M.E.
- Misc
  - Improve GF_API
  - Separate gfConfig and gfcoreConfig

## gf 0.11.0 (14 Jan 2019)

- Core module
  - Add serialization for gf::Circ and gf::Polygon
  - Add OutputStream::getWrittenBytesCount()
  - Add Entity::setPriority()
  - Add gf::Dice
  - Add Queue::wait()
  - Add gf::Ref
- Window module
- Graphics module
  - Add line spacing and letter spacing in {Basic,}Text
- Game module
  - Add point support in TMX
- Tools and games
- Misc
  - Separate gfcore and gf libraries

## gf 0.10.0 (14 Oct 2018)

- Core module
  - Add serialization for gf::Polyline, gf::Path, gf::Rect, gf::Array2D
  - Rework InputStream, add OutputStream, remove BinaryFile
  - Add compressed stream handling (zlib)
  - Add {Polygon,Polyline}::simplify() thanks to Douglas-Peucker algorithm
- Window module
- Graphics module
  - RenderStates is now passed by reference in Drawable::draw()
- Game module
  - Improve TMX support (bug fixes)
- Tools and games
- Misc
  - Add an option to enable ASan
  - Add a code of conduct

## gf 0.9.0 (14 Jul 2018)

- Core module
  - Add conversion to bool for gf::BinaryFile
  - Add Rect::getPositionFromAnchor()
- Window module
- Graphics module
  - Add BasicSprite and BasicText
  - Add VertexArray::{begin(),end()}
  - Rework UI widgets
- Game module
  - Add getStructures() in spatial index classes
  - Rework serialization (do not use MessagePack, remove DataObject)
  - Add serialization for enums, gf::Time
  - Add version support in serialization
- Tools and games
- Misc
  - Add support for macos (thanks @sherjilozair)
  - Add support for OpenGL 3.3

## gf 0.8.0 (14 Apr 2018)

- Core module
  - Add gf::Paths::getTemporaryDirectory() and gf::Paths::getUniquePath()
  - Add gf::BinaryFile
  - Add gf::LinearMove and gf::AngularMove
  - Add gf::triangulation() for computing Delaunay triangulation
  - Add gf::Random::computePosition()
  - Transform gf::SpaceTree into gf::RandomBinaryTree
  - Add gf::Box
  - Add gf::QuadTree and gf::RStarTree for spatial indexing
- Window module
  - Add gf::View::getBounds()
  - Add gf::GamepadAny for controlling any connected gamepad
  - Add gf::KonamiGamepadControl
  - Add gf::GamepadTracker
  - Add gf::Window::isMaximized()
- Graphics module
  - Add gf::TileLayer::getLocalBounds() and gf::TileLayer::setAnchor()
  - Add gf::Widget and its hierarchy for simple UI in games (thanks @Xeryko)
  - Add gf::UI::setCharacterSize()
- Game module
  - Add {Sequence,Parallel}Activity::clear()
  - Add gf::Serializer and gf::Deserializer based on MessagePack
  - Add gf::DataObject to serialize/deserialize any object
  - Add gf::RepeatedSequenceActivity
- Tools and games
  - Add Krokodile
- Misc

## gf 0.7.0 (14 Jan 2018)

- Core module
  - Add gf::convexHull()
  - Rework Vector, Matrix and Rect (to avoid anonymous struct)
- Window module
  - Add gf::Keyboard::localize() and gf::Keyboard::unlocalize()
  - Add gf::KonamiKeyboardControl
- Graphics module
  - Add flip support in gf::TileLayer
- Game module
  - Add non-const accessors in gf::SpaceTree
- Tools and games
- Misc
  - Minimum C++ version is now C++14
  - Lots of code cleaning and code improvements

## gf 0.6.0 (14 Oct 2017)

- Core module
  - Put index type as a template in range classes
  - Add gf::PositionRange
  - Add gf::formatString()
  - Improve gf::ArrayRef, gf::StringRef and gf::BufferRef
  - Add gf::Heightmap
  - Add gf::midpointDisplacement2D() and gf::diamondSquare2D()
  - Add a TMX parser
- Window module
  - Add gf::Clipboard
  - Add gf::Cursor
- Graphics module
  - Add gf::LockedView
- Game module
  - Add gf::SquareMap
  - Add gf::Console
  - Add gf::ConsoleFont (and gf::BitmapConsoleFont and gf::ColoredConsoleFont)
  - Add gf::SpaceTree
- Tools and games
- Misc
  - Improve Appveyor build


## gf 0.5.0 (14 Jul 2017)

- Core module
  - Add gf::unused()
  - Add Array2D::swap()
  - Add gf::niceNum()
  - Add gf::absdiff()
  - Improve Array2D visitors
  - Improve Time and make everything constexpr
  - Improve log date printing
  - Add index type in Array2D template parameters
  - Fix a bug in WaveletNoise3D
  - Add Noise3DTo2DAdapter
- Window module
  - Change views to handle viewport changes
  - Add ZoomingViewAdaptor
  - Add Action::addControl()
- Graphics module
  - Add GraphicsInfo
  - Add SquareGrid
  - Add UI::spacing
  - Add Logo
  - Change getTexture() from various classes to return a reference
- Game module
  - Add TextureAtlas::loadFromFile()
  - Add midpoint displacement algorithm in 1D
  - Add Bresenham algorithm (with a function and a class)
  - Add Tween and easing functions in Ease
  - Add Activity and activities
  - Add a RenderStates parameters in Entity::render()
  - Change `float dt` to `gf::Time time` in Entity::update()
- Tools and games
  - Add gf_info
  - Add Bygone Islands
  - Add gf_dungeons
- Misc
  - Limit framerate in all games
  - Can now quit all the examples with escape

## gf 0.4.0 (14 Apr 2017)

- Core module
  - Add degrees/radians conversions
  - Add gf::sign()
  - Add gf::Sqrt3 constant
  - Add Rotation, Translation and Transform
  - Remove categories from Log
  - Add BufferRef
  - Add Paths with useful functions
  - Fix a bug in Polygon::isConvex()
  - Add gf::sleep()
  - Add Polyline
- Window module
  - Add Window::setVisible and Window::setDecorated
  - Add TextEntered event support
  - Add Window::setFramerateLimit()
  - Add ActionContainer::{hasAction(),getAction()}
  - Add View::zoom with a fixed point
- Graphics module
  - Remove WindowGeometryTracker and add Coordinates
  - Add copy/paste support in UI
  - Add UI::edit()
  - Add UI::fileSelector()
  - Add UI::image()
  - Add SpriteBatch::draw with a texture
  - Add TileLayer::commitGeometry()
  - Add particles (PointParticles, ShapeParticles, SpriteParticles)
- Game module
  - Add polygon/circle collision
  - Add minimal physics engine (PhysicsBody, PhysicsGeometry, PhysicsModel)
- Tools and games
  - Add Bank Robbery For Dummies
- Misc
  - Add support for unity build

## gf 0.3.0 (14 Jan 2017)

- Core module
  - Split Vector.h in Vector.h (for types) and VectorOps.h (for operations)
  - Add Musgrave's fractal for noises
  - Add Better gradient noise
  - Add Polygon
  - Put _id in its own namespace gf::literals
  - Add ArrayRef and StringRef
  - Add value noise
  - Add vector triple product and 2D cross product
- Window module
  - WindowFlags are Flags and replace WindowHints
  - Modifiers are Flags
  - Add Window::toggleFullscreen()
  - Add Keyboard::getKeycodeFromName() and Keyboard::getScancodeFromName()
- Graphics module
  - Add a new UI class based on Nuklear
  - Fix a bug in text rendering for Center alignment
  - Add scissor handling functions in RenderTarget
  - Rename color conversion functions
- Game module
  - Add polygon collision (GJK and EPA)
- Tools and games
  - Add lux
- Misc
  - Generate headers from shaders and gamecontrollerdb.txt
  - Change update(const Event&) in processEvent(const Event&) in many classes

## gf 0.2.0 (14 Oct 2016)

- Core module
  - Add Vector::begin()/Vector::end() for direct iteration
  - Add Rect::getPosition()
  - Replace the specialization of identity() for Matrix3f by identityTransform()
  - Add Circ (similar to Rect)
  - Add Orientation with the same free functions as Direction
  - Add 3D noise classes
  - Move Alignment outside Text
  - Add enum based Flags
  - Fix lerp for vectors
- Window module
  - Add mouse cursor visibility
  - Add mouse cursor grab
  - Add WindowGeometryTracker
- Graphics module
  - Remove RenderTarget::getDefaultView()
  - Add mipmap generation support for textures
  - Fix a bug in Sprite
  - Improve TileLayer
  - Add constructors for adaptive views
  - Add immediate mode user interface classes
  - Add color conversions between float colors and 32-bit colors
- Game module
  - Use a hash value instead of a path in the map (14x speedup)
  - Add collision detection for Rect/Circ
- Tools and games
  - Add gf_pong
  - Add gf_2048
  - Add gf_noise
  - Add huaca


## gf 0.1.0 (14 Jul 2016)

- First public release
