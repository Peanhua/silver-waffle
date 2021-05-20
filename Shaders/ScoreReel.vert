// Blinn-Phong adaptation from https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model

#version 150

in vec3 in_vertex;
in vec3 in_color;
in vec3 in_normal;

out vec3 diffuse_color;
out vec3 vertex;
out vec3 normal;

uniform mat4 in_mvp;
uniform mat4 in_view;
uniform mat4 in_model;

void main()
{
  gl_Position = in_mvp * vec4(in_vertex,1);

  vec4 tmp = (in_view * in_model) * vec4(in_vertex, 1);
  vertex = tmp.xyz / tmp.w;

  diffuse_color = in_color;
  diffuse_color = vec3(1, 1, 1);
  
  normal = in_normal;
}
