#version 330 core

const float PI = radians(180.0f);

uniform mat4  in_mvp;
uniform float in_time;

in vec4 in_vertex;
in vec3 in_color;
in vec2 in_generic_vec2;

out VERTEX_DATA
{
  vec4 color;
} v2g;

void main()
{
  float start_time = in_vertex.w;
  float movement_speed = in_generic_vec2.x;
  float blinking_interval = in_generic_vec2.y;

  float a = 1;
  if(fract(in_time * blinking_interval) < 0.067)
    a = 0.5;

  v2g.color = vec4(in_color, a);

  gl_Position = in_mvp * vec4(in_vertex.x,
                              in_vertex.y - movement_speed * (in_time - start_time),
                              in_vertex.z,
                              1);
}
