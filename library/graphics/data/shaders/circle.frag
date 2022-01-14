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
#version 100

precision mediump float;

varying vec4 v_color;
varying vec2 v_texCoords;

uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform float u_progress;

uniform int u_direction; // > 0 means opening, < 0 means closing
uniform float u_ratio;

float square(float x) {
  return x * x;
}

float lengthSquared(vec2 v) {
  return v.x * v.x + v.y * v.y;
}

void main(void) {
  vec4 texel0 = texture2D(u_texture0, v_texCoords);
  vec4 texel1 = texture2D(u_texture1, v_texCoords);

  vec2 pos = v_texCoords - vec2(0.5);

  if (u_ratio >= 1.0) {
    pos.x *= u_ratio;
  } else {
    pos.y *= 1.0 / u_ratio;
  }

  if (u_direction > 0) {
    if (lengthSquared(pos) < square(u_progress)) {
      gl_FragColor = texel1;
    } else {
      gl_FragColor = texel0;
    }
  } else if (u_direction < 0) {
    if (lengthSquared(pos) > square(1.0 - u_progress)) {
      gl_FragColor = texel1;
    } else {
      gl_FragColor = texel0;
    }
  } else {
    gl_FragColor = texel0 * v_color;
  }

}
