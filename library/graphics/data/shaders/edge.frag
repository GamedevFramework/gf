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
#version 100

precision mediump float;

varying vec4 v_color;
varying vec2 v_texCoords;

uniform sampler2D u_texture;
uniform vec2 u_framebufferSize;

void main(void) {
  vec2 invSize = 1.0 / u_framebufferSize;

  vec4 horizontalEdge = vec4(0.0);
  horizontalEdge -= texture2D(u_texture, v_texCoords + vec2(-1, -1) * invSize) * 1.0;
  horizontalEdge -= texture2D(u_texture, v_texCoords + vec2(-1,  0) * invSize) * 2.0;
  horizontalEdge -= texture2D(u_texture, v_texCoords + vec2(-1,  1) * invSize) * 1.0;
  horizontalEdge += texture2D(u_texture, v_texCoords + vec2( 1, -1) * invSize) * 1.0;
  horizontalEdge += texture2D(u_texture, v_texCoords + vec2( 1,  0) * invSize) * 2.0;
  horizontalEdge += texture2D(u_texture, v_texCoords + vec2( 1,  1) * invSize) * 1.0;

  vec4 verticalEdge = vec4(0.0);
  verticalEdge -= texture2D(u_texture, v_texCoords + vec2(-1, -1) * invSize) * 1.0;
  verticalEdge -= texture2D(u_texture, v_texCoords + vec2( 0, -1) * invSize) * 2.0;
  verticalEdge -= texture2D(u_texture, v_texCoords + vec2( 1, -1) * invSize) * 1.0;
  verticalEdge += texture2D(u_texture, v_texCoords + vec2(-1,  1) * invSize) * 1.0;
  verticalEdge += texture2D(u_texture, v_texCoords + vec2( 0,  1) * invSize) * 2.0;
  verticalEdge += texture2D(u_texture, v_texCoords + vec2( 1,  1) * invSize) * 1.0;

  vec3 edge = sqrt((horizontalEdge.rgb * horizontalEdge.rgb) + (verticalEdge.rgb * verticalEdge.rgb));

  vec4 color = texture2D(u_texture, v_texCoords);

  if (length(edge) > 0.9) {
    color.rgb = vec3(1.0 - length(edge));
  }

//   vec4 color = vec4(edge, texture2D(u_texture, v_texCoords).a);

  gl_FragColor = color * v_color;
}
