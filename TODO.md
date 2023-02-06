# Things to do (or not) in gf

This file is a list of ideas for gf. Some of them will eventually be in gf. Others are just random thoughts. If you are interested in one of these items, read [CONTRIBUTING.md](CONTRIBUTING.md).

## core

- (geometry) add more algorithms from [here](http://geomalgorithms.com/algorithms.html)
- (spatial) add linear and quadratic RTree
- (color) color interpolation (and color space)
  - ideas: [blog post](https://howaboutanorange.com/blog/2011/08/10/color_interpolation/)
  - ideas: [blog post](https://www.alanzucconi.com/2016/01/06/colour-interpolation/)
  - ideas: [chroma.js](https://github.com/gka/chroma.js)
  - ideas: [gurki/vivid](https://github.com/gurki/vivid)

## graphics

- (event) add AnyScancode, AnyKeycode and AnyGamepadButton
- (font) use Signed Distance Field for big size fonts
- (curve/shape) add anti-aliasing to `Curve` and `Shape`
  - ideas: [vaserenderer](https://github.com/tyt2y3/vaserenderer),
  - ideas: [a forum thread](https://forum.libcinder.org/topic/smooth-thick-lines-using-geometry-shader#23286000001269127)
  - ideas: [a blog post](https://blog.mapbox.com/drawing-antialiased-lines-with-opengl-8766f34192dc)
- (curve) add different join styles and cap styles
  - ideas: [Polyline2D](https://github.com/CrushedPixel/Polyline2D),
- (scene) integrate post-processing in `Scene`/`SceneManager`
  - remove `RenderPipeline`
  - rename `Effect` in `PostProcEffect`
- (drawable) Remove `Drawable`, make it a named requirement (or a concept in the future)

## net

- (socket) add `peekRawBytes()` with `MSG_PEEK`
- (async) add `SocketAsyncManager`

## meta

- (portability) consider using [hedley](https://nemequ.github.io/hedley/)
  - blocking bug: nemequ/hedley#35
- (packaging) use CPack to create packages
- (binding) Python binding with [pybind11](https://github.com/pybind/pybind11)?
