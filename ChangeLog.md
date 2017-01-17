# ChangeLog for gf

## gf 0.3.0 (14 Jan 2016)

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
