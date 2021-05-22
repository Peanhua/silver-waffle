// Blinn-Phong adaptation from https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model

#version 330

in vec3 in_vertex;
in vec3 in_color;
in vec3 in_normal;

out vec3 v2f_position;
out vec3 v2f_normal;
out vec3 v2f_color;

uniform mat4 in_mvp;
uniform mat4 in_view;
uniform mat4 in_model;

void main()
{
  vec4 pos = in_view * in_model * vec4(in_vertex, 1);
  v2f_position = pos.xyz / pos.w;
  v2f_color = in_color;
  mat4 normal_matrix = in_view * in_model;
  v2f_normal = vec3(normal_matrix * vec4(in_normal, 0));
  gl_Position = in_mvp * vec4(in_vertex, 1);
}
