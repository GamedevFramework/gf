#ifndef BI_SINGLETONS_H
#define BI_SINGLETONS_H

#include <gf/MessageManager.h>
#include <gf/Random.h>
#include <gf/Singleton.h>
#include <gf/TextureAtlas.h>
#include <gf/ResourceManager.h>

namespace bi {
  extern gf::Singleton<gf::MessageManager> gMessageManager;
  extern gf::Singleton<gf::ResourceManager> gResourceManager;
  extern gf::Singleton<gf::TextureAtlas> gTextureAtlas;
  extern gf::Singleton<gf::Random> gRandom;
}

#endif // BI_SINGLETONS_H
