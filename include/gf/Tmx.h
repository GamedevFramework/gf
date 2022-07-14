/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2022 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#ifndef GF_TMX_H
#define GF_TMX_H

#include <cstdint>

#include <array>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "CellTypes.h"
#include "CoreApi.h"
#include "Flags.h"
#include "Id.h"
#include "Path.h"
#include "Rect.h"
#include "Time.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif
  /**
   * @ingroup core_tmx
   * @brief the render order of the tiles.
   */
  enum class TmxRenderOrder {
    RightDown, ///< Right down order
    RightUp,   ///< Right up order
    LeftDown,  ///< Left down order
    LeftUp,    ///< Left up order
  };

  /**
   * @ingroup core_tmx
   * @brief The properties for TMX entities
   *
   */
  class GF_CORE_API TmxProperties {
  public:
    /**
     * @brief Add a string property
     *
     * @param name The name of the property
     * @param value The value of the property
     */
    void addStringProperty(std::string name, std::string value);

    /**
     * @brief Add an integer property
     *
     * @param name The name of the property
     * @param value The value of the property
     */
    void addIntProperty(std::string name, int value);

    /**
     * @brief Add a float property
     *
     * @param name The name of the property
     * @param value The value of the property
     */
    void addFloatProperty(std::string name, double value);

    /**
     * @brief Add a boolean property
     *
     * @param name The name of the property
     * @param value The value of the property
     */
    void addBoolProperty(std::string name, bool value);

    /**
     * @brief Add a color property
     *
     * @param name The name of the property
     * @param value The value of the property
     */
    void addColorProperty(std::string name, Color4u value);

    /**
     * @brief Add a file property
     *
     * @param name The name of the property
     * @param value The value of the property
     */
    void addFileProperty(std::string name, Path value);

    /**
     * @brief Add an object property
     *
     * @param name The name of the property
     * @param value The value of the property
     */
    void addObjectProperty(std::string name, Id value);

    /**
     * @brief Add a class property
     *
     * @param name The name of the property
     * @param value The value of the property
     */
    void addClassProperty(std::string name, TmxProperties value);

    /**
     * @brief Get a string property
     *
     * @param name The name of the property
     * @param def The default value if the property does not exist
     * @returns The value of the given property
     */
    std::string getStringProperty(const std::string& name, const std::string& def) const;

    /**
     * @brief Get an integer property
     *
     * @param name The name of the property
     * @param def The default value if the property does not exist
     * @returns The value of the given property
     */
    int getIntProperty(const std::string& name, int def) const;

    /**
     * @brief Get a float property
     *
     * @param name The name of the property
     * @param def The default value if the property does not exist
     * @returns The value of the given property
     */
    double getFloatProperty(const std::string& name, double def) const;

    /**
     * @brief Get a boolean property
     *
     * @param name The name of the property
     * @param def The default value if the property does not exist
     * @returns The value of the given property
     */
    bool getBoolProperty(const std::string& name, bool def) const;

    /**
     * @brief Get a color property
     *
     * @param name The name of the property
     * @param def The default value if the property does not exist
     * @returns The value of the given property
     */
    Color4u getColorProperty(const std::string& name, const Color4u& def) const;

    /**
     * @brief Get a file property
     *
     * @param name The name of the property
     * @param def The default value if the property does not exist
     * @returns The value of the given property
     */
    Path getFileProperty(const std::string& name, const Path& def) const;

    /**
     * @brief Get an object property
     *
     * @param name The name of the property
     * @param def The default value if the property does not exist
     * @returns The value of the given property
     */
    Id getObjectProperty(const std::string& name, Id def) const;

    /**
     * @brief Get a class property
     *
     * @param name The name of the property
     * @param def The default value if the property does not exist
     * @returns The value of the given property
     */
    TmxProperties getClassProperty(const std::string& name, const TmxProperties& def) const;

  private:
    using Value = std::variant<bool, int, double, std::string, Color4u, Path, Id, TmxProperties>;
    std::map<std::string, Value> m_props;
  };

  struct TmxLayers;

  struct TmxTileLayer;
  struct TmxObjectLayer;
  struct TmxImageLayer;
  struct TmxGroupLayer;

  /**
   * @ingroup core_tmx
   * @brief A visitor for layers in the visitor pattern
   *
   * @sa gf::TmxLayer::accept()
   * @sa [Wikipedia - Visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern)
   */
  class GF_CORE_API TmxVisitor {
  public:
    /**
     * @brief Destructor
     */
    virtual ~TmxVisitor();

    /**
     * @brief Visit a tile layer.
     *
     * @param map The containing map
     * @param layer The tile layer
     */
    virtual void visitTileLayer(const TmxLayers& map, const TmxTileLayer& layer);

    /**
     * @brief Visit an object layer.
     *
     * @param map The containing map
     * @param layer The object layer
     */
    virtual void visitObjectLayer(const TmxLayers& map, const TmxObjectLayer& layer);

    /**
     * @brief Visit an image layer.
     *
     * @param map The containing map
     * @param layer The image layer
     */
    virtual void visitImageLayer(const TmxLayers& map, const TmxImageLayer& layer);

    /**
     * @brief Visit a group layer
     *
     * @param map The containing map
     * @param layer The group layer
     */
    virtual void visitGroupLayer(const TmxLayers& map, const TmxGroupLayer& layer);
  };

  /**
   * @ingroup core_tmx
   * @brief A layer in the whole map
   *
   * The are four kinds of layers: tile layers, image layers, object layers and
   * group layers.
   *
   * @sa gf::TmxTileLayer, gf::TmxImageLayer, gf::TmxObjectLayer, gf::TmxGroupLayer
   */
  struct GF_CORE_API TmxLayer {
    /**
     * @brief Destructor
     */
    virtual ~TmxLayer();

    /**
     * @brief Accept function in the visitor pattern
     *
     * @param map The containing map
     * @param visitor The visitor
     *
     * @sa [Wikipedia - Visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern)
     */
    virtual void accept(const TmxLayers& map, TmxVisitor& visitor) const = 0;

    TmxProperties properties; ///< The properties of the layer
    std::string name;         ///< The name of the layer
    double opacity;           ///< The opacity of the layer
    bool visible;             ///< The visibility of the layer
    Vector2i offset;          ///< The offset of the layer
  };

  /**
   * @ingroup core_tmx
   * @brief A cell in a tile layer
   */
  struct GF_CORE_API TmxCell {
    uint32_t gid;             ///< The global id of the tile
    Flags<Flip> flip = None;  ///< The flip properties of the tile
  };

  /**
   * @ingroup core_tmx
   * @brief A chunk in a tile layer (for infinite maps)
   */
  struct GF_CORE_API TmxChunk {
    Vector2i position;
    Vector2i size;
    std::vector<TmxCell> cells;
  };

  /**
   * @ingroup core_tmx
   * @brief A layer with tiles in cells
   */
  struct GF_CORE_API TmxTileLayer : public TmxLayer {
    std::vector<TmxCell> cells; ///< The cells of the layer
    std::vector<TmxChunk> chunks; ///< The chunks of the layer

    void accept(const TmxLayers& map, TmxVisitor& visitor) const override;
  };

  /**
   * @ingroup core_tmx
   * @brief The draw order of the objects
   */
  enum class TmxDrawOrder {
    TopDown,  ///< Top-down order
    Index,    ///< Index order
  };


  /**
   * @ingroup core_tmx
   * @brief A geometrical object
   *
   * There are seven kinds of objects:
   *
   * - rectangles (see gf::TmxRectangle)
   * - ellipses (see gf::TmxEllipse)
   * - polylines (see gf::TmxPolyline)
   * - polygons (see gf::TmxPolygon)
   * - tiles (see gf::TmxTileObject)
   * - texts (see gf::TmxText)
   * - points (see gf::TmxPoint)
   *
   * @sa gf::TmxObjectLayer
   */
  struct GF_CORE_API TmxObject {
    /**
     * @brief Destructor
     */
    virtual ~TmxObject();

    /**
     * @brief The kind of object
     */
    enum Kind {
      Rectangle,  ///< A rectangle object
      Ellipse,    ///< An ellipse object
      Polyline,   ///< A polyline object
      Polygon,    ///< A polygon object
      Tile,       ///< A tile object
      Text,       ///< A text object
      Point,      ///< A point object
    };

    Kind kind;  ///< The kind of the object

    TmxProperties properties; ///< The properties of the object

    int id;             ///< The id of the object
    std::string name;   ///< The name of the object
    std::string type;   ///< The type of the object
    Vector2f position;  ///< The position of the object
    double rotation;    ///< The rotation of the object
    bool visible;       ///< The visibility of the object
  };

  /**
   * @ingroup core_tmx
   * @brief A rectangle object
   */
  struct GF_CORE_API TmxRectangle : public TmxObject {
    Vector2f size;  ///< The size of the rectangle
  };

  /**
   * @ingroup core_tmx
   * @brief An ellipse object
   */
  struct GF_CORE_API TmxEllipse : public TmxObject {
    Vector2f size;  ///< The size of the ellipse
  };

  /**
   * @ingroup core_tmx
   * @brief An image put in the map identified by its global id
   */
  struct GF_CORE_API TmxTileObject : public TmxObject {
    uint32_t gid;
    Flags<Flip> flip;
  };

  /**
   * @ingroup core_tmx
   * @brief A polyline object
   *
   * A polyline is an open set of lines
   */
  struct GF_CORE_API TmxPolyline : public TmxObject {
    std::vector<Vector2f> points; ///< The points of the polyline
  };

  /**
   * @ingroup core_tmx
   * @brief A polygon object
   *
   * A polygon is a closed set of lines
   */
  struct GF_CORE_API TmxPolygon : public TmxObject {
    std::vector<Vector2f> points; ///< The points of the polygon
  };

  /**
   * @ingroup core_tmx
   * @brief A text object
   */
  struct GF_CORE_API TmxText : public TmxObject {
    std::string text;       ///< The text of the object

    std::string fontFamily; ///< The font family
    int sizeInPixels;       ///< The size of the text in pixel
    bool wrap;              ///< The wrap mode
    Color4u color;          ///< The color of the text
    bool bold;              ///< Is the text in bold?
    bool italic;            ///< Is the text in italic?
    bool underline;         ///< Is the text underlined?
    bool strikeout;         ///< Is the text striked out?
    bool kerning;           ///< Is the text using kerning?

    /**
     * @brief A horizontal alignment
     */
    enum class HAlign {
      Left,   ///< A left horizontal alignment
      Center, ///< A centered horizontal alignment
      Right,  ///< A right horizontal alignment
    };

    HAlign halign;  ///< The horizontal alignment of the text

    /**
     * @brief A vertical alignment
     */
    enum class VAlign {
      Top,    ///< A top vertical alignment
      Center, ///< A centered vertical alignment
      Bottom, ///< A bottom vertical alignment
    };

    VAlign valign;  ///< The vertical alignment of the text
  };

  /**
   * @ingroup core_tmx
   * @brief A point object
   */
  struct GF_CORE_API TmxPoint : public TmxObject {
  };

  /**
   * @ingroup core_tmx
   * @brief A layer with objects
   */
  struct GF_CORE_API TmxObjectLayer : public TmxLayer {
#ifdef _MSC_VER
    // stupid MSVC!
    TmxObjectLayer() = default;
    TmxObjectLayer(const TmxObjectLayer&) = delete;
    TmxObjectLayer& operator=(const TmxObjectLayer&) = delete;
#endif

    Color4u color;  ///< The color of the objects
    TmxDrawOrder drawOrder; ///< The draw order of the objects
    std::vector<std::unique_ptr<TmxObject>> objects;  ///< The objects of the layer

    void accept(const TmxLayers& map, TmxVisitor& visitor) const override;
  };

  /**
   * @ingroup core_tmx
   * @brief A reference to an image
   */
  struct GF_CORE_API TmxImage {
    std::string format;   ///< The format of the image
    Path source;          ///< The path to the image
    Color4u transparent;  ///< The transparent color
    Vector2i size;        ///< The size of the image
  };

  /**
   * @ingroup core_tmx
   * @brief A layer with an image
   */
  struct GF_CORE_API TmxImageLayer : public TmxLayer {
    std::unique_ptr<TmxImage> image;  ///< The image of the layer

    void accept(const TmxLayers& map, TmxVisitor& visitor) const override;
  };


  /**
   * @ingroup core_tmx
   * @brief A layer with other layers
   */
  struct GF_CORE_API TmxGroupLayer : public TmxLayer {
#ifdef _MSC_VER
    // stupid MSVC!
    TmxGroupLayer() = default;
    TmxGroupLayer(const TmxGroupLayer&) = delete;
    TmxGroupLayer& operator=(const TmxGroupLayer&) = delete;
#endif

    std::vector<std::unique_ptr<TmxLayer>> layers;  ///< The other layers

    void accept(const TmxLayers& map, TmxVisitor& visitor) const override;
  };

  /**
   * @ingroup core_tmx
   * @brief A frame in a tile animation
   *
   * @sa gf::TmxAnimation
   */
  struct GF_CORE_API TmxFrame {
    int tileId;
    Time duration;
  };

  /**
   * @ingroup core_tmx
   * @brief A tile animation
   */
  struct GF_CORE_API TmxAnimation {
    std::vector<TmxFrame> frames; ///< The frames of the animation
  };

  /**
   * @ingroup core_tmx
   * @brief A rectangular part of a tileset
   *
   * @sa gf::TmxTileset
   */
  struct GF_CORE_API TmxTile {
    TmxProperties properties; ///< The properties of the tile
    int id;                   ///< The local id of the tile
    std::string type;         ///< The type of the tile
    int probability;          ///< The probability of the tile

    std::unique_ptr<TmxImage> image;  ///< The image of this tile
    std::unique_ptr<TmxObjectLayer> objects;  ///< The objects in the tile
    std::unique_ptr<TmxAnimation> animation;  ///< The animation data of the tile
  };

  /**
   * @ingroup core_tmx
   * @brief A wang color
   *
   * @sa gf::TmxWangSet
   */
  struct GF_CORE_API TmxWangColor {
    TmxProperties properties; ///< The properties of the wang color
    std::string name;         ///< The name of the wang color
    Color4u color;            ///< The color of the wang color
    int tile;                 ///< The id of the tile representing the color
    int probability;          ///< The probability of the tile
  };

  /**
   * @ingroup core_tmx
   * @brief A wang tile
   *
   * @sa gf::TmxWangSet
   */
  struct GF_CORE_API TmxWangTile {
    static constexpr std::size_t Top          = 0; ///< Index of the top color
    static constexpr std::size_t TopRight     = 1; ///< Index of the top-right color
    static constexpr std::size_t Right        = 2; ///< Index of the right color
    static constexpr std::size_t BottomRight  = 3; ///< Index of the bottom-right color
    static constexpr std::size_t Bottom       = 4; ///< Index of the bottom color
    static constexpr std::size_t BottomLeft   = 5; ///< Index of the bottom-left color
    static constexpr std::size_t Left         = 6; ///< Index of the left color
    static constexpr std::size_t TopLeft      = 7; ///< Index of the top-left color

    int tileid;                 ///< The id of the tile
    std::array<int, 8> wangid;  ///< the wang colors of the corners and edges (top, top-right, right, bottom-right, bottom, bottom-left, left, top-left)
  };

  /**
   * @ingroup core_tmx
   * @brief A wang set
   *
   * @sa gf::TmxTileset
   */
  struct GF_CORE_API TmxWangSet {
    TmxProperties properties; ///< The properties of the wang set
    std::string name;         ///< The name of the wang set
    int tile;                 ///< the id of the tile representing the wang set

    std::vector<TmxWangColor> colors;
    std::vector<TmxWangTile> tiles;
  };

  /**
   * @ingroup core_tmx
   * @brief A set of tiles in a single file (image or TSX file)
   */
  struct GF_CORE_API TmxTileset {
    TmxProperties properties; ///< The properties of the tileset

    uint32_t firstGid;        ///< The first global id of the tileset
    std::string name;         ///< The name of the tileset
    Vector2i tileSize;        ///< The size of a tile in the tileset
    int spacing;              ///< The spacing between tiles (in pixels)
    int margin;               ///< The margin around tiles (in pixels)
    int tileCount;            ///< The number of tiles
    int columnCount;          ///< The number of columns

    Vector2i offset;          ///< The offset of the tileset

    std::unique_ptr<TmxImage> image;  ///< The image of the tileset
    std::vector<TmxTile> tiles;       ///< The tiles of the tileset
    std::vector<TmxWangSet> wangsets; ///< The wang set of the tileset

    /**
     * @brief Get the tile corresponding to an id.
     *
     * @param id The id of the tile
     * @returns The tile
     */
    const TmxTile *getTile(int id) const noexcept;

    /**
     * @brief Get the rectangle of a tile corresponding to an id.
     *
     * @param id The id of the tile
     * @param size The size of the image corresponding to the tile
     * @returns The rectangle of the tile
     */
    RectI getSubTexture(int id, Vector2i size) const noexcept;
  };

  /**
   * @ingroup core_tmx
   * @brief A TMX map
   */
  struct GF_CORE_API TmxLayers {
#ifdef _MSC_VER
    // stupid MSVC!
    TmxLayers() = default;
    TmxLayers(const TmxLayers&) = delete;
    TmxLayers& operator=(const TmxLayers&) = delete;
#endif

    TmxProperties properties;   ///< The properties of the map

    std::string version;        ///< The version of the map
    std::string tiledVersion;   ///< The tiled version of the map
    CellOrientation orientation;  ///< The orientation of the map
    TmxRenderOrder renderOrder; ///< The render order of the map

    bool infinite;              ///< Is the map infinite?
    Vector2i mapSize;           ///< The size of the map
    Vector2i tileSize;          ///< The size of the tiles

    int hexSideLength;          ///< The length of the side for hexagonal map
    CellAxis cellAxis;          ///< The stagger axis for hexagonal map
    CellIndex cellIndex;        ///< The stagger index for hexagonal map

    Color4u backgroundColor;    ///< The background color

    int nextObjectId;           ///< The next object id

    std::vector<TmxTileset> tilesets; ///< The tilesets used in the map
    std::vector<std::unique_ptr<TmxLayer>> layers;  ///< The layers of the map

  public:
    /**
     * @brief Get the tileset corresponding to a global id.
     *
     * @param gid A global id
     * @returns The corresponding tileset
     */
    const TmxTileset *getTileSetFromGID(uint32_t gid) const noexcept;

    /**
     * @brief Visit the layers with a visitor.
     *
     * @param visitor the visitor
     */
    void visitLayers(TmxVisitor& visitor) const;

    /**
     * @brief Load a TMX file
     *
     * @param filename The name of the TMX file
     */
    bool loadFromFile(const Path& filename);
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_TMX_H
