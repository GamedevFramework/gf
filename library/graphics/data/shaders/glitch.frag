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
uniform float u_intensity;

// https://www.shadertoy.com/view/4dXBW2

float sat( float t ) {
	return clamp( t, 0.0, 1.0 );
}

vec2 sat( vec2 t ) {
	return clamp( t, 0.0, 1.0 );
}

//remaps inteval [a;b] to [0;1]
float remap( float t, float a, float b ) {
	return sat( (t - a) / (b - a) );
}

//note: /\ t=[0;0.5;1], y=[0;1;0]
float linterp( float t ) {
	return sat( 1.0 - abs( 2.0*t - 1.0 ) );
}

vec3 spectrum_offset( float t ) {
	vec3 ret;
	float lo = step(t,0.5);
	float hi = 1.0-lo;
	float w = linterp( remap( t, 1.0/6.0, 5.0/6.0 ) );
	float neg_w = 1.0-w;
	ret = vec3(lo,1.0,hi) * vec3(neg_w, w, neg_w);
	return pow( ret, vec3(1.0/2.2) );
}

//note: [0;1]
float rand( vec2 n ) {
  return fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

//note: [-1;1]
float srand( vec2 n ) {
	return rand(n) * 2.0 - 1.0;
}

float mytrunc( float x, float num_levels )
{
	return floor(x*num_levels) / num_levels;
}
vec2 mytrunc( vec2 x, float num_levels )
{
	return floor(x*num_levels) / num_levels;
}

void main(void) {

  vec2 uv = v_texCoords;
  uv.y = uv.y;

  float iTime = u_progress * 0.5;

  float time = mod(iTime*100.0, 32.0)/110.0; // + modelmat[0].x + modelmat[0].z;

  float intensity = min(sin(u_progress*3.14)*5.0, 1.0);
  float GLITCH = 0.1 + intensity;

  float gnm = sat( GLITCH );
  float rnd0 = rand( mytrunc( vec2(time, time), 6.0 ) );
  float r0 = sat((1.0-gnm)*0.7 + rnd0);
  float rnd1 = rand( vec2(mytrunc( uv.x, 10.0*r0 ), time) ); //horz
  //float r1 = 1.0f - sat( (1.0f-gnm)*0.5f + rnd1 );
  float r1 = 0.5 - 0.5 * gnm + rnd1;
  r1 = 1.0 - max( 0.0, ((r1<1.0) ? r1 : 0.9999999) ); //note: weird ass bug on old drivers
  float rnd2 = rand( vec2(mytrunc( uv.y, 40.0*r1 ), time) ); //vert
  float r2 = sat( rnd2 );

  float rnd3 = rand( vec2(mytrunc( uv.y, 10.0*r0 ), time) );
  float r3 = (1.0-sat(rnd3+0.8)) - 0.1;

  float pxrnd = rand( uv + time );

  float ofs = 0.05 * r2 * GLITCH * ( rnd0 > 0.5 ? 1.0 : -1.0 );
  ofs += 0.5 * pxrnd * ofs;

  uv.y += 0.1 * r3 * GLITCH;

  const int NUM_SAMPLES = 20;
  const float RCP_NUM_SAMPLES_F = 1.0 / float(NUM_SAMPLES);

  vec4 sum = vec4(0.0);
  vec3 wsum = vec3(0.0);
  for( int i=0; i<NUM_SAMPLES; ++i )
  {
	float t = float(i) * RCP_NUM_SAMPLES_F;
	uv.x = sat( uv.x + ofs * t );
	vec4 tex1 = texture2D( u_texture0, uv ); // bias -10.0
	vec4 tex2 = texture2D( u_texture1, uv ); // bias -10.0
	vec4 samplecol = mix(tex1, tex2, u_progress); // bias -10.0
	vec3 s = spectrum_offset( t );
	samplecol.rgb = samplecol.rgb * s;
	sum += samplecol;
	wsum += s;
  }
  sum.rgb /= wsum;
  sum.a *= RCP_NUM_SAMPLES_F;

  gl_FragColor.a = sum.a;
  gl_FragColor.rgb = sum.rgb; // * outcol0.a;
}
