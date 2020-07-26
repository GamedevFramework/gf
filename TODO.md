# Things to do (or not) in gf

This file is a list of ideas for gf. Some of them will eventually be in gf. Others are just random thoughts. If you are interested in on of these items, read [CONTRIBUTING.md](CONTRIBUTING.md).

## core

- (vocabulary) merge `ArrayRef` and `BufferRef` in `Span`
- (vocabulary) rename `StringRef` in `StringView`
- (geometry) add more algorithms from [here](http://geomalgorithms.com/algorithms.html)
- (spatial) add linear and quadratic RTree
- (color) color interpolation (and color space)
  - ideas: [blog post](https://howaboutanorange.com/blog/2011/08/10/color_interpolation/)
  - ideas: [blog post](https://www.alanzucconi.com/2016/01/06/colour-interpolation/)
  - ideas: [chroma.js](https://github.com/gka/chroma.js)
  - ideas: [gurki/vivid](https://github.com/gurki/vivid)
- (activity) find a simple syntax for creating activities (templates spotted, wait for C++17)

## graphics

- (window/events) add a flag in `pollEvent`/`waitEvent` (`EventFlag::TouchAsMouse`, `EventFlag::NoWindowFilter`)
- (window/events) add more data in events (see SDL events data)
- (texture) add area in loading functions
- (texture) `setTexture(const Texture& texture, const RectF& textureRect = RectF(0, 0, 1, 1))` instead of `bool resetRect`
- (curve/shape) add anti-aliasing to `Curve` and `Shape`
  - ideas: [vaserenderer](https://github.com/tyt2y3/vaserenderer),
  - ideas: [a forum thread](https://forum.libcinder.org/topic/smooth-thick-lines-using-geometry-shader#23286000001269127)
  - ideas: [a blog post](https://blog.mapbox.com/drawing-antialiased-lines-with-opengl-8766f34192dc)
- (curve) add different join styles and cap styles
  - ideas: [Polyline2D](https://github.com/CrushedPixel/Polyline2D),
- (tilelayer) make `TileLayer` a base class for the different type of tile layers
  - create `OrthogonalTileLayer`, `IsometricTileLayer`, `StaggeredTileLayer`, `HexagonalTileLayer`
  - create helpers like `StaggerHelper` or `HexagonHelper`
  - [differences between these types](https://gamedev.stackexchange.com/questions/49847/difference-between-staggered-isometric-and-normal-isometric-tilemaps)
- (tilelayer) handle multiple tileset in `TileLayer`
- (scene) integrate post-processing in `Scene`/`SceneManager`
  - remove `RenderPipeline`
  - rename `Effect` in `PostProcEffect`
- (scene) create `GameManager` = `SceneManager` with `Random`, `ResourceManager`, `MessageManager`, etc.
- (console) make style a class and remove internal style state
- (physics) remove `PhysicsModel`, `PhysicsGeometry`, `PhysicsBody`
- (drawable) Remove `Drawable`, make it a named requirement (or a concept in the future)

## net

- (socket) add `peekRawBytes()` with `MSG_PEEK`
- (socket) add `MSG_NOSIGNAL` flag when using `send()` for Linux to prevent `SIGPIPE`s
  - SO: [How to prevent SIGPIPEs (or handle them properly)](https://stackoverflow.com/questions/108183/how-to-prevent-sigpipes-or-handle-them-properly)
- (async) add `SocketAsyncManager`

## meta

- (docs) split gf_dev_doc by modules (core, graphics, net)
- (tests) split gf_tests by modules (core, net)
- (portability) consider using [hedley](https://nemequ.github.io/hedley/)
  - blocking bug: nemequ/hedley#35
- (versions) go with C++17
  - check support in Debian stable and Ubuntu LTS
  - replace `boost::filesystem` by `std::filesystem`
- (games) put games in their own repository
- (sources) create a private include directory somewhere (`include-priv`)
  - put network portability functions
  - put debug functions (for GL and SDL)
