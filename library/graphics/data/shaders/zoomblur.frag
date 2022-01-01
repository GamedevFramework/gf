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

uniform float u_strength;

const float PI = 3.141592653589793;

float Linear_ease(in float begin, in float change, in float duration, in float time) {
  return change * time / duration + begin;
}

float Exponential_easeInOut(in float begin, in float change, in float duration, in float time) {
  if (time == 0.0) {
    return begin;
  }

  if (time == duration) {
    return begin + change;
  }

  time = time / (duration / 2.0);

  if (time < 1.0) {
    return change / 2.0 * pow(2.0, 10.0 * (time - 1.0)) + begin;
  }

  return change / 2.0 * (-pow(2.0, -10.0 * (time - 1.0)) + 2.0) + begin;
}

float Sinusoidal_easeInOut(in float begin, in float change, in float duration, in float time) {
  return -change / 2.0 * (cos(PI * time / duration) - 1.0) + begin;
}

/* random number between 0 and 1 */
float random(in vec2 scale, in float seed) {
  /* use the fragment position for randomness */
  return fract(sin(dot(v_texCoords + seed, scale)) * 43758.5453 + seed);
}

vec4 crossFade(in vec2 uv, in float dissolve) {
  return mix(texture2D(u_texture0, uv), texture2D(u_texture1, uv), dissolve);
}

void main() {
  float dissolve = Exponential_easeInOut(0.0, 1.0, 1.0, u_progress);

  // Mirrored sinusoidal loop. 0->strength then strength->0
  float strength = Sinusoidal_easeInOut(0.0, u_strength, 0.5, u_progress);

  vec4 color = vec4(0.0);
  float sum = 0.0;
  vec2 dir = vec2(0.5) - v_texCoords;

  /* randomize the lookup values to hide the fixed number of samples */
  float offset = random(vec2(12.9898, 78.233), 0.0);

  for (float t = 0.0; t <= 40.0; t++) {
    float percent = (t + offset) / 40.0;
    float weight = 4.0 * (percent - percent * percent);
    color += crossFade(v_texCoords + dir * percent * strength, dissolve) * weight;
    sum += weight;
  }

  gl_FragColor = (color / sum) * v_color;
}
