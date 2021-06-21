#version 330 core

#include "Include/Input.vert"
#include "Include/Output.vert"
#include "Include/Uniforms.glsl"

const float PI = radians(180.0f);


void main()
{
  float start_time = in_vertex.w;
  float movement_speed = in_generic_vec2.x;
  float blinking_interval = in_generic_vec2.y;

  float a = 1;
  if(fract(in_time * blinking_interval) < 0.067)
    a = 0.5;

  vout.color = vec4(in_color.rgb, a);

  gl_Position = in_mvp * vec4(in_vertex.x,
                              in_vertex.y - movement_speed * (in_time - start_time),
                              in_vertex.z,
                              1);
}
