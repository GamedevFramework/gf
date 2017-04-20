#ifndef BI_DECORATION_MANAGER_H
#define BI_DECORATION_MANAGER_H

#include <vector>

#include <gf/Entity.h>
#include <gf/Rect.h>
#include <gf/Texture.h>
#include <gf/Vector.h>

namespace bi {
  class DecorationManager: public gf::Entity {
  public:
    enum Type {
      Above,
      Below,
    };

    DecorationManager(Type type);

    void addDecoration(const gf::Vector2f position);

    virtual void render(gf::RenderTarget& target) override;

  private:
    struct Decoration {
      gf::Vector2f position;
      gf::RectF rect;
      float size;
      float angle;
    };

  private:
    Type m_type;
    std::vector<Decoration> m_decorations;
    const gf::Texture& m_texture;
  };
}

#endif // BI_DECORATION_MANAGER_H
