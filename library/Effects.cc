/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
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
#include <gf/Effects.h>

#include "generated/default.frag.h"
#include "generated/default.vert.h"
#include "generated/color_matrix.frag.h"
#include "generated/edge.frag.h"
#include "generated/fxaa.frag.h"

#include "config.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  DefaultEffect::DefaultEffect() {
    loadFromMemory(default_vert, default_frag);
  }


  AntiAliasingEffect::AntiAliasingEffect() {
    loadFromMemory(default_vert, fxaa_frag);
    setUniform("u_framebufferSize", Vector2f{ 1.0f, 1.0f });
  }

  void AntiAliasingEffect::setFramebufferSize(Vector2f size) {
    setUniform("u_framebufferSize", size);
  }


  ColorMatrixEffect::ColorMatrixEffect() {
    loadFromMemory(default_vert, color_matrix_frag);
  }

  void ColorMatrixEffect::setColorMatrix(const Matrix4f& mat) {
    setUniform("u_colorMatrix", mat);
  }


  ColorEffect::ColorEffect(Type type) {
    setType(type);
  }

  void ColorEffect::setType(Type type) {
    static constexpr Matrix4f NormalMatrix = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    };

    static constexpr Matrix4f GrayscaleMatrix = {
      0.299f, 0.587f, 0.114f, 0.0f,
      0.299f, 0.587f, 0.114f, 0.0f,
      0.299f, 0.587f, 0.114f, 0.0f,
      0.0f,   0.0f,   0.0f,   1.0f
    };

    static constexpr Matrix4f SepiaMatrix = {
      0.393f, 0.769f, 0.189f, 0.0f,
      0.349f, 0.686f, 0.168f, 0.0f,
      0.272f, 0.534f, 0.131f, 0.0f,
      0.0f,   0.0f,   0.0f,   1.0f
    };

    static constexpr Matrix4f NightVisionMatrix = {
      0.1f, 0.4f, 0.0f, 0.0f,
      0.3f, 1.0f, 0.3f, 0.0f,
      0.0f, 0.4f, 0.1f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    };

    static constexpr Matrix4f WarmMatrix = {
      1.06f, 0.0f,  0.0f,  0.0f,
      0.0f,  1.01f, 0.0f,  0.0f,
      0.0f,  0.0f,  0.93f, 0.0f,
      0.0f,  0.0f,  0.0f,  1.0f
    };

    static constexpr Matrix4f CoolMatrix = {
      0.99f, 0.0f,  0.0f,  0.0f,
      0.0f,  0.93f, 0.0f,  0.0f,
      0.0f,  0.0f,  1.08f, 0.0f,
      0.0f,  0.0f,  0.0f,  1.0f
    };

    switch (type) {
      case Normal:
        setColorMatrix(NormalMatrix);
        break;
      case Grayscale:
        setColorMatrix(GrayscaleMatrix);
        break;
      case Sepia:
        setColorMatrix(SepiaMatrix);
        break;
      case NightVision:
        setColorMatrix(NightVisionMatrix);
        break;
      case Warm:
        setColorMatrix(WarmMatrix);
        break;
      case Cool:
        setColorMatrix(CoolMatrix);
        break;
    }
  }


  ColorBlindEffect::ColorBlindEffect(Type type) {
    setType(type);
  }

  void ColorBlindEffect::setType(Type type) {
    static constexpr Matrix4f NormalMatrix = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    };

    static constexpr Matrix4f ProtanopiaMatrix = {
      0.567f, 0.433f, 0.0f,   0.0f,
      0.558f, 0.442f, 0.0f,   0.0f,
      0.0f,   0.242f, 0.758f, 0.0f,
      0.0f,   0.0f,   0.0f,   1.0f
    };

    static constexpr Matrix4f ProtanomalyMatrix = {
      0.817f, 0.183f, 0.0f,   0.0f,
      0.333f, 0.667f, 0.0f,   0.0f,
      0.0f,   0.125f, 0.875f, 0.0f,
      0.0f,   0.0f,   0.0f,   1.0f
    };

    static constexpr Matrix4f DeuteranopiaMatrix = {
      0.625f, 0.375f, 0.0f,   0.0f,
      0.7f,   0.3f,   0.0f,   0.0f,
      0.0f,   0.3f,   0.7f,   0.0f,
      0.0f,   0.0f,   0.0f,   1.0f
    };

    static constexpr Matrix4f DeuteranomalyMatrix = {
      0.8f,   0.2f,   0.0f,   0.0f,
      0.258f, 0.742f, 0.0f,   0.0f,
      0.0f,   0.142f, 0.858f, 0.0f,
      0.0f,   0.0f,   0.0f,   1.0f
    };

    static constexpr Matrix4f TritanopiaMatrix = {
      0.95f,  0.05f,  0.0f,   0.0f,
      0.0f,   0.433f, 0.567f, 0.0f,
      0.0f,   0.475f, 0.525f, 0.0f,
      0.0f,   0.0f,   0.0f,   1.0f
    };

  static constexpr Matrix4f TritanomalyMatrix = {
      0.967f, 0.033f, 0.0f,   0.0f,
      0.0f,   0.733f, 0.267f, 0.0f,
      0.0f,   0.183f, 0.817f, 0.0f,
      0.0f,   0.0f,   0.0f,   1.0f
    };

  static constexpr Matrix4f AchromatopsiaMatrix = {
      0.299f, 0.587f, 0.114f, 0.0f,
      0.299f, 0.587f, 0.114f, 0.0f,
      0.299f, 0.587f, 0.114f, 0.0f,
      0.0f,   0.0f,   0.0f,   1.0f
    };

  static constexpr Matrix4f AchromatomalyMatrix = {
      0.618f, 0.320f, 0.062f, 0.0f,
      0.163f, 0.775f, 0.062f, 0.0f,
      0.163f, 0.320f, 0.516f, 0.0f,
      0.0f,   0.0f,   0.0f,   1.0f
    };


    switch (type) {
      case Normal:
        setColorMatrix(NormalMatrix);
        break;
      case Protanopia:
        setColorMatrix(ProtanopiaMatrix);
        break;
      case Protanomaly:
        setColorMatrix(ProtanomalyMatrix);
        break;
      case Deuteranopia:
        setColorMatrix(DeuteranopiaMatrix);
        break;
      case Deuteranomaly:
        setColorMatrix(DeuteranomalyMatrix);
        break;
      case Tritanopia:
        setColorMatrix(TritanopiaMatrix);
        break;
      case Tritanomaly:
        setColorMatrix(TritanomalyMatrix);
        break;
      case Achromatopsia:
        setColorMatrix(AchromatopsiaMatrix);
        break;
      case Achromatomaly:
        setColorMatrix(AchromatomalyMatrix);
        break;
    }
  }

  EdgeEffect::EdgeEffect() {
    loadFromMemory(default_vert, edge_frag);
    setUniform("u_framebufferSize", Vector2f{ 1.0f, 1.0f });
  }

  void EdgeEffect::setFramebufferSize(Vector2f size) {
    setUniform("u_framebufferSize", size);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
