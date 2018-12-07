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

attribute vec2 a_position;
attribute vec4 a_color;
attribute vec2 a_texCoords;

varying vec4 v_color;
varying vec2 v_texCoords;

uniform mat3 u_transform;

void main(void) {
  v_texCoords = a_texCoords;
  v_color = a_color;

  vec3 worldPosition = vec3(a_position, 1);
  vec3 normalizedPosition = worldPosition * u_transform;
  // http://stackoverflow.com/questions/16893536/using-row-major-in-opengl-shader

  gl_Position = vec4(normalizedPosition.xy, 0, 1);
  gl_PointSize = 1.0;
}
