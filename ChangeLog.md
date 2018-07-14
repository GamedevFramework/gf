# ChangeLog for gf

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
