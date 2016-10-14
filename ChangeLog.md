# ChangeLog for gf

## gf 0.2.0 (14 Oct 2016)

- Core module
  - Add Vector::begin()/Vector::end() for direct iteration
  - Add Rect::getPosition()
  - Replace the specialization of identity() for Matrix3f by identityTransform()
  - Add Circ (similar to Rect)
  - Add collision detection for Rect/Circ
  - Add Orientation with the same free functions as Direction
  - Add 3D noise classes
  - Move Alignment outside Text
  - Add enum based Flags
  - Fix lerp for vectors
  - Add color conversions between float colors and 32-bit colors
- Window module
  - Add mouse cursor visibility
  - Add mouse cursor grab
  - Add WindowGeometryTracker
- Graphics module
  - Remove RenderTarget::getDefaultView()
  - Add mipmap generation support for textures
  - Fix a bug is Sprite
  - Improve TileLayer
  - Add constructors for adaptive views
  - Add immediate mode user interface classes
- Game module
  - Use a hash value instead of a path in the map (14x speedup)
- Tools and games
  - Add gf_pong
  - Add gf_2048
  - Add gf_noise
  - Add huaca


## gf 0.1.0 (14 Jul 2016)

- First public release
