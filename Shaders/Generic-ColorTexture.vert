#version 330 core

uniform mat4 in_mvp;

in vec3 in_vertex;
in vec4 in_color;
in vec2 in_texcoord;

out vec4 color;
out vec2 texcoord;

void main()
{
  color = in_color;
  texcoord = in_texcoord;
  gl_Position = in_mvp * vec4(in_vertex, 1.0f);
}
