#include "Singletons.h"

namespace bi {
  gf::Singleton<gf::MessageManager> gMessageManager;
  gf::Singleton<gf::ResourceManager> gResourceManager;
  gf::Singleton<gf::TextureAtlas> gTextureAtlas;
  gf::Singleton<gf::Random> gRandom;
}
