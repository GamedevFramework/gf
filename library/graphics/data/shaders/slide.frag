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
uniform int u_stripes;
uniform int u_orientation; // 0 = horizontal, 1 = vertical

void main(void) {
  float width = 1.0 / float(u_stripes);

  if (u_orientation == 0) {
    if (u_progress < mod(1.0 - v_texCoords.y, width) / width) {
      gl_FragColor = texture2D(u_texture0, v_texCoords) * v_color;
    } else {
      gl_FragColor = texture2D(u_texture1, v_texCoords) * v_color;
    }
  } else {
    if (u_progress < mod(v_texCoords.x, width) / width) {
      gl_FragColor = texture2D(u_texture0, v_texCoords) * v_color;
    } else {
      gl_FragColor = texture2D(u_texture1, v_texCoords) * v_color;
    }
  }
}
