#version 150

uniform mat4  in_mvp;
uniform float in_time;

in vec3 in_vertex;
in vec3 in_color;
in vec3 in_generic_vec3;

out vec4 diffuse_color;

void main()
{
  float a = max(0.0f, 2.0f - in_time);
  diffuse_color = vec4(in_color, a);
  gl_Position = in_mvp * vec4(in_vertex + in_generic_vec3 * in_time, 1.0f);
}
