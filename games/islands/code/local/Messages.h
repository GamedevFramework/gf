#ifndef BI_MESSAGES_H
#define BI_MESSAGES_H

#include <gf/Message.h>
#include <gf/Vector.h>

using namespace gf::literals;

namespace bi {

  struct HeroPosition: public gf::Message {
    static constexpr gf::Id type = "HeroPosition"_id;

    gf::Vector2f position;
    float angle;
    bool isOnIsland;
  };

  struct StartScan: public gf::Message {
    static constexpr gf::Id type = "StartScan"_id;
  };

  struct StopScan: public gf::Message {
    static constexpr gf::Id type = "StopScan"_id;
  };

  struct NearestTreasure: public gf::Message {
    static constexpr gf::Id type = "NearestTreasure"_id;

    gf::Vector2f position;
  };

  struct GoldLooted: public gf::Message {
    static constexpr gf::Id type = "GoldLooted"_id;

    unsigned value;
    gf::Vector2f next;
  };

  struct GameOver: public gf::Message {
    static constexpr gf::Id type = "GameOver"_id;
  };
}

#endif // BI_MESSAGES_H
