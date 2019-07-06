/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Flags.h"
#include "Flip.h"
#include "Id.h"
#include "Path.h"
#include "Portability.h"
#include "Rect.h"
#include "Stagger.h"
#include "Time.h"
#include "Vector.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif
  /**
   * @ingroup game
   * @brief The orientation of the map.
   */
  enum class TmxOrientation {
    Unknown,    ///< An unknown orientation
    Orthogonal, ///< An orthogonal orientation
    Isometric,  ///< An isometric orientation
    Staggered,  ///< A staggered orientation
    Hexagonal,  ///< A hexagonal orientation
  };

  /**
   * @ingroup game
   * @brief the render order of the tiles.
   */
  enum class TmxRenderOrder {
    RightDown, ///< Right down order
    RightUp,   ///< Right up order
    LeftDown,  ///< Left down order
    LeftUp,    ///< Left up order
  };

  /**
   * @ingroup game
   * @brief The properties for TMX entities
   *
   */
  class GF_API TmxProperties {
  public:
    /**
     * @brief Default constructor
     */
    TmxProperties() = default;

    /**
     * @brief Default move constructor
     */
    TmxProperties(TmxProperties&&) = default;

    /**
     * @brief Default move assignment
     */
    TmxProperties& operator=(TmxProperties&&) = default;

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

  private:
    std::map<std::string, std::string> m_stringProps;
    std::map<std::string, int> m_intProps;
    std::map<std::string, double> m_floatProps;
    std::map<std::string, bool> m_boolProps;
    std::map<std::string, Color4u> m_colorProps;
    std::map<std::string, Path> m_fileProps;
  };

  struct TmxLayers;

  struct TmxTileLayer;
  struct TmxObjectLayer;
  struct TmxImageLayer;
  struct TmxGroupLayer;

  /**
   * @ingroup game
   * @brief A visitor for layers in the visitor pattern
   *
   * @sa gf::TmxLayer::accept()
   * @sa [Wikipedia - Visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern)
   */
  class GF_API TmxVisitor {
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
   * @ingroup game
   * @brief A layer in the whole map
   *
   * The are four kinds of layers: tile layers, image layers, object layers and
   * group layers.
   *
   * @sa gf::TmxTileLayer, gf::TmxImageLayer, gf::TmxObjectLayer, gf::TmxGroupLayer
   */
  struct GF_API TmxLayer {
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
   * @ingroup game
   * @brief A cell in a tile layer
   */
  struct GF_API TmxCell {
    int gid;             ///< The global id of the tile
    Flags<Flip> flip = None;  ///< The flip properties of the tile
  };

  /**
   * @ingroup game
   * @brief A chunk in a tile layer (for infinite maps)
   */
  struct GF_API TmxChunk {
    Vector2i position;
    Vector2i size;
    std::vector<TmxCell> cells;
  };

  /**
   * @ingroup game
   * @brief A layer with tiles in cells
   */
  struct GF_API TmxTileLayer : public TmxLayer {
    std::vector<TmxCell> cells; ///< The cells of the layer
    std::vector<TmxChunk> chunks; ///< The chunks of the layer

    void accept(const TmxLayers& map, TmxVisitor& visitor) const override;
  };

  /**
   * @ingroup game
   * @brief The draw order of the objects
   */
  enum class TmxDrawOrder {
    TopDown,  ///< Top-down order
    Index,    ///< Index order
  };


  /**
   * @ingroup game
   * @brief A geometrical object
   *
   * There are six kinds of objects:
   *
   * - rectangles (see gf::TmxRectangle)
   * - ellipses (see gf::TmxEllipse)
   * - polylines (see gf::TmxPolyline)
   * - polygons (see gf::TmxPolygon)
   * - tiles (see gf::TmxTileObject)
   * - texts (see gf::TmxText)
   *
   * @sa gf::TmxObjectLayer
   */
  struct GF_API TmxObject {
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
   * @ingroup game
   * @brief A rectangle object
   */
  struct GF_API TmxRectangle : public TmxObject {
    Vector2f size;  ///< The size of the rectangle
  };

  /**
   * @ingroup game
   * @brief An ellipse object
   */
  struct GF_API TmxEllipse : public TmxObject {
    Vector2f size;  ///< The size of the ellipse
  };

  /**
   * @ingroup game
   * @brief An image put in the map identified by its global id
   */
  struct GF_API TmxTileObject : public TmxObject {
    int gid;
    Flags<Flip> flip;
  };

  /**
   * @ingroup game
   * @brief A polyline object
   *
   * A polyline is an open set of lines
   */
  struct GF_API TmxPolyline : public TmxObject {
    std::vector<Vector2f> points; ///< The points of the polyline
  };

  /**
   * @ingroup game
   * @brief A polygon object
   *
   * A polygon is a closed set of lines
   */
  struct GF_API TmxPolygon : public TmxObject {
    std::vector<Vector2f> points; ///< The points of the polygon
  };

  /**
   * @ingroup game
   * @brief A text object
   */
  struct GF_API TmxText : public TmxObject {
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
   * @ingroup game
   * @brief A point object
   */
  struct GF_API TmxPoint : public TmxObject {
  };

  /**
   * @ingroup game
   * @brief A layer with objects
   */
  struct GF_API TmxObjectLayer : public TmxLayer {
    Color4u color;  ///< The color of the objects
    TmxDrawOrder drawOrder; ///< The draw order of the objects
    std::vector<std::unique_ptr<TmxObject>> objects;  ///< The objects of the layer

    void accept(const TmxLayers& map, TmxVisitor& visitor) const override;
  };

  /**
   * @ingroup game
   * @brief A reference to an image
   */
  struct GF_API TmxImage {
    std::string format;   ///< The format of the image
    Path source;          ///< The path to the image
    Color4u transparent;  ///< The transparent color
    Vector2i size;        ///< The size of the image
  };

  /**
   * @ingroup game
   * @brief A layer with an image
   */
  struct GF_API TmxImageLayer : public TmxLayer {
    std::unique_ptr<TmxImage> image;  ///< The image of the layer

    void accept(const TmxLayers& map, TmxVisitor& visitor) const override;
  };


  /**
   * @ingroup game
   * @brief A layer with other layers
   */
  struct GF_API TmxGroupLayer : public TmxLayer {
    std::vector<std::unique_ptr<TmxLayer>> layers;  ///< The other layers

    void accept(const TmxLayers& map, TmxVisitor& visitor) const override;
  };

  /**
   * @ingroup game
   * @brief A frame in a tile animation
   *
   * @sa gf::TmxAnimation
   */
  struct GF_API TmxFrame {
    int tileId;
    Time duration;
  };

  /**
   * @ingroup game
   * @brief A tile animation
   */
  struct GF_API TmxAnimation {
    std::vector<TmxFrame> frames; ///< The frames of the animation
  };

  /**
   * @ingroup game
   * @brief A description of a kind of terrain on the map
   *
   * @sa gf::TmxTileset
   */
  struct GF_API TmxTerrain {
    TmxProperties properties; ///< The properties of the terrain
    std::string name;         ///< The name of the terrain
    int tile;                 ///< The representing tile for the terrain
  };

  /**
   * @ingroup game
   * @brief A rectangular part of a tileset
   *
   * @sa gf::TmxTileset
   */
  struct GF_API TmxTile {
    TmxProperties properties; ///< The properties of the tile
    int id;                   ///< The local id of the tile
    std::string type;         ///< The type of the tile
    std::array<int, 4> terrain;  ///< The terrain if the corners (top-left, top-right, bottom-left, bottom-right)
    int probability;          ///< The probability of the tile

    std::unique_ptr<TmxImage> image;  ///< The image of this tile
    std::unique_ptr<TmxObjectLayer> objects;  ///< The objects in the tile
    std::unique_ptr<TmxAnimation> animation;  ///< The animation data of the tile
  };

  /**
   * @ingroup game
   * @brief A set of tiles in a single file (image or TSX file)
   */
  struct GF_API TmxTileset {
    TmxProperties properties; ///< The properties of the tileset

    int firstGid;             ///< The first global id of the tileset
    std::string name;         ///< The name of the tileset
    Vector2i tileSize;        ///< The size of a tile in the tileset
    int spacing;              ///< The spacing between tiles (in pixels)
    int margin;               ///< The margin around tiles (in pixels)
    int tileCount;            ///< The number of tiles
    int columnCount;          ///< The number of columns

    Vector2i offset;          ///< The offset of the tileset

    std::unique_ptr<TmxImage> image;  ///< The image of the tileset
    std::vector<TmxTerrain> terrains; ///< The terrains of the tileset
    std::vector<TmxTile> tiles;       ///< The tiles of the tileset

  public:
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
   * @ingroup game
   * @brief A TMX map
   */
  struct GF_API TmxLayers {
    TmxProperties properties;   ///< The properties of the map

    std::string version;        ///< The version of the map
    std::string tiledVersion;   ///< The tiled version of the map
    TmxOrientation orientation; ///< The orientation of the map
    TmxRenderOrder renderOrder; ///< The render order of the map

    bool infinite;              ///< Is the map infinite?
    Vector2i mapSize;           ///< The size of the map
    Vector2i tileSize;          ///< The size of the tiles

    int hexSideLength;          ///< The length of the side for hexagonal map
    StaggerAxis staggerAxis;    ///< The stagger axis for hexagonal map
    StaggerIndex staggerIndex;  ///< The stagger index for hexagonal map

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
    const TmxTileset *getTileSetFromGID(int gid) const noexcept;

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
